#include "AM_LevelQuadControl.hpp"
#include "AM.hpp"

#include "AM_DataTypes.hpp"

#include "main.h"

int main() {
    // MUST INITIALIZE SYSTEM AND HAL

#warning This is unimplemented and shouldnt be run
    AM::ActuatorConfig m1{.channel = 0,
                          .stateMix = AM::StateMix(0, 0, 1, 1, 1, -1)};
    AM::ActuatorConfig m2{.channel = 0,
                          .stateMix = AM::StateMix(0, 0, 1, -1, -1, -1)};
    AM::ActuatorConfig m3{.channel = 0,
                          .stateMix = AM::StateMix(0, 0, 1, 1, -1, 1)};
    AM::ActuatorConfig m4{.channel = 0,
                          .stateMix = AM::StateMix(0, 0, 1, -1, 1, 1)};
    AM::LevelQuadControl quad1(m1, m2, m3, m4);
    AM::LevelQuadControl quad2(m4, m3, m2, m1);
    AM::AttitudeManager attitude_manager(&quad1, &quad2);

    // AM::std::vector<ControlInterface *> controllers =
    // attitude_manager.getControllers();

    while (1) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_Delay(1000);
    }

    return 0;
}
