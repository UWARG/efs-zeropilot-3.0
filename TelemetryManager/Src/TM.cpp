#include "TM.hpp"
#include "LOS_Comms.hpp"
#include "LOS_Telem.hpp"
#include "Messages.h"
#include <cstring>


TelemetryManager::TelemetryManager() {

}

uint8_t TelemetryManager::sendData(SM::SM_TM_Commands commands) {

    uint8_t buf[160] = { 0 };

    switch(commands.select) {
        case 0:
        {
            TelemMessages::GroundStationData msg;
            std::memcpy(msg.motor_outputs,commands.gspc_data.motor_outputs, sizeof(commands.gspc_data.motor_outputs));
            std::memcpy(msg.controller_values,commands.gspc_data.controller_values, sizeof(commands.gspc_data.controller_values));
            std::memcpy(msg.battery_voltages,commands.gspc_data.battery_voltages, sizeof(commands.gspc_data.battery_voltages));

            msg.data.roll = commands.gspc_data.sf_data.roll;
            msg.data.pitch = commands.gspc_data.sf_data.pitch;
            msg.data.yaw = commands.gspc_data.sf_data.yaw;
            
            msg.data.roll_rate = commands.gspc_data.sf_data.rollRate;
            msg.data.pitch_rate = commands.gspc_data.sf_data.pitchRate;
            msg.data.yaw_rate = commands.gspc_data.sf_data.yawRate;

            msg.data.air_speed = commands.gspc_data.sf_data.airspeed;
            msg.data.ground_speed = 0;
            msg.data.climb_rate = commands.gspc_data.sf_data.rateOfClimb;

            msg.data.heading = commands.gspc_data.sf_data.heading;

            msg.data.latitude = commands.gspc_data.sf_data.latitude;
            msg.data.longitude = commands.gspc_data.sf_data.longitude;
            msg.data.altitude = commands.gspc_data.sf_data.altitude;

            helpers::encodeGroundStationData(msg,buf, sizeof(buf));
            Los_Telem::getInstance().transmit(buf,sizeof(msg));

        } break;
        case 1:
        {
            TelemMessages::JetsonMovementRequest msg;



        } break;
        case 2:
        {
            
        } break;
    }

}

void TelemetryManager::updateCommsBuffer() {
    Los_Comms::getInstance().receive(commsBuffer);
}

void TelemetryManager::updateTelemBuffer() {
    Los_Telem::getInstance().receive(telemBuffer);
}

void TelemetryManager::parseBuffer(CircularBuffer& buf, SM::TM_SM_Commands& commands) {
    // 7 is the amount of the overhead, 145 is the size of the longest message
    uint8_t tempBuf[8 + 145] = { 0 };
    uint8_t byte = 0;
    uint8_t status = buf.peek(byte, 0);
    if(status == 0) return;

    // start byte
    if(byte != 0x7e) {
        // data is messed up i suppose
        // should loop through the buffer until i see a 0x7e
        return;
    }

    // length byte
    uint16_t length = 0;
    status = buf.peek(byte, 1);
    if(status == 0) return;
    length = (uint16_t)byte << 8;

    status = buf.peek(byte, 2);
    if(status == 0) return;

    length = length | (uint16_t)byte;

    // check the existing length based on type and confirm to ensure its good

    uint8_t type = 0xFF;
    status = buf.peek(type, 3);
    if(status == 0) return;

    // check if the entire message is in the buffer
    // current byte is the position of start flag, the length bytes, type byte
    int size = 1 + 2 + 1 + length + 4;
    status = buf.read(tempBuf,size);
    if(status == 0) return;

    switch(type) {
        case JETSON_ODOM:
            // this is output
            break;
        case JETSON_MOVEMENT_REQ:
            // this is output
            break;
        case JETSON_RELATIVE_MOVEMENT_COMMAND:
            {
                // this goes to PM
                TelemMessages::JetsonRelativeMovementCommand msg = helpers::decodeJetsonRelativeMovementCommand(tempBuf,size);
                if(msg.header.flag == 0) {
                    // the data is all messed up 
                    return;
                }
                commands.jetson_commands.dist_forward = msg.x;
                commands.jetson_commands.dist_right = msg.y;
                commands.jetson_commands.dist_up = msg.z;
                commands.jetson_commands.heading = msg.heading;
            } break;
        case JETSON_LANDING_COMMAND:
            // this goes to PM
            {

                TelemMessages::JetsonLandingInitiationCommand msg = helpers::decodeJetsonLandingInitiationCommand(tempBuf,size);
                if(msg.header.flag == 0) {
                    // the data is all messed up 
                    return;
                }
            
                commands.landing_initiation.start_landing = msg.req != 0;
            } break;
        case GSPC_WAYPOINTS:
            // this goes to PM
            {
                
                TelemMessages::GroundStationWaypoints msg = helpers::decodeGroundStationWaypoints(tempBuf,size);
                if(msg.header.flag == 0) {
                    // the data is all messed up 
                    return;
                }
            
                commands.waypoint_commands.num_waypoints = msg.num_waypoints;
                for (int x = 0; x < msg.num_waypoints; x++) {
                    commands.waypoint_commands.waypoints[x].altitude = msg.waypoints[x].altitude;
                    commands.waypoint_commands.waypoints[x].latitude = msg.waypoints[x].latitude;
                    commands.waypoint_commands.waypoints[x].longitude = msg.waypoints[x].longitude;
                    commands.waypoint_commands.waypoints[x].waypoint_id = msg.waypoints[x].waypoint_id;
                }

            } break;
        case GSPC_DISARM:
            // TODO: make these functions use references rather than just setting the flag to 0
            {
                TelemMessages::GroundStationDisarm msg = helpers::decodeGroundStationDisarm(tempBuf,size);
                if(msg.header.flag == 0) {
                    // the data is all fucked 
                    return;
                }
                commands.ground_station_disarm_command.set_armed = msg.arm != 0;
            } break;
        case GSPC_PID_VALUES:
            // this goes to AM
            {
                TelemMessages::GroundStationPIDValues msg = helpers::decodeGroundStationPIDValues(tempBuf,size);
                if(msg.header.flag == 0) {
                    // the data is all fucked 
                    return;
                }
                commands.gspc_pid_set_command.controller = msg.controller;
                commands.gspc_pid_set_command.axis = msg.axis;
                commands.gspc_pid_set_command.values.P = msg.values.P;
                commands.gspc_pid_set_command.values.I = msg.values.I;
                commands.gspc_pid_set_command.values.D = msg.values.D;
                // TODO: add support for A value
                // also figure out what A is.
            } break;
        case GSPC_DATA:
            // this is output
            break;
        case GSPC_PID_SET_RESPONSE:
            // this is output
            break;
    }
}

void TelemetryManager::receiveData() {
    uint8_t temp[SIZE_COMMS_BUFFER];
    memcpy(temp,commsArr, SIZE_COMMS_BUFFER);
    updateCommsBuffer();
    updateTelemBuffer();

    SM::TM_SM_Commands commands;

    parseBuffer(commsBuffer,commands);
}

void TelemetryManager::init() {
    Los_Telem::getInstance().init();
    Los_Comms::getInstance().init();
}
