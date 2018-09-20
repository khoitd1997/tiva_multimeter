#include "adc_sensor.hpp"

// system header
#include <cassert>
#include <cstdint>

// peripheral
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

// hardware
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

// application
#include "hardware_helper/hardware_helper.hpp"
#include "tiva_utils/bit_manipulation.h"

// freeRTOS
#include "FreeRTOS.h"
#include "task.h"

// parameter defining specs and hard coded configs of the adc
static const uint32_t ADC_RESOLUTION      = 4096;
static const float    ADC_MAX_VOLTAGE     = 3.3;
static const uint32_t OVERSAMPLING_FACTOR = 2;
static const float    ADC_COEFF           = ADC_MAX_VOLTAGE / ADC_RESOLUTION;

AdcSensor::AdcSensor(const uint32_t& adcModuleNum,
                     const uint32_t& adcSequencer,
                     const char&     adcPinPort,
                     const uint32_t& adcPinNum,
                     const uint32_t& adcPriority)
    : _adcModNum(adcModuleNum),
      _adcAddr(adcAddrFromName(adcModuleNum)),
      _adcPinClockAddr(gpioPeriAddrFromName(adcPinPort)),
      _portAddr(gpioPortAddrFromName(adcPinPort)),
      _pinBitMask(gpioMaskFromName(adcPinNum)),
      _adcSequencer(adcSequencer),
      _adcTotalSequence(adcTotalSequenceFromSequencer(adcSequencer)),
      _adcChannelMask(adcChannelMaskFromName(adcPinNum, adcPinPort)),
      _adcPriority(adcPriority),
      _adcPeriphClockAddr(adcPeriphAddrByName(adcModuleNum)),
      _adcBuffer({0}) {
  assert(adcPriority < 4);
}

void AdcSensor::init(uint32_t adcTriggerFlag, bool nonLastDataInt, bool lastDataInt) {
  _adcTriggerFlag = adcTriggerFlag;
  SysCtlPeripheralEnable(_adcPeriphClockAddr);
  while (!SysCtlPeripheralReady(_adcPeriphClockAddr)) {
    // wait for ADC to be ready
  }

  // enable analog pin
  SysCtlPeripheralEnable(_adcPinClockAddr);
  GPIOPinTypeADC(_portAddr, _pinBitMask);

  ADCSequenceConfigure(_adcAddr, _adcSequencer, adcTriggerFlag, _adcPriority);
  // ADCHardwareOversampleConfigure(_adcAddr, OVERSAMPLING_FACTOR);

  uint32_t nonLastDataIntFlag = (nonLastDataInt ? ADC_CTL_IE : 0);

  // configure individual sample in a sequence
  uint32_t sampleNum = 0;
  for (sampleNum = 0; sampleNum < _adcTotalSequence - 1; ++sampleNum) {
    ADCSequenceStepConfigure(
        _adcAddr, _adcSequencer, sampleNum, _adcChannelMask | nonLastDataIntFlag);
  }

  uint32_t lastDataIntFlag = (lastDataInt ? ADC_CTL_IE : 0);
  ADCSequenceStepConfigure(
      _adcAddr, _adcSequencer, sampleNum, ADC_CTL_END | _adcChannelMask | lastDataIntFlag);
}

void AdcSensor::enable(void) {
  ADCIntClear(_adcAddr, _adcSequencer);
  ADCSequenceEnable(_adcAddr, _adcSequencer);
}

void AdcSensor::disable(void) {
  while (ADCBusy(_adcAddr)) {
    // wait until the current sampling is done
  }
  ADCSequenceDisable(_adcAddr, _adcSequencer);
}

float AdcSensor::convertRawToVolt(uint32_t* adcBuffer) {
  uint32_t result = 0;
  for (uint32_t adcIndex = 0; adcIndex < _adcTotalSequence; ++adcIndex) {
    result += adcBuffer[adcIndex];
  }
  return (ADC_COEFF * result) / (_adcTotalSequence);
}

// TODO: optimize reading volt
// Use both sampler with DMA, interrupts
float AdcSensor::readVolt(void) {
  while (ADCBusy(_adcAddr)) {
    // wait until the current sampling is done
  }
  ADCProcessorTrigger(_adcAddr, _adcSequencer);

  while (!ADCIntStatus(_adcAddr, _adcSequencer, false)) {
    // Wait until the sample sequence has completed.
  }

  // clear interrupt flag after read
  ADCIntClear(_adcAddr, _adcSequencer);

  // Read raw value from the ADC.
  ADCSequenceDataGet(_adcAddr, _adcSequencer, _adcBuffer);

  return convertRawToVolt(_adcBuffer);
}

void AdcSensor::adcEnableDMA() {
  ADCIntClearEx(_adcAddr, adcDmaIntFlagFromSequencer(_adcSequencer));
  ADCIntEnableEx(_adcAddr, adcDmaIntFlagFromSequencer(_adcSequencer));
  ADCSequenceDMAEnable(_adcAddr, _adcSequencer);
}

uint32_t AdcSensor::getAdcAddr(void) const { return _adcAddr; }
uint32_t AdcSensor::getAdcSequencer(void) const { return _adcSequencer; }
uint32_t AdcSensor::getAdcTotalSequence(void) const { return _adcTotalSequence; }
uint32_t AdcSensor::getAdcFifoAddr(void) const {
  return _adcAddr + adcFifoOffsetFromName(_adcSequencer);
}

uint32_t AdcSensor::getAdcModNum(void) const { return _adcModNum; }