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

    template <typename... Args>
    AttitudeManager(Args... controllers)
        : controller_interfaces{controllers...} {};

    void runControlLoopIteration(AttitudeManagerInput instructions);

    const int getCurrentControlId() { return controller_id; }
    void setDesiredControlAlgorithm(int id) { desired_controller = id; }

   private:
    AttitudeManager();
    uint8_t controller_id = 0;
    uint8_t desired_controller = 0;
    const ControlInterfaceList controller_interfaces;
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
