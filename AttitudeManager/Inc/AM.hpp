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

#include "AM_ControlInterface.hpp"
#include "AM_DataTypes.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

class AttitudeManager {
   public:
    AttitudeManager(ControlInterfaceList controller_interfaces)
        : controller_interfaces(controller_interfaces){};

    // https://en.cppreference.com/w/cpp/language/parameter_pack
    template <typename... Args>
    AttitudeManager(Args... controllers)
        : controller_interfaces{controllers...} {};

    void runControlLoopIteration(const AttitudeManagerInput &instructions);

   private:
    AttitudeManager();

    enum ControllerIdx {
        CURRENT_CONTROLLER_IDX = 0, 
        DESIRED_CONTROLLER_IDX,
    };

    const ControlInterfaceList controller_interfaces;
    SFOutput_t current;
    float desired_airspeed = 0; // could this be determined by our desired controller index?
    float current_airspeed = 0;
    float transition_start_airspeed = 0;
    
    void setDesiredControlAlgorithm(uint8_t id);

    std::vector<ActuatorOutput> runTransitionMixingIteration(
        ControlInterface *current, ControlInterface *desired,
        const AttitudeManagerInput &instructions);
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
