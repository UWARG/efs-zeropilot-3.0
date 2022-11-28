#pragma once

// please change this name
enum WaypointType {PATH_FOLLOW = 0, ORBIT_FOLLOW, HOVER_WAYPOINT, TAKEOFF_WAYPOINT, LANDING_WAYPOINT, TRANSITION_WAYPOINT, TELEOP_MODE};

struct CommandsFromSMToPM{
    // please rename this
    WaypointType waypoint_type;
    WaypointsCommand telemetry_commands;
    JetsonToZpMovementCommand jetson_commands; 
    LandingInitiationCommand landing_initiation; 
    LosSFData sf_data;
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
}

struct JetsonOdometryData {
    LosSFData sensorData;
}

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
}