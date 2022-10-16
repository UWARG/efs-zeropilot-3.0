#include "PM_States.hpp"

/***********************************************************************************************************************
 * Static Member Variable Declarations
 **********************************************************************************************************************/

Telemetry_PIGO_t CommsWithTelemetry::_incomingData;
SFOutput_t SensorFusion::_sfOutputData;
IMU_Data_t SensorFusion::_imudata;
CoordinatedTurnAttitudeManagerCommands_t coordinateTurnElevation::_rollandrudder;
AltitudeAirspeedCommands_t coordinateTurnElevation::_pitchandairspeed;
AttitudeData CommsWithAttitude::_receivedData;


/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

void CommsWithAttitude::execute(pathManager* pathMgr)
{
    /*

    bool newDataAvailable = GetFromAMToPM(&_receivedData); // Gets attitude manager data

    // Gets data used to populate CommandsForAM struct
    CoordinatedTurnAttitudeManagerCommands_t * turnCommands = coordinateTurnElevation::GetRollAndRudder();
    AltitudeAirspeedCommands_t * altCommands = coordinateTurnElevation::GetPitchAndAirspeed();

    _WaypointManager_Data_Out * waypointOutput {}; 
    
    //deciding which stage we get output data from 
    switch(pathMgr->stage){
        case LANDING:
            waypointOutput = LandingStage::GetOutputData();
            break;
        case CRUISING:
            waypointOutput = cruisingState::GetOutputData();
            break;
        case PREFLIGHT:
            waypointOutput = PreflightStage::GetOutputData();
            break;
        case TAKEOFF:
           waypointOutput = TakeoffStage::GetOutputData();
            break;
        default:
            waypointOutput = cruisingState::GetOutputData();
    }
    
    CommandsForAM toSend {};
    toSend.roll = turnCommands->requiredRoll;
    toSend.pitch = altCommands->requiredPitch;
    toSend.rudderPercent = turnCommands->requiredRudderPosition;
    toSend.throttlePercent = altCommands->requiredThrottlePercent;

    SendFromPMToAM(&toSend); // Sends commands to attitude manager
    */

    pathMgr->setState(CommsWithTelemetry::getInstance());
}

pathManagerState& CommsWithAttitude::getInstance()
{
    static CommsWithAttitude singleton;
    return singleton;
}

void CommsWithTelemetry::execute(pathManager* pathMgr)
{
    GetTelemetryCommands(&_incomingData);

    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(SensorFusion::getInstance());
    }
}

pathManagerState& CommsWithTelemetry::getInstance()
{
    static CommsWithTelemetry singleton;
    return singleton;
}

void SensorFusion::execute(pathManager* pathMgr)
{
    SFError_t error = SF_GetResult(&_sfOutputData); // Gets current Sensor fusion output struct
    _imudata = SF_GetRawIMU();

    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(modeExecutor::getInstance());
    }
}

pathManagerState& SensorFusion::getInstance()
{
    static SensorFusion singleton;
    return singleton;
}

#ifdef UNIT_TESTING

#include "cruisingMode.hpp"

static Telemetry_Waypoint_Data_t createTelemetryWaypoint(long double lon, long double lat, int alt, float turnRadius, int type) {
    Telemetry_Waypoint_Data_t waypoint {};

    waypoint.longitude = lon;
    waypoint.latitude = lat;
    waypoint.altitude = alt;
    waypoint.turnRadius = turnRadius;
    waypoint.waypointType = type;

    return waypoint;
}

#endif
/* TODO: Needs to be reworked  */
void FlightModeSelector::execute(pathManager* pathMgr){
    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }

    if(!CommsWithTelemetry::GetTelemetryIncomingData()->takeoffCommand)
    {
        pathMgr->stage = LANDING;
    
    }

    if(CommsWithTelemetry::GetTelemetryIncomingData()->takeoffCommand && SensorFusion::GetSFOutput()->altitude < ON_GROUND_ALT)
    {
        pathMgr->stage = PREFLIGHT;

    }

    if (pathMgr->stage == PREFLIGHT && PreflightStage::getPreFlightCompleteStatus){
        pathMgr->stage = TAKEOFF; 

    }

    if(SensorFusion::GetSFOutput()->altitude > EXIT_TAKEOFF_ALTITUDE && CommsWithTelemetry::GetTelemetryIncomingData()->takeoffCommand)
    {
        pathMgr->stage = CRUISING;

    }
 
    switch(pathMgr->stage){
        case LANDING:
            pathMgr->setState(LandingStage::getInstance());
            break;
        case CRUISING:
            //no control details need to be reset for cruising state
            pathMgr->setState(cruisingState::getInstance());
            break;
        case PREFLIGHT:
            pathMgr->setState(PreflightStage::getInstance());
            break;
        case TAKEOFF:
            pathMgr->setState(TakeoffStage::getInstance());
            break;
        default:
            pathMgr->setState(cruisingState::getInstance());
    }
}

pathManagerState& FlightModeSelector::getInstance()
{
    static FlightModeSelector singleton; 
    return singleton; 
}

void CruisingStage::execute(pathManager* pathMgr)
{
    /*
    // telementryData is where we want to go, target waypoint
    fijo * telemetryData = CommsWithTelemetry::GetTelemetryIncomingData(); // Get struct from telemetry state with all of the commands and values.
    SFOutput_t * sensFusionOutput = SensorFusion::GetSFOutput(); // Get sensor fusion data

    // Set waypoint manager input struct 
    // input data is the current position, should be middle waypoint
    _inputdata.track = sensFusionOutput->track; // Gets track
    _inputdata.longitude = sensFusionOutput->longitude; // setting current waypoint longitude to SF longitude 
    _inputdata.latitude = sensFusionOutput->latitude;
    _inputdata.altitude = sensFusionOutput->altitude;

    // Call module functions - These will be changed later 

    //initializing current and target waypoint, setting the relationship of their pointers 
    ModifyFlightPathErrorCode editError = editFlightPath(cruisingStateManager, telemetryData, this->_inputdata); // Edit flight path if applicable
    
    
    GetNextDirectionsErrorCode pathError = pathFollow(cruisingStateManager, telemetryData, _inputdata, &_outputdata); // Get next direction or modify flight behaviour pattern
    setReturnValues(&_returnToGround, cruisingStateManager, editError, pathError); // Set error codes

    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }
    else
    {
        // pathMgr->setState(coordinateTurnElevation::getInstance());
         pathMgr->setState(CommsWithAttitude::getInstance()); 
    }
    */
}

pathManagerState& CruisingStage::getInstance()
{
    static CruisingStage singleton;
    return singleton;
}

void LandingStage::execute(pathManager* pathMgr)
{
    pathMgr->stage = LANDING;
    //load in sensor fusion data and telemtry data into input structure
    input.telemetryData = CommsWithTelemetry::GetTelemetryIncomingData();
    input.sensorOutput = SensorFusion::GetSFOutput();

    /* if(!pathMgr->madeLandingPoints)
    // {
        //Getting current location from sensor fusion 
        currentLocation = landingPath.initialize_waypoint(input.sensorOutput->longitude, input.sensorOutput->latitude, input.sensorOutput->altitude, LANDING_WAYPOINT); 

        // Gordon this is for you! - targetWaypoint Must have long, lat, alt, and waypointType = LANDING
        // targetWaypoint = someFancyLandingTakeoffMath

        //initializing flight path - sets the relationship between current location's and target's pointers
        waypointStatus = landingPath.initialize_flight_path(targetWaypoint, currentLocation);

        // pathMgr->madeLandingPoints = true;
     } 

    waypointInput.latitude = input.sensorOutput->latitude;
    waypointInput.longitude = input.sensorOutput->longitude;
    waypointInput.altitude = input.sensorOutput->altitude;
    waypointInput.track = input.sensorOutput->track;

    //going from current to target, 
    waypointStatus = landingPath.get_next_directions(waypointInput, &waypointOutput);

    //maintaining altitude
    waypointOutput.desiredAirspeed = CRUISING_AIRSPEED;
    */


    if(LandingStage::waypointStatus != WAYPOINT_SUCCESS)
    {
        pathMgr->isError = true;
    }

    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(CommsWithAttitude::getInstance());
    }
}

pathManagerState& LandingStage::getInstance()
{
    static LandingStage singleton;
    return singleton;
}


/****************************************************************************************************
TAKEOFF STATE FUNCTIONS
****************************************************************************************************/

//create a count to 1000 -> then proceed to takeoff. 
void PreflightStage::execute(pathManager* pathMgr)
{
    //load in sensor fusion data and telemtry data into input structure
    input.telemetryData = CommsWithTelemetry::GetTelemetryIncomingData();
    input.sensorOutput = SensorFusion::GetSFOutput();

    
    // This is the part where we ~wait~ before going to takeoff...not sure if it will work 

    if (cycleCount < 1000){
        cycleCount++; 
        preFlightComplete = false;
    } else { 
        preFlightComplete = true; 
        cycleCount = 0; 
    }


    if(PreflightStage::waypointStatus != WAYPOINT_SUCCESS)
    {
        pathMgr->isError = true;
    }

    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(CommsWithAttitude::getInstance());
    }
}

pathManagerState& PreflightStage::getInstance()
{
    static PreflightStage singleton;
    return singleton;
}

void TakeoffStage::execute(pathManager* pathMgr)
{
    //load in sensor fusion data and telemtry data into input structure
    input.telemetryData = CommsWithTelemetry::GetTelemetryIncomingData();
    input.sensorOutput = SensorFusion::GetSFOutput();

        waypointInput.latitude = input.sensorOutput->latitude;
        waypointInput.longitude = input.sensorOutput->longitude;
        waypointInput.altitude = input.sensorOutput->altitude;
        waypointInput.track = input.sensorOutput->track;

         //Getting current location from sensor fusion 
        currentLocation = takeOffPath.initialize_waypoint(input.sensorOutput->longitude, input.sensorOutput->latitude, input.sensorOutput->altitude, LANDING_WAYPOINT); 

        // targetWaypoint = someFancyLandingTakeoffMath

        //initializing flight path - sets the relationship between current location's and target's pointers
        waypointStatus = takeOffPath.initialize_flight_path(targetWaypoint, currentLocation);

        // Going from current to target
        waypointStatus = takeOffPath.get_next_directions(waypointInput, &waypointOutput);

        //maintaining altitude
        waypointOutput.desiredAirspeed = CRUISING_AIRSPEED;        
        

    if(waypointStatus != WAYPOINT_SUCCESS)
    {
        pathMgr->isError = true;
    }

    if(pathMgr->isError)
    {
        pathMgr->setState(fatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(CommsWithAttitude::getInstance());
    }
}

pathManagerState& TakeoffStage::getInstance()
{
    static TakeoffStage singleton;
    return singleton;
}

void fatalFailureMode::execute(pathManager* pathMgr)
{
    pathMgr->setState(fatalFailureMode::getInstance());
}

pathManagerState& fatalFailureMode::getInstance()
{
    static fatalFailureMode singleton;
    return singleton;
}


