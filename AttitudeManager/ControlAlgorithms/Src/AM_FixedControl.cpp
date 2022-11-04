#include "AM_FixedControl.hpp"
#include "PID.hpp"

 // #include math_constants if we plan to go this route
constexpr double ZP_PI = 3.14159265358979311599796346854; // more precise pi value

void DEG_TO_RAD(float angleInDegrees); 

void DEG_TO_RAD(float angleInDegrees) {
    ((angleInDegrees) * ZP_PI / 180.0);
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

   // fixed controls shoudl get commands detailed in confluence ? 
   
   // Get current attitude from sensorfusion (commented currently to avoid 
   //                                         cluttering in files)
   // SFOutput_t currentAttitude; // TODO: This needs to be retrieved from LOS
    
    // Compute the difference between our current and desired heading 
       
    float desiredHeading = currentAttitude.heading - instructions.heading;
    float desiredPitch = instructions.z_dir;
    float desiredAltitude = instructions.x_dir * maxPitchAngle;
    float desiredBank = instructions.y_dir * maxBankAngle;
     
    // Adjust heading difference for the PID
    if (desiredHeading <= (-180)) {
        desiredHeading += 180;
    } else if (desiredHeading > 180){
        desiredHeading -=360;
    }
   

    // replace bank and rudder with pitch roll and yaw ??
    PIDController pid_bank{bank_kp,        bank_ki,      bank_kd, 
                           bank_i_windup, -maxBankAngle, maxBankAngle };
    PIDController pid_rudder{rudder_kp,       rudder_ki, rudder_kd, 
                             rudder_i_windup, -100,      100};
    PIDController pid_pitch{pitch_kp,       pitch_ki,       pitch_kd, 
                            pitch_i_windup, -maxPitchAngle, maxPitchAngle};

    // do something
    float bankAngle = pid_bank.execute


    float desiredRoll =
    // return output
}

static float rudderPercent(float bankAngle) {
    return((rudder_scaling_factor*bankAngle) / (ZP_PI / 2.0)) * 100.0; // "very simple for now. Experiments may give us a better formula. The PID will fix any discrepancy though"
}

} // namespace AM
}  // namespace AM
