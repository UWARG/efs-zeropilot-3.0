/*
 * AM_StateManager.hpp
 *
 * Attitude State-Machine header
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */

#ifndef ZPSW3_AM_STATEMANAGER_HPP
#define ZPSW3_AM_STATEMANAGER_HPP

#include "AM.hpp"

class AttitudeManager;

class AttitudeState {
public:
    virtual void enter(AttitudeManager* attitudeMgr) = 0;
    virtual void execute(AttitudeManager* attitudeMgr) = 0;
    virtual void exit(AttitudeManager* attitudeMgr) = 0;

    bool operator==(const AttitudeState& rhs) const {return (this == &rhs);}

    virtual ~AttitudeState() {}
};
#endif //ZPSW3_AM_STATEMANAGER_HPP
