#include "AM_LevelQuadControl.hpp"
#include "PID.hpp"

// #include "LOS_SensorFusion.hpp" Whatever this is it will be needed
typedef struct {
    float roll, pitch, yaw;              // Degrees. Yaw of 180 is north.
    float rollRate, pitchRate, yawRate;  // Degrees/second
    float airspeed;                      // m/s
    float altitude;                      // m
    float rateOfClimb;                   // m/s
    long double latitude;                // Decimal degrees
    float latitudeSpeed;                 // m/s
    long double longitude;               // Decimal degrees
    float longitudeSpeed;                // m/s
    double track;                        // Degrees. Track of 0 is north.
    float groundSpeed;                   // m/s
    double heading;                      // Degrees. Heading of 0 is north.
} SFOutput_t;

namespace AM {

std::vector<ActuatorOutput> LevelQuadControl::runControlsAlgorithm(
    const AttitudeManagerInput &instructions) {
    // Get current attitude from sensorfusion
    // TODO: This needs to be retrieved from LOS
    SFOutput_t currentAttitude{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
