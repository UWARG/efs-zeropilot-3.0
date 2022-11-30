/*
 * SM_States.hpp
 *
 * System Manager State-Machine header file
 * 
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 *
 * Describes the states of SM:
 * 
 * Boot (executes only once) -> Disarm -> Ground Ops -> Flight
 * 
 * Also holds fatalFailure to shut down all other systems.
 *
 */

#ifndef ZPSW3_SM_STATES_HPP
#define ZPSW3_SM_STATES_HPP

#include "SM_StateManager.hpp"
#include "SM_DataTypes.hpp"
#include "cmsis_os.h"


class BootMode: public SystemState {
    public:
        void enter(SystemManager *sys_man) {
            (void) sys_man;
        }
        void execute(SystemManager *sys_man);
        void exit(SystemManager *sys_man) {
            (void) sys_man;
        }
        static SystemState& getInstance();
    private:
        BootMode() {
        }
        BootMode(const BootMode &other);
        BootMode& operator =(const BootMode &other);
};

class DisarmMode: public SystemState {
    public:
        void enter(SystemManager *sys_man) {
            (void) sys_man;
        }
        void execute(SystemManager *sys_man);
        void exit(SystemManager *sys_man) {
            (void) sys_man;
        }
        static SystemState& getInstance();
    private:
        DisarmMode() {
        }
        DisarmMode(const DisarmMode &other);
        DisarmMode& operator =(const DisarmMode &other);
};

class GroundOpMode: public SystemState {
    public:
        void enter(SystemManager *sys_man) {
            (void) sys_man;
        }
        void execute(SystemManager *sys_man);
        void exit(SystemManager *sys_man) {
            (void) sys_man;
        }
        static SystemState& getInstance();
    private:
        GroundOpMode() {
        }
        GroundOpMode(const GroundOpMode &other);
        GroundOpMode& operator =(const GroundOpMode &other);
};

class FlightMode: public SystemState {
    public:
        void enter(SystemManager *sys_man) {
            (void) sys_man;
        }
        void execute(SystemManager *sys_man);
        void exit(SystemManager *sys_man) {
            (void) sys_man;
        }
        static SystemState& getInstance();
    private:
        FlightMode() {
        }
        FlightMode(const FlightMode &other);
        FlightMode& operator =(const FlightMode &other);

        AttitudeManagerInput* FlightMode::RcToAmInput(LosLinkRx_t rc_message);
        // static AttitudeManagerInput* getManualWaypoint();
        // static CommandsFromSM* generatePmPacket();
};

class FatalFailureMode: public SystemState {
    public:
        void enter(SystemManager *sys_man) {
            (void) sys_man;
        }
        void execute(SystemManager *sys_man);
        void exit(SystemManager *sys_man) {
            (void) sys_man;
        }
        static SystemState& getInstance();
    private:
        FatalFailureMode() {
        }
        FatalFailureMode(const FatalFailureMode &other);
        FatalFailureMode& operator =(const FatalFailureMode &other);
};

#endif //ZPSW3_SM_STATES_HPP
