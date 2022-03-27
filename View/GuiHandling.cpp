#include "View.h"
#include "HelperFunctions.h"
#include "Components/IBaseComponent.h"

#include <QtWidgets>

void View::CreateGui()
{
    mRibbonMenu = new QTabWidget;

#warning disable all tabs while loading or all individual elements (plus styling)

    // Standard tabs
    mComponentsPage = new QWidget;
    mRibbonMenu->addTab(mComponentsPage, "Toolbox");

    mSimulationPage = new QWidget;
    mRibbonMenu->addTab(mSimulationPage, "Simulation");

    // Special tabs
    mClockPage = new QWidget;
    mRibbonMenu->addTab(mClockPage, "Clock");
    mRibbonMenu->setTabVisible(mRibbonMenu->indexOf(mClockPage), false);

    mRibbonMenu->setTabPosition(QTabWidget::TabPosition::North);
    mRibbonMenu->tabBar()->setStyleSheet("QTabBar::tab {border-radius: 1px; background: rgb(0, 31, 34); font-family: 'Quicksand Medium'; font-size: 16px; color: rgb(255, 255, 255);" \
    "height: 30px; width: 120px;} QTabBar::tab:hover {background: rgb(0, 45, 50);} QTabBar::tab:selected {background: rgb(0, 45, 50);} /*QTabBar::tab:last {color: rgb(0, 204, 143);}*/");
    mRibbonMenu->setStyleSheet("QWidget {background: rgb(0, 45, 50); color: white;} QTabWidget {border: none;} QTabWidget::pane {border: none;}");
    mRibbonMenu->setFixedHeight(156);

    mEditButton = new QToolButton;
    mEditButton->setText(tr("Edit"));
    mEditButton->setCheckable(true);
    mEditButton->setChecked(true);
    mEditButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43); font-family: 'Quicksand Medium'; font-size: 16px;} " \
        "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);}");
    mEditButton->setFixedSize(90, 58);

    mDeleteButton = new QToolButton;
    mDeleteButton->setText(tr("Delete"));
    mDeleteButton->setCheckable(false);
    mDeleteButton->setChecked(false);
    mDeleteButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43); font-family: 'Quicksand Medium'; font-size: 16px;} " \
        "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:pressed {border: 1px solid rgb(0, 204, 143);}");
    mDeleteButton->setFixedSize(90, 42);

    mCopyButton = new QToolButton;
    mCopyButton->setText(tr("Copy"));
    mCopyButton->setCheckable(false);
    mCopyButton->setChecked(false);
    mCopyButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43); font-family: 'Quicksand Medium'; font-size: 16px;} " \
        "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:pressed {border: 1px solid rgb(0, 204, 143);}");
    mCopyButton->setFixedSize(90, 42);

    {
        mAddWireButton = new QToolButton;
        mAddWireButton->setText(tr("Wiring tool"));
        mAddWireButton->setCheckable(true);
        mAddWireButton->setChecked(false);
        mAddWireButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/wire.png");
        mAddWireButton->setIcon(QIcon(pixmap));
        mAddWireButton->setIconSize(pixmap.rect().size());
        mAddWireButton->setFixedSize(90, 58);

        mAddWireButton->setToolTip(tr("Wiring tool"));
    }

    {
        mAddAndGateButton = new QToolButton;
        mAddAndGateButton->setText(tr("And Gate"));
        mAddAndGateButton->setCheckable(true);
        mAddAndGateButton->setChecked(false);
        mAddAndGateButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/and_gate.png");
        mAddAndGateButton->setIcon(QIcon(pixmap));
        mAddAndGateButton->setIconSize(pixmap.rect().size());
        mAddAndGateButton->setFixedSize(90, 106);

        mAddAndGateButton->setToolTip(tr("AND gate"));
    }

    {
        mAddOrGateButton = new QToolButton;
        mAddOrGateButton->setText(tr("Or Gate"));
        mAddOrGateButton->setCheckable(true);
        mAddOrGateButton->setChecked(false);
        mAddOrGateButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/or_gate.png");
        mAddOrGateButton->setIcon(QIcon(pixmap));
        mAddOrGateButton->setIconSize(pixmap.rect().size());
        mAddOrGateButton->setFixedSize(90, 106);

        mAddOrGateButton->setToolTip(tr("OR gate"));
    }

    {
        mAddXorGateButton = new QToolButton;
        mAddXorGateButton->setText(tr("Xor Gate"));
        mAddXorGateButton->setCheckable(true);
        mAddXorGateButton->setChecked(false);
        mAddXorGateButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/xor_gate.png");
        mAddXorGateButton->setIcon(QIcon(pixmap));
        mAddXorGateButton->setIconSize(pixmap.rect().size());
        mAddXorGateButton->setFixedSize(90, 106);

        mAddXorGateButton->setToolTip(tr("XOR gate"));
    }

    {
        mAddNotGateButton = new QToolButton;
        mAddNotGateButton->setText(tr("Not Gate"));
        mAddNotGateButton->setCheckable(true);
        mAddNotGateButton->setChecked(false);
        mAddNotGateButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/not_gate.png");
        mAddNotGateButton->setIcon(QIcon(pixmap));
        mAddNotGateButton->setIconSize(pixmap.rect().size());
        mAddNotGateButton->setFixedSize(90, 58);

        mAddNotGateButton->setToolTip(tr("NOT gate"));
    }

    {
        mAddInputButton = new QToolButton;
        mAddInputButton->setText(tr("Input"));
        mAddInputButton->setCheckable(true);
        mAddInputButton->setChecked(false);
        mAddInputButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/input.png");
        mAddInputButton->setIcon(QIcon(pixmap));
        mAddInputButton->setIconSize(pixmap.rect().size());
        mAddInputButton->setFixedSize(42, 42);

        mAddInputButton->setToolTip(tr("Input"));
    }

    {
        mAddButtonButton = new QToolButton;
        mAddButtonButton->setText(tr("Button"));
        mAddButtonButton->setCheckable(true);
        mAddButtonButton->setChecked(false);
        mAddButtonButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/button.png");
        mAddButtonButton->setIcon(QIcon(pixmap));
        mAddButtonButton->setIconSize(pixmap.rect().size());
        mAddButtonButton->setFixedSize(42, 42);

        mAddButtonButton->setToolTip(tr("Button"));
    }

    {
        mAddClockButton = new QToolButton;
        mAddClockButton->setText(tr("Clock"));
        mAddClockButton->setCheckable(true);
        mAddClockButton->setChecked(false);
        mAddClockButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/clock.png");
        mAddClockButton->setIcon(QIcon(pixmap));
        mAddClockButton->setIconSize(pixmap.rect().size());
        mAddClockButton->setFixedSize(90, 58);

        mAddClockButton->setToolTip(tr("Clock"));
    }

    {
        mAddOutputButton = new QToolButton;
        mAddOutputButton->setText(tr("Output"));
        mAddOutputButton->setCheckable(true);
        mAddOutputButton->setChecked(false);
        mAddOutputButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/output.png");
        mAddOutputButton->setIcon(QIcon(pixmap));
        mAddOutputButton->setIconSize(pixmap.rect().size());
        mAddOutputButton->setFixedSize(90, 42);

        mAddOutputButton->setToolTip(tr("Output"));
    }

    {
        mAddTextLabelButton = new QToolButton;
        mAddTextLabelButton->setText(tr("Label"));
        mAddTextLabelButton->setCheckable(true);
        mAddTextLabelButton->setChecked(false);
        mAddTextLabelButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/label.png");
        mAddTextLabelButton->setIcon(QIcon(pixmap));
        mAddTextLabelButton->setIconSize(pixmap.rect().size());
        mAddTextLabelButton->setFixedSize(90, 106);

        mAddTextLabelButton->setToolTip(tr("Text label"));
    }

    {
        mAddHalfAdderButton = new QToolButton;
        mAddHalfAdderButton->setText(tr("Half Adder"));
        mAddHalfAdderButton->setCheckable(true);
        mAddHalfAdderButton->setChecked(false);
        mAddHalfAdderButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/half_adder.png");
        mAddHalfAdderButton->setIcon(QIcon(pixmap));
        mAddHalfAdderButton->setIconSize(pixmap.rect().size());
        mAddHalfAdderButton->setFixedSize(90, 106);

        mAddHalfAdderButton->setToolTip(tr("Half adder"));
    }

    {
        mAddFullAdderButton = new QToolButton;
        mAddFullAdderButton->setText(tr("Full Adder"));
        mAddFullAdderButton->setCheckable(true);
        mAddFullAdderButton->setChecked(false);
        mAddFullAdderButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/full_adder.png");
        mAddFullAdderButton->setIcon(QIcon(pixmap));
        mAddFullAdderButton->setIconSize(pixmap.rect().size());
        mAddFullAdderButton->setFixedSize(90, 106);

        mAddFullAdderButton->setToolTip(tr("Full adder"));
    }

    {
        mAddRsFlipFlopButton = new QToolButton;
        mAddRsFlipFlopButton->setText(tr("RS Flip-Flop"));
        mAddRsFlipFlopButton->setCheckable(true);
        mAddRsFlipFlopButton->setChecked(false);
        mAddRsFlipFlopButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/rs_flipflop.png");
        mAddRsFlipFlopButton->setIcon(QIcon(pixmap));
        mAddRsFlipFlopButton->setIconSize(pixmap.rect().size());
        mAddRsFlipFlopButton->setFixedSize(90, 106);

        mAddRsFlipFlopButton->setToolTip(tr("RS flip-flop"));
    }

    {
        mAddDFlipFlopButton = new QToolButton;
        mAddDFlipFlopButton->setText(tr("D Flip-Flop"));
        mAddDFlipFlopButton->setCheckable(true);
        mAddDFlipFlopButton->setChecked(false);
        mAddDFlipFlopButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43);} " \
            "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:checked {border: 1px solid rgb(0, 204, 143);} " \
            "QToolTip {background-color: rgb(0, 39, 43); color: rgb(255, 255, 255); border: 1px solid rgb(0, 204, 143); border-radius: 1px; font-family: 'Quicksand Medium'; font-size: 16px;}");

        QPixmap pixmap(":/images/icons/d_flipflop.png");
        mAddDFlipFlopButton->setIcon(QIcon(pixmap));
        mAddDFlipFlopButton->setIconSize(pixmap.rect().size());
        mAddDFlipFlopButton->setFixedSize(90, 106);

        mAddDFlipFlopButton->setToolTip(tr("D flip-flop"));
    }

    mUndoButton = new QToolButton;
    mUndoButton->setText(tr("Undo"));
    mUndoButton->setCheckable(false);
    mUndoButton->setChecked(false);
    mUndoButton->setEnabled(false);
    mUndoButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43); font-family: 'Quicksand Medium'; font-size: 16px;} " \
        "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:pressed {border: 1px solid rgb(0, 204, 143);}");
    mUndoButton->setFixedSize(90, 58);

    mRedoButton = new QToolButton;
    mRedoButton->setText(tr("Redo"));
    mRedoButton->setCheckable(false);
    mRedoButton->setChecked(false);
    mRedoButton->setEnabled(false);
    mRedoButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43); font-family: 'Quicksand Medium'; font-size: 16px;} " \
        "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:pressed {border: 1px solid rgb(0, 204, 143);}");
    mRedoButton->setFixedSize(90, 42);

    mSimulationButton = new QToolButton;
    mSimulationButton->setText(tr("Start"));
    mSimulationButton->setCheckable(false);
    mSimulationButton->setChecked(false);
    mSimulationButton->setStyleSheet("QToolButton {border: 1px solid rgb(0, 39, 43); padding: 0; border-radius: 1px; background: rgb(0, 39, 43); font-family: 'Quicksand Medium'; font-size: 16px;} " \
        "QToolButton:hover {border: 1px solid rgb(0, 143, 100);} QToolButton:pressed {border: 1px solid rgb(0, 204, 143);}");
    mSimulationButton->setFixedSize(90, 106);

    mComponentsButtonGroup = new QButtonGroup(this);
    mComponentsButtonGroup->setExclusive(true);
    mComponentsButtonGroup->addButton(mEditButton);
    mComponentsButtonGroup->addButton(mDeleteButton);
    mComponentsButtonGroup->addButton(mCopyButton);
    mComponentsButtonGroup->addButton(mAddWireButton);
    mComponentsButtonGroup->addButton(mAddAndGateButton);
    mComponentsButtonGroup->addButton(mAddOrGateButton);
    mComponentsButtonGroup->addButton(mAddXorGateButton);
    mComponentsButtonGroup->addButton(mAddNotGateButton);
    mComponentsButtonGroup->addButton(mAddInputButton);
    mComponentsButtonGroup->addButton(mAddButtonButton);
    mComponentsButtonGroup->addButton(mAddClockButton);
    mComponentsButtonGroup->addButton(mAddOutputButton);
    mComponentsButtonGroup->addButton(mAddTextLabelButton);
    mComponentsButtonGroup->addButton(mAddHalfAdderButton);
    mComponentsButtonGroup->addButton(mAddFullAdderButton);
    mComponentsButtonGroup->addButton(mAddRsFlipFlopButton);
    mComponentsButtonGroup->addButton(mAddDFlipFlopButton);

#warning remove undo and redo buttons from components tab
    mComponentsButtonGroup->addButton(mUndoButton);
    mComponentsButtonGroup->addButton(mRedoButton);

    auto componentsMenuLayout = new QGridLayout;

    {
        componentsMenuLayout->setAlignment(Qt::AlignLeft);
        componentsMenuLayout->setContentsMargins(10, 10, 0, 0);
        componentsMenuLayout->addWidget(mEditButton, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mDeleteButton, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddWireButton, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mCopyButton, 1, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddAndGateButton, 0, 2, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddOrGateButton, 0, 3, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddXorGateButton, 0, 4, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddNotGateButton, 0, 5, 1, 2, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddInputButton, 1, 5, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddButtonButton, 1, 6, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddClockButton, 0, 7, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddOutputButton, 1, 7, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddTextLabelButton, 0, 8, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddHalfAdderButton, 0, 9, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddFullAdderButton, 0, 10, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddRsFlipFlopButton, 0, 11, 2, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mAddDFlipFlopButton, 0, 12, 2, 1, Qt::AlignLeft | Qt::AlignTop);

#warning remove undo and redo buttons from components tab
        componentsMenuLayout->addWidget(mUndoButton, 0, 13, 1, 1, Qt::AlignLeft | Qt::AlignTop);
        componentsMenuLayout->addWidget(mRedoButton, 1, 13, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    auto simulationMenuLayout = new QGridLayout;

    {
        simulationMenuLayout->setAlignment(Qt::AlignLeft);
        simulationMenuLayout->setContentsMargins(10, 10, 0, 0);

#warning add simulation button outside of the ribbon menu for easy access
        simulationMenuLayout->addWidget(mSimulationButton, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    }

    mZoomLabel = new QLabel(this);
    mZoomLabel->setText("100%");
    mZoomLabel->setStyleSheet("QWidget{padding: 5px; margin: 0 0 10px 10px; background: rgb(0, 39, 43); border: 1px solid rgb(0, 204, 143);\
        font-family: 'Quicksand Medium'; font-size: 16px; color: rgb(255, 255, 255); border-radius: 1px;}");

    QMovie *procImage = new QMovie(QString(":/images/loading.gif"));
    mProcessingOverlay = new QLabel();
    mProcessingOverlay->setMovie(procImage);
    mProcessingOverlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    procImage->start();
    mProcessingOverlay->hide();

    mComponentsPage->setLayout(componentsMenuLayout);
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

    HideSpecialTab();
    mRibbonMenu->setCurrentIndex(mRibbonMenu->indexOf(mSimulationPage));

#warning add styling for disabled tabs
    mRibbonMenu->setTabEnabled(mRibbonMenu->indexOf(mComponentsPage), false);
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

    const auto&& componentsTabIndex = mRibbonMenu->indexOf(mComponentsPage);

    mRibbonMenu->setTabEnabled(componentsTabIndex, true);
}

void View::SetGuiEnabled(bool pEnabled)
{
    for (auto& button : mComponentsButtonGroup->buttons())
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
