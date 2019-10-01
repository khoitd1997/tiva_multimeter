#include "display_measure_title_widget.hpp"

#include "ssd1306.h"

const char* const DisplayMeasureTitleWidget::_actionTitle[] = {
    "AC Volt", "DC Volt", "Current", "Resistance"};

DisplayMeasureTitleWidget::DisplayMeasureTitleWidget(const DisplayWidgetDimension& dimension,
                                                     const fontSetDesc&            font)
    : DisplayBaseWidget{dimension, font} {}

void DisplayMeasureTitleWidget::draw(const MeasureAction action) {
  if (action != _prevAction) {
    updateDisplay(actionToTitle(action));
    _prevAction = action;
  }
}

const char* DisplayMeasureTitleWidget::actionToTitle(const MeasureAction action) {
  const auto index = action - MeasureAction::FIRST_MEASURE_ACTION;
  return _actionTitle[index];
}