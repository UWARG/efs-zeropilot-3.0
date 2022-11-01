//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

namespace AM {

void AttitudeManager::runControlLoopIteration(
    AttitudeManagerInput instructions) {
    // Process Instructions

    if (num_controllers < 1 || num_actuator_channels < 1) {
        float controller_outputs[num_controllers][num_actuator_channels] = {0};
        // float actuatorOutputs[actuatorsLen] = {};

        // Run Control Algorithms
        for (uint8_t controller_idx = 0; controller_idx < num_controllers;
             ++controller_idx) {
            const ControlInterface *controller_interface = controller_interfaces[controller_idx];
            controller_interface->runControlsAlgo(instructions,
                                        controller_outputs[controller_idx],
                                        num_actuator_channels);
        }

        // Mix actuator outputs

        // Write actuator outputs
        // for (uint8_t i = 0; i < actuatorsLen; i++) {
        // LosActuators.set(actuatorOutputs[i])
        // }
    }
}

} // namespace AM
