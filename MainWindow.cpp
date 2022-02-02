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
    PopulateScene();

    mView.Scene(mScene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(&mView);
    setLayout(layout);

    setWindowTitle(tr(gui::WINDOW_TITLE));
    setWindowIcon(QIcon(":/images/icon.png"));
    showMaximized();
}

#warning temporary method to init scene
void MainWindow::PopulateScene()
{
    QGraphicsItem *item = new AndGate(10, Direction::RIGHT);
    item->setPos(0, 0);
    mScene.addItem(item);

    QGraphicsItem *item2 = new OrGate(2, Direction::UP);
    item2->setPos(100, 100);
    mScene.addItem(item2);

    QGraphicsItem *item3 = new XorGate(4, Direction::LEFT);
    item3->setPos(200, 200);
    mScene.addItem(item3);
}
