/*
 * AM_ControlInterface.hpp
 *
 * Attitude Manager Control Algorithm Interface
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers
 */
#ifndef ZPSW3_AM_CONTROL_INTERFACE_HPP
#define ZPSW3_AM_CONTROL_INTERFACE_HPP

#include "AM_DataTypes.hpp"

namespace AM {

class ControlInterface {
   public:
    ControlInterface(uint8_t num_actuators) : num_actuators(num_actuators){};

    /// @brief Run the controls algorithm for the given flight model.
    ///
    /// @param instructions
    ///   The instructions for the flight model to follow
    /// @param outputs
    ///   An array of actuator percentage throttles that the flight model
    ///   believes should be used before mixing to meet the instructions
    /// @param outputs_length
    ///   The length of the outputs array. This should be equivilent to the
    ///   number of actuator channels
    virtual std::vector<ActuatorOutput> runControlsAlgo(
        const AttitudeManagerInput &instructions) const = 0;

    virtual void updatePid() = 0;

    const uint8_t num_actuators;  //< Number of actuators for given flight model

   private:
    ControlInterface();
};
}  // namespace AM

#endif  // ZPSW3_AM_CONTROL_INTERFACE_HPP
