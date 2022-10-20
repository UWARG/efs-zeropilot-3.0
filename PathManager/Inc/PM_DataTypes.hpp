#ifndef PM_DATATYPES_HPP
#define PM_DATATYPES_HPP

#include <cstdint>


/********************************************************************
* Enums
********************************************************************/

enum FlightStage{PREFLIGHT = 0, TAKEOFF, CRUISING, LANDING}; //used to determine the stage of flight


// Used to specify the type of output
enum WaypointType {PATH_FOLLOW = 0, ORBIT_FOLLOW, HOVER_WAYPOINT, TAKEOFF_WAYPOINT, LANDING_WAYPOINT, TRANSITION_WAYPOINT};

// Also don't know where to put these 
enum ModifyFlightPathCommand { NO_FLIGHT_PATH_EDIT = 0, INITIALIZE_FLIGHT_PATH, APPEND, INSERT, UPDATE, DELETE, NUKE }; // Used by cruisingState
enum GetNextDirectionsCommand { REGULAR_PATH_FOLLOWING = 0, TOGGLE_HOLDING, TOGGLE_HEAD_HOME }; // Used by cruisingState


/********************************************************************
* Important Structs
********************************************************************/

/*
* Structure stores manual control information
* If a certain variable needs to be controlled, set the percent to the desired value and its respective boolean to true
*/
struct PassbyControl{
    double rollPercent;
    bool rollPassby;

    double rudderPercent;
    bool rudderPassby;

    double pitchPercent;
    bool pitchPassby;

    double throttlePercent;
    bool throttlePassby;
};


/**
* Structure stores information about the waypoints along our path to the destination and back.
*/
struct WaypointData {
    int waypointId;                   // Id of the waypoint
    WaypointData * next;                 // Next waypoint
    WaypointData * previous;             // Previous waypoint
    long double latitude;             // Latitude of waypoint
    long double longitude;            // Longitude of waypoint
    int altitude;                     // Altitude of waypoint
    WaypointType waypoint_type; 
    double velocity_target; 
};

struct LandingTakeoffInput //this structure is used to hold the telemetry and sensorfusion outputs, which are needed for landing and takeoff inputs
{
    Telemetry_PIGO_t * telemetryData;
    SFOutput_t * sensorOutput;
};

struct LandingTakeoffOutput //this structure is used to hold the outputs of each landing state
{
    double desiredHeading; //desired heading is used when useHeading is true
    double desiredTrack; //desired track is used when useHEading is false
    bool useHeading; //this variable is used to determine if heading needs to be controlled, rather than track
    double desiredAltitude;
    double desiredAirspeed;
    PassbyControl controlDetails; //this variable is used to store the controls if passby control is needed
};

struct WaypointManager_Data_In {
    long double latitude;
    long double longitude;
    int altitude;
    double track;
};

struct WaypointManager_Data_Out{
    float desiredTrack;              // Desired track to stay on path
    int desiredAltitude;                // Desired altitude at next waypoint
    long double distanceToNextWaypoint; // Distance to the next waypoint (helps with airspeed PID)
    float distanceX, distanceY, distanceZ;
    float rotation; 
    WaypointStatus errorCode;          // Contains error codes
    bool isDataNew;                     // Notifies PID modules if the data in this structure is new
    int desiredAirspeed;                // FUN FACT WE NEED THIS 
    uint32_t timeOfData;                // The time that the data in this structure was collected
    WaypointType waypoint_type;       // Output type (determines which parameters are defined)
};


// Don't know where to put this yet 
//Commands for path manager to send to attitude manager.
typedef struct CommandsForAM_t{
  WaypointType waypoint_type; 

  // heading unit vector and magnitude
  float heading_dist_x; 
  float heading_dist_y; 
  float heading_dist_z; 
  float heading_magnitude; // Magnitude distance to waypoint target
  double velocity_target; // Target velocity of drone approaching target
} CommandsForAM;


struct GpsCoordinates{
    double longitude; 
    double lattiude; 
};

struct TelemWaypointData {
    struct gpsCoordinatesFIJO gps_coords;
    int waypoint_id; 
};

/* Not needed right now 
//Data for path manager to send to telemetry.
typedef struct POGI{
	int errorCode; //Code for specified errors
	double gpsLattitude,gpsLongitude,curAltitude; //Current gps position and altitude
	float curAirspeed; //Airspeed data
	float roll,pitch,yaw;	//Current orientation (radians)
	float camRoll,camPitch,camYaw;	//Current camera orientation (radians)
	bool isLanded; //Switch to check if landed (maybe limit switch?)
	uint8_t editingFlightPathErrorCode,flightPathFollowingErrorCode; //Flight path error codes
	uint8_t currentWaypointId, currentWaypointIndex; //current waypoint data
	bool homeBaseInit; //is home base initialized
} POGI;
*/

/*TODO: fix this based on what TM is giving us*/
struct Telemetry_PIGO_t {
    /* Parameters for the waypoint manager (crusingState) */
    int numWaypoints;
    
    ModifyFlightPathCommand waypointModifyFlightPathCommand; 
    bool initializingHomeBase; // 0 = no, 1 = yes
    GetNextDirectionsCommand waypointNextDirectionsCommand; 
    int holdingAltitude;
    int holdingTurnRadius;
    uint8_t holdingTurnDirection; // 0 = CW, 1 = CCW

    // When modifying the flight path.
    int nextId;
    int prevId;
    int modifyId;

    // Expecting a list of waypoints from TM 
    TelemWaypointData waypoints[100]; // Somehow need to get PATH_BUFFER_SIZE here...
    TelemWaypointData homebase;

    // landing and takeoff 
    bool beginLanding;
    bool beginTakeoff;
    float stoppingDirectionHeading;
    double stoppingLongitude;
    double stoppingLatitude;
    float stoppingAltitude;
    float takeoffDirectionHeading;
    // Struct for stopping point gps coordinates


    // gimbal (pitch and yaw)
    float gimbalPitch;
    float gimbalYaw;

};

#endif

