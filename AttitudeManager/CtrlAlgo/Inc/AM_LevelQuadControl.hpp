/*
 * AM_LevelQuadControl.hpp
 *
 * Attitude Manager Level Mode Quad Controller Algorithm
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers
 */
#ifndef ZPSW3_AM_LEVEL_QUAD_CONTROL_HPP
#define ZPSW3_AM_LEVEL_QUAD_CONTROL_HPP

#include "AM_ControlInterface.hpp"

namespace AM {

class LevelQuadControl : public ControlInterface {
public:
    LevelQuadControl(ActuatorConfig &frontRight,
                     ActuatorConfig &frontLeft,
                     ActuatorConfig &backRight,
                     ActuatorConfig &backLeft) :
        ControlInterface(NumActuatorIdx),
        configs{frontRight, frontLeft, backRight, backLeft} {};
    
    void runControlsAlgo(const AttitudeManagerInput &instructions,
                         float outputs[]) const;
    enum ActuatorIdx {
        FrontRight = 0,
        FrontLeft,
        BackRight,
        BackLeft,
        NumActuatorIdx // Must always be last
    };

private:
    ActuatorConfig configs[NumActuatorIdx];
    ActuatorOutput outputs[NumActuatorIdx];    
};
} // namespace AM

#endif //ZPSW3_AM_LEVEL_QUAD_CONTROL_HPP
