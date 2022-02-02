#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CoreLogic.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QGraphicsScene &pScene);

protected:
    void PopulateScene(void);

    QGraphicsScene &mScene;
    View mView;
    CoreLogic mCoreLogic;
};

#endif // MAINWINDOW_H
