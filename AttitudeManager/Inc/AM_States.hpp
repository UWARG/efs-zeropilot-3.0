/*
 * AM_States.hpp
 *
 * Attitude Manager State-Machine header file
 *
 * Describes the states of AM:
 *
 * Init / Disarm / FetchInstructionsMode (all in one rn)
 * ControlLoopMode  [ run our control loops ]
 * OutputMixingMode [ where we mix outputs  ] ? does this include telemetry
 *
 */

#ifndef ZPSW3_AM_STATES_HPP
#define ZPSW3_AM_STATES_HPP

#include "AM_StateManager.hpp"
#include "AM_DataTypes.hpp"


/* not being used at the moment
 * will be brought it after things start working incrementally
 **/
class DisarmMode: public AttitudeState {
public:
    void enter(AttitudeManager *att_man) {
        (void) att_man;
    }
    void execute(AttitudeManager *att_man);
    void exit(AttitudeManager *att_man) {
        (void) att_man;
    }
    static AttitudeState& getInstance();
private:
    DisarmMode() {
    }
    DisarmMode(const DisarmMode &other);
    DisarmMode& operator =(const DisarmMode &other);
    static bool receieveArmDisarmInstruction(AttitudeManager *att_man);
    static bool isArmed();
    static uint8_t m_arm_disarm_ppm_val;
    static uint8_t m_arm_disarm_timeout_count;
};

class FetchInstructionsMode : public AttitudeState {
public:
    void enter(AttitudeManager *att_man) {
        (void) att_man;
    }

    void execute(AttitudeManager *att_man);

    void exit(AttitudeManager *att_man) {
        (void) att_man;
    }

    static AttitudeState &getInstance();

    static flight_mode getFlightMode(void) {
        return FetchInstructionsMode::m_flight_mode;
    }

    static Teleop_Instructions_t *getTeleopInstructions(void) {
        return &m_teleop_instructions;
    }

private:
    FetchInstructionsMode() {
        // TODO: to be implemented
    }

    FetchInstructionsMode(const FetchInstructionsMode &other);

    FetchInstructionsMode &operator=(const FetchInstructionsMode &other);

    static bool receiveTeleopInstructions(AttitudeManager *att_man);

    static bool isArmed();

    static Teleop_Instructions_t m_teleop_instructions;
    static flight_mode m_flight_mode;
    static uint8_t m_teleop_timeout_count;
    static Position_t m_pos;
};

class ControlLoopMode : public AttitudeState {
public:
    void enter(AttitudeManager *att_man) {
        (void) att_man;
    }
    void execute(AttitudeManager *att_man);
    void exit(AttitudeManager *att_man) {
        (void) att_man;
    }
    static AttitudeState& getInstance();
    static Controls_Output_t* getControlsOutput(void) {
        return m_controls_output
    }
private:
    ControlLoopMode() {

    }
    ControlLoopMode(const ControlLoopMode &other);
    ControlLoopMode& operator = (const ControlLoopMode &other);
    static Controls_Output_t *m_controls_output
};

class OutputMixingMode : public AttitudeState {
public:
    void enter(AttitudeManager *att_man) {
        (void) att_man;
    }
    void execute(AttitudeManager *att_man) {
        (void) att_man;
    }
    void exit(AttitudeManager *att_man) {
        (void) att_man;
    }
    static AttitudeState& getInstance();
    static float* getChannelOut(void) {
        return m_channel_out;
    }
private:
    OutputMixingMode() {

    }
    OutputMixingMode(const OutputMixingMode &other);
    OutputMixingMode& operator = (const OutputMixingMode &other);
    static float m_channel_out[5]; //FIXME: make this a data struct
};

class FatalFailureMode: public AttitudeState {
public:
    void enter(AttitudeManager *att_man) {
        (void) att_man;
    }
    void execute(AttitudeManager *att_man);
    void exit(AttitudeManager *att_man) {
        (void) att_man;
    }
    static AttitudeState& getInstance();
private:
    FatalFailureMode() {
    }
    FatalFailureMode(const FatalFailureMode &other);
    FatalFailureMode& operator =(const FatalFailureMode &other);
};

#endif //ZPSW3_AM_STATES_HPP
