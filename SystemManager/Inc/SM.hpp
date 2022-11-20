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
#include "SM_StateManager.hpp"
#include "../../AttitudeManager/Inc/AM_DataTypes.hpp"
#include "../../AttitudeManager/Inc/AM.hpp"
#include "../../LaminarOS/LOS_Interface/Inc/interface_datatypes.hpp"

class SystemState;

namespace SM {
    // Enumerates the current drone status
    enum Drone_Operation_Mode{BOOT, DISARMED, GROUND_OPS, TAKEOFF, FLIGHT, LANDING, FATAL_FAILURE = -1};

    // Enums for RSSI channel definitions
    enum RSSI_Channel{RC_THROTTLE_CHANNEL, RC_PITCH_CHANNEL, RC_ROLL_CHANNEL, RC_YAW_CHANNEL, RC_ARM_CHANNEL}; // Either reconfigure controller or this to match

    int AM_PERIOD_MS = 5; // TODO, figure out what these should be
    int PM_PERIOD_MS = 5; // 200Hz.
    int TM_PERIOD_SLOW_MS = 20; // 50Hz. All of these numbers should be decided.
    int TM_PERIOD_OPERATION_MS = 5;
}

class SystemManager {
    public:
        SystemManager();
        void execute();
        void setState(SystemState& newState);
    private:
        SystemState* currentState;
        SM::Drone_Operation_Mode operation_mode;
        SystemManager();

        // TODO PM, AM, TM instances here
        AttitudeManager& attitude_manager;
        // PathManager& attitude_manager;
        // TelemetryManager& attitude_manager;
        
        // Task Handles
        TaskHandle_t AM_handle = NULL;
        TaskHandle_t PM_handle = NULL;
        TaskHandle_t TM_handle = NULL;

        // Thread tasks here
        void AMOperationTask();
        void PMOperationTask();
        void TMOperationTask();
        void TMSlowTask();

        // Mail Queues here
        osMailQId TM_to_SM_queue;
        osMailQId SM_to_TM_queue;
        osMailQId SM_to_PM_queue;
        osMailQId PM_to_AM_queue;
        osMailQId SM_to_AM_queue;
        osMailQId AM_to_SM_queue;

        // LOS instance here
        LOS& los_interface = LOS::getInstance();

        // TODO Bulk message from telemetry stored here

        // TODO Message from RC here
        LosLinkRx_t rc_data;
        // TODO new_message flag here for RC or some other way to know if data is new

        // Data from SF
        LosSFData sf_data;
        
        // TODO Response from AM stored here to be merged and sent to Telemetry

        AttitudeManagerInputs AM_Waypoints;

        // Function to convert RC to AM input messages
        AttitudeManagerInputs RcToAmInput(LosLinkRx_t rc_message);
};

#endif //ZPSW3_SM_HPP
