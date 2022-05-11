#ifndef MAINWINDOWNEW_H
#define MAINWINDOWNEW_H

#include "View/View.h"
#include "QtAwesome.h"

#include <QMainWindow>
#include <QShortcut>
#include <QStandardItemModel>

namespace Ui {
class MainWindowNew;
}

class MainWindowNew : public QMainWindow
{
    Q_OBJECT

public:
    /// \brief Constructor for MainWindow
    /// \param pParent: The window's parent widget
    explicit MainWindowNew(QWidget *pParent = nullptr);

    /// Destructor for MainWindow
    ~MainWindowNew(void);

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

protected:
    /// \brief Creates all items for the toolbox tree
    void InitializeToolboxTree(void);

    /// \brief Creates global shortcuts
    void InitializeGlobalShortcuts(void);

    /// \brief Sets the current component input count to the given value if SW is in ControlMode::ADD
    /// \param pCount: The input count value to set to
    void SetComponentInputCountIfInAddMode(uint8_t pCount);

    /// \brief Sets the current component direction to the given direction if SW is in ControlMode::ADD
    /// \param pDirection: The direction to set to
    void SetComponentDirectionIfInAddMode(Direction pDirection);

protected:
    Ui::MainWindowNew *mUi;
    QGraphicsScene mScene;
    View mView;
    CoreLogic mCoreLogic;

    QtAwesome *mAwesome; // Contains Fontawesome logos

    QStandardItemModel mToolboxTreeModel;

    // Category items
    QStandardItem* mCategoryGatesItem;
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

    QShortcut* mComponentDirectionRightShortcut;
    QShortcut* mComponentDirectionDownShortcut;
    QShortcut* mComponentDirectionLeftShortcut;
    QShortcut* mComponentDirectionUpShortcut;

    QShortcut* mCopyShortcut;
    QShortcut* mPasteShortcut;

    QShortcut* mSaveShortcut;
    QShortcut* mOpenShortcut;

    QShortcut* mUndoShortcut;
    QShortcut* mRedoShortcut;

    QShortcut* mSimulationShortcut;

    QShortcut* mDeleteShortcut;

    QShortcut* mEscapeShortcut;
};

#endif // MAINWINDOWNEW_H
