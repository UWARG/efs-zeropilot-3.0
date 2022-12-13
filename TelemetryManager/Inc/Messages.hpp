#pragma once


const uint8_t START_FLAG = 0x7e;


#include "messages/GroundStationData.hpp"
#include "messages/GroundStationDisarm.hpp"
#include "messages/GroundStationPIDSetResponse.hpp"
#include "messages/GroundStationPIDValues.hpp"
#include "messages/GroundStationWaypoints.hpp"
#include "messages/Header.hpp"
#include "messages/JetsonLandingInitiationCommand.hpp"
#include "messages/JetsonMovementRequest.hpp"
#include "messages/JetsonOdometryData.hpp"
#include "messages/JetsonRelativeMovementCommand.hpp"
#include "messages/PIDController.hpp"
#include "messages/PIDValues.hpp"
#include "messages/SensorData.hpp"
#include "messages/Waypoint.hpp"


#include "helpers/GroundStationData_helper.hpp"
#include "helpers/GroundStationDisarm_helper.hpp"
#include "helpers/GroundStationPIDSetResponse_helper.hpp"
#include "helpers/GroundStationPIDValues_helper.hpp"
#include "helpers/GroundStationWaypoints_helper.hpp"
#include "helpers/JetsonLandingInitiationCommand_helper.hpp"
#include "helpers/JetsonMovementRequest_helper.hpp"
#include "helpers/JetsonOdometryData_helper.hpp"
#include "helpers/JetsonRelativeMovementCommand_helper.hpp"


