#ifndef CIRCULAR_QUEUE_HPP
#define CIRCULAR_QUEUE_HPP

#include "stdint.h"

const int SIZE_BUFFER = 150;

struct CircularBuffer {
    uint8_t buffer[SIZE_BUFFER] = { };

    int readPtr = 0;
    int writePtr = 0;
};

#endif