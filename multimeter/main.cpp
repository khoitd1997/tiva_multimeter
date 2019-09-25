
#include <stdbool.h>
#include <stdint.h>
#include <cassert>
#include <cstdio>

// FreeRTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// peripheral
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// hardware
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

// application
#include "core_sensor/core_sensor_manager.hpp"
#include "display/display_manager.hpp"
#include "freeRTOS_hook.h"
#include "input_handler.hpp"
#include "uart_util.hpp"

#define UART_BAUD 115200

#ifdef DEBUG
void __error__(char* pcFilename, uint32_t ui32Line) {}

#endif

// TODO: Change priority after test
UBaseType_t CORE_SENSOR_PRIORITY = configMAX_PRIORITIES - 6;
UBaseType_t DISPLAY_PRIORITY     = configMAX_PRIORITIES - 4;

int main(void) {
  // 80 MHz
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

  uartConfigure(UART_BAUD);

  UARTprintf("Creating tasks\n");

  //   auto sensorManagerTask = MainSensorManager::getTask(CORE_SENSOR_PRIORITY);
  //   InputHandler::create(sensorManagerTask);
  DisplayManager::create(DISPLAY_PRIORITY, NULL, 0);

  UARTprintf("Preparing to start scheduler\n");
  vTaskStartScheduler();

  UARTprintf("Scheduler failed\n");

  for (;;) {}
}
