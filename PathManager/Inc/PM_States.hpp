#ifndef ZPSW3_PM_STATES_HPP
#define ZPSW_PM_STATES_HPP

#include "PM_StateManager.hpp"
//#include "PM.hpp"
#include "PM_WaypointManager.hpp"
#include "PM_LandingTakeoffManager.hpp"
#include "PM_DataTypes.hpp"
#include "PM_CommsWithAM.hpp"
#include "PM_CommsWithSM.hpp"


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
        static PM_AM_Commands* GetCommFromAttitude(void) { return &receivedData; }
    private:
        CommsWithAttitude() { CommFromPMToAMInit(); } // Initializes module
        CommsWithAttitude(const CommsWithAttitude& other);
        CommsWithAttitude& operator =(const CommsWithAttitude& other);
        static PM_AM_Commands receivedData;
};

class CommsWithSystemManager : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static CommandsFromSM* GetSMIncomingData(void) {return &incomingData;}
    private:
        static CommandsFromSM incomingData; 
        CommsWithSystemManager() { CommWithSMInit(); }
        CommsWithSystemManager(const CommsWithSystemManager& other);
        CommsWithSystemManager& operator =(const CommsWithSystemManager& other);
        static CommandsFromSM incomingData; // Stores the commands sent by telemetry for easy access by other states in the pathmanager
};


class FlightModeSelector : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
    private:
        FlightModeSelector() {}
        FlightModeSelector(const FlightModeSelector& other);
        FlightModeSelector& operator =(const FlightModeSelector& other);

};

class TakeoffStage : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
        static PM_AM_Commands* getTakeoffDataForAM() {return &takeoffDataForAM;} 
    private:
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        //static WaypointManager_Data_In waypointInput;
        //static WaypointManager_Data_Out waypointOutput;
        static LosSFData LOSData; 
        static PM_AM_Commands takeoffDataForAM; 
        TakeoffStage() {}
        TakeoffStage(const TakeoffStage& other);
        TakeoffStage& operator =(const TakeoffStage& other);
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
        CruisingStage() {}
        CruisingStage(const CruisingStage& other);
        CruisingStage& operator =(const CruisingStage& other);

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
        static PM_AM_Commands* getLandingDataForAM() {return &landingDataForAM;} 
    private:
        //static WaypointManager_Data_In waypointInput;
        //static WaypointManager_Data_Out waypointOutput;
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        static LosSFData LOSData; 
        static PM_AM_Commands landingDataForAM;
        LandingStage() {}
        LandingStage(const LandingStage& other);
        LandingStage& operator =(const LandingStage& other);

};




class FatalFailureMode : public pathManagerState
{
    public:
        void enter(pathManager* pathMgr) {(void) pathMgr;}
        void execute(pathManager* pathMgr);
        void exit(pathManager* pathMgr) {(void) pathMgr;}
        static pathManagerState& getInstance();
    private:
        FatalFailureMode() {}
        FatalFailureMode(const FatalFailureMode& other);
        FatalFailureMode& operator =(const FatalFailureMode& other);
};
#endif

