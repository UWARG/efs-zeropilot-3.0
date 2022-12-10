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

namespace SM {

class SystemManager;

class SystemState {
    public:
        void enter(SystemManager* system_manager) { (void) system_manager; }
        virtual void execute(SystemManager* systemMgr) = 0;
        void exit(SystemManager* system_manager) { (void) system_manager; }

        bool operator==(const SystemState& rhs) const {return (this == &rhs);}

        virtual ~SystemState() {}
};

} // namespace SM

#endif //ZPSW3_SM_STATEMANAGER_HPP
