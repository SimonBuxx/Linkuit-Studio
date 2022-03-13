#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CoreLogic.h"

#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

///
/// \brief The MainWindow class represents the main window
///
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    /// \brief Constructor for MainWindow
    /// \param pScene: The main scene to display
    MainWindow(QGraphicsScene &pScene);

    /// \brief Getter for the view frame
    /// \return Reference to the View object
    View& GetView(void);

    /// \brief Getter for the core logic
    /// \return Reference to the core logic
    CoreLogic& GetCoreLogic(void);

protected:
    QGraphicsScene &mScene;
    View mView;
    CoreLogic mCoreLogic;

    QVBoxLayout* mMainLayout;
};

#endif // MAINWINDOW_H
