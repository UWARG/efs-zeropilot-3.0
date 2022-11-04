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

    std::vector<ActuatorOutput> runControlsAlgorithm(
        const AttitudeManagerInput &instructions) override;

    void updatePid() override { return; }

   private:
    void rudderPercent(float bankAngle);
    static float rudderPercent(float bankAngle);

    enum ActuatorIdx {
        Engine = 0,
        LeftAileron,
        RightAileron,
        Rudder,
        Elevator,
        NumActuatorIdx  // Must always be last
    };

    const ActuatorConfig configs[NumActuatorIdx];
    
    // confirm values before flight ??
    
    static constexpr float maxBankAngle = 20; // Max angle defined in degrees. See coordinated turns on confluence for more information.
    static constexpr float maxPitchAngle = 20; // Max angle defined in degrees. See coordinated turns on confluence for more infomration.

    static constexpr int max_i_windup = 1;

    static constexpr float bank_kp = 4.2;           
    static constexpr float bank_ki = 0.0;          
    static constexpr float bank_kd = 0.0;   
    static constexpr float bank_i_windup = 0.0; 

    static constexpr float rudder_kp = 0.2;          
    static constexpr float rudder_ki = 0.2;         
    static constexpr float rudder_kd = 7;  
    static constexpr float rudder_i_windup = 0.0;       

    static constexpr float pitch_kp = 1.0;            
    static constexpr float pitch_ki = 0.0;            
    static constexpr float pitch_kd = 0.0;
    static constexpr float pitch_i_windup = 0.0;

    static constexpr float rudder_scaling_factor = 0.5f; // should be experimentally determined
};
}  // namespace AM

#endif  // ZPSW3_AM_FIXED_CONTROL_HPP
