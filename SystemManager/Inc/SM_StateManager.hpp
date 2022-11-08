/*
 * SM_StateManager.hpp
 *
 * System State-Machine header
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#ifndef ZPSW3_SM_STATEMANAGER_HPP
#define ZPSW3_SM_STATEMANAGER_HPP

#include "SM.hpp"

class SystemManager;

class SystemState {
public:
    virtual void enter(SystemManager* systemMgr) = 0;
    virtual void execute(SystemManager* systemMgr) = 0;
    virtual void exit(SystemManager* systemMgr) = 0;

    bool operator==(const SystemState& rhs) const {return (this == &rhs);}

    virtual ~SystemState() {}
};
#endif //ZPSW3_AM_STATEMANAGER_HPP
