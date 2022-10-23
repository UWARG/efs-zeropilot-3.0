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
    ControlInterface(uint8_t numActuators) : numActuators(numActuators) {};
    virtual void runControlsAlgo(const AttitudeManagerInput &instructions,
                                 float outputs[]) const = 0;
    const uint8_t numActuators;
private:
    ControlInterface();
};
} // namespace AM

#endif //ZPSW3_AM_CONTROL_INTERFACE_HPP
