//
// Created by Anthony Luo on 2022-10-13.
//

#ifndef ZPSW3_AM_DATATYPES_HPP
#define ZPSW3_AM_DATATYPES_HPP

typedef struct {
    int some_data_here;
} Position_t;


enum flight_mode: int8_t {
    fm_fatal_failure = -1,
    fm_limp = 0,
    fm_stabilize = 1,
    fm_gps = 2,
    fm_autonomous = 3
};

#endif //ZPSW3_AM_DATATYPES_HPP
