/*
 * SM.hpp
 *
 * System Manager Header
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#ifndef ZPSW3_SM_HPP
#define ZPSW3_SM_HPP

#include <stdint.h>

#include "cmsis_os.h"
// #include "LOS_Link.hpp"
// #include "LOS_Actuators.hpp"
#include "AM.hpp"
#include "AM_LevelQuadControl.hpp"
#include "SM_StateManager.hpp"
#include "interface_datatypes.hpp"

namespace SM {

// Enumerates the current drone status
enum Drone_Operation_Mode {
    BOOT,
    DISARMED,
    GROUND_OPS,
    TAKEOFF,
    FLIGHT,
    LANDING,
    FATAL_FAILURE = -1
};

// Enums for RSSI channel definitions
enum RC_Controller_Channel {
    RC_THROTTLE_CHANNEL,
    RC_PITCH_CHANNEL,
    RC_ROLL_CHANNEL,
    RC_YAW_CHANNEL,
    RC_ARM_CHANNEL
};  // Either reconfigure controller or this to match

uint32_t AM_PERIOD_MS = 5;  // Current operation speed of 200 Hz.
// int PM_PERIOD_MS = 5; // 200Hz.
// int TM_PERIOD_SLOW_MS = 20; // 50Hz. All of these numbers should be decided.
// int TM_PERIOD_OPERATION_MS = 5;

AM::AttitudeManagerInput RcToAmInput(LosLinkRx_t rc_message);

class SystemState;

class SystemManager {
    public:
        SystemManager();
        void execute();
        void setState(SystemState& newState);
        Drone_Operation_Mode getMode();
        
        // Task Handles
        TaskHandle_t AM_handle = NULL;
        // TaskHandle_t PM_handle = NULL;
        // TaskHandle_t TM_handle = NULL;

        // Thread tasks here
        static void AMOperationTask(void *pvParameters);
        // static void PMOperationTask();
        // static void TMOperationTask();
        // static void TMSlowTask();

        // Mail Queues here
        // osMessageQId TM_to_SM_queue;
        // osMessageQId SM_to_TM_queue;
        // osMessageQId SM_to_PM_queue;
        // osMessageQId PM_to_AM_queue;
        osMessageQId SM_to_AM_queue;
        // osMessageQId AM_to_SM_queue;

        // TODO Bulk message from telemetry stored here

        // TODO Message from RC here
        LosLinkRx_t rc_data;
        // TODO new_message flag here for RC or some other way to know if data is new

        // Data from SF
        // LosSFData sf_data;

        // Message sent to AM
        // AM::AttitudeManagerInput to_am_data;
        
        // TODO Response from AM stored here to be merged and sent to Telemetry

        // AM init, might work better in a config file later
        AM::ActuatorConfig back_left_motor, front_right_motor, back_right_motor, front_left_motor;
        AM::LevelQuadControl quad;
        AM::AttitudeManager attitude_manager;

    private:
        SystemState* currentState;
        Drone_Operation_Mode operation_mode;

};

class SystemState {
   public:
    virtual void enter(SystemManager *sys_man);
    virtual void execute(SystemManager *sys_man);
    virtual void exit(SystemManager *sys_man);
    static SystemState &getInstance();
};

}  // namespace SM

#endif  // ZPSW3_SM_HPP
