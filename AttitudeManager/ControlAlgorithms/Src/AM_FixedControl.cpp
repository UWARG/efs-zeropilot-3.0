#include "AM_FixedControl.hpp"
#include "PID.hpp"

 // #include math_constants to remove sloppy definitions
constexpr double ZP_PI = 3.14159265358979311599796346854; // more precise pi value

static float DEG_TO_RAD(float angleInDegrees); 

static float DEG_TO_RAD(float angleInDegrees) {
    return ((angleInDegrees) * ZP_PI / 180.0);
}
// Remove in future to mitigate cluttering
typedef struct {
    float roll, pitch, yaw;             // Degrees. Yaw of 180 is north.
    float rollRate, pitchRate, yawRate; // Degrees/second
    float airspeed;                     // m/s
    float altitude;                     // m
    float rateOfClimb;                  // m/s
    long double latitude;               // Decimal degrees
    float latitudeSpeed;                // m/s
    long double longitude;              // Decimal degrees
    float longitudeSpeed;               // m/s
    double track;                       // Degrees. Track of 0 is north.
    float groundSpeed;                  // m/s
    double heading;                     // Degrees. Heading of 0 is north.
} SFOutput_t;

namespace AM {

std::vector<ActuatorOutput> FixedControl::runControlsAlgorithm(
    const AttitudeManagerInput &instructions) {
#warning Unimplemented
    // get attitude
    // interpret instructions
    // calculate pids
    // mix pids
void FixedControl::runControlsAlgo(const AttitudeManagerInput &instructions,
                                   float outputs[],
                                   uint8_t outputsLength) const {
   
   // Get current attitude from sensorfusion 
    SFOutput_t currentAttitude; // TODO: This needs to be retrieved from LOS
    
    // Compute target values
    float targetHeading = currentAttitude.heading - instructions.heading;
    float targetAltitude = instructions.z_dir; 
    float targetPitch = instructions.x_dir * maxPitchAngle;
    float targetBank = instructions.y_dir * maxBankAngle;
     
    // Adjust heading difference for the PID
    if (targetHeading <= (-180)) {
        targetHeading += 180;
    } else if (targetHeading > 180){
        targetHeading -=360;
    }
   
    // Define PID values for mixing
    PIDController pid_bank{bank_kp,        bank_ki,      bank_kd, 
                           bank_i_windup, -maxBankAngle, maxBankAngle };
    PIDController pid_rudder{rudder_kp,       rudder_ki, rudder_kd, 
                             rudder_i_windup, -100,      100};
    PIDController pid_pitch{pitch_kp,       pitch_ki,       pitch_kd, 
                            pitch_i_windup, -maxPitchAngle, maxPitchAngle};
    PIDController pid_airspeed{airspeed_kp,       airspeed_ki,  airspeed_kd, 
                               airspeed_i_windup, airspeed_min, airspeed_max};                        

    // Compute values for mixing
    float bank = pid_bank.execute(targetBank, currentAttitude.roll, currentAttitude.rollRate);

    float pitch = pid_pitch.execute(targetPitch, currentAttitude.pitch, currentAttitude.pitchRate);

    float rudderSet = rudderPercent(bank);
    
    rudderSet = pid_rudder.execute(0.0f, targetHeading);

    float airspeed = pid_airspeed.execute(targetAltitude, currentAttitude.altitude, // handling altitude with airspeed? Define altitude and airspeed seperately?
                                          currentAttitude.rateOfClimb);
    
    float roll = DEG_TO_RAD(bank);

    // mix the PID's
    float engineOutput = 
        mixPIDs(configs[Engine].stateMix, roll, pitch, rudderSet, airspeed); 
    float leftAileronOutput = 
        mixPIDs(configs[LeftAileron].stateMix, roll, pitch, rudderSet, airspeed);
    float rightAileronOutput = 
        mixPIDs(configs[RightAileron].stateMix, roll, pitch, rudderSet, airspeed);
    float rudder = 
        mixPIDs(configs[Rudder].stateMix, roll, pitch, rudderSet, airspeed);
    float elevator = 
        mixPIDs(configs[Elevator].stateMix, roll, pitch, rudderSet, airspeed);
    
    float engineOutput, leftAileronOutput, rightAileronOutput, rudder, elevator;

    // Verify values
    assert(configs[Engine].channel < outputsLength); // Question: Engine controlled as some percentage? Indicative of airspeed?
    outputs[configs[Engine].channel] = engineOutput;

    assert(configs[LeftAileron].channel < outputsLength); 
    outputs[configs[LeftAileron].channel] = leftAileronOutput;
        
    assert(configs[RightAileron].channel < outputsLength);
    outputs[configs[RightAileron].channel] = rightAileronOutput;
        
    assert(configs[Rudder].channel < outputsLength);
    outputs[configs[Rudder].channel] = rudder;
        
    assert(configs[Elevator].channel < outputsLength);
    outputs[configs[Elevator].channel] = elevator;

}

float FixedControl::mixPIDs(StateMix actuator, float roll, float pitch, float yaw,
                  float altitude) const {
    return constrain<float>(actuator.pitch * pitch + actuator.roll * roll +
                                actuator.yaw * yaw +
                                actuator.velocity_z * altitude,
                            100, 0);
}

float FixedControl::rudderPercent(float bankAngle) const {
    return((rudder_scaling_factor * bankAngle) / (ZP_PI / 2.0)) * 100.0; // "very simple for now. Experiments may give us a better formula. The PID will fix any discrepancy though"
}

} // namespace AM
}  // namespace AM
