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
    AttitudeManager(const ControlInterface *controllers[],
                    uint8_t numControllers, const uint8_t actuatorChannelMap[],
                    uint8_t numActuatorChannels)
        : numControllers(numControllers),
          numActuatorChannels(numActuatorChannels), controllers(controllers),
          actuatorChannelMap(actuatorChannelMap){};

    void runControlLoopIteration(AttitudeManagerInput instructions);

  private:
    const int numControllers = 0;
    const int numActuatorChannels = 0;

    const ControlInterface **controllers;
    const uint8_t *actuatorChannelMap;
};
} // namespace AM

#endif // ZPSW3_AM_HPP
