#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "View.h"
#include "QtAwesome.h"
#include "Gui/AboutDialog.h"
#include "Gui/WelcomeDialog.h"
#include "Gui/IconToolButton.h"
#include "Gui/TutorialFrame.h"

#include <QMainWindow>
#include <QShortcut>
#include <QStandardItemModel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QtWidgets>

// forward declarations
class View;

namespace Ui {
class MainWindow;
}

///
/// \brief The MainWindow class represents the application's main window
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// \brief Constructor for MainWindow
    /// \param pParent: The window's parent widget
    explicit MainWindow(QWidget *pParent = nullptr);

    /// \brief Destructor for MainWindow
    ~MainWindow(void) override;

    /// \brief Getter for the view frame
    /// \return Reference to the View object
    View& GetView(void);

    /// \brief Getter for the core logic
    /// \return Reference to the core logic
    CoreLogic& GetCoreLogic(void);

    /// \brief Shows the welcome dialog after a specified delay
    /// \param pDelay: The delay after which the dialog is shown
    void ShowWelcomeDialog(std::chrono::milliseconds pDelay);

    /// \brief Configures the welcome dialog
    /// \param pShowOnStartup: Whether the dialog should be displayed on startup (checkbox state)
    /// \param pRecentFilePaths: List of recently opened file paths
    void ConfigureWelcomeDialog(bool pShowOnStartup, const std::vector<QFileInfo>& pRecentFilePaths);

    /// \brief Configures the menu actions for recent files
    /// \param pRecentFilePaths: List of recently opened file paths
    void SetRecentFileMenuActions(const std::vector<QFileInfo>& pRecentFilePaths);

protected slots:
    /// \brief Inserts the given file into the recent files and configures the GUI
    /// \param pFileInfo: Info about the newly opened circuit file
    void OnCircuitFileOpenedSuccessfully(const QFileInfo& pFileInfo);

    /// \brief Raises a warning that the opened file is created by a newer compatible version
    /// \param pVersion: The newer file version
    void OnCircuitFileHasNewerCompatibleVersion(const QString& pVersion);

    /// \brief Raises an error that the opened file is created by a newer incompatible version
    /// \param pVersion: The newer file version
    void OnCircuitFileHasNewerIncompatibleVersion(const QString& pVersion);

    /// \brief Shows an error dialog box when opening the circuit file failed
    /// \param pFileInfo: Info about the circuit file that could not be opened
    /// \param pIsFromRecents: Whether the file is from the recent files list
    void OnCircuitFileOpeningFailed(const QFileInfo& pFileInfo, bool pIsFromRecents);

    /// \brief Configures the GUI and inserts the newly saved file into the recent files
    /// \param pFileInfo: Info about the newly saved circuit file
    void OnCircuitFileSavedSuccessfully(const QFileInfo& pFileInfo);

    /// \brief Shows an error dialog box when saving the circuit failed
    void OnCircuitFileSavingFailed(void);

    /// \brief Enters the ADD control mode with the selected item
    /// \param pIndex: The index of the item that has been clicked
    void OnToolboxTreeClicked(const QModelIndex &pIndex);

    /// \brief Performs all GUI adjustments to enter the new control mode
    /// \param pNewMode: The newly entered control mode
    void OnControlModeChanged(ControlMode pNewMode);

    /// \brief Performs all GUI adjustments to enter the new simulation mode
    /// \param pNewMode: The newly entered simulation mode
    void OnSimulationModeChanged(SimulationMode pNewMode);

    /// \brief Sets the displayed zoom level and the slider to the given values
    /// \param pPercentage: The new zoom value to display
    void UpdateZoomLabelAndSlider(uint8_t pPercentage, uint32_t pValue);

    /// \brief Positions the tutorial box at the correct position
    /// \param pStep: The current tutorial step
    void OnTutorialStepChanged(uint8_t pStep);

    /// \brief Allows or declines advancing to the next tutorial step
    /// \param pStep: The current tutorial step
    void OnAdvanceTutorialStepRequest(uint8_t pStep);

    /// \brief Fades out all GUI overlay widgets and saves if they were visible before
    void FadeOutGui(void);

    /// \brief Fades in all GUI overlay widgets that have been visible before fading out
    void FadeInGui(void);

    /// \brief Opens the given file if the user does not abort due to unsaved changes
    /// \param pFileInfo: Info of the file to open
    void OpenRecentFile(const QFileInfo& pFileInfo);

    // Clock Configuration

    /// \brief Displays the clock configurator with the given settings
    /// \param pMode: The clock mode, either toggle or pulse
    /// \param pToggle: The toggle speed
    /// \param pPulse: The pulse duration
    void ShowClockConfigurator(ClockMode pMode, uint32_t pToggle, uint32_t pPulse);

    /// \brief Show or hides the pulse value box and forwards the new clock mode
    /// \param pChecked: True, if the toggle button is checked
    void OnToggleButtonToggled(bool pChecked);

    // Item configuration

    /// \brief Sets the direction for new items to right if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemRightButtonToggled(bool pChecked);

    /// \brief Sets the direction for new items to down if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemDownButtonToggled(bool pChecked);

    /// \brief Sets the direction for new items to left if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemLeftButtonToggled(bool pChecked);

    /// \brief Sets the direction for new items to up if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnItemUpButtonToggled(bool pChecked);

#warning remove and directly call protected functions as slots
    /// \brief Sets the gate input count to the given value, if allowed
    /// \param pValue: The new gate input count
    void OnGateInputCountSliderValueChanged(int32_t pValue);

    /// \brief Sets the en-/decoder input count to the given value, if allowed
    /// \param pValue: The new en-/decoder input count
    void OnEncoderDecoderInputCountSliderValueChanged(int32_t pValue);

    /// \brief Sets the (de-)multiplexer bit width to the given value, if allowed
    /// \param pValue: The new (de-)multiplexer bit width
    void OnMultiplexerBitWidthSliderValueChanged(int32_t pValue);

    /// \brief Sets the shift register bit width to the value of the new index, if allowed
    /// \param pIndex: The new shift register box index
    void OnShiftRegisterWidthBoxIndexChanged(int32_t pIndex);

    /// \brief Sets the counter bit width to the given value, if allowed
    /// \param pValue: The new counter bit width
    void OnCounterBitWidthSliderValueChanged(int32_t pValue);

    /// \brief Sets the constant state for new constant inputs to HIGH if the button is checked
    /// \param pChecked: True, if the button is checked
    void OnConstantHighButtonToggled(bool pChecked);

    /// \brief Sets the flip-flop style to the style associated with the given index
    /// \param pIndex: The combo box index
    void OnFlipFlopStyleChanged(int32_t pIndex);

protected:
    // Initialization

    /// \brief Connects signals and slots of the GUI widgets and core logic
    void ConnectGuiSignalsAndSlots(void);

    /// \brief Creates all items for the toolbox tree
    void InitializeToolboxTree(void);

    /// \brief Sets icosn for all tool buttons and menu bar elements
    void InitializeGuiIcons(void);

    /// \brief Creates global shortcuts
    void InitializeGlobalShortcuts(void);

    /// \brief Creates message dialog boxes that can be triggered in various places
    void InitializeMessageBoxes(void);

    /// \brief Initializes and starts the tutorial; can be used to reset/restart the tutorial
    void InitializeTutorial(void);

    // Item configuration

    /// \brief Shows the item configurator with widgets according to the ConfiguratorMode
    /// \param pMode: The mode for the item configurator
    void ShowItemConfigurator(ConfiguratorMode pMode);

    /// \brief Sets the current gate input count to the given value if SW is in ControlMode::ADD and either AND, OR or XOR is selected
    /// \param pCount: The input count value to set to
    void SetGateInputCountIfAllowed(uint8_t pCount);

    /// \brief Sets the current en-/decoder input count to the given value if SW is in ControlMode::ADD and either ENCODER or DECODER is selected
    /// \param pCount: The input count value to set to
    void SetEncoderDecoderInputCountIfAllowed(uint8_t pCount);

    /// \brief Sets the current (de-)multiplexer bit width to the given value if SW is in ControlMode::ADD
    /// \param pBitWidth: The bit width to set to
    void SetMultiplexerBitWidthIfAllowed(uint8_t pBitWidth);

    /// \brief Sets the current shift register bit width to the given value if SW is in ControlMode::ADD
    /// \param pBitWidth: The bit width to set to
    void SetShiftRegisterBitWidthIfAllowed(uint8_t pBitWidth);

    /// \brief Sets the current component direction to the given direction if SW is in ControlMode::ADD
    /// \param pDirection: The direction to set to
    void SetComponentDirectionIfInAddMode(Direction pDirection);

    /// \brief Sets the current constant state to the given state if SW is in ControlMode::ADD
    /// \param pState: The logic state to set to
    void SetConstantStateIfAllowed(LogicState pState);

    /// \brief Sets the current en-/decoder input count to the given value if SW is in ControlMode::ADD and COUNTER is selected
    /// \param pCount: The counter bit width value to set to
    void SetCounterBitWidthIfAllowed(uint8_t pBitWidth);

    // Simulation

    /// \brief Enters the simulation mode if not currently in simulation mode
    void EnterSimulation(void);

    /// \brief Runs the simulation
    void RunSimulation(void);

    /// \brief Advances the simulation by one tick
    void StepSimulation(void);

    /// \brief Resets the simulation
    void ResetSimulation(void);

    /// \brief Pauses the simulation
    void PauseSimulation(void);

    /// \brief Leaves the simulation if currently in simulation mode
    void StopSimulation(void);

    // Undo/redo

    /// \brief Undos the last action
    void Undo(void);

    /// \brief Redos the last undone action
    void Redo(void);

    /// \brief Updates the enabled state of the undo/redo button and menu entry
    /// \param pEnable: If false, the widgets are disabled
    void UpdateUndoRedoEnabled(bool pEnable);

    // ////////////////////////////

    /// \brief Helper function to uncheck a button inside an exclusive button group
    /// \param pButton: Pointer to the button to uncheck
    void ForceUncheck(IconToolButton *pButton);

    /// \brief Extends the closing event to display a message box when there are unsaved changes
    /// \param pEvent: Pointer to the close event
    void closeEvent(QCloseEvent *pEvent);

    /// \brief Extends the key press event to fade out the GUI overlay after a specified interval
    /// \param pEvent: Pointer to the key event
    void keyPressEvent(QKeyEvent *pEvent);

    /// \brief Extends the key release event to fade in the GUI overlay after a specified interval
    /// \param pEvent: Pointer to the key event
    void keyReleaseEvent(QKeyEvent *pEvent);

    // Animation

    /// \brief Fades in the given GUI widget of type T
    /// \param pWidget: The GUI widget to fade in
    template <typename T>
    void FadeInWidget(T& pWidget);

    /// \brief Fades out the given GUI widget of type T
    /// \param pWidget: The GUI widget to fade out
    template <typename T>
    void FadeOutWidget(T& pWidget);

    /// \brief Asks if the changes should be saved if the current circuit is modified
    /// \return True, when the user pressed "Cancel"; False, if the circuit was unchanged,
    /// if the circuit has been saved or changes were discarded
    bool IsSaveChangesIfModifiedCanceled(void);

protected:
    Ui::MainWindow *mUi;
    QGraphicsScene mScene;
    View mView;
    CoreLogic mCoreLogic;

    QtAwesome mAwesome; // Contains Fontawesome icons

    QStandardItemModel mToolboxTreeModel;

    TutorialFrame mTutorialFrame;

    // Category items
    QStandardItem* mCategoryGatesItem;
    QStandardItem* mCategoryInputsItem;
    QStandardItem* mCategoryAddersItem;
    QStandardItem* mCategoryMemoryItem;
    QStandardItem* mCategoryConvertersItem;

    // Global shortcuts
    QShortcut* mOneGateInputShortcut;
    QShortcut* mTwoGateInputsShortcut;
    QShortcut* mThreeGateInputsShortcut;
    QShortcut* mFourGateInputsShortcut;
    QShortcut* mFiveGateInputsShortcut;
    QShortcut* mSixGateInputsShortcut;
    QShortcut* mSevenGateInputsShortcut;
    QShortcut* mEightGateInputsShortcut;
    QShortcut* mNineGateInputsShortcut;
    QShortcut* mEscapeShortcut;

    // Windows and dialog boxes

    AboutDialog mAboutDialog;
    WelcomeDialog mWelcomeDialog;

    QMessageBox mSaveChangesBox;
    QMessageBox mErrorOpenFileBox;
    QMessageBox mErrorOpenRecentFileBox;
    QMessageBox mErrorSaveFileBox;
    QMessageBox mNewerVersionCompatibleBox;
    QMessageBox mNewerVersionIncompatibleBox;

    // Icon color variants

    QVariantMap mMenuBarIconVariant;
    QVariantMap mConfigButtonIconVariant;
    QVariantMap mWhiteIconVariant;

    // Visibility states

    bool mIsClockConfiguratorVisible = false;
    bool mIsItemConfiguratorVisible = false;
    bool mIsToolboxVisible = true;

    QTimer mFadeOutOnCtrlTimer;
    bool mIsGuiHidden = false;
};

#endif // MAINWINDOW_H
