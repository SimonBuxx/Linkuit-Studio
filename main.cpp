#include "MainWindow.h"

#include <QApplication>
#include <QGraphicsScene>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QGraphicsScene scene;
#warning add constants for scene size
    scene.setSceneRect(QRectF(-16000, -16000, 32000, 32000));
    MainWindow window(scene);
    window.show();

    return app.exec();
}
