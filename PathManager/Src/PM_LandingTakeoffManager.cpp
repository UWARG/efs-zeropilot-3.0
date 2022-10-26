#include "PM_LandingTakeoffManager.hpp"

#include <math.h>
//#include "MathConstants.hpp"

double LandingTakeoffManager::getTakeoffAltitudeTarget(double currentAltitude)
{
    // Store the ground height when first planning the path
    if (groundHeight < 0) {
        groundHeight = currentAltitude;
    }

    //Calculate the takeoff range constant if just entering takeoff state.
    if (takeoffRangeConstant == 0) {
        takeoffRangeConstant = getRangeConstant(groundHeight + 2);
    }

    // Return the takeoff altitude target for use when determining when to exit Takeoff
    return groundHeight + TAKEOFF_TARGET;
}

double LandingTakeoffManager::getLandingAltitudeTarget(double currentAltitude)
{
    //Calculate the landing range constant if just entering landing state.
    if (landingRangeConstant == 0) {
        landingRangeConstant = getRangeConstant(groundHeight + (currentAltitude - groundHeight) / 2);
    }
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
    desiredWaypoint.heading_magnitude = 0; // Use velocity controller

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
    desiredWaypoint.heading_magnitude = 0; // Use velocity controller

    return desiredWaypoint;
}

double LandingTakeoffManager::getSpeedTarget(double currentAltitude, double rangeConstant)
{
    double speedTarget;
    speedTarget = MAX_SPEED * exp(-1.0 * (((currentAltitude - groundHeight - (TAKEOFF_TARGET / 2))^2) / rangeConstant))
    return speedTarget;
}

double LandingTakeoffManager::getRangeConstant(double midpointAltitude)
{
    return sqrt((midpointAltitude ^ 2) / (-2 * (log(LANDING_SPEED / MAX_SPEED))));
    // Note log here is log base e (ln)
}
