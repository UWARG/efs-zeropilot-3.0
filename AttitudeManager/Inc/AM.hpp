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

#include <vector>

namespace AM {

class AttitudeManager {
  public:
    AttitudeManager(std::vector<ControlInterface *> controller_interfaces)
        : controller_interfaces(controller_interfaces){};

    template <typename... Args>
    AttitudeManager(Args... controllers)
        : controller_interfaces{controllers...} {};

    void runControlLoopIteration(AttitudeManagerInput instructions);

  private:
    const std::vector<ControlInterface *> controller_interfaces;
};
} // namespace AM

#endif // ZPSW3_AM_HPP
