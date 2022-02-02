#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QColor>
#include <QFont>

#include "HelperStructures.h"

constexpr uint8_t MAX_UNDO_STACK_SIZE = 20;

namespace gui
{
constexpr const char* WINDOW_TITLE = "Welcome to LS22";
} // namespace

namespace canvas
{
const QColor BACKGROUND_COLOR(QColor(240, 240, 240));
const QColor GRID_COLOR(QColor(230, 230, 230));

const uint8_t GRID_STROKE_WIDTH = 3;
constexpr uint8_t GRID_SIZE = 20;

constexpr uint8_t ZOOM_SPEED = 6;
constexpr int32_t DEFAULT_ZOOM_LEVEL = 250;
constexpr int32_t MIN_ZOOM_LEVEL = 100;
constexpr int32_t MAX_ZOOM_LEVEL = 300;

constexpr int32_t MIN_GRID_ZOOM_LEVEL = 200;
} // namespace

namespace components
{
const QColor FILL_COLOR(Qt::white);
const QColor BORDER_COLOR(Qt::black);
const QColor SELECTED_BORDER_COLOR(200, 50, 50);
const QColor HOVER_BORDER_COLOR(Qt::black);

constexpr uint8_t BORDER_WIDTH = 3;

constexpr const double TEXT_MIN_LEVEL_OF_DETAIL = 0.2f;
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
