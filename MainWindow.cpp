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
    setWindowIcon(QIcon(":/images/linkuit_icon4.png"));
    setStyleSheet("QWidget {background: rgb(255, 255, 255);}");

    mMainLayout->addWidget(&mView);
    setLayout(mMainLayout);
}

View& MainWindow::GetView()
{
    return mView;
}

CoreLogic& MainWindow::GetCoreLogic()
{
    return mCoreLogic;
}
