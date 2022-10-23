/*
 * AM_Interface.h
 *
 * Interface so that freeRTOS can call C-style functions while dev is in c++
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */

#ifndef ZPSW3_AM_INTERFACE_H
#define ZPSW3_AM_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

void AM_InterfaceInit(void);
void AM_InterfaceExecute(void);

#ifdef __cplusplus
}
#endif

#endif // ZPSW3_AM_INTERFACE_H
