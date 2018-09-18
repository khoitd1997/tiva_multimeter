#ifndef _ADC_MUL_H
#define _ADC_MUL_H

#include <cstdint>

static const uint8_t MAX_SEQUENCE_SAMPLE = 8;
static const float   ADC_MARGIN_ERROR    = 0.08;  // experimental value
class AdcSensor {
 private:
  uint32_t _adcAddr;
  uint32_t _clockPeriPortAddr;
  uint32_t _portAddr;
  uint8_t  _pinBitMask;
  uint8_t  _adcTotalSequence;
  uint8_t  _adcSequencer;
  uint8_t  _adcChannelMask;
  uint8_t  _adcPriority;
  uint32_t _adcPeriphAddr;
  uint32_t _adcResult[MAX_SEQUENCE_SAMPLE];
  float    convertRawToVolt(void);

 public:
  AdcSensor(const uint8_t& adcModuleNum,
            const uint8_t& adcSequencer,
            const char&    adcPinPort,
            const uint8_t& adcPinNum,
            const uint8_t& adcPriority);

  virtual float readVolt(void);
  virtual void  init(void);
  virtual ~AdcSensor(void) {}
};

#endif