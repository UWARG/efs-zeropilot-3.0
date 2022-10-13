/*
 * AM.hpp
 *
 * Attitude Manager Header
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */
#ifndef ZPSW3_AM_HPP
#define ZPSW3_AM_HPP

#include "LOS_Link.hpp"
#include "LOS_Actuators.hpp"
#include "AM_StateManager.hpp"

class AttitudeState;

namespace AM {
    // Gives status of attitude manager so we know when it has completed a cycle (its state is FetchInstructionsMode) or entered failure mode
    enum _Attitude_Manager_Cycle_Status {COMPLETED_CYCLE = 0, IN_CYCLE, FAILURE_MODE};
}

class AttitudeManager {
public:
    AttitudeManager(LOS_Link *link, LOS_Actuators *output);
    inline AttitudeState* getCurrentState() const {return currentState;}
    void execute();
    void setState(AttitudeState& newState);
    AM::_Attitude_Manager_Cycle_Status getStatus() {return status;}
    LOS_Link *link;
    LOS_Actuators *output;
private:
    AttitudeState* currentState;
    AM::_Attitude_Manager_Cycle_Status status;
    AttitudeManager();
};

#endif //ZPSW3_AM_HPP
