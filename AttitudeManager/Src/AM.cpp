//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"
#include "LOS_Actuators.hpp"
#include <array>

namespace AM {

void AttitudeManager::runControlLoopIteration(
    const AttitudeManagerInput &instructions) {
    // Process Instructions

    // Run Control Algorithms
    std::vector<ActuatorOutput> controller_output;
    if (current_controller_index == desired_controller_index) {
        controller_output = controller_interfaces[current_controller_index]
                                ->runControlsAlgorithm(instructions);
    } else {
        // Handle transition between flight modes
        controller_output = runTransitionMixingIteration(
            controller_interfaces[current_controller_index],
            controller_intfaces[desired_controller_index], instructions);
    }

    // Write actuator outputs
    LOS_Actuators outputs;
    for (auto output : controller_output) {
         outputs.set(output.channel, output.percent);
    }
}

void AttitudeManager::setDesiredControlAlgorithm(uint8_t id) {
    // Only change desired controller if we aren't trying to transition already
    if (id < controller_interfaces.size() &&
        desired_controller_index == current_controller_index) {
        desired_controller_index = id;

#warning This should be getting the current time
        transition_start_time_ms = 0;  // TODO: Get the current time from los
    }
}

std::vector<ActuatorOutput> AttitudeManager::runTransitionMixingIteration(
    ControlInterface *current_controller, ControlInterface *desired_controller,
    const AttitudeManagerInput &instructions) {
    // Run control algorithms to mix
    std::vector<ActuatorOutput> current_output =
        current_controller->runControlsAlgorithm(instructions);
    std::vector<ActuatorOutput> desired_output =
        desired_controller->runControlsAlgorithm(instructions);

#warning This should be getting the current time
    // Determine how much of each mix to apply
    const uint64_t current_time_ms = 0;  // TODO: Get the current time from los
    const float transition_percent =
        (transition_start_time_ms - current_time_ms) / transition_time_ms;
    const float inv_transition_percent = 1 - transition_percent;

    auto transition_end_time_ms = transition_start_time_ms + transition_time_ms;
    if (current_time_ms > transition_end_time_ms) {
        // Update the active controller
        current_controller_index = desired_controller_index;
    }

    // Mix the outputs
    std::vector<ActuatorOutput> mixed_output = {};
    for (auto current_actuator : current_output) {
        ActuatorOutput value = {
            current_actuator.channel,
            current_actuator.percent * inv_transition_percent};

        for (auto desired_actuator : desired_output) {
            if (current_actuator.channel == desired_actuator.channel) {
                value.percent += desired_actuator.percent * transition_percent;
            }
        }
        mixed_output.push_back(value);
    }

    return mixed_output;
}

}  // namespace AM
