#ifndef PM_DATATYPES_HPP
#define PM_DATATYPES_HPP

struct PM_AM_Commands {
  // heading unit vector and magnitude
  float dist_x; 
  float dist_y; 
  float dist_z; 
  float magnitude; // Magnitude distance to waypoint target
  float heading; // heading at target waypoint
  double velocity_target; // Target velocity of drone approaching target
}; 

struct SM_PM_Commands {
    WaypointsCommand telemetry_commands;
    JetsonToZpMovementCommand jetson_commands; 
    LandingInitiationCommand landing_initiation; 
    LosSFData sf_data;
};

// this struct doesnt seem to exist in any form in the PM PR, so heres my best guess at it
struct PM_SM_Commands {
    JetsonMovementRequest jetson_movement_req;
};


struct JetsonToZpMovementCommand {
  float x;
  float y;
  float z;
  float heading;
};

struct LandingInitiationCommand {
    bool start_landing; 
};

struct JetsonMovementRequest {
    bool request;
};

struct LosSFData_t {
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
};

struct WaypointsCommand {
    uint8_t num_waypoints; // number of valid waypoints in the list 
    TelemWaypoint waypoints[5]; 
};

struct TelemWaypoint {
    uint8_t waypoint_id;
    double longitude;
    double latitude;
    double altitude;
};

#endif
