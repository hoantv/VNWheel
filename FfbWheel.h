#ifndef WHEEL_h
#define WHEEL_h

#include "WHID.h"
#include "Encoder.h"
#include "FfbEngine.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

class Wheel_
{
  private:
    WheelReport _wheelReport;
    void SendReport(void* data, int len);

  public:
    Wheel_(void);

    void begin(void);
    void end(void);
    void write(void);
    void press(uint8_t b);
    void release(uint8_t b);
    void releaseAll(void);
    void buttons(uint8_t b);
    void xAxis(int16_t a);
    void yAxis(int16_t a);
    void zAxis(int16_t a);
    void rxAxis(int16_t a);
    uint8_t AvailableReport();
    int  RecvReport(void* data, int len);
    void RecvFfbReport();
    WheelConfig wheelConfig;
    Encoder encoder;    
    FfbEngine ffbEngine;

};
extern Wheel_ Wheel;

#endif
#endif
