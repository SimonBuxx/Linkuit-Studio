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

    /// Destructor for MainWindow
    ~MainWindow(void);

    /// \brief Getter for the view frame
    /// \return Reference to the View object
    View& GetView(void);

    /// \brief Getter for the core logic
    /// \return Reference to the core logic
    CoreLogic& GetCoreLogic(void);

protected slots:
    /// \brief OnToolboxTreeClicked
    /// \param pIndex
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

#warning missing documentation
    void OnToggleButtonToggled(bool pChecked);

    void OnToggleSliderValueChanged(int32_t pValue);

    void OnPulseSliderValueChanged(int32_t pValue);

    void DisplayClockConfiguration(ClockMode pMode, uint32_t pToggle, uint32_t pPulse);

    void HideConfigurationGui(void);

    void OnItemRightButtonToggled(bool pChecked);
    void OnItemDownButtonToggled(bool pChecked);
    void OnItemLeftButtonToggled(bool pChecked);
    void OnItemUpButtonToggled(bool pChecked);

    void OnItemInputCountSliderValueChanged(int32_t pValue);

    void ShowItemConfigurationGui(void);
    void HideItemConfigurationGui(void);

protected:
    /// \brief Connects signals and slots of the GUI widgets and core logic
    void ConnectGuiSignalsAndSlots(void);

    /// \brief Creates all items for the toolbox tree
    void InitializeToolboxTree(void);

    /// \brief Sets icosn for all tool buttons and menu bar elements
    void InitializeGuiIcons(void);

    /// \brief Creates global shortcuts
    void InitializeGlobalShortcuts(void);

    /// \brief Sets the current component input count to the given value if SW is in ControlMode::ADD
    /// \param pCount: The input count value to set to
    void SetComponentInputCountIfInAddMode(uint8_t pCount);

    /// \brief Sets the current component direction to the given direction if SW is in ControlMode::ADD
    /// \param pDirection: The direction to set to
    void SetComponentDirectionIfInAddMode(Direction pDirection);

    /// \brief Helper function to uncheck a button inside an exclusive button group
    /// \param pButton: Pointer to the button to uncheck
    void ForceUncheck(IconToolButton *pButton);

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

    /// \brief Updates the enabled state of the undo/redo button and menu entry
    /// \param pEnable: If false, the widgets are disabled
    void UpdateUndoRedoEnabled(bool pEnable);

    /// \brief Undos the last action
    void Undo(void);

    /// \brief Redos the last undone action
    void Redo(void);

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

    AboutDialog mAboutDialog;

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
