#include "View.h"
#include "HelperFunctions.h"
#include "Components/IBaseComponent.h"

#include <QtWidgets>

void View::CreateGui()
{
    mAwesome = new QtAwesome(this);
    mAwesome->initFontAwesome();
    mAwesome->setDefaultOption("color", QColor(0, 204, 143));
    mAwesome->setDefaultOption("color-disabled", QColor(100, 100, 100));

    InitializeRibbonMenu();

    mZoomLabel = new QLabel(this);
    mZoomLabel->setObjectName("zoomLabel");
    mZoomLabel->setText("100%");

    QMovie *procImage = new QMovie(QString(":/images/loading.gif"));
    mProcessingOverlay = new QLabel();
    mProcessingOverlay->setMovie(procImage);
    mProcessingOverlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    procImage->start();
    mProcessingOverlay->hide();

    mMainLayout = new QGridLayout();
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);

    mMainLayout->addWidget(mRibbonMenu, 0, 0, Qt::AlignTop);
    mMainLayout->addWidget(&mGraphicsView, 0, 0);
    mMainLayout->addWidget(mProcessingOverlay, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    mMainLayout->addWidget(mZoomLabel, 0, 0, Qt::AlignBottom | Qt::AlignLeft);

    setLayout(mMainLayout);

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
    Q_ASSERT(mRibbonMenu);

    SetStartTabEnabled(true);
    SetToolboxTabEnabled(false);
    SetSimulationTabEnabled(true);

    mSimulationButton->setText(tr("Stop"));
    mSimulationButton->setIcon(mAwesome->icon(fa::stop));

    HideSpecialTab();

    mRibbonMenu->setCurrentIndex(mRibbonMenu->indexOf(mSimulationPage));
    mRibbonMenu->setTabEnabled(mRibbonMenu->indexOf(mToolboxPage), false);
}

void View::PrepareGuiForEditing()
{
    Q_ASSERT(mRibbonMenu);

    SetGuiEnabled(true);
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

void View::SetStartTabEnabled(bool pEnabled)
{
    for (auto& button : mStartButtonGroup->buttons())
    {
        button->setEnabled(pEnabled);
    }
}

void View::SetSimulationTabEnabled(bool pEnabled)
{
    for (auto& button : mSimulationButtonGroup->buttons())
    {
        button->setEnabled(pEnabled);
    }
}

void View::SetGuiEnabled(bool pEnabled)
{
    SetToolboxTabEnabled(pEnabled);
    SetStartTabEnabled(pEnabled);
    SetSimulationTabEnabled(pEnabled);
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

    QObject::connect(mAddMultiplexerButton, &QAbstractButton::clicked, [&]()
    {
        mCoreLogic.EnterAddControlMode(ComponentType::MULTIPLEXER);
    });

    QObject::connect(mDeleteButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::DeleteSelectedComponents);
    QObject::connect(mCopyButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::CopySelectedComponents);
    QObject::connect(mUndoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Undo);
    QObject::connect(mRedoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Redo);
}

void View::InitializeRibbonMenu()
{
    mRibbonMenu = new QTabWidget();
    mRibbonMenu->setObjectName("ribbonMenu");

    mRibbonMenu->setTabPosition(QTabWidget::TabPosition::North);
    mRibbonMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mRibbonMenu->setFixedHeight(170);

    // Standard tabs
    mStartPage = new QWidget();
    mRibbonMenu->addTab(mStartPage, "Start");

    mToolboxPage = new QWidget();
    mRibbonMenu->addTab(mToolboxPage, "Toolbox");

    mSimulationPage = new QWidget();
    mRibbonMenu->addTab(mSimulationPage, "Simulation");

    mRibbonMenu->setCurrentIndex(1); // Select toolbox tab

    // Special tabs
    mClockPage = new QWidget();
    mRibbonMenu->addTab(mClockPage, "Clock");
    mRibbonMenu->setTabVisible(mRibbonMenu->indexOf(mClockPage), false);

    InitializeStartTabWidgets();
    InitializeToolboxTabWidgets();
    InitializeSimulationTabWidgets();

    FillRibbonMenuButtonGroups();

    InitializeRibbonMenuTabLayouts();
}

void View::InitializeStartTabWidgets()
{
    mCategoryInfoLabel = new QLabel("Info");
    mCategoryInfoLabel->setAccessibleName("category-label");
    mCategoryInfoLabel->setAlignment(Qt::AlignCenter);
    mCategoryInfoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mStartSeparatorLine1 = CreateSeparator();

    mOpenCircuitButton = new QToolButton();
    mOpenCircuitButton->setAccessibleName("icon-and-text");
    mOpenCircuitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mOpenCircuitButton->setText(tr("Open"));
    mOpenCircuitButton->setIcon(mAwesome->icon(fa::folderopeno));
    mOpenCircuitButton->setIconSize(QSize(30, 30));
    mOpenCircuitButton->setFixedSize(90, 98);

    mSaveCircuitButton = new QToolButton();
    mSaveCircuitButton->setAccessibleName("icon-and-text");
    mSaveCircuitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mSaveCircuitButton->setText(tr("Save"));
    mSaveCircuitButton->setIcon(mAwesome->icon(fa::floppyo));
    mSaveCircuitButton->setIconSize(QSize(30, 30));
    mSaveCircuitButton->setFixedSize(90, 98);

    mUpdateButton = new QToolButton();
    mUpdateButton->setAccessibleName("icon-and-text");
    mUpdateButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mUpdateButton->setText(tr("Update"));
    mUpdateButton->setIcon(mAwesome->icon(fa::arrowcircleoup));
    mUpdateButton->setIconSize(QSize(30, 30));
    mUpdateButton->setFixedSize(90, 98);

    mHelpButton = new QToolButton();
    mHelpButton->setAccessibleName("icon-and-text");
    mHelpButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mHelpButton->setText(tr("Help"));
    mHelpButton->setIcon(mAwesome->icon(fa::question));
    mHelpButton->setIconSize(QSize(30, 30));
    mHelpButton->setFixedSize(90, 98);

    mAboutDialogButton = new QToolButton();
    mAboutDialogButton->setAccessibleName("icon-and-text");
    mAboutDialogButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mAboutDialogButton->setText(tr("About"));
    mAboutDialogButton->setIcon(mAwesome->icon(fa::info));
    mAboutDialogButton->setIconSize(QSize(30, 30));
    mAboutDialogButton->setFixedSize(90, 98);
}

void View::InitializeToolboxTabWidgets()
{
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

    mCategoryConvertersLabel = new QLabel("Converters");
    mCategoryConvertersLabel->setAccessibleName("category-label");
    mCategoryConvertersLabel->setAlignment(Qt::AlignCenter);
    mCategoryConvertersLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mSeparatorLine1 = CreateSeparator();
    mSeparatorLine2 = CreateSeparator();
    mSeparatorLine3 = CreateSeparator();
    mSeparatorLine4 = CreateSeparator();
    mSeparatorLine5 = CreateSeparator();
    mSeparatorLine6 = CreateSeparator();

    mEditButton = new QToolButton();
    mEditButton->setAccessibleName("icon-and-text");
    mEditButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mEditButton->setCheckable(true);
    mEditButton->setChecked(true);
    mEditButton->setText(tr("Edit"));
    mEditButton->setIcon(mAwesome->icon(fa::pencil));
    mEditButton->setIconSize(QSize(30, 30));
    mEditButton->setFixedSize(90, 98);
    mEditButton->setToolTip(tr("Edit tool"));

    mCopyButton = new QToolButton();
    mCopyButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mCopyButton->setText(tr("Copy"));
    mCopyButton->setIcon(mAwesome->icon(fa::clone));
    mCopyButton->setIconSize(QSize(15, 15));
    mCopyButton->setFixedSize(90, 47);
    mCopyButton->setStyleSheet("padding-left: 12px;"); // Center manually
    mCopyButton->setToolTip(tr("Copy"));

    mDeleteButton = new QToolButton();
    mDeleteButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mDeleteButton->setText(tr("Delete"));
    mDeleteButton->setIcon(mAwesome->icon(fa::trash));
    mDeleteButton->setIconSize(QSize(20, 20));
    mDeleteButton->setStyleSheet("padding-left: 5px;"); // Center manually
    mDeleteButton->setFixedSize(90, 47);
    mDeleteButton->setToolTip(tr("Delete"));

    mAddWireButton = new QToolButton();
    mAddWireButton->setAccessibleName("icon-and-text");
    mAddWireButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mAddWireButton->setCheckable(true);
    mAddWireButton->setText(tr("Wires"));
    mAddWireButton->setIcon(CreateIcon(":/images/icons/wire.png", ":/images/icons/wire_disabled.png"));
    mAddWireButton->setIconSize(QSize(30, 30));
    mAddWireButton->setFixedSize(90, 98);
    mAddWireButton->setToolTip(tr("Wiring tool"));

    mAddAndGateButton = new QToolButton();
    mAddAndGateButton->setCheckable(true);
    mAddAndGateButton->setIcon(CreateIcon(":/images/icons/and_gate.png", ":/images/icons/and_gate_disabled.png"));
    mAddAndGateButton->setIconSize(mAddAndGateButton->icon().availableSizes().first() * 0.9f);
    mAddAndGateButton->setFixedSize(90, 98);
    mAddAndGateButton->setToolTip(tr("AND gate"));

    mAddOrGateButton = new QToolButton();
    mAddOrGateButton->setCheckable(true);
    mAddOrGateButton->setIcon(CreateIcon(":/images/icons/or_gate.png", ":/images/icons/or_gate_disabled.png"));
    mAddOrGateButton->setIconSize(mAddOrGateButton->icon().availableSizes().first() * 0.9f);
    mAddOrGateButton->setFixedSize(90, 98);
    mAddOrGateButton->setToolTip(tr("OR gate"));

    mAddXorGateButton = new QToolButton();
    mAddXorGateButton->setCheckable(true);
    mAddXorGateButton->setIcon(CreateIcon(":/images/icons/xor_gate.png", ":/images/icons/xor_gate_disabled.png"));
    mAddXorGateButton->setIconSize(mAddXorGateButton->icon().availableSizes().first() * 0.9f);
    mAddXorGateButton->setFixedSize(90, 98);
    mAddXorGateButton->setToolTip(tr("XOR gate"));

    mAddNotGateButton = new QToolButton();
    mAddNotGateButton->setCheckable(true);
    mAddNotGateButton->setIcon(CreateIcon(":/images/icons/not_gate.png", ":/images/icons/not_gate_disabled.png"));
    mAddNotGateButton->setIconSize(mAddNotGateButton->icon().availableSizes().first() * 0.9f);
    mAddNotGateButton->setFixedSize(90, 98);
    mAddNotGateButton->setToolTip(tr("NOT gate"));

    mAddInputButton = new QToolButton();
    mAddInputButton->setCheckable(true);
    mAddInputButton->setIcon(CreateIcon(":/images/icons/input.png", ":/images/icons/input_disabled.png"));
    mAddInputButton->setIconSize(mAddInputButton->icon().availableSizes().first());
    mAddInputButton->setFixedSize(47, 47);
    mAddInputButton->setToolTip(tr("Input"));

    mAddButtonButton = new QToolButton();
    mAddButtonButton->setCheckable(true);
    mAddButtonButton->setIcon(CreateIcon(":/images/icons/button.png", ":/images/icons/button_disabled.png"));
    mAddButtonButton->setIconSize(mAddButtonButton->icon().availableSizes().first());
    mAddButtonButton->setFixedSize(47, 47);
    mAddButtonButton->setToolTip(tr("Button"));

    mAddClockButton = new QToolButton();
    mAddClockButton->setCheckable(true);
    mAddClockButton->setIcon(CreateIcon(":/images/icons/clock.png", ":/images/icons/clock_disabled.png"));
    mAddClockButton->setIconSize(mAddClockButton->icon().availableSizes().first() * 0.9f);
    mAddClockButton->setFixedSize(90, 47);
    mAddClockButton->setToolTip(tr("Clock"));

    mAddOutputButton = new QToolButton();
    mAddOutputButton->setCheckable(true);
    mAddOutputButton->setIcon(CreateIcon(":/images/icons/output.png", ":/images/icons/output_disabled.png"));
    mAddOutputButton->setIconSize(mAddOutputButton->icon().availableSizes().first());
    mAddOutputButton->setFixedSize(99, 47);
    mAddOutputButton->setToolTip(tr("Output"));

    mAddTextLabelButton = new QToolButton();
    mAddTextLabelButton->setCheckable(true);
    mAddTextLabelButton->setIcon(CreateIcon(":/images/icons/label.png", ":/images/icons/label_disabled.png"));
    mAddTextLabelButton->setIconSize(mAddTextLabelButton->icon().availableSizes().first());
    mAddTextLabelButton->setFixedSize(90, 47);
    mAddTextLabelButton->setToolTip(tr("Text label"));

    mAddHalfAdderButton = new QToolButton();
    mAddHalfAdderButton->setCheckable(true);
    mAddHalfAdderButton->setIcon(CreateIcon(":/images/icons/half_adder.png", ":/images/icons/half_adder_disabled.png"));
    mAddHalfAdderButton->setIconSize(mAddHalfAdderButton->icon().availableSizes().first() * 0.9f);
    mAddHalfAdderButton->setFixedSize(90, 98);
    mAddHalfAdderButton->setToolTip(tr("Half adder"));

    mAddFullAdderButton = new QToolButton();
    mAddFullAdderButton->setCheckable(true);

    mAddFullAdderButton->setIcon(CreateIcon(":/images/icons/full_adder.png", ":/images/icons/full_adder_disabled.png"));
    mAddFullAdderButton->setIconSize(mAddFullAdderButton->icon().availableSizes().first() * 0.9f);
    mAddFullAdderButton->setFixedSize(90, 98);
    mAddFullAdderButton->setToolTip(tr("Full adder"));

    mAddRsFlipFlopButton = new QToolButton();
    mAddRsFlipFlopButton->setCheckable(true);
    mAddRsFlipFlopButton->setIcon(CreateIcon(":/images/icons/rs_flipflop.png", ":/images/icons/rs_flipflop_disabled.png"));
    mAddRsFlipFlopButton->setIconSize(mAddRsFlipFlopButton->icon().availableSizes().first() * 0.9f);
    mAddRsFlipFlopButton->setFixedSize(90, 98);
    mAddRsFlipFlopButton->setToolTip(tr("RS flip-flop"));

    mAddDFlipFlopButton = new QToolButton();
    mAddDFlipFlopButton->setCheckable(true);
    mAddDFlipFlopButton->setIcon(CreateIcon(":/images/icons/d_flipflop.png", ":/images/icons/d_flipflop_disabled.png"));
    mAddDFlipFlopButton->setIconSize(mAddDFlipFlopButton->icon().availableSizes().first() * 0.9f);
    mAddDFlipFlopButton->setFixedSize(90, 98);
    mAddDFlipFlopButton->setToolTip(tr("D flip-flop"));

    mAddMultiplexerButton = new QToolButton();
    mAddMultiplexerButton->setCheckable(true);
    mAddMultiplexerButton->setText(tr("Multiplexer"));
    mAddMultiplexerButton->setIcon(CreateIcon(":/images/icons/multiplexer.png", ":/images/icons/multiplexer_disabled.png"));
    mAddMultiplexerButton->setIconSize(mAddDFlipFlopButton->icon().availableSizes().first() * 0.9f);
    mAddMultiplexerButton->setFixedSize(90, 98);
    mAddMultiplexerButton->setToolTip(tr("Multiplexer"));

    mUndoButton = new QToolButton();
    mUndoButton->setText(tr("Undo"));
    mUndoButton->setEnabled(false);
    mUndoButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mUndoButton->setIcon(mAwesome->icon(fa::stepbackward));
    mUndoButton->setIconSize(QSize(20, 20));
    mUndoButton->setStyleSheet("padding-left: 10px;"); // Center manually
    mUndoButton->setFixedSize(90, 47);

    mRedoButton = new QToolButton();
    mRedoButton->setText(tr("Redo"));
    mRedoButton->setEnabled(false);
    mRedoButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mRedoButton->setIcon(mAwesome->icon(fa::stepforward));
    mRedoButton->setIconSize(QSize(20, 20));
    mRedoButton->setStyleSheet("padding-left: 10px;"); // Center manually
    mRedoButton->setFixedSize(90, 47);
}

void View::InitializeSimulationTabWidgets()
{
    mSimulationButton = new QToolButton();
    mSimulationButton->setAccessibleName("icon-and-text");
    mSimulationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mSimulationButton->setText(tr("Start"));
    mSimulationButton->setIcon(mAwesome->icon(fa::play));
    mSimulationButton->setIconSize(QSize(30, 30));
    mSimulationButton->setFixedSize(90, 98);
}

void View::FillRibbonMenuButtonGroups()
{
    mStartButtonGroup = new QButtonGroup(this);
    mStartButtonGroup->addButton(mOpenCircuitButton);
    mStartButtonGroup->addButton(mSaveCircuitButton);
    mStartButtonGroup->addButton(mUpdateButton);
    mStartButtonGroup->addButton(mHelpButton);
    mStartButtonGroup->addButton(mAboutDialogButton);

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
    mToolboxButtonGroup->addButton(mAddMultiplexerButton);

    mSimulationButtonGroup = new QButtonGroup(this);
    mSimulationButtonGroup->addButton(mSimulationButton);

    mToolboxButtonGroup->addButton(mUndoButton);
    mToolboxButtonGroup->addButton(mRedoButton);
}

void View::InitializeRibbonMenuTabLayouts()
{
    mStartTabLayout = new QGridLayout();
    mStartTabLayout->setAlignment(Qt::AlignLeft);
    mStartTabLayout->setSpacing(5);

    mStartTabLayout->addWidget(mOpenCircuitButton, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    mStartTabLayout->addWidget(mSaveCircuitButton, 0, 1, Qt::AlignLeft | Qt::AlignTop);
    mStartTabLayout->addWidget(mStartSeparatorLine1, 0, 2, 2, 1, Qt::AlignCenter | Qt::AlignTop);
    mStartTabLayout->addWidget(mCategoryInfoLabel, 1, 3, 1, 3, Qt::AlignCenter | Qt::AlignTop);
    mStartTabLayout->addWidget(mUpdateButton, 0, 3, Qt::AlignLeft | Qt::AlignTop);
    mStartTabLayout->addWidget(mHelpButton, 0, 4, Qt::AlignLeft | Qt::AlignTop);
    mStartTabLayout->addWidget(mAboutDialogButton, 0, 5, Qt::AlignLeft | Qt::AlignTop);

    mToolboxTabLayout = new QGridLayout();
    mToolboxTabLayout->setAlignment(Qt::AlignLeft);
    mToolboxTabLayout->setSpacing(5);

    mToolboxTabLayout->addWidget(mCategoryToolsLabel, 2, 0, 1, 3, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mEditButton, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddWireButton, 0, 1, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mCopyButton, 0, 2, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mDeleteButton, 1, 2, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mSeparatorLine1, 0, 3, 3, 1, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddAndGateButton, 0, 4, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddOrGateButton, 0, 5, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mCategoryGatesLabel, 2, 4, 1, 4, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddXorGateButton, 0, 6, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddNotGateButton, 0, 7, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mSeparatorLine2, 0, 8, 3, 1, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddInputButton, 0, 9, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddButtonButton, 0, 10, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddOutputButton, 1, 9, 1, 2, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddClockButton, 0, 11, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddTextLabelButton, 1, 11, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mSeparatorLine3, 0, 12, 3, 1, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mCategoryAddersLabel, 2, 13, 1, 2, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddHalfAdderButton, 0, 13, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddFullAdderButton, 0, 14, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mSeparatorLine4, 0, 15, 3, 1, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mCategoryMemoryLabel, 2, 16, 1, 2, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddRsFlipFlopButton, 0, 16, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddDFlipFlopButton, 0, 17, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mSeparatorLine5, 0, 18, 3, 1, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mCategoryConvertersLabel, 2, 19, 1, 1, Qt::AlignCenter | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mAddMultiplexerButton, 0, 19, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mSeparatorLine6, 0, 20, 3, 1, Qt::AlignCenter | Qt::AlignTop);

#warning remove undo and redo buttons from toolbox tab? => is undo/redo needed outside toolbox?
    mToolboxTabLayout->addWidget(mUndoButton, 0, 21, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    mToolboxTabLayout->addWidget(mRedoButton, 1, 21, 1, 1, Qt::AlignLeft | Qt::AlignTop);

    mSimulationTabLayout = new QGridLayout();
    mSimulationTabLayout->setAlignment(Qt::AlignLeft);
    mSimulationTabLayout->setSpacing(5);

#warning add simulation button outside of the ribbon menu for easy access
    mSimulationTabLayout->addWidget(mSimulationButton, 0, 0, Qt::AlignLeft | Qt::AlignTop);

    mStartPage->setLayout(mStartTabLayout);
    mToolboxPage->setLayout(mToolboxTabLayout);
    mSimulationPage->setLayout(mSimulationTabLayout);
}

QFrame* View::CreateSeparator()
{
    auto separator = new QFrame();
    separator->setAccessibleName("separator");
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setFixedSize(QSize(10, 120));

    return separator;
}

QIcon View::CreateIcon(QString pNormalPath, QString pDisabledPath) const
{
    QIcon icon;
    icon.addPixmap(QPixmap(pNormalPath), QIcon::Normal);
    icon.addPixmap(QPixmap(pDisabledPath), QIcon::Disabled);

    return icon;
}
