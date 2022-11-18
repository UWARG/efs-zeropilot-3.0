#include "AM_FixedControl.hpp"
#include "PID.hpp"
#include "MathConstants.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

std::vector<ActuatorOutput> FixedControl::runControlsAlgorithm(
    const AttitudeManagerInput &instructions) {
#warning Unimplemented
   
    SFOutput_t current_attitude; 
    
    // Compute target values
    float target_heading = instructions.heading;
    float target_throttle = instructions.speed; 
    float target_pitch = instructions.x_dir * MAX_PITCH_ANGLE;
    float target_bank = instructions.y_dir * MAX_BANK_ANGLE;
     
    // Adjust heading difference for the PID. Can be reworked 
    // if (targetHeading <= (-180)) {
    //     targetHeading += 180;
    // } else if (targetHeading > 180){
    //     targetHeading -=360;
    // }                                      

    float bank = pid_bank.execute(target_bank, current_attitude.roll, current_attitude.rollRate);

    float pitch = pid_pitch.execute(target_pitch, current_attitude.pitch, current_attitude.pitchRate);

    float yaw = pid_rudder.execute(target_heading, current_attitude.heading, current_attitude.yawRate);

    float throttle = pid_airspeed.execute(target_throttle, current_attitude.airspeed);

    // mix the PID's 
    float engine_output = 
        mixPIDs(configs[Engine].stateMix, bank, pitch, yaw, throttle);
    float left_aileron_output = 
        mixPIDs(configs[LeftAileron].stateMix, bank, pitch, yaw, throttle);
    float right_aileron_output = 
        mixPIDs(configs[RightAileron].stateMix, bank, pitch, yaw, throttle);
    float rudder = 
        mixPIDs(configs[Rudder].stateMix, bank, pitch, yaw, throttle);
    float elevator = 
        mixPIDs(configs[Elevator].stateMix, bank, pitch, yaw, throttle);
    

    return std::vector<ActuatorOutput> {
        {configs[Engine].channel, engine_output},
        {configs[LeftAileron].channel, left_aileron_output},
        {configs[RightAileron].channel, right_aileron_output},
        {configs[Rudder].channel, rudder},
        {configs[Elevator].channel, elevator}
    };
}
float FixedControl::mixPIDs(StateMix actuator, float bank, float pitch, float yaw,
                  float throttle) const {
    return constrain<float>(actuator.pitch * pitch + actuator.roll * bank +
                                actuator.yaw * yaw +
                                actuator.velocity_x * throttle,
                            100, 0);

}

} // namespace AM
