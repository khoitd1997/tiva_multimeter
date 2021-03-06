#include "user_input_manager.hpp"

#include <cstdbool>
#include <cstdint>
#include <cstring>

#include <algorithm>

#include "action_def.hpp"

#include "swo_segger.h"

static UserInputManager* manager = nullptr;
UserInputManager::UserInputManager() { manager = this; }
void UserInputManager::setSubcriptions(const std::vector<UserInputEventSubReq>& reqs) {
  _subs = reqs;
}

void UserInputManager::measureModeHandler(const bool isClockwise) {
  static auto       currMode  = MeasureAction::STARTUP_MEASURE_ACTION;
  static TickType_t lastInput = 0;
  const auto        currTick  = xTaskGetTickCountFromISR();
  if ((currTick - lastInput) > kRotaryEncoderDebounce) {
    BaseType_t higherTaskWoken = pdFALSE;
    const auto prevMode        = currMode;
    lastInput                  = currTick;
    // SWO_PrintStringLine("inside rotary interrupt");

    if (isClockwise) {
      currMode = static_cast<MeasureAction>(currMode + 1);
      currMode = static_cast<MeasureAction>(currMode > MeasureAction::LAST_MEASURE_ACTION
                                                ? MeasureAction::FIRST_MEASURE_ACTION
                                                : currMode);
    } else {
      currMode = static_cast<MeasureAction>(currMode - 1);
      currMode = static_cast<MeasureAction>(currMode < MeasureAction::FIRST_MEASURE_ACTION
                                                ? MeasureAction::LAST_MEASURE_ACTION
                                                : currMode);
    }

    if (prevMode != currMode) {
      // SWO_PrintStringLine("notifying subscribers");
      manager->notifySubscriber(currMode, &higherTaskWoken);
    }
    portYIELD_FROM_ISR(higherTaskWoken);
  }
}

void UserInputManager::brightnessHandler(const uint32_t intStatus) {
  static TickType_t lastInput = 0;
  const auto        currTick  = xTaskGetTickCountFromISR();
  if ((currTick - lastInput) > kBrightnessDebounce) {
    // SWO_PrintStringLine("handling brightness input");
    lastInput = currTick;

    BaseType_t       higherTaskWoken = pdFALSE;
    BrightnessAction type;

    if (bit_get(intStatus, kBrightnessCtrlButtons)) {
      if (bit_get(intStatus, kBrightnessIncButton)) {
        type = BrightnessAction::BRIGHTNESS_INC;
      } else if (bit_get(intStatus, kBrightnessDecButton)) {
        type = BrightnessAction::BRIGHTNESS_DEC;
      } else {
        return;
      }
    }

    manager->notifySubscriber(type, &higherTaskWoken);
    portYIELD_FROM_ISR(higherTaskWoken);
  }
}

void UserInputManager::bluetoothHandler(const uint32_t intStatus) {
  static TickType_t lastInput = 0;
  const auto        currTick  = xTaskGetTickCountFromISR();

  static auto currBluetoothMode = BluetoothAction::STARTUP_BLUETOOTH_ACTION;

  if ((currTick - lastInput) > kBluetoothDebounce) {
    SWO_PrintStringLine("handling bluetooth input");
    lastInput = currTick;

    auto higherTaskWoken = pdFALSE;

    if (bit_get(intStatus, kBluetoothButton)) {
      currBluetoothMode =
          ((currBluetoothMode == BluetoothAction::BLUETOOTH_OFF) ? BluetoothAction::BLUETOOTH_ON
                                                                 : BluetoothAction::BLUETOOTH_OFF);
    }

    manager->notifySubscriber(currBluetoothMode, &higherTaskWoken);
    portYIELD_FROM_ISR(higherTaskWoken);
  }
}