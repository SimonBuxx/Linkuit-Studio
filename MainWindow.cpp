#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow),
    mView(mCoreLogic),
    mCoreLogic(mView)
{
    mUi->setupUi(this);

    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();

    mScene.setSceneRect(canvas::DIMENSIONS);
    mView.SetScene(mScene);

    mUi->uViewLayout->addWidget(&mView, 1, 1);

    mView.stackUnder(mUi->uLeftContainer);

    QObject::connect(&mCoreLogic, &CoreLogic::ControlModeChangedSignal, this, &MainWindow::OnControlModeChanged);
    QObject::connect(&mCoreLogic, &CoreLogic::SimulationModeChangedSignal, this, &MainWindow::OnSimulationModeChanged);

    ConnectGuiSignalsAndSlots();

    InitializeToolboxTree();
    InitializeGuiIcons();
    InitializeGlobalShortcuts();

    mAboutDialog.setAttribute(Qt::WA_QuitOnClose, false); // Make about dialog close when main window closes
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::ConnectGuiSignalsAndSlots()
{
    QObject::connect(mUi->uEditButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });

    QObject::connect(mUi->uWiringButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::WIRE);
    });

    QObject::connect(mUi->uDeleteButton, &QAbstractButton::clicked, [&]()
    {
        if (!mCoreLogic.IsSimulationRunning())
        {
            mCoreLogic.DeleteSelectedComponents();
        }
    });

    QObject::connect(mUi->uCopyButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::CopySelectedComponents);

    QObject::connect(mUi->uUndoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Undo);
    QObject::connect(mUi->uRedoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Redo);

    QObject::connect(mUi->uStartButton, &QAbstractButton::clicked, this, &MainWindow::EnterSimulation);
    QObject::connect(mUi->uRunButton, &QAbstractButton::clicked, this, &MainWindow::RunSimulation);
    QObject::connect(mUi->uStepButton, &QAbstractButton::clicked, this, &MainWindow::StepSimulation);
    QObject::connect(mUi->uResetButton, &QAbstractButton::clicked, this, &MainWindow::ResetSimulation);
    QObject::connect(mUi->uPauseButton, &QAbstractButton::clicked, this, &MainWindow::PauseSimulation);
    QObject::connect(mUi->uStopButton, &QAbstractButton::clicked, this, &MainWindow::StopSimulation);

    QObject::connect(mUi->uActionAbout, &QAction::triggered, &mAboutDialog, &AboutDialog::show);

    QObject::connect(mUi->uActionStart, &QAction::triggered, this, &MainWindow::EnterSimulation);
    QObject::connect(mUi->uActionRun, &QAction::triggered, this, &MainWindow::RunSimulation);
    QObject::connect(mUi->uActionStep, &QAction::triggered, this, &MainWindow::StepSimulation);
    QObject::connect(mUi->uActionReset, &QAction::triggered, this, &MainWindow::ResetSimulation);
    QObject::connect(mUi->uActionPause, &QAction::triggered, this, &MainWindow::PauseSimulation);
    QObject::connect(mUi->uActionStop, &QAction::triggered, this, &MainWindow::StopSimulation);

}

void MainWindow::EnterSimulation()
{
    if (!mCoreLogic.IsSimulationRunning())
    {
        mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
    }
}

void MainWindow::RunSimulation()
{
    mCoreLogic.RunSimulation();
}

void MainWindow::StepSimulation()
{
    mCoreLogic.StepSimulation();
}

void MainWindow::ResetSimulation()
{
    mCoreLogic.ResetSimulation();
}

void MainWindow::PauseSimulation()
{
    mCoreLogic.PauseSimulation();
}

void MainWindow::StopSimulation()
{
    if (mCoreLogic.IsSimulationRunning())
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    }
}

void MainWindow::OnControlModeChanged(ControlMode pNewMode)
{
    switch (pNewMode)
    {
        case ControlMode::EDIT:
        {
            mUi->uToolboxTree->clearSelection();
            mUi->uToolboxTree->setEnabled(true);

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uCopyButton->setEnabled(true);
            mUi->uDeleteButton->setEnabled(true);
            mUi->uUndoButton->setEnabled(true);
            mUi->uRedoButton->setEnabled(true);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(false);
#warning introduce simulation sub modes for step/run
            mUi->uStepButton->setEnabled(false);
            mUi->uResetButton->setEnabled(false);
            mUi->uPauseButton->setEnabled(false);
            mUi->uStopButton->setEnabled(false);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(false);
            mUi->uActionReset->setEnabled(false);
            mUi->uActionStep->setEnabled(false);
            mUi->uActionPause->setEnabled(false);
            mUi->uActionStop->setEnabled(false);

            mUi->uEditButton->setChecked(true);
            ForceUncheck(mUi->uRunButton);
            ForceUncheck(mUi->uWiringButton);
            ForceUncheck(mUi->uPauseButton);
            break;
        }
        case ControlMode::WIRE:
        {
            mUi->uToolboxTree->clearSelection();
            mUi->uToolboxTree->setEnabled(true);

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uCopyButton->setEnabled(true);
            mUi->uDeleteButton->setEnabled(true);
            mUi->uUndoButton->setEnabled(true);
            mUi->uRedoButton->setEnabled(true);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(false);
            mUi->uStepButton->setEnabled(false);
            mUi->uResetButton->setEnabled(false);
            mUi->uPauseButton->setEnabled(false);
            mUi->uStopButton->setEnabled(false);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(false);
            mUi->uActionReset->setEnabled(false);
            mUi->uActionStep->setEnabled(false);
            mUi->uActionPause->setEnabled(false);
            mUi->uActionStop->setEnabled(false);

            ForceUncheck(mUi->uEditButton);
            mUi->uWiringButton->setChecked(true);
            ForceUncheck(mUi->uRunButton);
            ForceUncheck(mUi->uPauseButton);
            break;
        }
        case ControlMode::ADD:
        {
            mUi->uToolboxTree->setEnabled(true);

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uCopyButton->setEnabled(true);
            mUi->uDeleteButton->setEnabled(true);
            mUi->uUndoButton->setEnabled(true);
            mUi->uRedoButton->setEnabled(true);
            mUi->uStartButton->setEnabled(true);
            mUi->uRunButton->setEnabled(false);
            mUi->uStepButton->setEnabled(false);
            mUi->uResetButton->setEnabled(false);
            mUi->uPauseButton->setEnabled(false);
            mUi->uStopButton->setEnabled(false);

            mUi->uActionStart->setEnabled(true);
            mUi->uActionRun->setEnabled(false);
            mUi->uActionReset->setEnabled(false);
            mUi->uActionStep->setEnabled(false);
            mUi->uActionPause->setEnabled(false);
            mUi->uActionStop->setEnabled(false);

            ForceUncheck(mUi->uEditButton);
            ForceUncheck(mUi->uWiringButton);
            ForceUncheck(mUi->uRunButton);
            ForceUncheck(mUi->uPauseButton);
            break;
        }
        case ControlMode::SIMULATION:
        {
            mUi->uToolboxTree->clearSelection();
            mUi->uToolboxTree->setEnabled(false);

            mUi->uEditButton->setEnabled(false);
            mUi->uWiringButton->setEnabled(false);
            mUi->uCopyButton->setEnabled(false);
            mUi->uDeleteButton->setEnabled(false);
            mUi->uUndoButton->setEnabled(false);
            mUi->uRedoButton->setEnabled(false);
            mUi->uStartButton->setEnabled(false);
            mUi->uRunButton->setEnabled(true);
            mUi->uStepButton->setEnabled(true);
            mUi->uResetButton->setEnabled(true);
            mUi->uPauseButton->setEnabled(true);
            mUi->uStopButton->setEnabled(true);

            mUi->uActionStart->setEnabled(false);
            mUi->uActionRun->setEnabled(true);
            mUi->uActionReset->setEnabled(true);
            mUi->uActionStep->setEnabled(true);
            mUi->uActionPause->setEnabled(false);
            mUi->uActionStop->setEnabled(true);

            ForceUncheck(mUi->uEditButton);
            ForceUncheck(mUi->uWiringButton);
            ForceUncheck(mUi->uRunButton);
            mUi->uPauseButton->setChecked(true);
            break;
        }
        default:
        {
            break;
        }
    }

    mScene.clearSelection();
}

void MainWindow::OnSimulationModeChanged(SimulationMode pNewMode)
{
    switch (pNewMode)
    {
        case SimulationMode::STOPPED:
        {
            mUi->uPauseButton->setChecked(true);
            mUi->uStepButton->setEnabled(true);

            mUi->uActionRun->setEnabled(true);
            mUi->uActionPause->setEnabled(false);
            mUi->uActionStep->setEnabled(true);
            break;
        }
        case SimulationMode::RUNNING:
        {
            mUi->uRunButton->setChecked(true);
            mUi->uStepButton->setEnabled(false);

            mUi->uActionRun->setEnabled(false);
            mUi->uActionPause->setEnabled(true);
            mUi->uActionStep->setEnabled(false);
            break;
        }
        default:
        {
            break;
        }
    }
}

void MainWindow::ForceUncheck(IconToolButton *pButton)
{
    if (nullptr != pButton->group() && pButton->group()->exclusive())
    {
        pButton->group()->setExclusive(false);
        pButton->setChecked(false);
        pButton->group()->setExclusive(true);
    }
    else
    {
        pButton->setChecked(false);
    }
}

#warning clear selection on simulation start
void MainWindow::InitializeToolboxTree()
{
    mChevronIconVariant.insert("color", QColor(0, 45, 50));
    mChevronIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mChevronIconVariant.insert("color-active", QColor(0, 45, 50));
    mChevronIconVariant.insert("color-selected", QColor(0, 45, 50));

    QObject::connect(mUi->uToolboxTree, &QTreeView::pressed, this, &MainWindow::OnToolboxTreeClicked);

    // Tracks the currently selected item when it is changed by dragging
    QObject::connect(mUi->uToolboxTree, &QTreeView::entered, this, [&](const QModelIndex &pIndex)
    {
        if ((QGuiApplication::mouseButtons() == Qt::LeftButton) && (mUi->uToolboxTree->currentIndex().row() >= 0))
        {
            if (!mToolboxTreeModel.itemFromIndex(pIndex)->isSelectable())
            {
                mUi->uToolboxTree->clearSelection();
            }
            OnToolboxTreeClicked(pIndex);
        }
    });


    // Expand/collapse on single click
    QObject::connect(mUi->uToolboxTree, &QTreeView::clicked, [this]()
    {
        if (mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->hasChildren())
        {
            if (mUi->uToolboxTree->isExpanded(mUi->uToolboxTree->currentIndex()))
            {
                mUi->uToolboxTree->collapse(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(mAwesome->icon(fa::chevrondown, mChevronIconVariant));
            }
            else
            {
                mUi->uToolboxTree->expand(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(mAwesome->icon(fa::chevronup, mChevronIconVariant));
            }
        }
    });

    // Create category and root level items
    mCategoryGatesItem = new QStandardItem(mAwesome->icon(fa::chevronup, mChevronIconVariant), "Gates");
    mCategoryGatesItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryGatesItem);

    mCategoryInputsItem = new QStandardItem(mAwesome->icon(fa::chevronup, mChevronIconVariant), "Inputs");
    mCategoryInputsItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryInputsItem);

    auto outputItem = new QStandardItem(QIcon(":images/icons/output_icon.png"), "Output");
    mToolboxTreeModel.appendRow(outputItem);

    mCategoryAddersItem = new QStandardItem(mAwesome->icon(fa::chevrondown, mChevronIconVariant), "Adders");
    mCategoryAddersItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryAddersItem);

    mCategoryMemoryItem = new QStandardItem(mAwesome->icon(fa::chevrondown, mChevronIconVariant), "Memory");
    mCategoryMemoryItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryMemoryItem);

    mCategoryConvertersItem = new QStandardItem(mAwesome->icon(fa::chevrondown, mChevronIconVariant), "Converters");
    mCategoryConvertersItem->setSelectable(false);
    mToolboxTreeModel.appendRow(mCategoryConvertersItem);

    auto textLabelItem = new QStandardItem(QIcon(":images/icons/label_icon.png"), "Text label");
    mToolboxTreeModel.appendRow(textLabelItem);

#warning idea: extend fields for configurations on select

    // Create component items
    mCategoryGatesItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "AND gate⁺"));
    mCategoryGatesItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "OR gate⁺"));
    mCategoryGatesItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "XOR gate⁺"));
    mCategoryGatesItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "NOT gate"));
    mCategoryGatesItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "Buffer gate"));

    mCategoryInputsItem->appendRow(new QStandardItem(QIcon(":images/icons/input_icon.png"), "Switch"));
    mCategoryInputsItem->appendRow(new QStandardItem(QIcon(":images/icons/button_icon.png"), "Button"));
    mCategoryInputsItem->appendRow(new QStandardItem(QIcon(":images/icons/clock_icon.png"), "Clock⁺"));

    mCategoryAddersItem->appendRow(new QStandardItem(QIcon(":images/icons/flipflop_icon.png"), "Half adder"));
    mCategoryAddersItem->appendRow(new QStandardItem(QIcon(":images/icons/full_adder_icon.png"), "Full adder"));

    mCategoryMemoryItem->appendRow(new QStandardItem(QIcon(":images/icons/flipflop_icon.png"), "RS flip-flop"));
    mCategoryMemoryItem->appendRow(new QStandardItem(QIcon(":images/icons/flipflop_icon.png"), "D flip-flop"));

    mCategoryConvertersItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "Multiplexer⁺"));
    mCategoryConvertersItem->appendRow(new QStandardItem(QIcon(":images/icons/gate.png"), "Demultiplexer⁺"));

    mUi->uToolboxTree->setModel(&mToolboxTreeModel);
    mUi->uToolboxTree->setExpanded(mCategoryGatesItem->index(), true);
    mUi->uToolboxTree->setExpanded(mCategoryInputsItem->index(), true);
}

void MainWindow::InitializeGuiIcons()
{
    // Initialize icon color variants
    mMenuBarIconVariant.insert("color", QColor(0, 39, 43));
    mMenuBarIconVariant.insert("color-disabled", QColor(100, 100, 100));
    mMenuBarIconVariant.insert("color-active", QColor(0, 39, 43));
    mMenuBarIconVariant.insert("color-selected", QColor(0, 39, 43));

    mUncheckedButtonVariant.insert("color", QColor(0, 45, 50));
    mUncheckedButtonVariant.insert("color-disabled", QColor(200, 200, 200));
    mUncheckedButtonVariant.insert("color-active", QColor(0, 45, 50));
    mUncheckedButtonVariant.insert("color-selected", QColor(0, 45, 50));

    mCheckedButtonVariant.insert("color", QColor(255, 255, 255));
    mCheckedButtonVariant.insert("color-disabled", QColor(200, 200, 200));
    mCheckedButtonVariant.insert("color-active", QColor(255, 255, 255));
    mCheckedButtonVariant.insert("color-selected", QColor(255, 255, 255));

    // Icons for GUI buttons
    mUi->uEditButton->SetCheckedIcon(mAwesome->icon(fa::mousepointer, mCheckedButtonVariant));
    mUi->uEditButton->SetUncheckedIcon(mAwesome->icon(fa::mousepointer, mUncheckedButtonVariant));

    mUi->uWiringButton->SetCheckedIcon(mAwesome->icon(fa::exchange, mCheckedButtonVariant));
    mUi->uWiringButton->SetUncheckedIcon(mAwesome->icon(fa::exchange, mUncheckedButtonVariant));

    mUi->uCopyButton->SetIcon(mAwesome->icon(fa::clipboard, mUncheckedButtonVariant));
    mUi->uDeleteButton->SetIcon(mAwesome->icon(fa::trash, mUncheckedButtonVariant));
    mUi->uUndoButton->SetIcon(mAwesome->icon(fa::undo, mUncheckedButtonVariant));
    mUi->uRedoButton->SetIcon(mAwesome->icon(fa::repeat, mUncheckedButtonVariant));

    mUi->uStartButton->SetIcon(mAwesome->icon(fa::cog, mUncheckedButtonVariant));
    mUi->uRunButton->SetUncheckedIcon(mAwesome->icon(fa::play, mUncheckedButtonVariant));
    mUi->uRunButton->SetCheckedIcon(mAwesome->icon(fa::play, mCheckedButtonVariant));
    mUi->uPauseButton->SetUncheckedIcon(mAwesome->icon(fa::pause, mUncheckedButtonVariant));
    mUi->uPauseButton->SetCheckedIcon(mAwesome->icon(fa::pause, mCheckedButtonVariant));
    mUi->uStepButton->SetIcon(mAwesome->icon(fa::stepforward, mUncheckedButtonVariant));
    mUi->uResetButton->SetIcon(mAwesome->icon(fa::refresh, mUncheckedButtonVariant));
    mUi->uStopButton->SetIcon(mAwesome->icon(fa::stop, mUncheckedButtonVariant));

    // Icons for menu bar elements
    mUi->uActionStart->setIcon(mAwesome->icon(fa::cog, mMenuBarIconVariant));
    mUi->uActionRun->setIcon(mAwesome->icon(fa::play, mMenuBarIconVariant));
    mUi->uActionStep->setIcon(mAwesome->icon(fa::stepforward, mMenuBarIconVariant));
    mUi->uActionReset->setIcon(mAwesome->icon(fa::refresh, mMenuBarIconVariant));
    mUi->uActionPause->setIcon(mAwesome->icon(fa::pause, mMenuBarIconVariant));
    mUi->uActionStop->setIcon(mAwesome->icon(fa::stop, mMenuBarIconVariant));

    mUi->uActionAbout->setIcon(mAwesome->icon(fa::info, mMenuBarIconVariant));
}

void MainWindow::InitializeGlobalShortcuts()
{
    // Shortcuts to change component input count
    mOneGateInputShortcut    = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_1), this);
    mTwoGateInputsShortcut   = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_2), this);
    mThreeGateInputsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_3), this);
    mFourGateInputsShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_4), this);
    mFiveGateInputsShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_5), this);
    mSixGateInputsShortcut   = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_6), this);
    mSevenGateInputsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_7), this);
    mEightGateInputsShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_8), this);
    mNineGateInputsShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_9), this);

    mOneGateInputShortcut->setAutoRepeat(false);
    mTwoGateInputsShortcut->setAutoRepeat(false);
    mThreeGateInputsShortcut->setAutoRepeat(false);
    mFourGateInputsShortcut->setAutoRepeat(false);
    mFiveGateInputsShortcut->setAutoRepeat(false);
    mSixGateInputsShortcut->setAutoRepeat(false);
    mSevenGateInputsShortcut->setAutoRepeat(false);
    mEightGateInputsShortcut->setAutoRepeat(false);
    mNineGateInputsShortcut->setAutoRepeat(false);

    QObject::connect(mOneGateInputShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(1);
    });
    QObject::connect(mTwoGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(2);
    });
    QObject::connect(mThreeGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(3);
    });
    QObject::connect(mFourGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(4);
    });
    QObject::connect(mFiveGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(5);
    });
    QObject::connect(mSixGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(6);
    });
    QObject::connect(mSevenGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(7);
    });
    QObject::connect(mEightGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(8);
    });
    QObject::connect(mNineGateInputsShortcut, &QShortcut::activated, this, [&]()
    {
       SetComponentInputCountIfInAddMode(9);
    });

    // Shortcuts to change component direction
    mComponentDirectionRightShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), this);
    mComponentDirectionDownShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down), this);
    mComponentDirectionLeftShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left), this);
    mComponentDirectionUpShortcut    = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up), this);

    mComponentDirectionRightShortcut->setAutoRepeat(false);
    mComponentDirectionDownShortcut->setAutoRepeat(false);
    mComponentDirectionLeftShortcut->setAutoRepeat(false);
    mComponentDirectionUpShortcut->setAutoRepeat(false);

    QObject::connect(mComponentDirectionRightShortcut, &QShortcut::activated, this, [&]()
    {
        SetComponentDirectionIfInAddMode(Direction::RIGHT);
    });
    QObject::connect(mComponentDirectionDownShortcut, &QShortcut::activated, this, [&]()
    {
        SetComponentDirectionIfInAddMode(Direction::DOWN);
    });
    QObject::connect(mComponentDirectionLeftShortcut, &QShortcut::activated, this, [&]()
    {
        SetComponentDirectionIfInAddMode(Direction::LEFT);
    });
    QObject::connect(mComponentDirectionUpShortcut, &QShortcut::activated, this, [&]()
    {
        SetComponentDirectionIfInAddMode(Direction::UP);
    });

    mCopyShortcut  = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_C), this);
    mPasteShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_V), this);

    mCopyShortcut->setAutoRepeat(false);
    mPasteShortcut->setAutoRepeat(false);

    QObject::connect(mCopyShortcut, &QShortcut::activated, this, [&]()
    {
#warning temporary implementation of Ctrl-C
        mCoreLogic.CopySelectedComponents();
    });
    QObject::connect(mPasteShortcut, &QShortcut::activated, this, [&]()
    {
        qDebug() << "Not implemented";
    });

    mSaveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    mOpenShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);

    mSaveShortcut->setAutoRepeat(false);
    mOpenShortcut->setAutoRepeat(false);

    QObject::connect(mSaveShortcut, &QShortcut::activated, this, [&]()
    {
        qDebug() << "Not implemented";
    });
    QObject::connect(mOpenShortcut, &QShortcut::activated, this, [&]()
    {
        qDebug() << "Not implemented";
    });

    mUndoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
    mRedoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);

    mUndoShortcut->setAutoRepeat(true);
    mRedoShortcut->setAutoRepeat(true);

    QObject::connect(mUndoShortcut, &QShortcut::activated, this, [&]()
    {
        mCoreLogic.Undo();
    });
    QObject::connect(mRedoShortcut, &QShortcut::activated, this, [&]()
    {
        mCoreLogic.Redo();
    });

    mEnterSimulationShortcut = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_Return), this);

    mEnterSimulationShortcut->setAutoRepeat(false);

    QObject::connect(mEnterSimulationShortcut, &QShortcut::activated, this, [&]()
    {
        if (mCoreLogic.IsSimulationRunning())
        {
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
        }
        else
        {
            mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
        }
    });

    mStepSimulationShortcut = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_Right), this);

    mStepSimulationShortcut->setAutoRepeat(true);

    QObject::connect(mStepSimulationShortcut, &QShortcut::activated, this, [&]()
    {
        if (mCoreLogic.IsSimulationRunning())
        {
            mCoreLogic.StepSimulation();
        }
    });

    mDeleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);

    mDeleteShortcut->setAutoRepeat(false);

    QObject::connect(mDeleteShortcut, &QShortcut::activated, this, [&]()
    {
        if (!mCoreLogic.IsSimulationRunning())
        {
            mCoreLogic.DeleteSelectedComponents();
        }
    });

    mEscapeShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);

    mEscapeShortcut->setAutoRepeat(false);

    QObject::connect(mEscapeShortcut, &QShortcut::activated, this, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
        mScene.clearSelection();
#warning clear selection on ControlModeChangedSignal
        mUi->uToolboxTree->clearSelection();
    });
}

void MainWindow::SetComponentInputCountIfInAddMode(uint8_t pCount)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetComponentInputCount(pCount);
    }
}

void MainWindow::SetComponentDirectionIfInAddMode(Direction pDirection)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetComponentDirection(pDirection);
    }
}

View& MainWindow::GetView()
{
    return mView;
}

CoreLogic& MainWindow::GetCoreLogic()
{
    return mCoreLogic;
}

void MainWindow::OnToolboxTreeClicked(const QModelIndex &pIndex)
{
#warning use isTopLevel instead
    if (pIndex.row() == -1)
    {
        throw std::logic_error("Model index invalid");
    }
    else if (pIndex.parent().row() == -1)
    {
        // Item is on root level
        switch(pIndex.row())
        {
            case 2: // Output
            {
                mCoreLogic.EnterAddControlMode(ComponentType::OUTPUT);
                break;
            }
            case 6: // Text label
            {
                mCoreLogic.EnterAddControlMode(ComponentType::TEXT_LABEL);
                break;
            }
            default:
            {
                mCoreLogic.EnterControlMode(ControlMode::EDIT);
                mScene.clearSelection();
                break;
            }
        }
    }
    else if (pIndex.parent().parent().row() == -1)
    {
        // Item is on second level
        switch (pIndex.parent().row())
        {
            case 0: // Gates
            {
                switch(pIndex.row())
                {
                    case 0: // AND gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::AND_GATE);
                        break;
                    }
                    case 1: // OR gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::OR_GATE);
                        break;
                    }
                    case 2: // XOR gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::XOR_GATE);
                        break;
                    }
                    case 3: // NOT gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::NOT_GATE);
                        break;
                    }
                    case 4: // Buffer gate
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::BUFFER_GATE);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown gate";
                        break;
                    }
                }
                break;
            }
            case 1: // Inputs
            {
                switch(pIndex.row())
                {
                    case 0: // Switch
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::INPUT);
                        break;
                    }
                    case 1: // Button
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::BUTTON);
                        break;
                    }
                    case 2: // Clock
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::CLOCK);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown input";
                        break;
                    }
                }
                break;
            }
            case 3: // Adders
            {
                switch(pIndex.row())
                {
                    case 0: // Half adder
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::HALF_ADDER);
                        break;
                    }
                    case 1: // Full adder
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::FULL_ADDER);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown adder";
                        break;
                    }
                }
                break;
            }
            case 4: // Memory
            {
                switch(pIndex.row())
                {
                    case 0: // RS flip flop
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::RS_FLIPFLOP);
                        break;
                    }
                    case 1: // D flip flop
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::D_FLIPFLOP);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown memory";
                        break;
                    }
                }
                break;
            }
            case 5: // Converters
            {
                switch(pIndex.row())
                {
                    case 0: // Multiplexer
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::MULTIPLEXER);
                        break;
                    }
                    case 1: // Demultiplexer
                    {
                        mCoreLogic.EnterAddControlMode(ComponentType::DEMULTIPLEXER);
                        break;
                    }
                    default:
                    {
                        qDebug() << "Unknown converter";
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        qDebug() << "Unknown higher level item";
    }
}

