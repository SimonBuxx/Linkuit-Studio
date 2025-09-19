#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QColor>
#include <QFont>
#include <QRectF>
#include <chrono>

#include "HelperStructures.h"

static constexpr uint8_t MAX_UNDO_STACK_SIZE = 100;

static auto versionList = QString(FULL_VERSION).split('.');

static uint8_t MAJOR_VERSION = versionList.at(0).toInt();
static uint8_t MINOR_VERSION = versionList.at(1).toInt();
static uint8_t PATCH_VERSION = versionList.at(2).toInt();
static uint8_t BUILD_VERSION = versionList.at(3).toInt();

namespace file
{
static constexpr const SaveFormat SAVE_FORMAT = SaveFormat::BINARY;

static constexpr auto JSON_COMPONENTS_IDENTIFIER = "components";
static constexpr auto JSON_TYPE_IDENTIFIER = "type";
static constexpr auto JSON_MAJOR_VERSION_IDENTIFIER = "major";
static constexpr auto JSON_MINOR_VERSION_IDENTIFIER = "minor";
static constexpr auto JSON_PATCH_VERSION_IDENTIFIER = "patch";

static constexpr auto JSON_COMPATIBLE_MAJOR_VERSION_IDENTIFIER = "comp_major";
static constexpr auto JSON_COMPATIBLE_MINOR_VERSION_IDENTIFIER = "comp_minor";
static constexpr auto JSON_COMPATIBLE_PATCH_VERSION_IDENTIFIER = "comp_patch";

namespace runtime_config
{
static constexpr auto RUNTIME_CONFIG_RELATIVE_PATH = "/runtime_config.json";

static constexpr auto JSON_WELCOME_DIALOG_ENABLED_IDENTIFIER = "show_welcome_dialog_on_startup";
static constexpr auto JSON_RECENT_FILES_IDENTIFIER = "recent_files";
static constexpr auto JSON_LAST_FILE_PATH_IDENTIFIER = "last_file_path";
} // namespace runtime_config
} // namespace

namespace gui
{
static constexpr auto DEFAULT_WINDOW_TITLE = "Linkuit Studio - Untitled";
static constexpr auto OPEN_FILE_DIALOG_TITLE = "Open Circuit File";
static constexpr auto SAVE_FILE_DIALOG_TITLE = "Save Circuit File";

static constexpr bool SHOW_WELCOME_DIALOG_ON_STARTUP = true;
static constexpr std::chrono::milliseconds SHOW_WELCOME_DIALOG_DELAY{300};


static constexpr std::chrono::milliseconds PROCESSING_OVERLAY_TIMEOUT{100};

static constexpr bool FADE_OUT_GUI_ON_CTRL = true;
static constexpr std::chrono::milliseconds FADE_OUT_GUI_TIMEOUT{1000};

static constexpr uint32_t FADE_ANIMATION_DURATION{200};
static constexpr uint32_t PIE_MENU_ANIMATION_DURATION{100};
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
static constexpr std::chrono::milliseconds PROPAGATION_DELAY(1);
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
static constexpr auto AND_TEXT = "&";
static constexpr auto OR_TEXT = "≥1";
static constexpr auto XOR_TEXT = "=1";
static constexpr auto NOT_TEXT = "1";
static constexpr auto BUFFER_TEXT = "1";

static constexpr uint8_t DEFAULT_INPUT_COUNT = 2;
static constexpr uint8_t MIN_INPUT_COUNT = 1;
static constexpr uint8_t MAX_INPUT_COUNT = 9;
static constexpr uint8_t GRID_WIDTH = 3;

static const QFont FONT("Calibri", 25, QFont::Light);
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
static const QFont FONT("Quicksand", 12, QFont::Medium);
static constexpr QColor FONT_COLOR(255, 255, 255);
} // namespace

namespace inputs
{
static constexpr uint32_t BUTTON_TOGGLE_TICKS(500);

// The default clock mode (whether the clock should toggle at timeout or pulse)
static constexpr ClockMode DEFAULT_CLOCK_MODE = ClockMode::PULSE;

// The default pulse duration
static constexpr uint32_t DEFAULT_CLOCK_PULSE_TICKS = 100;

// The default amount of ticks to pass before the clock timeouts
static constexpr uint32_t DEFAULT_CLOCK_TOGGLE_TICKS = 500;

namespace constant
{
static constexpr LogicState DEFAULT_STATE = LogicState::HIGH;
static const QFont FONT("Quicksand", 10, QFont::Medium);
static constexpr QColor LOW_FONT_COLOR(255, 255, 255);
static constexpr QColor HIGH_FONT_COLOR(255, 255, 255);
}
} // namespace

namespace outputs
{
static constexpr QColor DEFAULT_COLOR_LOW(0, 100, 70);
static constexpr QColor DEFAULT_COLOR_HIGH(0, 204, 143);
static constexpr QColor GREEN_COLOR_LOW(22, 100, 22);
static constexpr QColor GREEN_COLOR_HIGH(0, 255, 0);
static constexpr QColor RED_COLOR_LOW(131, 33, 33);
static constexpr QColor RED_COLOR_HIGH(200, 50, 50);
static constexpr QColor BLUE_COLOR_LOW(5, 51, 97);
static constexpr QColor BLUE_COLOR_HIGH(10, 100, 190);
static constexpr QColor YELLOW_COLOR_LOW(134, 134, 9);
static constexpr QColor YELLOW_COLOR_HIGH(240, 240, 50);
} // namespace

namespace multiplexer
{
static constexpr uint8_t MIN_BIT_WIDTH = 1;
static constexpr uint8_t MAX_BIT_WIDTH = 5;
static constexpr uint8_t DEFAULT_BIT_WIDTH = 2;
} // namespace

namespace encoder_decoder
{
static constexpr uint8_t MIN_INPUT_COUNT = 3;
static constexpr uint8_t MAX_INPUT_COUNT = 5;
static constexpr uint8_t DEFAULT_INPUT_COUNT = 3;
} // namespace

namespace shift_register
{
static constexpr uint8_t MIN_BIT_WIDTH = 2;
static constexpr uint8_t MAX_BIT_WIDTH = 32;
static constexpr uint8_t DEFAULT_BIT_WIDTH = 4;
static const QFont FONT("Quicksand", 10, QFont::Medium);
static constexpr QColor LOW_FONT_COLOR(255, 255, 255);
static constexpr QColor HIGH_FONT_COLOR(255, 255, 255);
} // namespace

namespace counter
{
static constexpr uint8_t MIN_BIT_WIDTH = 2;
static constexpr uint8_t MAX_BIT_WIDTH = 32;
static constexpr uint8_t DEFAULT_BIT_WIDTH = 4;
} // namespace

namespace complex_logic
{
static constexpr auto HALF_ADDER_TEXT = "HA";
static constexpr auto FULL_ADDER_TEXT = "FA";
static constexpr auto RS_FLIPFLOP_TEXT = "RS";
static constexpr auto D_FLIPFLOP_TEXT = "FF";
static constexpr auto T_FLIPFLOP_TEXT = "TF";
static constexpr auto JK_FLIPFLOP_TEXT = "JK";
static constexpr auto RS_MS_FLIPFLOP_TEXT = "RS-MS";
static constexpr auto D_MS_FLIPFLOP_TEXT = "FF-MS";
static constexpr auto JK_MS_FLIPFLOP_TEXT = "JK-MS";
static constexpr auto MULTIPLEXER_TEXT = "MUX";
static constexpr auto DEMULTIPLEXER_TEXT = "DEMUX";
static constexpr auto DECODER_TEXT = "";
static constexpr auto ENCODER_TEXT = "";
static constexpr auto COUNTER_TEXT = "COUNT";

static constexpr auto CLOCK_SYMBOL_STRING{">"};

static const QFont FONT("Source Sans Pro");
static constexpr uint8_t DEFAULT_DESCRIPTION_FONT_SIZE = 20;
static const QFont CONNECTOR_FONT("Calibri", 10, QFont::Light);
static constexpr QColor FONT_COLOR(255, 255, 255);
static constexpr QColor SMALL_TEXT_FONT_COLOR(225, 225, 225);
} // namespace

} // namespace

#endif // CONFIGURATION_H
