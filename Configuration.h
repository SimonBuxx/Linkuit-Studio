#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QColor>
#include <QFont>
#include <QRectF>

#include "HelperStructures.h"

static constexpr uint8_t MAX_UNDO_STACK_SIZE = 100;

namespace gui
{
static constexpr const char* WINDOW_TITLE = "LS22 - Logic Simulator 22";
} // namespace

namespace canvas
{
static constexpr QColor BACKGROUND_COLOR(0, 39, 43);
static constexpr QColor GRID_COLOR(0, 45, 50);

static constexpr uint8_t GRID_SIZE = 20;

static constexpr uint8_t ZOOM_SPEED = 5;
static constexpr int32_t DEFAULT_ZOOM_LEVEL = 250;
static constexpr int32_t MIN_ZOOM_LEVEL = 90;
static constexpr int32_t MAX_ZOOM_LEVEL = 300;

static constexpr int32_t MIN_GRID_ZOOM_LEVEL = 200;

static constexpr QRectF DIMENSIONS(-16000, -16000, 32000, 32000);
} // namespace

namespace components
{
static constexpr QColor FILL_COLOR(0, 88, 61);
static constexpr QColor BORDER_COLOR(0, 88, 61);
static constexpr QColor SELECTED_BORDER_COLOR(255, 255, 255);

static constexpr uint8_t BORDER_WIDTH = 2;

static constexpr double ROUNDED_CORNERS_MIN_LOD = 0.5f;
static constexpr double DESCRIPTION_TEXT_MIN_LOD = 0.25f;
static constexpr double CONNECTORS_MIN_LOD = 0.2f;
static constexpr Direction DEFAULT_DIRECTION = Direction::RIGHT;

namespace gates
{
static constexpr const char* AND_TEXT = "&";
static constexpr const char* OR_TEXT = "≥1";
static constexpr const char* XOR_TEXT = "=1";
static constexpr const char* NOT_TEXT = "1";

static constexpr uint8_t DEFAULT_INPUT_COUNT = 2;
static constexpr uint8_t GRID_WIDTH = 3;

static constexpr int32_t Z_VALUE = 100;

static const QFont FONT("Calibri Light", 25);
} // namespace

namespace inputs
{
static constexpr int32_t Z_VALUE = 110;
} // namespace

namespace wires
{
static constexpr int32_t Z_VALUE = 80;

// The height or width of the wire bounding rect
static constexpr uint8_t BOUNDING_RECT_SIZE = 20;

} // namespace
} // namespace

#endif // CONFIGURATION_H
