#include "View.h"
#include "HelperFunctions.h"
#include "Components/IBaseComponent.h"

#include <QtWidgets>

void View::CreateGui()
{
    mRibbonMenu = new QTabWidget;
    mRibbonMenu->setObjectName("ribbonMenu");

#warning disable all tabs while loading or all individual elements (plus styling)

    // Standard tabs
    mToolboxPage = new QWidget;
    mRibbonMenu->addTab(mToolboxPage, "Toolbox");

    mSimulationPage = new QWidget;
    mRibbonMenu->addTab(mSimulationPage, "Simulation");

    // Special tabs
    mClockPage = new QWidget;
    mRibbonMenu->addTab(mClockPage, "Clock");
    mRibbonMenu->setTabVisible(mRibbonMenu->indexOf(mClockPage), false);
    mRibbonMenu->setTabPosition(QTabWidget::TabPosition::North);
    mRibbonMenu->setFixedHeight(156);

    mEditButton = new QToolButton;
    mEditButton->setText(tr("Edit"));
    mEditButton->setCheckable(true);
    mEditButton->setChecked(true);
    mEditButton->setFixedSize(90, 58);

    mDeleteButton = new QToolButton;
    mDeleteButton->setText(tr("Delete"));
    mDeleteButton->setFixedSize(90, 42);

    mCopyButton = new QToolButton;
    mCopyButton->setText(tr("Copy"));
    mCopyButton->setFixedSize(90, 42);

    {
        mAddWireButton = new QToolButton;
        mAddWireButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/wire.png");
        mAddWireButton->setIcon(QIcon(pixmap));
        mAddWireButton->setIconSize(pixmap.rect().size());
        mAddWireButton->setFixedSize(90, 58);
        mAddWireButton->setToolTip(tr("Wiring tool"));
    }

    {
        mAddAndGateButton = new QToolButton;
        mAddAndGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/and_gate.png");
        mAddAndGateButton->setIcon(QIcon(pixmap));
        mAddAndGateButton->setIconSize(pixmap.rect().size());
        mAddAndGateButton->setFixedSize(90, 106);
        mAddAndGateButton->setToolTip(tr("AND gate"));
    }

    {
        mAddOrGateButton = new QToolButton;
        mAddOrGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/or_gate.png");
        mAddOrGateButton->setIcon(QIcon(pixmap));
        mAddOrGateButton->setIconSize(pixmap.rect().size());
        mAddOrGateButton->setFixedSize(90, 106);
        mAddOrGateButton->setToolTip(tr("OR gate"));
    }

    {
        mAddXorGateButton = new QToolButton;
        mAddXorGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/xor_gate.png");
        mAddXorGateButton->setIcon(QIcon(pixmap));
        mAddXorGateButton->setIconSize(pixmap.rect().size());
        mAddXorGateButton->setFixedSize(90, 106);
        mAddXorGateButton->setToolTip(tr("XOR gate"));
    }

    {
        mAddNotGateButton = new QToolButton;
        mAddNotGateButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/not_gate.png");
        mAddNotGateButton->setIcon(QIcon(pixmap));
        mAddNotGateButton->setIconSize(pixmap.rect().size());
        mAddNotGateButton->setFixedSize(90, 58);
        mAddNotGateButton->setToolTip(tr("NOT gate"));
    }

    {
        mAddInputButton = new QToolButton;
        mAddInputButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/input.png");
        mAddInputButton->setIcon(QIcon(pixmap));
        mAddInputButton->setIconSize(pixmap.rect().size());
        mAddInputButton->setFixedSize(42, 42);
        mAddInputButton->setToolTip(tr("Input"));
    }

    {
        mAddButtonButton = new QToolButton;
        mAddButtonButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/button.png");
        mAddButtonButton->setIcon(QIcon(pixmap));
        mAddButtonButton->setIconSize(pixmap.rect().size());
        mAddButtonButton->setFixedSize(42, 42);
        mAddButtonButton->setToolTip(tr("Button"));
    }

    {
        mAddClockButton = new QToolButton;
        mAddClockButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/clock.png");
        mAddClockButton->setIcon(QIcon(pixmap));
        mAddClockButton->setIconSize(pixmap.rect().size());
        mAddClockButton->setFixedSize(90, 58);
        mAddClockButton->setToolTip(tr("Clock"));
    }

    {
        mAddOutputButton = new QToolButton;
        mAddOutputButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/output.png");
        mAddOutputButton->setIcon(QIcon(pixmap));
        mAddOutputButton->setIconSize(pixmap.rect().size());
        mAddOutputButton->setFixedSize(90, 42);
        mAddOutputButton->setToolTip(tr("Output"));
    }

    {
        mAddTextLabelButton = new QToolButton;
        mAddTextLabelButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/label.png");
        mAddTextLabelButton->setIcon(QIcon(pixmap));
        mAddTextLabelButton->setIconSize(pixmap.rect().size());
        mAddTextLabelButton->setFixedSize(90, 106);
        mAddTextLabelButton->setToolTip(tr("Text label"));
    }

    {
        mAddHalfAdderButton = new QToolButton;
        mAddHalfAdderButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/half_adder.png");
        mAddHalfAdderButton->setIcon(QIcon(pixmap));
        mAddHalfAdderButton->setIconSize(pixmap.rect().size());
        mAddHalfAdderButton->setFixedSize(90, 106);
        mAddHalfAdderButton->setToolTip(tr("Half adder"));
    }

    {
        mAddFullAdderButton = new QToolButton;
        mAddFullAdderButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/full_adder.png");
        mAddFullAdderButton->setIcon(QIcon(pixmap));
        mAddFullAdderButton->setIconSize(pixmap.rect().size());
        mAddFullAdderButton->setFixedSize(90, 106);
        mAddFullAdderButton->setToolTip(tr("Full adder"));
    }

    {
        mAddRsFlipFlopButton = new QToolButton;
        mAddRsFlipFlopButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/rs_flipflop.png");
        mAddRsFlipFlopButton->setIcon(QIcon(pixmap));
        mAddRsFlipFlopButton->setIconSize(pixmap.rect().size());
        mAddRsFlipFlopButton->setFixedSize(90, 106);
        mAddRsFlipFlopButton->setToolTip(tr("RS flip-flop"));
    }

    {
        mAddDFlipFlopButton = new QToolButton;
        mAddDFlipFlopButton->setCheckable(true);

        QPixmap pixmap(":/images/icons/d_flipflop.png");
        mAddDFlipFlopButton->setIcon(QIcon(pixmap));
        mAddDFlipFlopButton->setIconSize(pixmap.rect().size());
        mAddDFlipFlopButton->setFixedSize(90, 106);
        mAddDFlipFlopButton->setToolTip(tr("D flip-flop"));
    }

    mUndoButton = new QToolButton;
    mUndoButton->setText(tr("Undo"));
    mUndoButton->setEnabled(false);
    mUndoButton->setFixedSize(90, 58);

    mRedoButton = new QToolButton;
    mRedoButton->setText(tr("Redo"));
    mRedoButton->setEnabled(false);
    mRedoButton->setFixedSize(90, 42);

    mSimulationButton = new QToolButton;
    mSimulationButton->setText(tr("Start"));
    mSimulationButton->setFixedSize(90, 106);

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

    auto toolboxTabLayout = new QGridLayout;

    {
        toolboxTabLayout->setAlignment(Qt::AlignLeft);
        toolboxTabLayout->setContentsMargins(10, 10, 0, 0);
        toolboxTabLayout->addWidget(mEditButton, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mDeleteButton, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddWireButton, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mCopyButton, 1, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddAndGateButton, 0, 2, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddOrGateButton, 0, 3, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddXorGateButton, 0, 4, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddNotGateButton, 0, 5, 1, 2, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddInputButton, 1, 5, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddButtonButton, 1, 6, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddClockButton, 0, 7, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddOutputButton, 1, 7, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddTextLabelButton, 0, 8, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddHalfAdderButton, 0, 9, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddFullAdderButton, 0, 10, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddRsFlipFlopButton, 0, 11, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mAddDFlipFlopButton, 0, 12, 2, 1, Qt::AlignLeft | Qt::AlignTop);

#warning remove undo and redo buttons from toolbox tab
        toolboxTabLayout->addWidget(mUndoButton, 0, 13, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        toolboxTabLayout->addWidget(mRedoButton, 1, 13, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    auto simulationMenuLayout = new QGridLayout;

    {
        simulationMenuLayout->setAlignment(Qt::AlignLeft);
        simulationMenuLayout->setContentsMargins(10, 10, 0, 0);

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

    mToolboxPage->setLayout(toolboxTabLayout);
    mSimulationPage->setLayout(simulationMenuLayout);

    QGridLayout *mainGridLayout = new QGridLayout;

    mainGridLayout->setSpacing(0);
    mainGridLayout->addWidget(mRibbonMenu, 0, 0);
    mainGridLayout->addWidget(&mGraphicsView, 1, 0);
    mainGridLayout->addWidget(mProcessingOverlay, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    mainGridLayout->addWidget(mZoomLabel, 1, 0, Qt::AlignBottom | Qt::AlignLeft);
    mainGridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainGridLayout);

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
    QObject::connect(mEditButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });

    QObject::connect(mSimulationButton, &QAbstractButton::clicked, [&](){
        if (mCoreLogic.IsSimulationRunning())
        {
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
        }
        else
        {
            mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
        }
    });

    QObject::connect(mAddWireButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterControlMode(ControlMode::WIRE);
    });

    QObject::connect(mAddAndGateButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::AND_GATE);
    });

    QObject::connect(mAddOrGateButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::OR_GATE);
    });

    QObject::connect(mAddXorGateButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::XOR_GATE);
    });

    QObject::connect(mAddNotGateButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::NOT_GATE);
    });

    QObject::connect(mAddInputButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::INPUT);
    });

    QObject::connect(mAddButtonButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::BUTTON);
    });

    QObject::connect(mAddClockButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::CLOCK);
    });

    QObject::connect(mAddOutputButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::OUTPUT);
    });

    QObject::connect(mAddTextLabelButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::TEXT_LABEL);
    });

    QObject::connect(mAddHalfAdderButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::HALF_ADDER);
    });

    QObject::connect(mAddFullAdderButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::FULL_ADDER);
    });

    QObject::connect(mAddRsFlipFlopButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::RS_FLIPFLOP);
    });

    QObject::connect(mAddDFlipFlopButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::D_FLIPFLOP);
    });

    QObject::connect(mDeleteButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::DeleteSelectedComponents);
    QObject::connect(mCopyButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::CopySelectedComponents);
    QObject::connect(mUndoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Undo);
    QObject::connect(mRedoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Redo);
}
