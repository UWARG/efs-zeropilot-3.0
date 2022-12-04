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

static PM_AM_Commands LandingTakeoffManager::createTakeoffWaypoint(const LosSFData & input)
{
    PM_AM_Commands desiredWaypoint;

    desiredWaypoint.dist_x = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_y = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_z = 1;
    desiredWaypoint.waypointType = TAKEOFF_WAYPOINT;

    double currentAltitude = input.altitude;

    desiredWaypoint.velocity_target = getSpeedTarget(currentAltitude);
    desiredWaypoint.magnitude = 0; // Use velocity controller

    return desiredWaypoint;
}

static PM_AM_Commands LandingTakeoffManager::createLandingWaypoint(const LosSFData & input)
{
    PM_AM_Commands desiredWaypoint;

    desiredWaypoint.dist_x = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_y = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_z = -1;
    desiredWaypoint.waypoint_type = LANDING_WAYPOINT;

    double currentAltitude = input.altitude;

    desiredWaypoint.velocity_target = getSpeedTarget(currentAltitude);
    desiredWaypoint.magnitude = 0; // Use velocity controller

    return desiredWaypoint;
}

double LandingTakeoffManager::getSpeedTarget(double currentAltitude, double rangeConstant)
{
    double speedTarget;
    speedTarget = MAX_SPEED * exp(-1.0 * (((currentAltitude - groundHeight - (TAKEOFF_TARGET / 2))^2) / rangeConstant)); 
    return speedTarget;
}

double LandingTakeoffManager::getRangeConstant(double midpointAltitude)
{
    return sqrt((midpointAltitude ^ 2) / (-2 * (log(LANDING_SPEED / MAX_SPEED))));
    // Note log here is log base e (ln)
}
