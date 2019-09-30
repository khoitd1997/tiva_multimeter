
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

// tasks
#include "core_sensor_manager.hpp"
#include "display_manager.hpp"
#include "extra_sensor_manager.hpp"
#include "user_input_manager.hpp"

#include "freeRTOS_hook.h"
#include "swo_segger.h"
#include "uart_util.hpp"
#include "user_input_manager.hpp"

#include "action_def.hpp"

#define UART_BAUD 115200

#ifdef DEBUG
void __error__(char* pcFilename, uint32_t ui32Line) {}

#endif

int main(void) {
  // 80 MHz
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

  uartConfigure(UART_BAUD);

  SWO_PrintStringLine("Initializing tasks");
  static ExtraSensorManager extraSensorManager{configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 8};
  static DisplayManager     displayManager{
      configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 4, nullptr, 0};
  static CoreSensorManager coreSensorManager{configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 6};
  static UserInputManager  inputManager(
      {{displayManager.inputEventQueue, ActionCategory::BRIGHTNESS},
       {coreSensorManager.inputEventQueue, ActionCategory::MEASURE}});

  SWO_PrintStringLine("Preparing to start scheduler");
  vTaskStartScheduler();

  SWO_PrintStringLine("Scheduler failed\n");

  for (;;) {}
}
