#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"

#include "MainWindow.h"
#include "View/View.h"
#include "Configuration.h"

MainWindow::MainWindow(QGraphicsScene &pScene):
    mScene(pScene),
    mView(mCoreLogic),
    mCoreLogic(mView)
{
    mView.SetScene(mScene);

    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(0, 10, 0, 0);

    setWindowTitle(tr(gui::WINDOW_TITLE));
    setWindowIcon(QIcon(":/images/linkuit_icon5.png"));
    setObjectName("mainWindow");

    mMainLayout->addWidget(&mView);
    setLayout(mMainLayout);

    InitializeGlobalShortcuts();
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
