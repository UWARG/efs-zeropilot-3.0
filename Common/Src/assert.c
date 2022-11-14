#include "assert.h"
#include <stdbool.h>

void zp_assert(bool expr) {
    if (expr) {
#warning This should call the los hardfault handler, or something more appropriate
        while (1) {
        }
    }
}
