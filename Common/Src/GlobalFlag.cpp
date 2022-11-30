/*
 * GlobalFlag.cpp
 *
 * Global Flag
 *
 * Created on: Nov 29, 2022
 * Author(s): Gordon Fountain
 */

#include "../Inc/GlobalFlag.hpp"

void GlobalFlag::writeFlag(bool new_value) {
    // Take spot in line for service
    xSemaphoreTake(serviceQueue, TIMEOUT);
    // Once at front take the resource mutex
    xSemaphoreTake(flagResourceMux, TIMEOUT);
    // Let next in line get served now that resource is held
    xSemaphoreGive(serviceQueue);
    // Write to the flag
    flag = new_value;
    // Release resource mux
    xSemaphoreGive(flagResourceMux);
}

bool GlobalFlag::readFlag() {
    // Take spot in line for service
    xSemaphoreTake(serviceQueue, TIMEOUT);
    // Once at front of line take the reader count mutex
    xSemaphoreTake(readerCountMux, TIMEOUT);
    // Increment the number of readers accessing resource
    readerCount++;
    // If this is the first reader then take the resource mutex
    if (readerCount == 1) {
        xSemaphoreTake(flagResourceMux, TIMEOUT);
    }
    // Let the next call get service
    xSemaphoreGive(serviceQueue);
    // Release the reader count mutex to the next reader
    xSemaphoreGive(readerCountMux);
    // Store the flag value
    bool return_value = flag;
    // Get the reader count mutex
    xSemaphoreTake(readerCountMux, TIMEOUT);
    // Decrement the number of readers
    readerCount--;
    // Check if all readers are finished to release resource
    if (readerCount == 0) {
        xSemaphoreGive(flagResourceMux);
    }
    // Release reader count mutex
    xSemaphoreGive(readerCountMux);
    // Return the stored value
    return return_value;
}
