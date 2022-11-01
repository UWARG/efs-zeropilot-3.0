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
    AttitudeManager(const ControlInterface *controller_interfaces[],
                    uint8_t num_atti_controllers, const uint8_t actuator_channel_map[],
                    uint8_t num_actuator_channels)
        : num_controllers(num_controllers),
          num_actuator_channels(num_actuator_channels), controller_interfaces(controller_interfaces),
          actuator_channel_map(actuator_channel_map){};

    void runControlLoopIteration(AttitudeManagerInput instructions);

  private:
    const int num_controllers = 0;
    const int num_actuator_channels = 0;

    const ControlInterface **controller_interfaces;
    const uint8_t *actuator_channel_map;
};
} // namespace AM

#endif // ZPSW3_AM_HPP
