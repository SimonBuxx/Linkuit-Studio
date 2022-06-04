#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QColor>
#include <QFont>
#include <QRectF>

#include "HelperStructures.h"

static constexpr uint8_t MAX_UNDO_STACK_SIZE = 100;

static constexpr const char* SW_VERSION_STRING = "0.6.0alpha";

namespace gui
{
static constexpr const char* WINDOW_TITLE = "Linkuit Studio - Unnamed Circuit";

static constexpr uint32_t HORIZONTAL_SCROLL_SPEED = 30;

static constexpr std::chrono::milliseconds PROCESSING_OVERLAY_TIMEOUT(100);
} // namespace

namespace canvas
{
static constexpr QColor BACKGROUND_COLOR(0, 39, 43);
//static constexpr QColor GRID_COLOR(0, 39, 43);
static constexpr QColor GRID_COLOR(0, 45, 50);

static constexpr uint8_t GRID_SIZE = 20;

static constexpr uint8_t ZOOM_SPEED = 5;
static constexpr int32_t DEFAULT_ZOOM_LEVEL = 250;
static constexpr int32_t MIN_ZOOM_LEVEL = 90;
static constexpr int32_t MAX_ZOOM_LEVEL = 300;

static constexpr int32_t MIN_GRID_ZOOM_LEVEL = 200;

static constexpr QRectF DIMENSIONS(-32000, -32000, 64000, 64000);
} // namespace

namespace simulation
{
static constexpr std::chrono::milliseconds PROPAGATION_DELAY(10);
} // namespace

namespace components
{
enum zvalues
{
    WIRE = 0,
    CONPOINT,
    GATE,
    INPUT,
    OUTPUT,
    TEXT_LABEL,
    FRONT
};

static constexpr QColor FILL_COLOR(0, 100, 70);
static constexpr QColor SELECTED_BORDER_COLOR(255, 255, 255);
static constexpr QColor SELECTED_COMPONENT_COLOR(255, 255, 255);
static constexpr QColor HIGH_COLOR(0, 204, 143);

static constexpr uint8_t BORDER_WIDTH = 2;

// Minimal levels of detail to show component details
static constexpr double SMALL_TEXT_MIN_LOD = 0.4f;
static constexpr double DESCRIPTION_TEXT_MIN_LOD = 0.3f;
static constexpr double COMPONENT_DETAILS_MIN_LOD = 0.2f;

static constexpr Direction DEFAULT_DIRECTION = Direction::RIGHT;

namespace gates
{
static constexpr const char* AND_TEXT = "&";
static constexpr const char* OR_TEXT = "≥1";
static constexpr const char* XOR_TEXT = "=1";
static constexpr const char* NOT_TEXT = "1";
static constexpr const char* BUFFER_TEXT = "1";

static constexpr uint8_t DEFAULT_INPUT_COUNT = 2;
static constexpr uint8_t GRID_WIDTH = 3;

static const QFont FONT("Calibri Light", 25);
static constexpr QColor FONT_COLOR(255, 255, 255);
} // namespace

namespace wires
{
// The height or width of the wire bounding rect
static constexpr uint8_t BOUNDING_RECT_SIZE = 20;
static constexpr QColor WIRE_LOW_COLOR(0, 64, 45);
static constexpr QColor WIRE_HIGH_COLOR(0, 143, 100);
static constexpr uint8_t WIRE_WIDTH = 2;
} // namespace

namespace conpoints
{
static constexpr double MIN_VISIBLE_LOD = 0.2f;
static constexpr QColor CONPOINTS_LOW_COLOR(0, 88, 61);
static constexpr QColor CONPOINTS_HIGH_COLOR(0, 204, 143);
static constexpr uint32_t SIZE = 6;
} // namespace

namespace diodes
{
static constexpr uint32_t SIZE = 15;
} // namespace

namespace text_label
{
static const QFont FONT("Quicksand Medium", 10);
static constexpr QColor FONT_COLOR(255, 255, 255);
} // namespace

namespace inputs
{
static constexpr std::chrono::milliseconds BUTTON_TOGGLE_INTERVAL(50 * simulation::PROPAGATION_DELAY);

// Whether the clock should toggle at timeout or pulse for CLOCK_PULSE_TICKS ticks
static constexpr bool CLOCK_ENABLE_PULSE = true;

static constexpr uint32_t CLOCK_PULSE_TICKS = 10;

// The amount of ticks to pass before the clock timeouts
static constexpr uint32_t CLOCK_TOGGLE_TICKS = 100;
} // namespace

namespace multiplexer
{
static constexpr uint8_t MAX_INPUT_DIGITS = 5;
} // namespace

namespace complex_logic
{
static constexpr const char* HALF_ADDER_TEXT = "HA";
static constexpr const char* FULL_ADDER_TEXT = "FA";
static constexpr const char* RS_FLIPFLOP_TEXT = "RS";
static constexpr const char* D_FLIPFLOP_TEXT = "FF";
static constexpr const char* MULTIPLEXER_TEXT = "MUX";
static constexpr const char* DEMULTIPLEXER_TEXT = "DEMUX";

static const QFont FONT("Source Sans Pro", 20);
static const QFont CONNECTOR_FONT("Calibri Light", 10);
static constexpr QColor FONT_COLOR(255, 255, 255);
static constexpr QColor SMALL_TEXT_FONT_COLOR(225, 225, 225);
} // namespace

} // namespace

#endif // CONFIGURATION_H
