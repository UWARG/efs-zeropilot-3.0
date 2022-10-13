/*
 * AM_States.cpp
 *
 * Attitude Manager State-Machine classes
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */


#include "../Inc/AM_States.hpp"
#include "LOS_Link.hpp"
#include "LOS_Actuators.hpp"

/********************
 * DisArm Mode
 ********************/
void DisarmMode::execute(AttitudeManager *att_man) {
    // set all pwm to 0
    const uint8_t TIMEOUT_THRESHOLD = 2; //Max cycles without data until connection is considered broken

    //Get Arm Disarm instruction
    if (ReceiveArmDisarmInstruction(attitudeMgr)) {
        armDisarmTimeoutCount = 0;
    } else {
        if (armDisarmTimeoutCount < TIMEOUT_THRESHOLD)
            armDisarmTimeoutCount++;
    }


    /*
        3 possibilities:
             1. Go into FatalFailureMode bec of timeout
             2. Go into fetchInstructionsMode bec "Arm" instruction was sent
             3. Do nothing, stay in the disarm state
    */
    if (armDisarmTimeoutCount > TIMEOUT_THRESHOLD && CommsFailed()) {
        //Abort due to timeout failures
        attitudeMgr->setState(FatalFailureMode::getInstance());
        return;
    } else if (isArmed()) {
        attitudeMgr->setState(fetchInstructionsMode::getInstance());
    } else {
        //Do nothing, stay in this state
        //attitudeMgr->setState(DisarmMode::getInstance());
    }
}

AttitudeState& DisarmMode::getInstance() {
    static DisarmMode singleton;
    return singleton;
}

bool DisarmMode::receiveArmDisarmInstruction(AttitudeManager* att_man) {
    // if am dc; return false
    // if connected, return arm channel value
}

bool DisarmMode::isArmed() {
    // check arm channel value > min arm value

}


/********************
 * FetchInstructions Mode
 ********************/
void FetchInstructionsMode::execute(AttitudeManager *att_man) {
    const uint8_t TIMEOUT_THRESH = 2;

    FetchInstructionsMode::m_flight_mode = fm_stabilize;
    if (receiveTeleopInstructions(att_man)) {
        m_teleop_timeout_count = 0;
    } else {
        if (m_teleop_timeout_count < TIMEOUT_THRESH) {
            m_teleop_timeout_count++;
        }
    }

    // TODO: add section to move to disarmed state

    if (m_teleop_timeout_count < TIMEOUT_THRESH) {
        // TODO: add comms failed cehck
        FetchInstructionsMode::m_flight_mode = fm_stabilize;
    } else {
        att_man->setState(FatalFailureMode::getInstance());
    }

    /* at this point, we can run our actual instructions processing */
    // TODO: get position data
    // TODO: move flight mode as needed
}

AttitudeState &FetchInstructionsMode::getInstance() {
    static FetchInstructionsMode singleton;
    return singleton;
}

bool FetchInstructionsMode::receiveTeleopInstructions(AttitudeManager *att_man) {
    bool is_dc{true};
    if (is_dc) {
        return false;
    }

    // !TODO: get information from the link
    // (and somehow process it)
}

bool FetchInstructionsMode::isArmed() {
    bool retval = false;
    if (m_teleop_instructions.is_armed >= MIN_ARM_VALUE) {
        retval = true;
    }
    return retval;
}

/********************
 * ControlLoop Mode
 ********************/

void ControlLoopMode::execute(AttitudeManager *att_man) {
    Controls_Output_t *ctrl_out = nullptr;
    Teleop_Instructions_t *m_teleop_instructions = FetchInstructionsMode::getTeleopInstructions();
    if (FetchInstructionsMode::getFlightMode() == fm_autonomous) {
        // run certain modes
    } else if (FetchInstructionsMode::getFlightMode() == fm_gps) {

    } else if (FetchInstructionsMode::getFlightMode() == fm_stabilize) {

    } else if (FetchInstructionsMode::getFlightMode() == fm_limp) {
        // limp mode
    } else {
        // fatal failure mode
    }
}

/********************
 * OutputMixing Mode
 ********************/

void OutputMixingMode::execute(AttitudeManager *att_man) {
    Controls_Output_t *ctrl_out = ControlLoopMode::getControlsOutput();

    // match types?
    att_man->output->set(ctrl_out);
}

/********************
 * FatalFailure Mode
 ********************/

void FatalFailureMode::execute(AttitudeManager *att_man) {
    att_man->setState(FatalFailureMode::getInstance());
}

AttitudeState &FatalFailureMode::getInstance() {
    static FatalFailureMode singleton;
    return singleton;
}
