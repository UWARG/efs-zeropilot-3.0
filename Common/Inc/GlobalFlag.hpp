/*
 * GlobalFlag.hpp
 *
 * Global Flag Header
 *
 * Created on: Nov 29, 2022
 * Author(s): Gordon Fountain
 */

#ifndef ZPSW3_GLOBAL_FLAG_HPP
#define ZPSW3_GLOBAL_FLAG_HPP

#include "FreeRTOS.h"

class GlobalFlag {
    public:
        GlobalFlag(bool flag_value = false, int timeout_ms = 1) :: TIMEOUT(timeout_ms / portTICK_PERIOD_MS) {
            flag = flag_value;
            flagResourceMux = xSemaphoreCreateMutex();
            readerCountMux = xSemaphoreCreateMutex();
            serviceQueue = xSemaphoreCreateCounting(10, 1);
        };

        void writeFlag(bool new_value);

        bool readFlag();

    private:
        bool flag;
        int readerCount = 0;
        SemaphoreHandle_t flagResourceMux;
        SemaphoreHandle_t readerCountMux;
        SemaphoreHandle_t serviceQueue;
        // Constant of ticks until timeout
        static const TickType_t TIMEOUT;

};

#endif  // ZPSW3_GLOBAL_FLAG_HPP
