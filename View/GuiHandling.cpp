#include "View.h"
#include "HelperFunctions.h"
#include "Components/IBaseComponent.h"

#include <QtWidgets>

void View::CreateGui()
{
    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();
    mAwesome->setDefaultOption("color", QColor(0, 204, 143));
    mAwesome->setDefaultOption("color-disabled", QColor(0, 100, 70));

    mRibbonMenu = new QTabWidget;
    mRibbonMenu->setObjectName("ribbonMenu");

#warning disable all tabs while loading or all individual elements (plus styling)

    // Standard tabs
    mStartPage = new QWidget;
    mRibbonMenu->addTab(mStartPage, "Start");

    mToolboxPage = new QWidget;
    mRibbonMenu->addTab(mToolboxPage, "Toolbox");

    mSimulationPage = new QWidget;
    mRibbonMenu->addTab(mSimulationPage, "Simulation");

    mRibbonMenu->setCurrentIndex(1); // Select toolbox tab

    // Special tabs
    mClockPage = new QWidget;
    mRibbonMenu->addTab(mClockPage, "Clock");
    mRibbonMenu->setTabVisible(mRibbonMenu->indexOf(mClockPage), false);

    mRibbonMenu->setTabPosition(QTabWidget::TabPosition::North);
    mRibbonMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mRibbonMenu->setFixedHeight(170);

    mCategoryToolsLabel = new QLabel("Tools");
    mCategoryToolsLabel->setAccessibleName("category-label");
    mCategoryToolsLabel->setAlignment(Qt::AlignCenter);
    mCategoryToolsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mCategoryGatesLabel = new QLabel("Gates");
    mCategoryGatesLabel->setAccessibleName("category-label");
    mCategoryGatesLabel->setAlignment(Qt::AlignCenter);
    mCategoryGatesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mCategoryAddersLabel = new QLabel("Adders");
    mCategoryAddersLabel->setAccessibleName("category-label");
    mCategoryAddersLabel->setAlignment(Qt::AlignCenter);
    mCategoryAddersLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mCategoryMemoryLabel = new QLabel("Memory");
    mCategoryMemoryLabel->setAccessibleName("category-label");
    mCategoryMemoryLabel->setAlignment(Qt::AlignCenter);
    mCategoryMemoryLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mEditButton = new QToolButton;
    mEditButton->setAccessibleName("icon-and-text");
    mEditButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mEditButton->setCheckable(true);
    mEditButton->setChecked(true);
    mEditButton->setText(tr("Edit"));
    mEditButton->setIcon(mAwesome->icon(fa::pencil));
    mEditButton->setIconSize(QSize(30, 30));
    mEditButton->setFixedSize(90, 98);
    mEditButton->setToolTip(tr("Edit tool"));

    mCopyButton = new QToolButton;
    mCopyButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mCopyButton->setText(tr("Copy"));
    mCopyButton->setIcon(mAwesome->icon(fa::clone));
    mCopyButton->setIconSize(QSize(15, 15));
    mCopyButton->setFixedSize(90, 47);
    mCopyButton->setStyleSheet("padding-left: 12px;"); // Center manually
    mCopyButton->setToolTip(tr("Copy"));

    mDeleteButton = new QToolButton;
    mDeleteButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mDeleteButton->setText(tr("Delete"));
    mDeleteButton->setIcon(mAwesome->icon(fa::trash));
    mDeleteButton->setIconSize(QSize(20, 20));
    mDeleteButton->setStyleSheet("padding-left: 5px;"); // Center manually
    mDeleteButton->setFixedSize(90, 47);
    mDeleteButton->setToolTip(tr("Delete"));

    {
        mAddWireButton = new QToolButton;
        mAddWireButton->setAccessibleName("icon-and-text");
        mAddWireButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        mAddWireButton->setCheckable(true);
        mAddWireButton->setText(tr("Wires"));

        QPixmap pixmap(":/images/icons/wire.png");
        mAddWireButton->setIcon(QIcon(pixmap));
        mAddWireButton->setIconSize(QSize(30, 30));
        mAddWireButton->setFixedSize(90, 98);
        mAddWireButton->setToolTip(tr("Wiring tool"));
    }

    mSeparatorLine1 = new QFrame;
    mSeparatorLine1->setAccessibleName("separator");
    mSeparatorLine1->setFrameShape(QFrame::VLine);
    mSeparatorLine1->setFrameShadow(QFrame::Plain);
    mSeparatorLine1->setFixedSize(QSize(10, 98));

    {
        mAddAndGateButton = new QToolButton;
        mAddAndGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/and_gate.png");
        mAddAndGateButton->setIcon(QIcon(pixmap));
        mAddAndGateButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddAndGateButton->setFixedSize(90, 98);
        mAddAndGateButton->setToolTip(tr("AND gate"));
    }

    {
        mAddOrGateButton = new QToolButton;
        mAddOrGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/or_gate.png");
        mAddOrGateButton->setIcon(QIcon(pixmap));
        mAddOrGateButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddOrGateButton->setFixedSize(90, 98);
        mAddOrGateButton->setToolTip(tr("OR gate"));
    }

    {
        mAddXorGateButton = new QToolButton;
        mAddXorGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/xor_gate.png");
        mAddXorGateButton->setIcon(QIcon(pixmap));
        mAddXorGateButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddXorGateButton->setFixedSize(90, 98);
        mAddXorGateButton->setToolTip(tr("XOR gate"));
    }

    {
        mAddNotGateButton = new QToolButton;
        mAddNotGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/not_gate.png");
        mAddNotGateButton->setIcon(QIcon(pixmap));
        mAddNotGateButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddNotGateButton->setFixedSize(90, 98);
        mAddNotGateButton->setToolTip(tr("NOT gate"));
    }

    mSeparatorLine2 = new QFrame;
    mSeparatorLine2->setAccessibleName("separator");
    mSeparatorLine2->setFrameShape(QFrame::VLine);
    mSeparatorLine2->setFrameShadow(QFrame::Plain);
    mSeparatorLine2->setFixedSize(QSize(10, 98));

    {
        mAddInputButton = new QToolButton;
        mAddInputButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/input.png");
        mAddInputButton->setIcon(QIcon(pixmap));
        mAddInputButton->setIconSize(pixmap.rect().size());
        mAddInputButton->setFixedSize(47, 47);
        mAddInputButton->setToolTip(tr("Input"));
    }

    {
        mAddButtonButton = new QToolButton;
        mAddButtonButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/button.png");
        mAddButtonButton->setIcon(QIcon(pixmap));
        mAddButtonButton->setIconSize(pixmap.rect().size());
        mAddButtonButton->setFixedSize(47, 47);
        mAddButtonButton->setToolTip(tr("Button"));
    }

    {
        mAddClockButton = new QToolButton;
        mAddClockButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/clock.png");
        mAddClockButton->setIcon(QIcon(pixmap));
        mAddClockButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddClockButton->setFixedSize(90, 47);
        mAddClockButton->setToolTip(tr("Clock"));
    }

    {
        mAddOutputButton = new QToolButton;
        mAddOutputButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/output.png");
        mAddOutputButton->setIcon(QIcon(pixmap));
        mAddOutputButton->setIconSize(pixmap.rect().size());
        mAddOutputButton->setFixedSize(99, 47);
        mAddOutputButton->setToolTip(tr("Output"));
    }

    {
        mAddTextLabelButton = new QToolButton;
        mAddTextLabelButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/label.png");
        mAddTextLabelButton->setIcon(QIcon(pixmap));
        mAddTextLabelButton->setIconSize(pixmap.rect().size());
        mAddTextLabelButton->setFixedSize(90, 47);
        mAddTextLabelButton->setToolTip(tr("Text label"));
    }

    mSeparatorLine3 = new QFrame;
    mSeparatorLine3->setAccessibleName("separator");
    mSeparatorLine3->setFrameShape(QFrame::VLine);
    mSeparatorLine3->setFrameShadow(QFrame::Plain);
    mSeparatorLine3->setFixedSize(QSize(10, 98));

    {
        mAddHalfAdderButton = new QToolButton;
        mAddHalfAdderButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/half_adder.png");
        mAddHalfAdderButton->setIcon(QIcon(pixmap));
        mAddHalfAdderButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddHalfAdderButton->setFixedSize(90, 98);
        mAddHalfAdderButton->setToolTip(tr("Half adder"));
    }

    {
        mAddFullAdderButton = new QToolButton;
        mAddFullAdderButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/full_adder.png");
        mAddFullAdderButton->setIcon(QIcon(pixmap));
        mAddFullAdderButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddFullAdderButton->setFixedSize(90, 98);
        mAddFullAdderButton->setToolTip(tr("Full adder"));
    }

    mSeparatorLine4 = new QFrame;
    mSeparatorLine4->setAccessibleName("separator");
    mSeparatorLine4->setFrameShape(QFrame::VLine);
    mSeparatorLine4->setFrameShadow(QFrame::Plain);
    mSeparatorLine4->setFixedSize(QSize(10, 98));

    {
        mAddRsFlipFlopButton = new QToolButton;
        mAddRsFlipFlopButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/rs_flipflop.png");
        mAddRsFlipFlopButton->setIcon(QIcon(pixmap));
        mAddRsFlipFlopButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddRsFlipFlopButton->setFixedSize(90, 98);
        mAddRsFlipFlopButton->setToolTip(tr("RS flip-flop"));
    }

    {
        mAddDFlipFlopButton = new QToolButton;
        mAddDFlipFlopButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/d_flipflop.png");
        mAddDFlipFlopButton->setIcon(QIcon(pixmap));
        mAddDFlipFlopButton->setIconSize(pixmap.rect().size() * 0.9f);
        mAddDFlipFlopButton->setFixedSize(90, 98);
        mAddDFlipFlopButton->setToolTip(tr("D flip-flop"));
    }

    mSeparatorLine5 = new QFrame;
    mSeparatorLine5->setAccessibleName("separator");
    mSeparatorLine5->setFrameShape(QFrame::VLine);
    mSeparatorLine5->setFrameShadow(QFrame::Plain);
    mSeparatorLine5->setFixedSize(QSize(10, 98));

    mUndoButton = new QToolButton;
    mUndoButton->setText(tr("Undo"));
    mUndoButton->setEnabled(false);
    mUndoButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mUndoButton->setIcon(mAwesome->icon(fa::stepbackward));
    mUndoButton->setIconSize(QSize(20, 20));
    mUndoButton->setStyleSheet("padding-left: 10px;"); // Center manually
    mUndoButton->setFixedSize(90, 47);

    mRedoButton = new QToolButton;
    mRedoButton->setText(tr("Redo"));
    mRedoButton->setEnabled(false);
    mRedoButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mRedoButton->setIcon(mAwesome->icon(fa::stepforward));
    mRedoButton->setIconSize(QSize(20, 20));
    mRedoButton->setStyleSheet("padding-left: 10px;"); // Center manually
    mRedoButton->setFixedSize(90, 47);

    mSimulationButton = new QToolButton;
    mSimulationButton->setAccessibleName("icon-and-text");
    mSimulationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mSimulationButton->setText(tr("Start"));
    mSimulationButton->setIcon(mAwesome->icon(fa::play));
    mSimulationButton->setIconSize(QSize(30, 30));
    mSimulationButton->setFixedSize(90, 98);

#warning organize into button group and set properties in loop
    mOpenCircuitButton = new QToolButton();
    mOpenCircuitButton->setAccessibleName("icon-and-text");
    mOpenCircuitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mOpenCircuitButton->setText(tr("Open"));
    mOpenCircuitButton->setIcon(mAwesome->icon(fa::folderopeno));
    mOpenCircuitButton->setIconSize(QSize(30, 30));
    mOpenCircuitButton->setFixedSize(90, 98);

    mSaveCircuitButton = new QToolButton;
    mSaveCircuitButton->setAccessibleName("icon-and-text");
    mSaveCircuitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mSaveCircuitButton->setText(tr("Save"));
    mSaveCircuitButton->setIcon(mAwesome->icon(fa::floppyo));
    mSaveCircuitButton->setIconSize(QSize(30, 30));
    mSaveCircuitButton->setFixedSize(90, 98);

    mUpdateButton = new QToolButton;
    mUpdateButton->setAccessibleName("icon-and-text");
    mUpdateButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mUpdateButton->setText(tr("Look for\nUpdate"));
    mUpdateButton->setIcon(mAwesome->icon(fa::arrowcircleoup));
    mUpdateButton->setIconSize(QSize(30, 30));
    mUpdateButton->setFixedSize(90, 98);

    mHelpButton = new QToolButton;
    mHelpButton->setAccessibleName("icon-and-text");
    mHelpButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mHelpButton->setText(tr("Help"));
    mHelpButton->setIcon(mAwesome->icon(fa::question));
    mHelpButton->setIconSize(QSize(30, 30));
    mHelpButton->setFixedSize(90, 98);

    mAboutDialogButton = new QToolButton;
    mAboutDialogButton->setAccessibleName("icon-and-text");
    mAboutDialogButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mAboutDialogButton->setText(tr("About"));
    mAboutDialogButton->setIcon(mAwesome->icon(fa::info));
    mAboutDialogButton->setIconSize(QSize(30, 30));
    mAboutDialogButton->setFixedSize(90, 98);

    mToolboxButtonGroup = new QButtonGroup(this);
    mToolboxButtonGroup->setExclusive(true);
    mToolboxButtonGroup->addButton(mEditButton);
    mToolboxButtonGroup->addButton(mDeleteButton);
    mToolboxButtonGroup->addButton(mCopyButton);
    mToolboxButtonGroup->addButton(mAddWireButton);
    mToolboxButtonGroup->addButton(mAddAndGateButton);
    mToolboxButtonGroup->addButton(mAddOrGateButton);
    mToolboxButtonGroup->addButton(mAddXorGateButton);
    mToolboxButtonGroup->addButton(mAddNotGateButton);
    mToolboxButtonGroup->addButton(mAddInputButton);
    mToolboxButtonGroup->addButton(mAddButtonButton);
    mToolboxButtonGroup->addButton(mAddClockButton);
    mToolboxButtonGroup->addButton(mAddOutputButton);
    mToolboxButtonGroup->addButton(mAddTextLabelButton);
    mToolboxButtonGroup->addButton(mAddHalfAdderButton);
    mToolboxButtonGroup->addButton(mAddFullAdderButton);
    mToolboxButtonGroup->addButton(mAddRsFlipFlopButton);
    mToolboxButtonGroup->addButton(mAddDFlipFlopButton);

#warning remove undo and redo buttons from toolbox tab
    mToolboxButtonGroup->addButton(mUndoButton);
    mToolboxButtonGroup->addButton(mRedoButton);

    auto startTabLayout = new QGridLayout;

    {
        startTabLayout->setAlignment(Qt::AlignLeft);
        startTabLayout->setSpacing(5);

        startTabLayout->addWidget(mOpenCircuitButton, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        startTabLayout->addWidget(mSaveCircuitButton, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        startTabLayout->addWidget(mUpdateButton, 0, 2, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        startTabLayout->addWidget(mHelpButton, 0, 3, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        startTabLayout->addWidget(mAboutDialogButton, 0, 4, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    auto toolboxTabLayout = new QGridLayout;

    {
        toolboxTabLayout->setAlignment(Qt::AlignLeft);
        toolboxTabLayout->setSpacing(5);
        toolboxTabLayout->addWidget(mCategoryToolsLabel, 2, 0, 1, 3, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mEditButton, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddWireButton, 0, 1, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mCopyButton, 0, 2, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mDeleteButton, 1, 2, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mSeparatorLine1, 0, 3, 3, 1, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddAndGateButton, 0, 4, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddOrGateButton, 0, 5, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mCategoryGatesLabel, 2, 4, 1, 4, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddXorGateButton, 0, 6, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddNotGateButton, 0, 7, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mSeparatorLine2, 0, 8, 3, 1, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddInputButton, 0, 9, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddButtonButton, 0, 10, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddOutputButton, 1, 9, 1, 2, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddClockButton, 0, 11, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddTextLabelButton, 1, 11, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mSeparatorLine3, 0, 12, 3, 1, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mCategoryAddersLabel, 2, 13, 1, 2, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddHalfAdderButton, 0, 13, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddFullAdderButton, 0, 14, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mSeparatorLine4, 0, 15, 3, 1, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mCategoryMemoryLabel, 2, 16, 1, 2, Qt::AlignCenter | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddRsFlipFlopButton, 0, 16, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddDFlipFlopButton, 0, 17, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mSeparatorLine5, 0, 18, 3, 1, Qt::AlignCenter | Qt::AlignTop);

#warning remove undo and redo buttons from toolbox tab?
        toolboxTabLayout->addWidget(mUndoButton, 0, 19, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mRedoButton, 1, 19, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    auto simulationMenuLayout = new QGridLayout;

    {
        simulationMenuLayout->setAlignment(Qt::AlignLeft);
        simulationMenuLayout->setSpacing(5);

#warning add simulation button outside of the ribbon menu for easy access
        simulationMenuLayout->addWidget(mSimulationButton, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    mZoomLabel = new QLabel(this);
    mZoomLabel->setObjectName("zoomLabel");
    mZoomLabel->setText("100%");

    QMovie *procImage = new QMovie(QString(":/images/loading.gif"));
    mProcessingOverlay = new QLabel();
    mProcessingOverlay->setMovie(procImage);
    mProcessingOverlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    procImage->start();
    mProcessingOverlay->hide();

    mStartPage->setLayout(startTabLayout);
    mToolboxPage->setLayout(toolboxTabLayout);
    mSimulationPage->setLayout(simulationMenuLayout);

    QGridLayout *mainGridLayout = new QGridLayout;

    mainGridLayout->setSpacing(0);
    mainGridLayout->addWidget(mRibbonMenu, 0, 0, Qt::AlignTop);
    mainGridLayout->addWidget(&mGraphicsView, 0, 0);
    mainGridLayout->addWidget(mProcessingOverlay, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    mainGridLayout->addWidget(mZoomLabel, 0, 0, Qt::AlignBottom | Qt::AlignLeft);
    mainGridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainGridLayout);

    mGraphicsView.stackUnder(mRibbonMenu);
    mGraphicsView.stackUnder(mProcessingOverlay);
    mGraphicsView.stackUnder(mZoomLabel);
}

void View::FadeInProcessingOverlay()
{
    if (!mProcessingOverlay->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        mProcessingOverlay->setGraphicsEffect(effect);
        mProcessingOverlay->show();

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(500);
        anim->setStartValue(0.0f);
        anim->setEndValue(1.0f);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QObject::connect(anim, &QPropertyAnimation::finished, [&]()
        {
            delete mProcessingOverlay->graphicsEffect();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void View::FadeOutProcessingOverlay()
{
    if (mProcessingOverlay->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        mProcessingOverlay->setGraphicsEffect(effect);

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(500);
        anim->setStartValue(1.0f);
        anim->setEndValue(0.0f);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QObject::connect(anim, &QPropertyAnimation::finished, [&]()
        {
            delete mProcessingOverlay->graphicsEffect();
            mProcessingOverlay->hide();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void View::PrepareGuiForSimulation()
{
    SetToolboxTabEnabled(false);

#warning introduce mSimulationButtonGroup
    mSimulationButton->setText(tr("Stop"));
    mSimulationButton->setIcon(mAwesome->icon(fa::stop));

    HideSpecialTab();

#warning add styling for disabled tabs
    mRibbonMenu->setCurrentIndex(mRibbonMenu->indexOf(mSimulationPage));
    mRibbonMenu->setTabEnabled(mRibbonMenu->indexOf(mToolboxPage), false);
}

void View::PrepareGuiForEditing()
{
    SetToolboxTabEnabled(true);
    SetUndoRedoButtonsEnableState();

    mSimulationButton->setText(tr("Start"));
    mSimulationButton->setIcon(mAwesome->icon(fa::play));

    const auto&& componentsTabIndex = mRibbonMenu->indexOf(mToolboxPage);
    mRibbonMenu->setTabEnabled(componentsTabIndex, true);
}

void View::SetToolboxTabEnabled(bool pEnabled)
{
    for (auto& button : mToolboxButtonGroup->buttons())
    {
        button->setEnabled(pEnabled);
    }
}

void View::SetUndoRedoButtonsEnableState()
{
    mUndoButton->setEnabled(!mCoreLogic.IsSimulationRunning() && !mCoreLogic.IsUndoQueueEmpty());
    mRedoButton->setEnabled(!mCoreLogic.IsSimulationRunning() && !mCoreLogic.IsRedoQueueEmpty());
}

void View::ConnectGuiSignalsAndSlots()
{
    QObject::connect(mEditButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });

    QObject::connect(mSimulationButton, &QAbstractButton::clicked, [&]()
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

    QObject::connect(mAddWireButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterControlMode(ControlMode::WIRE);
    });

    QObject::connect(mAddAndGateButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::AND_GATE);
    });

    QObject::connect(mAddOrGateButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::OR_GATE);
    });

    QObject::connect(mAddXorGateButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::XOR_GATE);
    });

    QObject::connect(mAddNotGateButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::NOT_GATE);
    });

    QObject::connect(mAddInputButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::INPUT);
    });

    QObject::connect(mAddButtonButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::BUTTON);
    });

    QObject::connect(mAddClockButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::CLOCK);
    });

    QObject::connect(mAddOutputButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::OUTPUT);
    });

    QObject::connect(mAddTextLabelButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::TEXT_LABEL);
    });

    QObject::connect(mAddHalfAdderButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::HALF_ADDER);
    });

    QObject::connect(mAddFullAdderButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::FULL_ADDER);
    });

    QObject::connect(mAddRsFlipFlopButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::RS_FLIPFLOP);
    });

    QObject::connect(mAddDFlipFlopButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::D_FLIPFLOP);
    });

    QObject::connect(mDeleteButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::DeleteSelectedComponents);
    QObject::connect(mCopyButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::CopySelectedComponents);
    QObject::connect(mUndoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Undo);
    QObject::connect(mRedoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Redo);
}
