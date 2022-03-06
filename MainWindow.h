#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framelesswindow.h"
#include "CoreLogic.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

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

    QWidget * mCentralWidget;
    QWidget *mTitleBarWidget;
    QLabel *mTitleBarLabel;

    QPushButton *mCloseButton;
    QPushButton *mMinimizeButton;

    QImage *mLogoImage;
};

#endif // MAINWINDOW_H
