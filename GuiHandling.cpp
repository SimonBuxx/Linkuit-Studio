#include "View.h"

#include <QtWidgets>

void View::CreateGui()
{
    QHBoxLayout *topButtonsLayout = new QHBoxLayout;

    mEditButton = new QToolButton;
    mEditButton->setText(tr("Edit"));
    mEditButton->setCheckable(true);
    mEditButton->setChecked(true);

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

    mUndoButton = new QToolButton;
    mUndoButton->setText(tr("Undo"));
    mUndoButton->setCheckable(false);
    mUndoButton->setChecked(false);

    mRedoButton = new QToolButton;
    mRedoButton->setText(tr("Redo"));
    mRedoButton->setCheckable(false);
    mRedoButton->setChecked(false);

    QButtonGroup *topButtonsGroup = new QButtonGroup(this);
    topButtonsGroup->setExclusive(true);
    topButtonsGroup->addButton(mEditButton);
    topButtonsGroup->addButton(mAddAndGateButton);
    topButtonsGroup->addButton(mAddOrGateButton);
    topButtonsGroup->addButton(mAddXorGateButton);
    topButtonsGroup->addButton(mAddXorGateButton);
    topButtonsGroup->addButton(mUndoButton);
    topButtonsGroup->addButton(mRedoButton);

    topButtonsLayout->addStretch();
    topButtonsLayout->addWidget(mEditButton);
    topButtonsLayout->addWidget(mAddAndGateButton);
    topButtonsLayout->addWidget(mAddOrGateButton);
    topButtonsLayout->addWidget(mAddXorGateButton);
    topButtonsLayout->addWidget(mUndoButton);
    topButtonsLayout->addWidget(mRedoButton);
    topButtonsLayout->addStretch();

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(topButtonsLayout, 0, 0);
    topLayout->addWidget(&mGraphicsView, 1, 0);
    setLayout(topLayout);

}

void View::ConnectGuiSignalsAndSlots()
{
    QObject::connect(mEditButton, &QAbstractButton::clicked, this, [&](){
        mCoreLogic.EnterControlMode(ControlMode::EDIT);
    });
    QObject::connect(mAddAndGateButton, &QAbstractButton::clicked, this, [&](){
        mCoreLogic.EnterControlMode(ControlMode::ADD_AND_GATE);
    });
    QObject::connect(mAddOrGateButton, &QAbstractButton::clicked, this, [&](){
        mCoreLogic.EnterControlMode(ControlMode::ADD_OR_GATE);
    });
    QObject::connect(mAddXorGateButton, &QAbstractButton::clicked, this, [&](){
        mCoreLogic.EnterControlMode(ControlMode::ADD_XOR_GATE);
    });
    QObject::connect(mUndoButton, &QAbstractButton::clicked, this, [&](){
        qDebug() << "Undo";
    });
    QObject::connect(mRedoButton, &QAbstractButton::clicked, this, [&](){
        qDebug() << "Redo";
    });
}
