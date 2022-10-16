#pragma once

#include "PM_StateManager.hpp"
//#include "pathModeSelector.hpp"
#include "PM.hpp"
#include "PM_WaypointManager.hpp"
#include "PM_LandingTakeoffManager.hpp"
#include "PM_DataTypes.hpp"
#include "PM_CommsWithAM.hpp"
#include "PM_CommsWithTM.hpp"
#include "SensorFusion.hpp"
//#include "AttitudePathInterface.hpp"
//#include "gimbal.hpp"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

class CommsWithAttitude : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
       // static AttitudeData* GetCommWithAttitudeData(void) { return &_receivedData; }
    private:
        commsWithAttitude() { CommFromPMToAMInit(); } // Initializes module
        commsWithAttitude(const commsWithAttitude& other);
        commsWithAttitude& operator =(const commsWithAttitude& other);
        //static AttitudeData _receivedData;
};

class CommsWithTelemetry : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static Telemetry_PIGO_t* GetTelemetryIncomingData(void) {return &_incomingData;}
    private:
        commsWithTelemetry() {CommWithTelemInit();}
        commsWithTelemetry(const commsWithTelemetry& other);
        commsWithTelemetry& operator =(const commsWithTelemetry& other);
        static Telemetry_PIGO_t _incomingData; // Stores the commands sent by telemetry for easy access by other states in the pathmanager
};

class SensorFusion : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static SFOutput_t* GetSFOutput(void) { return &_sfOutputData; }
        static IMU_Data_t* GetIMUData(void) { return &_imudata; }
    private:
        sensorFusion() {}
        sensorFusion(const sensorFusion& other);
        sensorFusion& operator =(const sensorFusion& other);
        static SFOutput_t _sfOutputData;
        static IMU_Data_t _imudata;
        // If additional raw sensor data is needed, add the static struct here and implement
        // it like the IMU_Data_t struct above
};

class FlightModeSelector : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
    private:
        flightModeSelector() {}
        flightModeSelector(const flightModeSelector& other);
        flightModeSelector& operator =(const flightModeSelector& other);

};

/* TODO: Talk w Gordon about how we should structure takeoff/landing / what is needed */

class PreflightStage : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static WaypointManager preflightPath;
        bool getPreFlightCompleteStatus (void) {return preFlightComplete;}
        static WaypointStatus waypointStatus;
        static WaypointManager_Data_Out* GetOutputData(void) {return &waypointOutput;}
        static LandingTakeoffOutput* getControlOutput(){return &output;}

    private:
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        int cycleCount = 0; 
        static LandingTakeoffInput input;
        static LandingTakeoffOutput output;
        static WaypointManager_Data_Out waypointOutput;
        bool preFlightComplete; 
        preflightStage() {}
        preflightStage(const preflightStage& other);
        preflightStage& operator =(const preflightStage& other);

};

class TakeoffStage : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static WaypointManager takeOffPath;
        static WaypointStatus waypointStatus;
        static WaypointManager_Data_Out* GetOutputData(void) {return &waypointOutput;}
        static LandingTakeoffOutput* getControlOutput(){return &output;}
    private:
        static LandingTakeoffInput input;
        static LandingTakeoffOutput output;
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        static WaypointManager_Data_In waypointInput;
        static WaypointManager_Data_Out waypointOutput;
        takeoffStage() {}
        takeoffStage(const takeoffStage& other);
        takeoffStage& operator =(const takeoffStage& other);
};

class CruisingStage : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static WaypointManager_Data_Out* GetOutputData(void) {return &_outputdata;}
       // static _CruisingState_Telemetry_Return* GetErrorCodes(void) {return &_returnToGround;}

        #ifdef UNIT_TESTING
            WaypointManager* GetWaypointManager(void) {return &cruisingStateManager;}
            int* GetWaypointIdArray(void) {return waypointIDArray;}
        #endif
    private:
        cruisingState() {}
        cruisingState(const cruisingState& other);
        cruisingState& operator =(const cruisingState& other);

        WaypointManager cruisingStateManager;
        static WaypointManager_Data_In _inputdata;
        static WaypointManager_Data_Out _outputdata;
       // static _CruisingState_Telemetry_Return _returnToGround;
     
};

class LandingStage : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static WaypointManager_Data_Out* GetOutputData(void) {return &waypointOutput;}
        static LandingTakeoffOutput* getControlOutput(){return &output;}
        static WaypointManager landingPath;
        static WaypointStatus waypointStatus; //used to catch errors
        static LandingPath path; //used to load in path
    private:
        static LandingTakeoffInput input;
        static LandingTakeoffOutput output;
        static WaypointManager_Data_In waypointInput;
        static WaypointManager_Data_Out waypointOutput;
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        landingStage() {}
        landingStage(const landingStage& other);
        landingStage& operator =(const landingStage& other);

};




class FatalFailureMode : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
    private:
        fatalFailureMode() {}
        fatalFailureMode(const fatalFailureMode& other);
        fatalFailureMode& operator =(const fatalFailureMode& other);
};


