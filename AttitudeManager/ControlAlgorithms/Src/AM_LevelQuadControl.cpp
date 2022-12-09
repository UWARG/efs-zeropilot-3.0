#include "AM_LevelQuadControl.hpp"
#include "PID.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

std::vector<ActuatorOutput> LevelQuadControl::runControlsAlgorithm(
    const AttitudeManagerInput &instructions) {
    
    // Get current attitude from sensorfusion
    SFOutput_t currentAttitude {};

    // convert instructions into level mode quad instructions
    float targetPitch = instructions.x_dir * max_pitch_deg;
    float targetRoll = instructions.y_dir * max_roll_deg;
    float targetYaw = currentAttitude.yaw + instructions.heading;
    float targetAltitude = instructions.z_dir;

    // get PID result
    float pitch = pid_pitch.execute(targetPitch, currentAttitude.pitch,
                                    currentAttitude.pitchRate);
    float roll = pid_roll.execute(targetRoll, currentAttitude.roll,
                                  currentAttitude.rollRate);
    float yaw = pid_yaw.execute(targetYaw, currentAttitude.yawRate);
    float velocity_z =
        pid_z.execute(targetAltitude, currentAttitude.rateOfClimb);

    // mix the PID's.
    float frontRightOutput =
        mixPIDs(configs[FrontRight].stateMix, roll, pitch, yaw, velocity_z);
    float frontLeftOutput =
        mixPIDs(configs[FrontLeft].stateMix, roll, pitch, yaw, velocity_z);
    float backLeftOutput =
        mixPIDs(configs[BackLeft].stateMix, roll, pitch, yaw, velocity_z);
    float backRightOutput =
        mixPIDs(configs[BackRight].stateMix, roll, pitch, yaw, velocity_z);

    // return output
    return std::vector<ActuatorOutput>{
        {configs[FrontRight].channel, frontRightOutput},
        {configs[FrontLeft].channel, frontLeftOutput},
        {configs[BackRight].channel, backRightOutput},
        {configs[BackLeft].channel, backLeftOutput}};
}

float LevelQuadControl::mixPIDs(StateMix actuator, float roll, float pitch,
                                float yaw, float velocity_z) const {
    return constrain<float, 100, 0>(
        ((actuator.pitch * pitch) + (actuator.roll * roll) +
         (actuator.yaw * yaw) + (actuator.velocity_z * velocity_z)));
}

}  // namespace AM
