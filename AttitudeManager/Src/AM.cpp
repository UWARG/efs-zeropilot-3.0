//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"


namespace AM
{

void AttitudeManager::runControlLoopIteration(AttitudeManagerInput instructions) {
    // Process Instructions

    // Run Control Algorithms
    float controllerOutputs[controllersLen][outputsLen] = {{0}};
    float actuatorOutputs[] = {0};
    
    for (uint8_t controllerIdx = 0; controllerIdx < controllersLen; controllerIdx++) {
        ControlInterface* controller = controllers[i];
        controller->runControlsAlgo(instructions, controllerOutputs[controllerIdx]);
        
        // get the computed actuators
        for (uint8_t actuator = 0; actuator < controller->numActuators; actuator++) {
            ActuatorOutput output = controller->getActuatorOutput(actuator);
            controllerOutputs[controllerIdx][output.channel] = output.percent;
        }
    }

    // Mix actuator outputs

    // Write actuator outputs
    // for (uint8_t i = 0; i < actuatorsLen; i++) {
        //LosActuators.set(actuatorOutputs[i])
    // }
}

} // namespace AM
