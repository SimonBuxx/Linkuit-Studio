#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "View/View.h"
#include "QtAwesome.h"
#include "AboutDialog.h"
#include "IconToolButton.h"

#include <QMainWindow>
#include <QShortcut>
#include <QStandardItemModel>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>

class View;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// \brief Constructor for MainWindow
    /// \param pParent: The window's parent widget
    explicit MainWindow(QWidget *pParent = nullptr);

    /// \brief Destructor for MainWindow
    ~MainWindow(void);

    /// \brief Getter for the view frame
    /// \return Reference to the View object
    View& GetView(void);

    /// \brief Getter for the core logic
    /// \return Reference to the core logic
    CoreLogic& GetCoreLogic(void);

protected slots:
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

    // Clock Configuration

    /// \brief Displays the clock configurator with the given settings
    /// \param pMode: The clock mode, either toggle or pulse
    /// \param pToggle: The toggle speed
    /// \param pPulse: The pulse duration
    void ShowClockConfigurator(ClockMode pMode, uint32_t pToggle, uint32_t pPulse);

    /// \brief Show or hides the pulse slider and forwards the new clock mode
    /// \param pChecked: True, if the toggle button is checked
    void OnToggleButtonToggled(bool pChecked);

    /// \brief Updates the toggle speed label and forwards the change to the core logic
    /// \param pValue: Tne new toggle speed
    void OnToggleSliderValueChanged(int32_t pValue);

    /// \brief Updates the pulse duration label and forwards the change to the core logic
    /// \param pValue: Tne new pulse duration
    void OnPulseSliderValueChanged(int32_t pValue);

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

    /// \brief Sets the item input count to the given value, if allowed
    /// \param pValue: The new item input count
    void OnItemInputCountSliderValueChanged(int32_t pValue);

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

    // Item configuration

    /// \brief Shows the item configurator with widgets according to the ConfiguratorMode
    /// \param pMode: The mode for the item configurator
    void ShowItemConfigurator(ConfiguratorMode pMode);

    /// \brief Sets the current gate input count to the given value if SW is in ControlMode::ADD and either AND, OR or XOR is selected
    /// \param pCount: The input count value to set to
    void SetGateInputCountIfAllowed(uint8_t pCount);

    /// \brief Sets the current component direction to the given direction if SW is in ControlMode::ADD
    /// \param pDirection: The direction to set to
    void SetComponentDirectionIfInAddMode(Direction pDirection);

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

protected:
    Ui::MainWindow *mUi;
    QGraphicsScene mScene;
    View mView;
    CoreLogic mCoreLogic;

    QtAwesome *mAwesome; // Contains Fontawesome logos

    QStandardItemModel mToolboxTreeModel;

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

    // ////////////////////////////

    AboutDialog mAboutDialog;

    // Icon color variants

    QVariantMap mMenuBarIconVariant;
    QVariantMap mChevronIconVariant;
    QVariantMap mCheckedButtonVariant;
    QVariantMap mUncheckedButtonVariant;
    QVariantMap mStatusBarIconVariant;
    QVariantMap mPlusMinusIconVariant;
    QVariantMap mConfigButtonIconVariant;
    QVariantMap mWhiteIconVariant;
};

#endif // MAINWINDOW_H
