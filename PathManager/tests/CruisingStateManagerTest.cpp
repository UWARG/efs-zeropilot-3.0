#include <gtest/gtest.h>
#include "PM_CruisingStateManager.hpp"

using namespace PM;

enum ArrayStatus {ARRAY_SUCCESS = 0, ARRAY_DIFFERENT};
enum _WaypointSt {WAYPOINT_CORRECT = 0, WAYPOINT_INCORRECT};
enum _OutputStatus {OUTPUT_CORRECT = 0, OUTPUT_INCORRECT};

static ArrayStatus compare_arrays(WaypointData ** testArray, TelemWaypoint * ans, int numElements) {
    WaypointData * nextWaypoint;
    nextWaypoint = testArray[0];

    // Checks if nexts are linked properly
    for(int i = 0; i < numElements-1; i++) {
        if(ans[i+1].altitude != testArray[i]->next->altitude || ans[i+1].latitude != testArray[i]->next->latitude || ans[i+1].longitude != testArray[i]->next->longitude || ans[i+1].waypoint_id != testArray[i]->next->waypointId) {
            return ARRAY_DIFFERENT;
        }
    }
    if (testArray[numElements-1]->next != nullptr) {
        return ARRAY_DIFFERENT;
    }

    // Checks if previous are linked properly
    for (int i = numElements-1; i>0; i--) {
        if(ans[i-1].altitude != testArray[i]->previous->altitude || ans[i-1].latitude != testArray[i]->previous->latitude || ans[i-1].longitude != testArray[i]->previous->longitude || ans[i-1].waypoint_id != testArray[i]->previous->waypointId) {
            return ARRAY_DIFFERENT;
        }
    }
    if (testArray[0]->previous != nullptr) {
        return ARRAY_DIFFERENT;
    }

    // Checks if indexes are the same
    for(int i = 0; i < numElements; i++) {
        if(ans[i].altitude != testArray[i]->altitude || ans[i].latitude != testArray[i]->latitude || ans[i].longitude != testArray[i]->longitude || ans[i].waypoint_id != testArray[i]->waypointId) {
            return ARRAY_DIFFERENT;
        }
    }

    return ARRAY_SUCCESS;
}

static _WaypointSt compare_waypoint(WaypointData * ans, WaypointData * test) {
    if(ans->waypointId != test->waypointId && ans->longitude != test->longitude && ans->latitude != test->latitude && ans->altitude != test->altitude && ans->waypointType != test->waypointType && ans->turnRadius != test->turnRadius) {
        // cout << "Waypoint Equality Check: " << ans->waypointId << " " << test->waypointId << " | " << ans->longitude << " " << test->longitude << " | " << ans->latitude << " " << test->latitude << " | " << ans->altitude << " " << test->altitude << " | " << ans->waypointType << " " << test->waypointType << " | " << ans->turnRadius << " " << test->turnRadius << endl;
        return WAYPOINT_INCORRECT;
    }

    return WAYPOINT_CORRECT;
}

static _OutputStatus compare_output_data(WaypointManager_Data_Out *ans, WaypointManager_Data_Out *test) {
    if(ans->desiredAltitude == test->desiredAltitude && ans->desiredTrack == test->desiredTrack && ans->distanceToNextWaypoint == round(test->distanceToNextWaypoint) && ans->radius == test->radius && ans->turnDirection == test->turnDirection && ans->out_type == test->out_type) {
        // cout << "Comparing Output Data: Alt " << ans->desiredAltitude << " " << test->desiredAltitude << " | Track " << ans->desiredTrack << " " << test->desiredTrack << " | Distance " << ans->distanceToNextWaypoint << " " << test->distanceToNextWaypoint << " | Radius " << ans->radius << " " << test->radius << " | Direction " << ans->turnDirection << " " << test->turnDirection << " | OutType " << ans->out_type << " " << test->out_type << endl;
        return OUTPUT_CORRECT;
    } else {
        //cout << "Comparing Output Data: Alt " << ans->desiredAltitude << " " << test->desiredAltitude << " | Track " << ans->desiredTrack << " " << test->desiredTrack << " | Distance " << ans->distanceToNextWaypoint << " " << test->distanceToNextWaypoint << " | Radius " << ans->radius << " " << test->radius << " | Direction " << ans->turnDirection << " " << test->turnDirection << " | OutType " << ans->out_type << " " << test->out_type << endl;
        return OUTPUT_INCORRECT;
    }
}

static _OutputStatus compare_coordinates(float * ans, float * test) {
    // Accept an error of maximum 0.01 degrees
    if (abs((ans[0]- test[0])/ans[0]) < 0.01 && abs((ans[1]- test[1])/ans[1]) < 0.01 && abs((ans[2]- test[2])/ans[2]) < 0.01) {
        // cout << "Comparing Coordinates: " << ans[0] << " " << test[0] << " | " << ans[1] << " " << test[1] << " | " << ans[2] << " " << test[2] << endl;
        return OUTPUT_CORRECT; 
    } else {
        // cout << "Comparing Coordinates: " << ans[0] << " " << test[0] << " | " << ans[1] << " " << test[1] << " | " << ans[2] << " " << test[2] << endl;
        return OUTPUT_INCORRECT;
    }
}


/************************ TESTING INITIALIZING AND SETTING WAYPOINT MANAGER OBJECT ************************/



/************************ TESTING GETTING DESIRED TRACK/ALTITUDE/ETC ************************/


/************************ TESTING FLIGHT PATH ************************/
TEST(CrusingStateManager, ShortFlightPath) {
    /***********************SETUP***********************/
    CruisingStateManager * crusingStateManagerInstance = new CruisingStateManager();
    int numWaypoint = 3;

    uint8_t id = 0;
    double longitude = 1;
    double latitude = 10;
    double altitude = 100;

    // initialize waypoint structs
    TelemWaypoint Test[numWaypoint];
    for (int i=0; i<numWaypoint; ++i) {
        Test[i] = {
                longitude = longitude++,
                latitude = latitude++, 
                altitude = altitude++,
                waypoint_id = id++
            };
    }

    // initialiize Telem Structs
    int numTelemStructs = numWaypoint/5 + 1;
    int currTestWaypoint = 0;
    WaypointsCommand Commands[numTelemStructs] = {};
    for (int i=0; i<numTelemStructs; ++i)
    {
        Commands[i].num_waypoints = (numWaypoint-5*i) % 5;
        for (int j=0; j<Commands[i]->num_waypoints; ++j) {
            Commands[i].waypoints[j] = Test[currTestWaypoint++];
        }
    }


    /***********************EXECUTE***********************/

    WaypointStatus result[numTelemStructs];
    for (int i=0; i<numTelemStructs; ++i)
    {
        result[i] = crusingStateManagerInstance->editFlightPath(&Commands[i], NULL; (i == 0));
    }

    WaypointData** data = crusingStateManagerInstance->get_waypoint_buffer();

    ArrayStatus res = compare_arrays(data, Test, numWaypoint);

    /***********************CLEANUP***********************/
    delete crusingStateManagerInstance;

    /***********************ASSERT***********************/
    for (int i=0; i<numTelemStructs; ++i)
    {
        EXPECT_EQ(result[i], WAYPOINT_SUCCESS); 
    }
    EXPECT_EQ(res, ARRAY_SUCCESS); 

}

TEST(CruisingStateManager, LongFlightPath) {
    /***********************SETUP***********************/
    CruisingStateManager * crusingStateManagerInstance = new CruisingStateManager();
    int numWaypoint = 50;

    uint8_t id = 0;
    double longitude = 1;
    double latitude = 10;
    double altitude = 100;

    // initialize waypoint structs
    TelemWaypoint Test[numWaypoint];
    for (int i=0; i<numWaypoint; ++i) {
        Test[i] = {
                longitude = longitude++,
                latitude = latitude++, 
                altitude = altitude++,
                waypoint_id = id++
            };
    }

    // initialiize Telem Structs
    int numTelemStructs = numWaypoint/5 + 1;
    int currTestWaypoint = 0;
    WaypointsCommand Commands[numTelemStructs] = {};
    for (int i=0; i<numTelemStructs; ++i)
    {
        Commands[i].num_waypoints = (numWaypoint-5*i) % 5;
        for (int j=0; j<Commands[i]->num_waypoints; ++j) {
            Commands[i].waypoints[j] = Test[currTestWaypoint++];
        }
    }


    /***********************EXECUTE***********************/

    WaypointStatus result[numTelemStructs];
    for (int i=0; i<numTelemStructs; ++i)
    {
        result[i] = crusingStateManagerInstance->editFlightPath(&Commands[i], NULL; (i == 0));
    }

    WaypointData** data = crusingStateManagerInstance->get_waypoint_buffer();

    ArrayStatus res = compare_arrays(data, Test, numWaypoint);

    /***********************CLEANUP***********************/
    delete crusingStateManagerInstance;

    /***********************ASSERT***********************/
    for (int i=0; i<numTelemStructs; ++i)
    {
        EXPECT_EQ(result[i], WAYPOINT_SUCCESS); 
    }
    EXPECT_EQ(res, ARRAY_SUCCESS); 
}

TEST(CrusingStateManager, OverwriteFlightPath) {
    /***********************SETUP***********************/
    CruisingStateManager * crusingStateManagerInstance = new CruisingStateManager();
    int numWaypoint = 6;
    int firstNumWaypoint = numWaypoint/2;
    int secondNumWaypoint = numWaypoint - firstNumWaypoint;

    uint8_t id = 0;
    double longitude = 1;
    double latitude = 10;
    double altitude = 100;

    // initialize waypoint structs
    TelemWaypoint firstTest[firstNumWaypoint];
    for (int i=0; i<firstNumWaypoint; ++i) {
        firstTest[i] = {
                longitude = longitude++,
                latitude = latitude++, 
                altitude = altitude++,
                waypoint_id = id++
            };
    }

    TelemWaypoint secondTest[secondNumWaypoint];
    for (int i=0; i<secondNumWaypoint; ++i) {
        secondTest[i] = {
                longitude = longitude++,
                latitude = latitude++, 
                altitude = altitude++,
                waypoint_id = id++
            };
    }

    // initialiize Telem Structs
    int currTestWaypoint = 0;

    int firstNumTelemStructs = firstNumWaypoint/5 + 1;
    WaypointsCommand firstCommands[firstNumTelemStructs] = {};
    for (int i=0; i<firstNumTelemStructs; ++i)
    {
        firstCommands[i].num_waypoints = (firstNumWaypoint-5*i) % 5;
        for (int j=0; j<firstCommands[i]->num_waypoints; ++j) {
            firstCommands[i].waypoints[j] = firstTest[currTestWaypoint++];
        }
    }

    int secondNumTelemStructs = secondNumWaypoint/5 + 1;
    WaypointsCommand secondCommands[secondNumTelemStructs] = {};
    for (int i=0; i<secondNumTelemStructs; ++i)
    {
        secondCommands[i].num_waypoints = (secondNumWaypoint-5*i) % 5;
        for (int j=0; j<secondCommands[i]->num_waypoints; ++j) {
            secondCommands[i].waypoints[j] = secondTest[currTestWaypoint++];
        }
    }


    /***********************EXECUTE***********************/

    WaypointStatus firstResult[firstNumTelemStructs];
    for (int i=0; i<firstNumTelemStructs; ++i)
    {
        firstResult[i] = crusingStateManagerInstance->editFlightPath(&firstCommands[i], NULL; (i == 0));
    }
    WaypointData** firstData = crusingStateManagerInstance->get_waypoint_buffer();
    ArrayStatus firstRes = compare_arrays(firstData, firstTest, firstNumWaypoint);

    WaypointStatus secondResult[secondNumTelemStructs];
    for (int i=0; i<secondNumTelemStructs; ++i)
    {
        firstResult[i] = crusingStateManagerInstance->editFlightPath(&secondCommands[i], NULL; (i == 0));
    }
    WaypointData** secondData = crusingStateManagerInstance->get_waypoint_buffer();
    ArrayStatus secondRes = compare_arrays(secondData, secondTest, secondNumWaypoint);

    /***********************CLEANUP***********************/
    delete crusingStateManagerInstance;

    /***********************ASSERT***********************/
    for (int i=0; i<firstNumTelemStructs; ++i)
    {
        EXPECT_EQ(firstResult[i], WAYPOINT_SUCCESS); 
    }
    for (int i=0; i<secondNumTelemStructs; ++i)
    {
        EXPECT_EQ(secondResult[i], WAYPOINT_SUCCESS); 
    }
    EXPECT_EQ(firstRes, ARRAY_SUCCESS); 
    EXPECT_EQ(secondRes, ARRAY_SUCCESS); 
}

TEST(CrusingStateManager, TooLongFlightPathThrowsError) {
    /***********************SETUP***********************/
    CruisingStateManager * crusingStateManagerInstance = new CruisingStateManager();
    int numWaypoint = 102;

    uint8_t id = 0;
    double longitude = 1;
    double latitude = 10;
    double altitude = 100;

    // initialize waypoint structs
    TelemWaypoint Test[numWaypoint];
    for (int i=0; i<numWaypoint; ++i) {
        Test[i] = {
                longitude = longitude++,
                latitude = latitude++, 
                altitude = altitude++,
                waypoint_id = id++
            };
    }

    // initialiize Telem Structs
    int numTelemStructs = numPaths/5 + 1;
    int currTestWaypoint = 0;
    WaypointsCommand Commands[numTelemStructs] = {};
    for (int i=0; i<numTelemStructs; ++i)
    {
        Commands[i].num_waypoints = (numWaypoint-5*i) % 5;
        for (int j=0; j<Commands[i]->num_waypoints; ++j) {
            Commands[i].waypoints[j] = Test[currTestWaypoint++];
        }
    }


    /***********************EXECUTE***********************/

    WaypointStatus result;
    for (int i=0; i<numTelemStructs; ++i)
    {
        result= crusingStateManagerInstance->editFlightPath(&Commands[i], NULL; (i == 0));
    }

    /***********************CLEANUP***********************/
    delete crusingStateManagerInstance;

    /***********************ASSERT***********************/
    EXPECT_NEQ(result, WAYPOINT_SUCCESS); 
}

