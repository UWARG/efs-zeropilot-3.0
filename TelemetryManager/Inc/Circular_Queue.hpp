#ifndef CIRCULAR_QUEUE_HPP
#define CIRCULAR_QUEUE_HPP

#include "stdint.h"

const int SIZE_QUEUE = 150;

uint8_t queue[SIZE_QUEUE] = { };

uint8_t* readPtr = &queue[0];
uint8_t* writePtr = &queue[0];


#endif