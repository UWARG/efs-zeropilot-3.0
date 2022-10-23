//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"


namespace AM
{

void AttitudeManager::runControlLoopIteration(AttitudeManagerInput instructions) {
    // Process Instructions

    float controllerOutputs[numControllers][numActuatorChannels] = {0};
    // float actuatorOutputs[actuatorsLen] = {};
    
    // Run Control Algorithms
    for (uint8_t controllerIdx = 0; controllerIdx < numControllers; controllerIdx++) {
        const ControlInterface* controller = controllers[controllerIdx];
        controller->runControlsAlgo(instructions, controllerOutputs[controllerIdx]);
    }

    // Mix actuator outputs

    // Write actuator outputs
    // for (uint8_t i = 0; i < actuatorsLen; i++) {
        //LosActuators.set(actuatorOutputs[i])
    // }
}

} // namespace AM
