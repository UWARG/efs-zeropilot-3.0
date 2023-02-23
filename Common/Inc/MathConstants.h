#ifndef MATH_CONSTANTS_HPP
#define MATH_CONSTANTS_HPP

/***********************************************************************************************************************
 * Constants
 **********************************************************************************************************************/

#ifndef ZP_PI 
    constexpr double ZP_PI = 3.14159265358979311599796346854;
#endif

#ifndef ZP_E 
    constexpr double ZP_E = 2.71828182845904523536028747135;
#endif

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

#ifndef DEG_TO_RAD
    #define DEG_TO_RAD(angleInDegrees) ((angleInDegrees) * ZP_PI / 180.0)
#endif

#ifndef RAD_TO_DEG
    #define RAD_TO_DEG(angleInRadians) ((angleInRadians) * 180.0 / ZP_PI)
#endif

#endif