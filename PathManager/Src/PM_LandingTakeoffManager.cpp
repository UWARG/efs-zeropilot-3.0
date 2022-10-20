#include "PM_LandingTakeoffManager.hpp"

#include <math.h>
//#include "MathConstants.hpp"

double LandingTakeoffManager::getTakeoffAltitudeTarget(double currentAltitude)
{
    // Store the ground height when first planning the path
    if (groundHeight < 0) {
        groundHeight = currentAltitude;
    }

    // Return the takeoff altitude target for use when determining when to exit Takeoff
    return groundHeight + TAKEOFF_TARGET;
}

double LandingTakeoffManager::getLandingAltitudeTarget(double currentAltitude)
{
    // Return ground height for use to determine when to transition out of Landing
    return groundHeight;
}


static CommandsForAM_t LandingTakeoffManager::createTakeoffWaypoint(const SFOutput_t & input)
{
    CommandsForAM_t desiredWaypoint;

    desiredWaypoint.heading_dist_x = 0; // No Horizontal movement for MVP
    desiredWaypoint.heading_dist_y = 0; // No Horizontal movement for MVP
    desiredWaypoint.heading_dist_z = 1;
    desiredWaypoint.waypointType = TAKEOFF_WAYPOINT;

    double currentAltitude = input.altitude;

    desiredWaypoint.velocity = getSpeedTarget(currentAltitude);
    desiredWaypoint.heading_magnitude = currentAltitude + desiredWaypoint.velocity * (1.0 / CONTROLLER_FREQ);
    // TODO Find Controller Freq definition/location

    return desiredWaypoint;
}

static CommandsForAM_t LandingTakeoffManager::createLandingWaypoint(const SFOutput_t & input)
{
    CommandsForAM_t desiredWaypoint;

    desiredWaypoint.heading_dist_x = 0; // No Horizontal movement for MVP
    desiredWaypoint.heading_dist_y = 0; // No Horizontal movement for MVP
    desiredWaypoint.heading_dist_z = -1;
    desiredWaypoint.waypoint_type = LANDING_WAYPOINT;

    double currentAltitude = input.altitude;

    desiredWaypoint.velocity_target = getSpeedTarget(currentAltitude);
    desiredWaypoint.heading_magnitude = currentAltitude - desiredWaypoint.velocity * (1.0 / CONTROLLER_FREQ);

    return desiredWaypoint;
}

double LandingTakeoffManager::getSpeedTarget(double currentAltitude)
{
    double speedTarget;
    speedTarget = MAX_SPEED * exp(-1.0 * (((currentAltitude - groundHeight - (TAKEOFF_TARGET / 2))^2) / RANGE_CONSTANT))
    return speedTarget;
}
