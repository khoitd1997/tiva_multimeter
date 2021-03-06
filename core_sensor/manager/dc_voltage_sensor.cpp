#include "dc_voltage_sensor.hpp"

// peripheral
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/udma.h"

// hardware
#include "inc/hw_adc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

// voltage divider value in ohm
// TODO: Finalize resistor values
static const float R1 = 3000;
static const float R2 = 4000;
// static const float DC_COEFF = (R1 + R2) / R1;
// TODO: Change the coeff back after testing
static const float DC_COEFF = 1;

const uint32_t DC_SAMPLING_PERIOD_MS = 10;

DcVoltageSensor::DcVoltageSensor()
    : Sensor(SensorType::DC_VOLT,
             DC_SAMPLING_PERIOD_MS,
             SYSCTL_PERIPH_GPIOB,
             GPIO_PORTB_BASE,
             GPIO_PIN_4,
             true),
      _adc(ADC0_BASE, 8, 0) {}

float DcVoltageSensor::read(void) { return _adc.read() * DC_COEFF; }
void  DcVoltageSensor::init(void) {
  _adc.init(SYSCTL_PERIPH_ADC0, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_3, ADC_CTL_CH0, 64);
}
void DcVoltageSensor::disableCallback() { _adc.disable(); }
void DcVoltageSensor::enableCallback() { _adc.enable(); }