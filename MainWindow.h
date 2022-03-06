#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CoreLogic.h"

#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QGraphicsScene &pScene);

    View& GetView(void);
    CoreLogic& GetCoreLogic(void);

protected:
    QGraphicsScene &mScene;
    View mView;
    CoreLogic mCoreLogic;

    QVBoxLayout* mMainLayout;
};

#endif // MAINWINDOW_H
