#ifndef WHEELCONFIG_h
#define WHEELCONFIG_h

#include <Arduino.h>

#define PULSE_DIR 1
#define PWM_POS_NEG 2
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
    uint8_t constantGainConfig;
    uint8_t rampGainConfig;
    uint8_t squareGainConfig;
    uint8_t sinGainConfig;
    uint8_t triangleGainConfig;
    uint8_t sawToothDownGainConfig;
    uint8_t sawToothUpGainConfig;
    uint8_t springGainConfig;
    uint8_t damperGainConfig;
    uint8_t inertiaGainConfig;
    uint8_t frictionGainConfig;
    uint8_t totalGainConfig;
 

};

#endif
