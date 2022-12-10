/*
 * AM_FixedControl.hpp
 *
 * Attitude Manager Level Mode Quad Controller Algorithm
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers, Jack Greenwood
 */
#ifndef ZPSW3_AM_ACRO_FIXED_CONTROL_HPP
#define ZPSW3_AM_ACRO_FIXED_CONTROL_HPP

#include "AM_FixedControl.hpp"
#include "PID.hpp"

namespace AM {

class ManualFixedControl : public FixedControl {
   public:
    ManualFixedControl(ActuatorConfig &engine, ActuatorConfig &leftAileron,
                       ActuatorConfig &rightAileron, ActuatorConfig &rudder,
                       ActuatorConfig &elevator)
        : FixedControl(engine, leftAileron, rightAileron, rudder, elevator){};

    std::vector<ActuatorOutput> runControlsAlgorithm(
        const AttitudeManagerInput &instructions) override;

   private:
    float mixOutputs(StateMix actuator, float bank, float pitch, float yaw, float throttle) const;
};
}  // namespace AM

#endif  // ZPSW3_AM_ACRO_FIXED_CONTROL_HPP
