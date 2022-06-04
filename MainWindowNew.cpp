#include "MainWindowNew.h"
#include "ui_MainWindowNew.h"

MainWindowNew::MainWindowNew(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindowNew),
    mView(mCoreLogic),
    mCoreLogic(mView)
{
    mUi->setupUi(this);

    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();
    mAwesome->setDefaultOption("color", QColor(255, 255, 255));
    mAwesome->setDefaultOption("color-disabled", QColor(100, 100, 100));
    mAwesome->setDefaultOption("color-active", QColor(0, 204, 143));
    mAwesome->setDefaultOption("color-selected", QColor(255, 255, 255));

    mScene.setSceneRect(canvas::DIMENSIONS);
    mView.SetScene(mScene);

    mUi->uViewLayout->addWidget(&mView, 1, 1);

    mView.stackUnder(mUi->uLeftContainer);

    QObject::connect(&mCoreLogic, &CoreLogic::ControlModeChangedSignal, this, &MainWindowNew::OnControlModeChanged);

    ConnectGuiSignalsAndSlots();

    InitializeToolboxTree();

    InitializeGlobalShortcuts();

    mToolButtonVariant.insert("color", QColor(0, 204, 143));
    mToolButtonVariant.insert("color-disabled", QColor(64, 64, 64));
    mToolButtonVariant.insert("color-active", QColor(0, 45, 50));
    mToolButtonVariant.insert("color-selected", QColor(0, 204, 143));

    mUncheckedButtonVariant.insert("color", QColor(0, 204, 143));
    mUncheckedButtonVariant.insert("color-disabled", QColor(64, 64, 64));
    mUncheckedButtonVariant.insert("color-active", QColor(0, 204, 143));
    mUncheckedButtonVariant.insert("color-selected", QColor(0, 204, 143));

    mCheckedButtonVariant.insert("color", QColor(0, 18, 20));
    mCheckedButtonVariant.insert("color-disabled", QColor(64, 64, 64));
    mCheckedButtonVariant.insert("color-active", QColor(0, 18, 20));
    mCheckedButtonVariant.insert("color-selected", QColor(0, 18, 20));

    //mUi->uEditButton->setIcon(mAwesome->icon(fa::mousepointer, mToolButtonVariant));

    mUi->uEditButton->SetCheckedIcon(mAwesome->icon(fa::mousepointer, mCheckedButtonVariant));
    mUi->uEditButton->SetUncheckedIcon(mAwesome->icon(fa::mousepointer, mUncheckedButtonVariant));
    mUi->uWiringButton->SetCheckedIcon(mAwesome->icon(fa::exchange, mCheckedButtonVariant));
    mUi->uWiringButton->SetUncheckedIcon(mAwesome->icon(fa::exchange, mUncheckedButtonVariant));

    mUi->uCopyButton->SetIcon(mAwesome->icon(fa::clone, mUncheckedButtonVariant));
    //mUi->uCopyButton->setIcon(mAwesome->icon(fa::clone, mToolButtonVariant));
    //mUi->uSimulationButton->setIcon(mAwesome->icon(fa::play, mToolButtonVariant));

    mUi->uSimulationButton->SetCheckedIcon(mAwesome->icon(fa::stop, mCheckedButtonVariant));
    mUi->uSimulationButton->SetUncheckedIcon(mAwesome->icon(fa::play, mUncheckedButtonVariant));

    mUi->uActionAbout->setIcon(mAwesome->icon(fa::infocircle, mToolButtonVariant));

    mAboutDialog.setAttribute(Qt::WA_QuitOnClose, false); // Make about dialog close when main window closes
}

MainWindowNew::~MainWindowNew()
{
    delete mUi;
}

void MainWindowNew::ConnectGuiSignalsAndSlots()
{
    QObject::connect(mUi->uEditButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });

    QObject::connect(mUi->uWiringButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::WIRE);
    });

    QObject::connect(mUi->uCopyButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::CopySelectedComponents);

    QObject::connect(mUi->uSimulationButton, &QAbstractButton::clicked, [&]()
    {
        /*QVariantMap buttonIconVariant;
        buttonIconVariant.insert("color", QColor(0, 204, 143));*/
        if (mCoreLogic.IsSimulationRunning())
        {
#warning move into separate function
            //mUi->uSimulationButton->setIcon(mAwesome->icon(fa::play, buttonIconVariant));
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
        }
        else
        {
            //mUi->uSimulationButton->setIcon(mAwesome->icon(fa::stop, buttonIconVariant));
            mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
        }
    });

    QObject::connect(mUi->uActionAbout, &QAction::triggered, &mAboutDialog, &AboutDialog::show);
}

void MainWindowNew::OnControlModeChanged(ControlMode pNewMode)
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
            mUi->uEditButton->setChecked(true);
            mUi->uWiringButton->setChecked(false);
            mUi->uSimulationButton->setChecked(false);
            break;
        }
        case ControlMode::WIRE:
        {
            mUi->uToolboxTree->clearSelection();
            mUi->uToolboxTree->setEnabled(true);

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uCopyButton->setEnabled(true);
            mUi->uEditButton->setChecked(false);
            mUi->uWiringButton->setChecked(true);
            mUi->uSimulationButton->setChecked(false);
            break;
        }
        case ControlMode::ADD:
        {
            mUi->uToolboxTree->setEnabled(true);

            mUi->uEditButton->setEnabled(true);
            mUi->uWiringButton->setEnabled(true);
            mUi->uCopyButton->setEnabled(true);
            mUi->buttonGroup->setExclusive(false);
            mUi->uEditButton->setChecked(false);
            mUi->uWiringButton->setChecked(false);
            mUi->buttonGroup->setExclusive(true);
            mUi->uSimulationButton->setChecked(false);
            break;
        }
        case ControlMode::SIMULATION:
        {
            mUi->uToolboxTree->clearSelection();
            mUi->uToolboxTree->setEnabled(false);

            mUi->uEditButton->setEnabled(false);
            mUi->uWiringButton->setEnabled(false);
            mUi->uCopyButton->setEnabled(false);
            mUi->buttonGroup->setExclusive(false);
            mUi->uEditButton->setChecked(false);
            mUi->uWiringButton->setChecked(false);
            mUi->buttonGroup->setExclusive(true);
            mUi->uSimulationButton->setChecked(true);
            break;
        }
        default:
        {
            break;
        }
    }

    mScene.clearSelection();
}


#warning clear selection on simulation start
void MainWindowNew::InitializeToolboxTree()
{
    //const QBrush folderBrush(QColor(220, 220, 220));

    QObject::connect(mUi->uToolboxTree, &QTreeView::pressed, this, &MainWindowNew::OnToolboxTreeClicked);

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
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(mAwesome->icon(fa::chevrondown));
            }
            else
            {
                mUi->uToolboxTree->expand(mUi->uToolboxTree->currentIndex());
                mToolboxTreeModel.itemFromIndex(mUi->uToolboxTree->currentIndex())->setIcon(mAwesome->icon(fa::chevronup));
            }
        }
    });

    // Drop shadows

    /*auto leftContainerShadow = new QGraphicsDropShadowEffect;
    leftContainerShadow->setBlurRadius(4);
    leftContainerShadow->setXOffset(2);
    leftContainerShadow->setYOffset(0);
    leftContainerShadow->setColor(QColor(0, 0, 0, 50));

    mUi->uLeftContainer->setGraphicsEffect(leftContainerShadow);*/

    /*auto topBarShadow = new QGraphicsDropShadowEffect;
    topBarShadow->setBlurRadius(4);
    topBarShadow->setXOffset(0);
    topBarShadow->setYOffset(2);
    topBarShadow->setColor(QColor(0, 0, 0, 50));

    mUi->uTopBar->setGraphicsEffect(topBarShadow);*/

    // Create category and root level items
    mCategoryGatesItem = new QStandardItem(mAwesome->icon(fa::chevronup), "Gates");
    mCategoryGatesItem->setSelectable(false);
    //mCategoryGatesItem->setBackground(folderBrush);
    mToolboxTreeModel.appendRow(mCategoryGatesItem);

    mCategoryInputsItem = new QStandardItem(mAwesome->icon(fa::chevronup), "Inputs");
    mCategoryInputsItem->setSelectable(false);
    //mCategoryInputsItem->setBackground(folderBrush);
    mToolboxTreeModel.appendRow(mCategoryInputsItem);

    auto outputItem = new QStandardItem(QIcon(":images/icons/output_icon.png"), "Output");
    mToolboxTreeModel.appendRow(outputItem);

    mCategoryAddersItem = new QStandardItem(mAwesome->icon(fa::chevrondown), "Adders");
    mCategoryAddersItem->setSelectable(false);
    //mCategoryAddersItem->setBackground(folderBrush);
    mToolboxTreeModel.appendRow(mCategoryAddersItem);

    mCategoryMemoryItem = new QStandardItem(mAwesome->icon(fa::chevrondown), "Memory");
    mCategoryMemoryItem->setSelectable(false);
    //mCategoryMemoryItem->setBackground(folderBrush);
    mToolboxTreeModel.appendRow(mCategoryMemoryItem);

    mCategoryConvertersItem = new QStandardItem(mAwesome->icon(fa::chevrondown), "Converters");
    mCategoryConvertersItem->setSelectable(false);
    //mCategoryConvertersItem->setBackground(folderBrush);
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

void MainWindowNew::InitializeGlobalShortcuts()
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

    mSimulationShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return), this);

    mSimulationShortcut->setAutoRepeat(false);

    QObject::connect(mSimulationShortcut, &QShortcut::activated, this, [&]()
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

void MainWindowNew::SetComponentInputCountIfInAddMode(uint8_t pCount)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetComponentInputCount(pCount);
    }
}

void MainWindowNew::SetComponentDirectionIfInAddMode(Direction pDirection)
{
    if (mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.SetComponentDirection(pDirection);
    }
}

View& MainWindowNew::GetView()
{
    return mView;
}

CoreLogic& MainWindowNew::GetCoreLogic()
{
    return mCoreLogic;
}

void MainWindowNew::OnToolboxTreeClicked(const QModelIndex &pIndex)
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

