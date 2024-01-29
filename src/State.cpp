#include "State.h"

State::State(){
    _activeLedMode = LedMode::RGB_DEFAULT;
    currentAnimation = AnimationType::NONE;
}

void State::begin(int ledPin, int numLeds)
{
  _ledPin = ledPin;
  _numLeds = numLeds;
  pixels = new Adafruit_NeoPixel(_numLeds, _ledPin, NEO_GRB + NEO_KHZ800);
  pixels->begin();

  preferences.begin("my-app", false);
  unsigned int savedLedMode = preferences.getUInt("LedMode", 1);
  setLedMode( intToLedMode(savedLedMode) );
  Serial.print("savedLedMode ");
  Serial.println(savedLedMode);

  unsigned int savedDeviceMode = preferences.getUInt("DeviceMode", 1);
  setDeviceMode( intToDeviceMode(savedDeviceMode) );
  Serial.print("savedDeviceMode ");
  Serial.println(savedDeviceMode);
}

bool State::animating()
{
  return currentAnimation != AnimationType::NONE;
}

void State::maintain()
{
  if( currentAnimation == AnimationType::HOLD )
  {
    unsigned long now = millis();
    if( now - animationStart > 3000 ){ // finished animating state
      currentAnimation = AnimationType::NONE;
    } else if( now - animationStart > 2000 ){
      setLedSubMode(3);
    } else if( now - animationStart > 1500 ){
      setLedSubMode(2);
    } else if( now - animationStart > 1000 ){
      setLedSubMode(1);
    } else if( now - animationStart > 500 ){
      setLedSubMode(0);
    } else {
      setLedSubMode(3);
    }
  }
  else if( currentAnimation == AnimationType::FADE )
  {
    unsigned long now = millis();
    if( now - animationStart > 3000 ){ // finished animating state
      currentAnimation = AnimationType::NONE;
    } else if( now - animationStart > 2000 ){
      setDeviceSubMode(3);
    } else if( now - animationStart > 1500 ){
      setDeviceSubMode(2);
    } else if( now - animationStart > 1000 ){
      setDeviceSubMode(1);
    } else if( now - animationStart > 500 ){
      setDeviceSubMode(0);
    } else {
      setDeviceSubMode(3);
    }
  }
}

// LED modes handle how the LEDs light up for debugging
void State::showActiveLedMode()
{
  currentAnimation = AnimationType::HOLD;
  animationStart = millis();
}

void State::setLedMode( LedMode newState )
{
  if( newState != _activeLedMode )
  {
    Serial.print("LedMode::setLedMode ");
    Serial.println((int)newState);
    _activeLedMode = newState;
    
    // Save to prefs
    int tmpMode = (int)_activeLedMode;
    preferences.putUInt("LedMode", tmpMode);
    Serial.print("Wrote LedMode to prefs ");
    Serial.println(tmpMode);
  }
}

void State::setLedSubMode( int newState )
{
  if( _activeSubState != newState )
  {
    int statePosition = static_cast<int>(_activeLedMode)*STATESIZE;
    int subStatePosition = newState*STATEWIDTH;
    int r = ALLSTATES[statePosition+subStatePosition+0];
    int g = ALLSTATES[statePosition+subStatePosition+1];
    int b = ALLSTATES[statePosition+subStatePosition+2];
    /*
    Serial.print(millis());
    Serial.print(", ");
    Serial.print(statePosition);
    Serial.print(", ");
    Serial.print(statePosition+subStatePosition+0);
    Serial.print(", ");
    Serial.print(statePosition+subStatePosition+1);
    Serial.print(", ");
    Serial.print(statePosition+subStatePosition+2);
    Serial.print(", ");
    
    Serial.print("LedState::setLedSubMode ");
    Serial.println(newState);
    */
    setPixels(r, g, b);
  }
  _activeSubState = newState;
}

LedMode State::getLedMode()
{
  return _activeLedMode;
}

// Device modes handle how the device works

void State::showActiveDeviceMode()
{
  Serial.print("DeviceMode::showActiveDeviceMode ");
  currentAnimation = AnimationType::FADE;
  animationStart = millis();
}

void State::setDeviceMode( DeviceMode newDeviceMode )
{
  if( newDeviceMode != _activeDeviceMode )
  {
    Serial.print("DeviceMode::setDeviceMode ");
    Serial.println((int)newDeviceMode);
    _activeDeviceMode = newDeviceMode;
    // Save to prefs
    int tmpMode = (int)_activeDeviceMode;
    preferences.putUInt("DeviceMode", tmpMode);
    Serial.print("Wrote DeviceMode to prefs ");
    Serial.println(tmpMode);
  }
}

void State::setDeviceSubMode( int newDeviceMode )
{
  int statePosition = (static_cast<int>(_activeDeviceMode)+LEDMODES) * STATESIZE;
  int subStatePosition = newDeviceMode*STATEWIDTH;

  if( _activeSubState != newDeviceMode )
  {
    fadeBrightness = 255;
    /*
    Serial.print(millis());
    Serial.print(", ");
    Serial.print(statePosition);
    Serial.print(", ");
    Serial.print(statePosition+subStatePosition+0);
    Serial.print(", ");
    Serial.print(statePosition+subStatePosition+1);
    Serial.print(", ");
    Serial.print(statePosition+subStatePosition+2);
    Serial.print(", ");
    
    Serial.print("State::setDeviceSubMode ");
    Serial.println(newDeviceMode);
    */
  }

  // Always fade...
  int r = ALLSTATES[statePosition+subStatePosition+0] > 10 ? fadeBrightness : 5;
  int g = ALLSTATES[statePosition+subStatePosition+1] > 10 ? fadeBrightness : 5;
  int b = ALLSTATES[statePosition+subStatePosition+2] > 10 ? fadeBrightness : 5;
  setPixels(r, g, b);
  fadeBrightness -= 1;
  fadeBrightness < 0 ? fadeBrightness = 0 : fadeBrightness;
  
  _activeSubState = newDeviceMode;
}

DeviceMode State::getDeviceMode()
{
  return _activeDeviceMode;
}

void State::setPixels(uint8_t r, uint8_t g, uint8_t b)
{
  for(int i=0;i<_numLeds;i++)
  {
    pixels->setPixelColor(i, r, g, b);
  }
  pixels->show();
}

LedMode State::intToLedMode( int state )
{
  switch( state )
  {
    case 0: return LedMode::OFF;
    case 1: return LedMode::RGB_DEFAULT;
    case 2: return LedMode::DESIGNER;
    case 3: return LedMode::FRENCH;
    case 4: return LedMode::AFRIKAN;
    default: return LedMode::RGB_DEFAULT;
  }
}

DeviceMode State::intToDeviceMode( int state )
{
  switch( state )
  {
    case 0: return DeviceMode::DEFAULT_SLOW;
    case 1: return DeviceMode::DEFAULT_MEDIUM;
    case 2: return DeviceMode::DEFAULT_FAST;
    case 3: return DeviceMode::BOTH_SLOW;
    case 4: return DeviceMode::BOTH_MEDIUM;
    case 5: return DeviceMode::BOTH_FAST;
    default: return DeviceMode::DEFAULT_MEDIUM;
  }
}