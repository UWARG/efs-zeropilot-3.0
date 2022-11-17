#ifndef PM_DATATYPES_HPP
#define PM_DATATYPES_HPP

#include <cstdint>


/********************************************************************
* Enums
********************************************************************/

// Used by PM to determine the stage of flight
enum FlightStage{PREFLIGHT = 0, TAKEOFF, CRUISING, LANDING, LANDED, DISARMED, TELEOP}; 


// Used to specify the type of output
// Where TELEOP_MODE is used by SM to indicate to PM to pass data directly to AM 
enum WaypointType {PATH_FOLLOW = 0, ORBIT_FOLLOW, HOVER_WAYPOINT, TAKEOFF_WAYPOINT, LANDING_WAYPOINT, TRANSITION_WAYPOINT, TELEOP_MODE};

enum ModifyFlightPathCommand { NO_FLIGHT_PATH_EDIT = 0, INITIALIZE_FLIGHT_PATH, APPEND, INSERT, UPDATE, DELETE, NUKE }; // Used by cruisingState
enum GetNextDirectionsCommand { REGULAR_PATH_FOLLOWING = 0, TOGGLE_HOLDING, TOGGLE_HEAD_HOME }; // Used by cruisingState


/********************************************************************
* Important Structs
********************************************************************/


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

// Used in Cruising State 
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


//Commands for path manager to send to attitude manager.
// TODO: Figure out what we are sending in teleop 

typedef struct CommandsForAM_t{
2  WaypointType waypoint_type;  
3
4  // heading unit vector and magnitude
5  float dist_x; 
6  float dist_y; 
7  float dist_z; 
8  float magnitude; // Magnitude distance to waypoint target
9  float heading; // heading at target waypoint
10  double speed_target; // Target velocity of drone approaching target
11} CommandsForAM;


// Data given from CV/TM to PM during regular cruising 
struct TelemWaypointData {    
  double longitude;     
  double lattiude;   
  double altitude; 
  uint8_t waypoint_id; 
};

typedef struct CommandsFromTM{
    uint8_t num_waypoints; // number of waypoints in the list 
    TelemWaypointData waypoints[num_waypoints]; 
} CommandsFromTM;

// Data given from CV/TM during search and landing 
struct JetsonToZpMovementCommand {
  float x;
  float y;
  float z;
  float heading;
}

struct LandingInitiationCommand {
    bool start_landing; 
}

/* DATA FROM SF */
typedef struct LosSFData_t {
    float roll, pitch, yaw; //rad
    float rollRate, pitchRate, yawRate; //rad/s
    float airspeed; //m/s
    float altitude; //m
    float rateOfClimb; //m/s
    long double latitude; //Decimal degrees
    float latitudeSpeed; //m/s
    long double longitude; //Decimal degrees
    float longitudeSpeed; //m/s
    double track; // degrees
    double heading; //degrees
} LosSFData;

// Receiving LOS data, and CM/TM commands from SM 
typedef struct CommandsFromSM{
    WaypointType waypoint_type;
    CommandsFromTM telemetry_commands;
    JetsonToZpMovementCommand jetson_commands; 
    LandingInitiationCommand landing_initiation; 
    // TODO: add RC data for teleop mode
    LosSFData sf_data;
} CommandsFromSM;



#endif

