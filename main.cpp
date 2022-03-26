#include "MainWindow.h"

#include <QApplication>
#include <QGraphicsScene>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QFontDatabase::addApplicationFont(":/fonts/Quicksand-Medium.ttf");

    QGraphicsScene scene;
    scene.setSceneRect(canvas::DIMENSIONS);

    MainWindow window(scene);
    window.showMaximized();

    return app.exec();
}
