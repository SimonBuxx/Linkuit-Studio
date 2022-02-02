#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QColor>
#include <QFont>
#include <QRectF>

#include "HelperStructures.h"

constexpr uint8_t MAX_UNDO_STACK_SIZE = 50;

namespace gui
{
constexpr const char* WINDOW_TITLE = "LS22 - Logic Simulator 22";
} // namespace

namespace canvas
{
constexpr QColor BACKGROUND_COLOR(240, 240, 240);
constexpr QColor GRID_COLOR(230, 230, 230);

constexpr uint8_t GRID_STROKE_WIDTH = 3;
constexpr uint8_t GRID_SIZE = 20;

constexpr uint8_t ZOOM_SPEED = 6;
constexpr int32_t DEFAULT_ZOOM_LEVEL = 250;
constexpr int32_t MIN_ZOOM_LEVEL = 75;
constexpr int32_t MAX_ZOOM_LEVEL = 300;

constexpr int32_t MIN_GRID_ZOOM_LEVEL = 200;

constexpr QRectF DIMENSIONS(-16000, -16000, 32000, 32000);
} // namespace

namespace components
{
constexpr QColor FILL_COLOR(255, 255, 255);
constexpr QColor BORDER_COLOR(0, 0, 0);
constexpr QColor SELECTED_BORDER_COLOR(200, 50, 50);
constexpr QColor HOVER_BORDER_COLOR(0, 0, 0);

constexpr uint8_t BORDER_WIDTH = 3;

constexpr double TEXT_MIN_LEVEL_OF_DETAIL = 0.2f;
constexpr Direction DEFAULT_DIRECTION = Direction::RIGHT;

namespace gates
{
constexpr const char* AND_TEXT = "&";
constexpr const char* OR_TEXT = "≥1";
constexpr const char* XOR_TEXT = "=1";

constexpr uint8_t DEFAULT_INPUT_COUNT = 2;
constexpr uint8_t GRID_WIDTH = 3;

const QFont FONT("Helvetica", 30);
} // namespace
} // namespace

#endif // CONFIGURATION_H
