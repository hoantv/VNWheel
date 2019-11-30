#ifndef WHEELCONFIG_h
#define WHEELCONFIG_h

#include <Arduino.h>

#define PULSE_DIR 1
#define PULSE_POS_NEG 2
/* Initial config defines */
#define InitialConfigNotDone 0

class WheelConfig {
  public:
    WheelConfig(void);
    ~WheelConfig(void);
    uint32_t configCPR;
    uint16_t configMaxAngle;    
    bool configInverted;
    bool configUsePinZ;    
    bool configResetEncoderPosition;
    uint8_t controlMode;
    void SetDefault();

};

#endif
