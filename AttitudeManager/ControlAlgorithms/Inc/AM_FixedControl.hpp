/*
 * AM_LevelQuadControl.hpp
 *
 * Attitude Manager Level Mode Quad Controller Algorithm
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers
 */
#ifndef ZPSW3_AM_FIXED_CONTROL_HPP
#define ZPSW3_AM_FIXED_CONTROL_HPP

#include "AM_ControlInterface.hpp"

namespace AM {

class FixedControl : public ControlInterface {
   public:
    FixedControl(ActuatorConfig &engine, ActuatorConfig &leftAileron,
                 ActuatorConfig &rightAileron, ActuatorConfig &rudder,
                 ActuatorConfig &elevator)
        : ControlInterface(NumActuatorIdx),
          configs{engine, leftAileron, rightAileron, rudder, elevator} {};

    std::vector<ActuatorOutput> runControlsAlgo(
        const AttitudeManagerInput &instructions) const override;

    void updatePid() override { return; }

   private:
    enum ActuatorIdx {
        Engine = 0,
        LeftAileron,
        RightAileron,
        Rudder,
        Elevator,
        NumActuatorIdx  // Must always be last
    };

    const ActuatorConfig configs[NumActuatorIdx];
};
}  // namespace AM

#endif  // ZPSW3_AM_FIXED_CONTROL_HPP
