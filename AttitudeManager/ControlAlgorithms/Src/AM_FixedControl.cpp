#include "AM_FixedControl.hpp"
#include "PID.hpp"

 // #include math_constants if we plan to go this route
constexpr double ZP_PI = 3.14159265358979311599796346854; // more precise pi value

static float DEG_TO_RAD(float angleInDegrees); 

static float DEG_TO_RAD(float angleInDegrees) {
    return ((angleInDegrees) * ZP_PI / 180.0);
}

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
   
   // Get current attitude from sensorfusion (commented currently to avoid 
   //                                         cluttering in files)
   // SFOutput_t currentAttitude; // TODO: This needs to be retrieved from LOS
    
    // Compute target (target) values
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
   

    // replace bank and rudder with pitch roll and yaw ??
    PIDController pid_bank{bank_kp,        bank_ki,      bank_kd, 
                           bank_i_windup, -maxBankAngle, maxBankAngle };
    PIDController pid_rudder{rudder_kp,       rudder_ki, rudder_kd, 
                             rudder_i_windup, -100,      100};
    PIDController pid_pitch{pitch_kp,       pitch_ki,       pitch_kd, 
                            pitch_i_windup, -maxPitchAngle, maxPitchAngle};
    PIDController pid_airspeed{airspeed_kp,       airspeed_ki,  airspeed_kd, 
                               airspeed_i_windup, airspeed_min, airspeed_max};                        

    // do something
    float bank = pid_bank.execute(targetBank, currentAttitude.roll, currentAttitude.rollRate);

    float pitch = pid_pitch.execute(targetPitch, currentAttitude.pitch, currentAttitude.pitchRate);

    float rudder = rudderPercent(bank);
    
    rudder = pid_rudder.execute(0.0f, targetHeading);

    float airspeed = pid_airspeed.execute(targetAltitude, currentAttitude.altitude);
    
    float targetRoll = DEG_TO_RAD(bank);

    // mix the PID's ?
    float engineOutput = 
        mixPIDs(configs[Engine].stateMix, bank, pitch, rudder, altitude);
    float engineOutput = 
        mixPIDs(configs[LeftAileron].stateMix, bank, pitch, rudder, altitude);
    float engineOutput = 
        mixPIDs(configs[RightAileron].stateMix, bank, pitch, rudder, altitude);
    float engineOutput = 
        mixPIDs(configs[Rudder].stateMix, bank, pitch, rudder, altitude);
    float engineOutput = 
        mixPIDs(configs[Elevator].stateMix, bank, pitch, rudder, altitude);
    // what should our actuator outputs be ?
    
    float engineOutput, leftAileronOutput, rightAileronOutput, rudder, elevator;

    // return output
    assert(configs[Engine].channel < outputsLength); // I assume engine is airspeed ?
    outputs[configs[Engine].channel] = engineOutput;

    assert(configs[LeftAileron].channel < outputsLength); 
    outputs[configs[LeftAileron].channel] = leftAileronOutput;
        
    assert(configs[RightAileron].channel < outputsLength);
    outputs[configs[RightAileron].channel] = rightAileronOutput;
        
    assert(configs[Rudder].channel < outputsLength);
    outputs[configs[Rudder].channel] = rudder;
        
    assert(configs[Elevator].channel < outputsLength);
    outputs[configs[Elevator].channel] = elevator;

    // ActuatorOutput actuator;
    // assert(actuator.channel < outputsLength);
    // outputs[actuator.channel] = actuator.percent;
}
// Roll and yaw get combined into bank? Thoughts?
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
