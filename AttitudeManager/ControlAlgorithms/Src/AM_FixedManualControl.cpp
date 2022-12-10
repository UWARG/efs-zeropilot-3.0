#include "AM_FixedManualControl.hpp"

namespace AM {

std::vector<ActuatorOutput> ManualFixedControl::runControlsAlgorithm(
    const AttitudeManagerInput &instructions) {
    // Compute target values, measured as percent of maximum surface deflection
    float yaw = instructions.heading / 360 * 100;
    float throttle = instructions.z_dir * 100;
    float pitch = instructions.x_dir * 100;
    float bank = instructions.y_dir * 100;

    // mix the targets
    float engine_output = mixOutputs(configs[Engine].stateMix, bank, pitch, yaw, throttle);
    float left_aileron_output =
        mixOutputs(configs[LeftAileron].stateMix, bank, pitch, yaw, throttle);
    float right_aileron_output =
        mixOutputs(configs[RightAileron].stateMix, bank, pitch, yaw, throttle);
    float rudder = mixOutputs(configs[Rudder].stateMix, bank, pitch, yaw, throttle);
    float elevator = mixOutputs(configs[Elevator].stateMix, bank, pitch, yaw, throttle);

    return std::vector<ActuatorOutput>{{configs[Engine].channel, engine_output},
                                       {configs[LeftAileron].channel, left_aileron_output},
                                       {configs[RightAileron].channel, right_aileron_output},
                                       {configs[Rudder].channel, rudder},
                                       {configs[Elevator].channel, elevator}};
}

float ManualFixedControl::mixOutputs(StateMix actuator, float bank, float pitch, float yaw,
                                     float throttle) const {
    return constrain<float>(actuator.pitch * pitch + actuator.roll * bank + actuator.yaw * yaw +
                                actuator.velocity_x * throttle,
                            100, 0);
}

}  // namespace AM
