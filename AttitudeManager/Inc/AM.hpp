/*
 * AM.hpp
 *
 * Attitude Manager Header
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */
#ifndef ZPSW3_AM_HPP
#define ZPSW3_AM_HPP

// #include "LOS_Actuators.hpp"
#include "AM_ControlInterface.hpp"

namespace AM {


class AttitudeManager {
public:
    template<class ...Controller, class ...Actuator>
    AttitudeManager(const Controller &...controllers, 
                    const Actuator &...actuators) :
        controllers{controllers...},
        controllersLen(sizeof...(controllers)),
        actuators{actuators...},
        actuatorsLen(sizeof...(actuators)),
        outputsLen(actuatorsLen)
         {};

    void runControlLoopIteration(AttitudeManagerInput instructions);
private:
    void setup();
    static const int controllersLen;
    const ControlInterface* controllers[];

    static const int actuatorsLen;
    const ActuatorConfig actuators[];

    // Could be optimized for cases where a single actuator is used for 
    // multiple control algorithms
    static const int outputsLen; 
    // stored as [Controller][Output Channel]
    
};
} // namespace AM

#endif //ZPSW3_AM_HPP
