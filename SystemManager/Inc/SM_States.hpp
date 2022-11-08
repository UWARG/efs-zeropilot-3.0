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
 * boot (executes only once) ->
 * readTelemetry -> readLosSensors -> 
 * generateFlightPlan (writePathManager OR throttleToAttitude) ->
 * writeAttitudeManager -> writeTelemetryManager -> repeat 
 * Also holds fatalFailure to shut down all other systems.
 *
 */

#ifndef ZPSW3_SM_STATES_HPP
#define ZPSW3_SM_STATES_HPP

#include "SM_StateManager.hpp"
#include "SM_DataTypes.hpp"


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

class ReadTelemetryMode: public SystemState {
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
    ReadTelemetryMode() {
    }
    ReadTelemetryMode(const ReadTelemetryMode &other);
    ReadTelemetryMode& operator =(const ReadTelemetryMode &other);

    static enum _System_Manager_Drone_Operation_Mode getOperationMode();

    // static bool receieveArmDisarmInstruction(SystemManager *sys_man);
    // static bool isArmed();
};

class ReadLosSensorsMode: public SystemState {
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
    ReadLosSensorsMode() {
    }
    ReadLosSensorsMode(const ReadLosSensorsMode &other);
    ReadLosSensorsMode& operator =(const ReadLosSensorsMode &other);

};

class GenerateFlightPlanMode: public SystemState {
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
    GenerateFlightPlanMode() {
    }
    GenerateFlightPlanMode(const GenerateFlightPlanMode &other);
    GenerateFlightPlanMode& operator =(const GenerateFlightPlanMode &other);

    static inputs_to_AM_t* throttleToWaypoint(); // Get RSSI info from bulk telemetry message stored
    static inputs_to_AM_t* getPathManagerWaypoint();
};

class WriteAttitudeManagerMode: public SystemState {
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
    WriteAttitudeManagerMode() {
    }
    WriteAttitudeManagerMode(const WriteAttitudeManagerMode &other);
    WriteAttitudeManagerMode& operator =(const WriteAttitudeManagerMode &other);

};

class WriteTelemetryManagerMode: public SystemState {
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
    WriteTelemetryManagerMode() {
    }
    WriteTelemetryManagerMode(const WriteTelemetryManagerMode &other);
    WriteTelemetryManagerMode& operator =(const WriteTelemetryManagerMode &other);

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
