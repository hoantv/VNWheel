#include "WHID.h"
#include "FfbWheelDescriptor.h"
#include "FfbWheel.h"


Wheel_::Wheel_(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

void Wheel_::SendReport(void* data, int length) {
  HID().SendReport(HID_REPORTID_WHEEL, data, length);
}

int Wheel_::RecvReport(void* data, int len)
{
  return HID().RecvReport(&data, len);
}

uint8_t Wheel_::AvailableReport()
{
  return HID().AvailableReport();
}

void Wheel_::RecvFfbReport() {
  HID().RecvFfbReport();
}

void Wheel_::begin(void) {
  // release all buttons
  ffbEngine.SetFfb(&HID().ffbReportHandler);
  ffbEngine.SetGain(wheelConfig);
  encoder.setConfig(wheelConfig);

  end();
}

void Wheel_::end(void) {
  memset(&_wheelReport, 0x00, sizeof(_wheelReport));
  SendReport(&_wheelReport, sizeof(_wheelReport));
}

void Wheel_::write(void) {
  SendReport(&_wheelReport, sizeof(_wheelReport));
}


void Wheel_::press(uint8_t b) {
  _wheelReport.buttons |= (uint8_t)1 << (b - 1);
}


void Wheel_::release(uint8_t b) {
  _wheelReport.buttons &= ~((uint8_t)1 << (b - 1));
}


void Wheel_::releaseAll(void) {
  memset(&_wheelReport, 0x00, sizeof(_wheelReport));
}

void Wheel_::buttons(uint8_t b) {
  _wheelReport.buttons = b;
}


void Wheel_::xAxis(int16_t a) {
  _wheelReport.xAxis = a;
}


void Wheel_::yAxis(int16_t a) {
  _wheelReport.yAxis = a;
}


void Wheel_::zAxis(int16_t a) {
  _wheelReport.zAxis = a;
}
void Wheel_::rxAxis(int16_t a) {
  _wheelReport.rxAxis = a;
}
