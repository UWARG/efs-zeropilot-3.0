#pragma once

struct CommandsFromPMToAM{
  // heading unit vector and magnitude
  float dist_x; 
  float dist_y; 
  float dist_z; 
  float magnitude; // Magnitude distance to waypoint target
  float heading; // heading at target waypoint
  double speed_target; // Target velocity of drone approaching target
}; 

struct SM_PM_Commands {
    WaypointsCommand telemetry_commands;
    JetsonToZpMovementCommand jetson_commands; 
    LandingInitiationCommand landing_initiation; 
    LosSFData sf_data;
};

//TODO add CommandsFromPMToSM either here or in SM
struct PM_SM_Commands {

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
