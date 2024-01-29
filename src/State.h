#ifndef LedState_h
#define LedState_h

#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#include "LedStateData.h"

// Make sure that ALLSTATES array reflects this order!
// Also update intToLedMode-function if changed!
enum class LedMode {
  OFF = 0, // Turn LED off
  RGB_DEFAULT = 1, // RGB
  DESIGNER = 2, // CMY
  FRENCH = 3, // RWB
  AFRIKAN = 4, // RYG
};

// Update intToDeviceMode-function if changed!
enum class DeviceMode {
  DEFAULT_SLOW = 0,
  DEFAULT_MEDIUM = 1,
  DEFAULT_FAST = 2,
  BOTH_SLOW = 3,
  BOTH_MEDIUM = 4,
  BOTH_FAST = 5,
};

enum class AnimationType {
  NONE = 0,
  HOLD = 1,
  FADE = 2,
};

class State {
    public:
        State();
        void begin( int ledPin, int numLeds );
        void maintain();
        bool animating();

        LedMode getLedMode();
        void showActiveLedMode();
        void setLedMode( LedMode newMode );
        void setLedSubMode( int newState );

        DeviceMode getDeviceMode();
        void showActiveDeviceMode();
        void setDeviceMode( DeviceMode newMode );
        void setDeviceSubMode( int newState );
    private:
        int _ledPin;
        int _numLeds;
        AnimationType currentAnimation;
        unsigned long animationStart;
        int STATE_DATA[STATESIZE];
        void setPixels(uint8_t r, uint8_t g, uint8_t b);
        LedMode _activeLedMode;
        DeviceMode _activeDeviceMode;
        int _activeSubState;
        float fadeBrightness;
        Adafruit_NeoPixel *pixels;
        LedMode currentLedState;
        LedMode previousLedState;
        LedMode intToLedMode( int state );
        DeviceMode intToDeviceMode( int state );
        Preferences preferences;
};

#endif
