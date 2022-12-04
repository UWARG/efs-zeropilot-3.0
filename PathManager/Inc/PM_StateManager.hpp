/**
 *  Path Manager State Machine Header
 */

#ifndef ZPSW3_PM_STATE_MANAGER_HPP
#define ZPSW3_PM_STATE_MANAGER_HPP

#include "PM.hpp"

class pathManager;

class pathManagerState
{
    public:
        virtual void enter(pathManager* pathMgr) = 0;
        virtual void execute(pathManager* pathMgr) = 0;
        virtual void exit(pathManager* pathMgr) = 0;

        bool operator==(const pathManagerState& rhs) const {return (this == &rhs);} 

        virtual ~pathManagerState() {}

};

#endif

