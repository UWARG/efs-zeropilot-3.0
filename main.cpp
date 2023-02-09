#include "main.h"

#include "FreeRTOS.h"
#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "task.h"

#include "usart.h"
#include <cstdarg>

void myprintf(const char *fmt, ...);
void MAVLinkTest_Heartbeat(void *params);
const static auto SM_PERIOD_MS = 5;


int main() {
    losInit();

    TaskHandle_t SM_handle = NULL;

    xTaskCreate(MAVLinkTest_Heartbeat, "MAVLink Test (Heartbeat) Thread", 400U, NULL, osPriorityNormal, NULL);

    losKernelStart();

    // should not get here bec losInit() starts the scheduler
    while (1) {
    }

    return 0;
}

void myprintf(const char *fmt, ...) {
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&hlpuart1, (uint8_t*)buffer, len, -1);
}

void MAVLinkTest_Heartbeat(void *params) {
	MAVLinkMessage_t mavlink_message = {};
	uint8_t retval = 0;

	for (uint8_t i = 1; i <= 3; ++i) {
		myprintf("Round #%d\r\n", i);

		mavlink->sendHeartbeat();
		myprintf("Sent heartbeat\r\n");

		HAL_Delay(1000);

		retval = mavlink->receiveMessage(mavlink_message);

		if (retval && mavlink_message.type == MAVLinkMessageType::HEARTBEAT) {
			myprintf("Received heartbeat:\r\n");
			myprintf("    type = %d\r\n", mavlink_message.heartbeat.type);
			myprintf("    system status = %d\r\n", mavlink_message.heartbeat.system_status);
			myprintf("    base mode = %d\r\n", mavlink_message.heartbeat.base_mode);
		} else {
			myprintf("Failed to receive heartbeat!\r\n");
		}
	}
}

