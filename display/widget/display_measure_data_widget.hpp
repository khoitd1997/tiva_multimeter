#pragma once

#include "display_base_widget.hpp"

#include "action_def.hpp"

class DisplayMeasureDataWidget : public DisplayBaseWidget {
 public:
  DisplayMeasureDataWidget(const DisplayWidgetDimension& dimension, const fontSetDesc& font);

  void draw(const MeasureAction action, const float value);

 private:
  char                     _buf[30] = {0};  // scratch buffer
  static const char* const _actionUnit[];

  const char* actionToUnit(const MeasureAction action);
};