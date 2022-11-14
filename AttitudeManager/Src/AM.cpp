//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

namespace AM {

void AttitudeManager::runControlLoopIteration(
    AttitudeManagerInput instructions) {
    // Process Instructions

    // Run Control Algorithms
    for (auto controller : controller_interfaces) {
        std::vector<ActuatorOutput> output =
            controller->runControlsAlgorithm(instructions);
    }

    // Mix actuator outputs

    // Write actuator outputs
    // for (uint8_t i = 0; i < actuatorsLen; i++) {
    // LosActuators.set(actuatorOutputs[i])
    // }
}

}  // namespace AM
