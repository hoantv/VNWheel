#ifndef WHEELCONFIG_h
#define WHEELCONFIG_h

#include <Arduino.h>

/* Wheel indexing modes */
#define startAtCenterPhasedIndexing 1

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

};

#endif
