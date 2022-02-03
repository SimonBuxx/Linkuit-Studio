#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"

#include "MainWindow.h"
#include "View/View.h"
#include "Configuration.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QGraphicsScene &pScene):
    mScene(pScene),
    mView(mCoreLogic),
    mCoreLogic(mView)
{
    mView.Scene(mScene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(&mView);
    setLayout(layout);

    setWindowTitle(tr(gui::WINDOW_TITLE));
    setWindowIcon(QIcon(":/images/icon.png"));
    showMaximized();
}
