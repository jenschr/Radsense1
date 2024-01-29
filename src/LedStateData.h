// Low tech solution to store multiple states
// By sticking to a single array, we're setting
// aside unfragmented memory & that should be
// faster to read.

#ifndef LedStateData_h
#define LedStateData_h
 
// four sets of color, one per state
const int STATESIZE = 12;
// three colors (RGB) in each substate
const int STATEWIDTH = 3;
const int LEDMODES = 5;
const int DEVICEMODES = 6;

const int ALLSTATES[] = {
// LEDMODE_OFF
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
// LEDMODE_RGB
    0, 255, 0,
    0, 0, 255,
    255, 0, 0,
    0, 0, 0,
// LEDMODE_DESIGNER
    255, 205, 0,
    255, 0, 255,
    0, 255, 255,
    0, 0, 0,
// LEDMODE_FRENCH
    255, 0, 0,
    255, 255, 255,
    0, 0, 255,
    0, 0, 0,
// LEDMODE_AFRIKAN
    255, 0, 0,
    255, 205, 0,
    0, 255, 0,
    0, 0, 0,
// DEVICEMODE_DEFAULT_SLOW
    255, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
// DEVICEMODE_DEFAULT_MEDIUM
    255, 0, 0,
    255, 0, 0,
    0, 0, 0,
    0, 0, 0,
// DEVICEMODE_DEFAULT_FAST
    255, 0, 0,
    255, 5, 5,
    255, 5, 5,
    0, 0, 0,
// DEVICEMODE_BOTH_SLOW
    0, 255, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
// DEVICEMODE_BOTH_MEDIUM
    0, 255, 0,
    5, 255, 5,
    0, 0, 0,
    0, 0, 0,
// DEVICEMODE_BOTH_FAST
    0, 255, 0,
    5, 255, 5,
    5, 255, 5,
    0, 0, 0,
};

#endif
