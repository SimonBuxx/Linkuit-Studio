#include "View.h"
#include "HelperFunctions.h"
#include "Components/BaseComponent.h"

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

    mAddOutputButton = new QToolButton;
    mAddOutputButton->setText(tr("Output"));
    mAddOutputButton->setCheckable(true);
    mAddOutputButton->setChecked(false);

    mAddTextLabelButton = new QToolButton;
    mAddTextLabelButton->setText(tr("Label"));
    mAddTextLabelButton->setCheckable(true);
    mAddTextLabelButton->setChecked(false);

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

    QButtonGroup *topButtonsGroup = new QButtonGroup(this);
    topButtonsGroup->setExclusive(true);
    topButtonsGroup->addButton(mEditButton);
    topButtonsGroup->addButton(mDeleteButton);
    topButtonsGroup->addButton(mCopyButton);
    topButtonsGroup->addButton(mAddWireButton);
    topButtonsGroup->addButton(mAddAndGateButton);
    topButtonsGroup->addButton(mAddOrGateButton);
    topButtonsGroup->addButton(mAddXorGateButton);
    topButtonsGroup->addButton(mAddNotGateButton);
    topButtonsGroup->addButton(mAddInputButton);
    topButtonsGroup->addButton(mAddOutputButton);
    topButtonsGroup->addButton(mAddTextLabelButton);
    topButtonsGroup->addButton(mUndoButton);
    topButtonsGroup->addButton(mRedoButton);
    topButtonsGroup->addButton(mSimulationButton);

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
    topButtonsLayout->addWidget(mAddOutputButton);
    topButtonsLayout->addWidget(mAddTextLabelButton);
    topButtonsLayout->addWidget(mUndoButton);
    topButtonsLayout->addWidget(mRedoButton);
    topButtonsLayout->addWidget(mSimulationButton);
    topButtonsLayout->addStretch();

    mZoomLabel = new QLabel(this);
    mZoomLabel->setText("100%");
    mZoomLabel->setStyleSheet("QWidget{padding: 5px; margin: 0 0 10px 10px; background: #00583D; font-family: \"Calibri Light\"; font-size: 16px; color: #fff; border-radius: 3px;}");

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(topButtonsLayout, 0, 0);
    topLayout->addWidget(&mGraphicsView, 1, 0);
    topLayout->addWidget(mZoomLabel, 1, 0, Qt::AlignBottom | Qt::AlignLeft);
    topLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(topLayout);

    mGraphicsView.stackUnder(mZoomLabel);
}

void View::PrepareGuiForSimulation()
{
    mEditButton->setEnabled(false);
    mAddWireButton->setEnabled(false);
    mAddAndGateButton->setEnabled(false);
    mAddOrGateButton->setEnabled(false);
    mAddXorGateButton->setEnabled(false);
    mAddNotGateButton->setEnabled(false);
    mAddInputButton->setEnabled(false);
    mAddOutputButton->setEnabled(false);
    mAddTextLabelButton->setEnabled(false);
    mDeleteButton->setEnabled(false);
    mCopyButton->setEnabled(false);
    mUndoButton->setEnabled(false);
    mRedoButton->setEnabled(false);

    mSimulationButton->setText(tr("Stop"));
}

void View::PrepareGuiForEditing()
{
    mEditButton->setEnabled(true);
    mAddWireButton->setEnabled(true);
    mAddAndGateButton->setEnabled(true);
    mAddOrGateButton->setEnabled(true);
    mAddXorGateButton->setEnabled(true);
    mAddNotGateButton->setEnabled(true);
    mAddInputButton->setEnabled(true);
    mAddOutputButton->setEnabled(true);
    mAddTextLabelButton->setEnabled(true);
    mDeleteButton->setEnabled(true);
    mCopyButton->setEnabled(true);

    SetUndoRedoButtonsEnableState();

    mSimulationButton->setText(tr("Start"));
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

    QObject::connect(mAddOutputButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::OUTPUT);
    });

    QObject::connect(mAddTextLabelButton, &QAbstractButton::clicked, [&](){
        mCoreLogic.EnterAddControlMode(ComponentType::TEXT_LABEL);
    });

    QObject::connect(mDeleteButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::DeleteSelectedComponents);
    QObject::connect(mCopyButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::CopySelectedComponents);
    QObject::connect(mUndoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Undo);
    QObject::connect(mRedoButton, &QAbstractButton::clicked, &mCoreLogic, &CoreLogic::Redo);
}
