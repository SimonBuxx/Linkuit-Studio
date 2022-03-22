#include "View.h"
#include "HelperFunctions.h"
#include "Components/IBaseComponent.h"

#include <QtWidgets>

void View::CreateGui()
{
    QHBoxLayout *topButtonsLayout = new QHBoxLayout;

    mEditButton = new QToolButton;
    mEditButton->setText(tr("Edit"));
    mEditButton->setCheckable(true);
    mEditButton->setChecked(true);

    mDeleteButton = new QToolButton;
    mDeleteButton->setText(tr("Delete"));
    mDeleteButton->setCheckable(false);
    mDeleteButton->setChecked(false);

    mCopyButton = new QToolButton;
    mCopyButton->setText(tr("Copy"));
    mCopyButton->setCheckable(false);
    mCopyButton->setChecked(false);

    mAddWireButton = new QToolButton;
    mAddWireButton->setText(tr("Wire"));
    mAddWireButton->setCheckable(true);
    mAddWireButton->setChecked(false);

    mAddAndGateButton = new QToolButton;
    mAddAndGateButton->setText(tr("And Gate"));
    mAddAndGateButton->setCheckable(true);
    mAddAndGateButton->setChecked(false);

    mAddOrGateButton = new QToolButton;
    mAddOrGateButton->setText(tr("Or Gate"));
    mAddOrGateButton->setCheckable(true);
    mAddOrGateButton->setChecked(false);

    mAddXorGateButton = new QToolButton;
    mAddXorGateButton->setText(tr("Xor Gate"));
    mAddXorGateButton->setCheckable(true);
    mAddXorGateButton->setChecked(false);

    mAddNotGateButton = new QToolButton;
    mAddNotGateButton->setText(tr("Not Gate"));
    mAddNotGateButton->setCheckable(true);
    mAddNotGateButton->setChecked(false);

    mAddInputButton = new QToolButton;
    mAddInputButton->setText(tr("Input"));
    mAddInputButton->setCheckable(true);
    mAddInputButton->setChecked(false);

    mAddButtonButton = new QToolButton;
    mAddButtonButton->setText(tr("Button"));
    mAddButtonButton->setCheckable(true);
    mAddButtonButton->setChecked(false);

    mAddClockButton = new QToolButton;
    mAddClockButton->setText(tr("Clock"));
    mAddClockButton->setCheckable(true);
    mAddClockButton->setChecked(false);

    mAddOutputButton = new QToolButton;
    mAddOutputButton->setText(tr("Output"));
    mAddOutputButton->setCheckable(true);
    mAddOutputButton->setChecked(false);

    mAddTextLabelButton = new QToolButton;
    mAddTextLabelButton->setText(tr("Label"));
    mAddTextLabelButton->setCheckable(true);
    mAddTextLabelButton->setChecked(false);

    mAddHalfAdderButton = new QToolButton;
    mAddHalfAdderButton->setText(tr("Half Adder"));
    mAddHalfAdderButton->setCheckable(true);
    mAddHalfAdderButton->setChecked(false);

    mAddFullAdderButton = new QToolButton;
    mAddFullAdderButton->setText(tr("Full Adder"));
    mAddFullAdderButton->setCheckable(true);
    mAddFullAdderButton->setChecked(false);

    mAddRsFlipFlopButton = new QToolButton;
    mAddRsFlipFlopButton->setText(tr("RS Flip-Flop"));
    mAddRsFlipFlopButton->setCheckable(true);
    mAddRsFlipFlopButton->setChecked(false);

    mAddDFlipFlopButton = new QToolButton;
    mAddDFlipFlopButton->setText(tr("D Flip-Flop"));
    mAddDFlipFlopButton->setCheckable(true);
    mAddDFlipFlopButton->setChecked(false);

    mUndoButton = new QToolButton;
    mUndoButton->setText(tr("Undo"));
    mUndoButton->setCheckable(false);
    mUndoButton->setChecked(false);
    mUndoButton->setEnabled(false);

    mRedoButton = new QToolButton;
    mRedoButton->setText(tr("Redo"));
    mRedoButton->setCheckable(false);
    mRedoButton->setChecked(false);
    mRedoButton->setEnabled(false);

    mSimulationButton = new QToolButton;
    mSimulationButton->setText(tr("Start"));
    mSimulationButton->setCheckable(false);
    mSimulationButton->setChecked(false);

    mTopButtonsGroup = new QButtonGroup(this);
    mTopButtonsGroup->setExclusive(true);
    mTopButtonsGroup->addButton(mEditButton);
    mTopButtonsGroup->addButton(mDeleteButton);
    mTopButtonsGroup->addButton(mCopyButton);
    mTopButtonsGroup->addButton(mAddWireButton);
    mTopButtonsGroup->addButton(mAddAndGateButton);
    mTopButtonsGroup->addButton(mAddOrGateButton);
    mTopButtonsGroup->addButton(mAddXorGateButton);
    mTopButtonsGroup->addButton(mAddNotGateButton);
    mTopButtonsGroup->addButton(mAddInputButton);
    mTopButtonsGroup->addButton(mAddButtonButton);
    mTopButtonsGroup->addButton(mAddClockButton);
    mTopButtonsGroup->addButton(mAddOutputButton);
    mTopButtonsGroup->addButton(mAddTextLabelButton);
    mTopButtonsGroup->addButton(mAddHalfAdderButton);
    mTopButtonsGroup->addButton(mAddFullAdderButton);
    mTopButtonsGroup->addButton(mAddRsFlipFlopButton);
    mTopButtonsGroup->addButton(mAddDFlipFlopButton);
    mTopButtonsGroup->addButton(mUndoButton);
    mTopButtonsGroup->addButton(mRedoButton);
    mTopButtonsGroup->addButton(mSimulationButton);

    topButtonsLayout->addStretch();
    topButtonsLayout->addWidget(mEditButton);
    topButtonsLayout->addWidget(mDeleteButton);
    topButtonsLayout->addWidget(mCopyButton);
    topButtonsLayout->addWidget(mAddWireButton);
    topButtonsLayout->addWidget(mAddAndGateButton);
    topButtonsLayout->addWidget(mAddOrGateButton);
    topButtonsLayout->addWidget(mAddXorGateButton);
    topButtonsLayout->addWidget(mAddNotGateButton);
    topButtonsLayout->addWidget(mAddInputButton);
    topButtonsLayout->addWidget(mAddButtonButton);
    topButtonsLayout->addWidget(mAddClockButton);
    topButtonsLayout->addWidget(mAddOutputButton);
    topButtonsLayout->addWidget(mAddTextLabelButton);
    topButtonsLayout->addWidget(mAddHalfAdderButton);
    topButtonsLayout->addWidget(mAddFullAdderButton);
    topButtonsLayout->addWidget(mAddRsFlipFlopButton);
    topButtonsLayout->addWidget(mAddDFlipFlopButton);
    topButtonsLayout->addWidget(mUndoButton);
    topButtonsLayout->addWidget(mRedoButton);
    topButtonsLayout->addWidget(mSimulationButton);
    topButtonsLayout->addStretch();

    mZoomLabel = new QLabel(this);
    mZoomLabel->setText("100%");
    mZoomLabel->setStyleSheet("QWidget{padding: 5px; margin: 0 0 10px 10px; background: #00583D; font-family: \"Calibri Light\"; font-size: 16px; color: #fff;}");

    QMovie *procImage = new QMovie(QString(":/images/loading.gif"));
    mProcessingOverlay = new QLabel();
    mProcessingOverlay->setMovie(procImage);
    mProcessingOverlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    procImage->start();
    mProcessingOverlay->hide();

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(topButtonsLayout, 0, 0);
    topLayout->addWidget(&mGraphicsView, 1, 0);
    topLayout->addWidget(mProcessingOverlay, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    topLayout->addWidget(mZoomLabel, 1, 0, Qt::AlignBottom | Qt::AlignLeft);
    topLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(topLayout);

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
    SetGuiEnabled(true); // Enable all top buttons to ensure known button state

    mEditButton->setEnabled(false);
    mAddWireButton->setEnabled(false);
    mAddAndGateButton->setEnabled(false);
    mAddOrGateButton->setEnabled(false);
    mAddXorGateButton->setEnabled(false);
    mAddNotGateButton->setEnabled(false);
    mAddInputButton->setEnabled(false);
    mAddButtonButton->setEnabled(false);
    mAddClockButton->setEnabled(false);
    mAddOutputButton->setEnabled(false);
    mAddTextLabelButton->setEnabled(false);
    mAddHalfAdderButton->setEnabled(false);
    mAddFullAdderButton->setEnabled(false);
    mAddRsFlipFlopButton->setEnabled(false);
    mAddDFlipFlopButton->setEnabled(false);
    mDeleteButton->setEnabled(false);
    mCopyButton->setEnabled(false);
    mUndoButton->setEnabled(false);
    mRedoButton->setEnabled(false);

    mSimulationButton->setEnabled(true);
    mSimulationButton->setText(tr("Stop"));
}

void View::PrepareGuiForEditing()
{
    SetGuiEnabled(false); // Disable all top buttons to ensure known button state

    mEditButton->setEnabled(true);
    mAddWireButton->setEnabled(true);
    mAddAndGateButton->setEnabled(true);
    mAddOrGateButton->setEnabled(true);
    mAddXorGateButton->setEnabled(true);
    mAddNotGateButton->setEnabled(true);
    mAddInputButton->setEnabled(true);
    mAddButtonButton->setEnabled(true);
    mAddClockButton->setEnabled(true);
    mAddOutputButton->setEnabled(true);
    mAddTextLabelButton->setEnabled(true);
    mAddHalfAdderButton->setEnabled(true);
    mAddFullAdderButton->setEnabled(true);
    mAddRsFlipFlopButton->setEnabled(true);
    mAddDFlipFlopButton->setEnabled(true);
    mDeleteButton->setEnabled(true);
    mCopyButton->setEnabled(true);
    mSimulationButton->setEnabled(true);

    SetUndoRedoButtonsEnableState();

    mSimulationButton->setText(tr("Start"));
}

void View::SetGuiEnabled(bool pEnabled)
{
    for (auto& button : mTopButtonsGroup->buttons())
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
