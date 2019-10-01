#include "display_base_widget.hpp"

#include <cstring>

DisplayBaseWidget::DisplayBaseWidget(const DisplayWidgetDimension& dimension,
                                     const fontSetDesc&            font)
    : _dimension{dimension}, _font{font}, _clearStr(dimension.length, ' ') {
  _buf.reserve(dimension.length + 1);
}
DisplayBaseWidget::~DisplayBaseWidget() {}

void DisplayBaseWidget::updateDisplay(const char* str) {
  _buf = _clearStr;

  auto counter = 0;
  while (str[counter]) {
    _buf[counter] = str[counter];
    ++counter;
  }

  ssd1306PrintString(_buf.c_str(), _dimension.lineNum, _dimension.colNum, _font);
}

void DisplayBaseWidget::clear() {
  ssd1306PrintString(_clearStr.c_str(), _dimension.lineNum, _dimension.colNum, _font);
}