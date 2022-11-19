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
#include "AM_DataTypes.hpp"

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

    uint8_t current_controller_index = 0;
    uint8_t desired_controller_index = 0;
    const ControlInterfaceList controller_interfaces;
    static constexpr uint64_t transition_time_ms = 5000;
    static constexpr uint64_t desired_airspeed = 60; 
    uint64_t transition_start_time_ms = 0;
    uint64_t transition_start_airspeed = 0;

    void setDesiredControlAlgorithm(uint8_t id);

    std::vector<ActuatorOutput> runTransitionMixingIteration(
        ControlInterface *current, ControlInterface *desired,
        const AttitudeManagerInput &instructions);
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
