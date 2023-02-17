#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "task.h"

#include "usart.h"
#include <cstdarg>

void myprintf(const char *fmt, ...);

void MAVLinkTest_Heartbeat(void *params);

SemaphoreHandle_t sem_heartbeat;
SemaphoreHandle_t sem_got_ack;
SemaphoreHandle_t mutex_expected_ack;
SemaphoreHandle_t sem_test_end;
TaskHandle_t msg_receive_task;
TaskHandle_t heartbeat_task;
TaskHandle_t instruct_send_task;
MAVLinkACK_t expected_ack;
const TickType_t timeout = pdMS_TO_TICKS(5000);
const TickType_t delay = pdMS_TO_TICKS(100);
void MAVLinkTest_ACK(void *params);
void heartbeat_send_thread(void* params);
void instruct_send_thread(void* params);
void msg_receive_thread(void* params);

const static auto SM_PERIOD_MS = 5;



int main() {
    losInit();

    TaskHandle_t SM_handle = NULL;

    // Semaphore test
    SemaphoreHandle_t sem_1 = xSemaphoreCreateBinary();
    xSemaphoreGive(sem_1);
    // We just gave the semaphore so we should be able to take it immediately
    if (xSemaphoreTake(sem_1, timeout) == pdFAIL) {
        myprintf("We couldn't take the semaphore??!!\r\n");
        return -1;
    }
    // Now that we've taken the semaphore, we shouldn't be able to take it again
    if (xSemaphoreTake(sem_1, timeout) != pdFAIL) {
        myprintf("We can take the semaphore??!!\r\n");
        return -1;
    }
    myprintf("Semaphore test passed.\r\n");


    // Uncomment whichever test you want to run.
    
    // xTaskCreate(MAVLinkTest_Heartbeat, "MAVLink Heartbeat Test", 400U, NULL, osPriorityNormal, NULL);
    xTaskCreate(MAVLinkTest_ACK, "MAVLink ACK Test", 400U, NULL, osPriorityNormal, NULL);

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

void MAVLinkTest_ACK(void *params) {
    sem_heartbeat = xSemaphoreCreateBinary();
    mutex_expected_ack = xSemaphoreCreateBinary();
    sem_got_ack = xSemaphoreCreateBinary();
    sem_test_end = xSemaphoreCreateBinary();

    xSemaphoreGive(mutex_expected_ack);

    xTaskCreate(msg_receive_thread, "MAVLink ACK Test - msg receive thread", 400U, NULL, osPriorityNormal, &msg_receive_task);
    xTaskCreate(heartbeat_send_thread, "MAVLink ACK Test - heartbeat thread", 400U, NULL, osPriorityNormal, &heartbeat_task);
    xTaskCreate(instruct_send_thread, "MAVLink ACK Test - ACK thread", 400U, NULL, osPriorityNormal, &instruct_send_task);
    
    xSemaphoreTake(sem_test_end, pdMS_TO_TICKS(60000));

    vTaskDelete(heartbeat_task);
    vTaskDelete(instruct_send_task);
    vTaskDelete(msg_receive_task);
}

void heartbeat_send_thread(void* params) {
    while (true) {
        mavlink->sendHeartbeat();
        if (xSemaphoreTake(sem_heartbeat, timeout) == pdFAIL) {
            myprintf("Heartbeat not received!\r\n");
            xSemaphoreGive(sem_test_end);
        }
    }
}

void instruct_send_thread(void* params) {
    /* Initial configs. */
    xSemaphoreTake(mutex_expected_ack, timeout);
    expected_ack = mavlink->sendInitialConfigs();
    xSemaphoreGive(mutex_expected_ack);

    if (xSemaphoreTake(sem_got_ack, timeout) == pdFAIL) {
        myprintf("Initial configs instruction not ACKed!\r\n");
        xSemaphoreGive(sem_test_end);
    }

    vTaskDelay(delay);

    /* Arm. */
    xSemaphoreTake(mutex_expected_ack, timeout);
    expected_ack = mavlink->sendArmDisarm(true);
    xSemaphoreGive(mutex_expected_ack);

    if (xSemaphoreTake(sem_got_ack, timeout) == pdFAIL) {
        myprintf("Arm instruction not ACKed!\r\n");
        xSemaphoreGive(sem_test_end);
    }

    vTaskDelay(delay);

    /* Set AUTO flight mode. */
    xSemaphoreTake(mutex_expected_ack, timeout);
    expected_ack = mavlink->sendFlightModeChange(PLANE_MODE_AUTO);
    xSemaphoreGive(mutex_expected_ack);

    if (xSemaphoreTake(sem_got_ack, timeout) == pdFAIL) {
        myprintf("Set AUTO flight mode instruction not ACKed!\r\n");
        xSemaphoreGive(sem_test_end);
    }

    vTaskDelay(delay);

    /* Set MANUAL flight mode. */
    xSemaphoreTake(mutex_expected_ack, timeout);
    expected_ack = mavlink->sendFlightModeChange(PLANE_MODE_MANUAL);
    xSemaphoreGive(mutex_expected_ack);

    if (xSemaphoreTake(sem_got_ack, timeout) == pdFAIL) {
        myprintf("Set MANUAL flight mode instruction not ACKed!\r\n");
        xSemaphoreGive(sem_test_end);
    }

    vTaskDelay(delay);

    /* Disarm. */
    xSemaphoreTake(mutex_expected_ack, timeout);
    expected_ack = mavlink->sendFlightModeChange(PLANE_MODE_MANUAL);
    xSemaphoreGive(mutex_expected_ack);

    if (xSemaphoreTake(sem_got_ack, timeout) == pdFAIL) {
        myprintf("Disarm instruction not ACKed!\r\n");
        xSemaphoreGive(sem_test_end);
    }

    vTaskDelay(delay);

    myprintf("ACK test passed!");
    xSemaphoreGive(sem_test_end);
}

void msg_receive_thread(void* params) {
    MAVLinkMessage_t mavlink_message;

    while (true) {
        if (mavlink->receiveMessage(mavlink_message)) {
            switch(mavlink_message.type) {
                case MAVLinkMessageType::HEARTBEAT:
                    xSemaphoreGive(sem_heartbeat);
                    break;
                case MAVLinkMessageType::ACK:
                    xSemaphoreTake(mutex_expected_ack, timeout);
                    if (mavlink->checkMessageACK(mavlink_message, expected_ack)) {
                        xSemaphoreGive(sem_got_ack);
                    }
                    xSemaphoreGive(mutex_expected_ack);
                    break;
            }
        }
    }
}

