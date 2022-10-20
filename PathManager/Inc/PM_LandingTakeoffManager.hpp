#pragma once

/* Related Confluence Pages
    https://uwarg-docs.atlassian.net/wiki/spaces/ZP/pages/1607630921/Landing+and+Takeoff
*/

#include "PM_WaypointManager.hpp"
//#include "AutoSteer.hpp"
#include "SensorFusion.hpp"
#include "PM_DataTypes.hpp"
//#include "TelemPathInterface.hpp"

/********************************************************************
* Constants
********************************************************************/

const double TAKEOFF_TARGET = 4.0;
const double LANDING_SPEED = 0.2;
const double MAX_SPEED = 1.0;
const double RANGE_CONSTANT = 2.48533973824; // See Landing/Takeoff page for an explanation

class LandingTakeoffManager{

    public:
        /******************
         * EXTERNAL FUNCTIONS
         ******************/

        /*
            This function sets the ground height if it is called for the first time
            This function returns the takeoff altitude target (ground height + TAKEOFF_TARGET)

            @param double currentAltitude - this variable holds the current altitude value of the aircraft

            @return double - this structure holds the altitude point
        */
        static double getTakeoffAltitudeTarget(double currentAltitude);

        /*
            This function returns the ground height that was set during takeoff

            @param double currentAltitude - this variable holds the current altitude value of the aircraft

            @return double - this structure holds the altitude point
        */
        static double getLandingAltitudeTarget(double currentAltitude);

        /******************
         * LANDING FUNCTIONS
         ******************/

        /*
            This function returns a waypoint that the drone should follow
            This waypoint does not have a horizontal component yet, this will come as corrections from the CV team

            @param const SFOutput_t & input - this variable holds the sensor fusion module output

            @return WaypointData - this structure holds the climb point, which the plane will follow until it exits below a certain altitude
        */
        static WaypointData createLandingWaypoint(const SFOutput_t & input);

        /******************
         * TAKEOFF FUNCTIONS
         *****************/
        /*

        /*
            This function returns a waypoint that the drone should follow
            This waypoint does not have a horizontal component yet, this will come as corrections from the CV team

            @param const SFOutput_t & input - this variable holds the sensor fusion module output

            @return WaypointData - this structure holds the climb point, which the plane will follow until it exits at a certain altitude
        */
        static WaypointData createTakeoffWaypoint(const SFOutput_t & input);

        /***********************************
        COMMON LANDING AND TAKEOFF FUNCTIONS
        ************************************/

         /*
            This function returns the desired climb/descend speed given the current altitude
            Because the takeoff and landing target velocities 

            @param double currentAltitude - this double holds the current aircraft altitude

            @return double - this function will return the desired climb speed for the aircraft
        */
        double getSpeedTarget(double currentAltitude);

    private:
        double groundHeight = -1; // Set by takeoff and used by landing
};
