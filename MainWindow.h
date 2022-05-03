#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CoreLogic.h"

#include <QVBoxLayout>
#include <QShortcut>

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
    /// \brief Creates global shortcuts
    void InitializeGlobalShortcuts(void);

    /// \brief Sets the current component input count to the given value if SW is in ControlMode::ADD
    /// \param pCount: The input count value to set to
    void SetComponentInputCountIfInAddMode(uint8_t pCount);

    /// \brief Sets the current component direction to the given direction if SW is in ControlMode::ADD
    /// \param pDirection: The direction to set to
    void SetComponentDirectionIfInAddMode(Direction pDirection);

protected:
    QGraphicsScene &mScene;
    View mView;
    CoreLogic mCoreLogic;

    QVBoxLayout* mMainLayout;

    // Global shortcuts
    QShortcut* mOneGateInputShortcut;
    QShortcut* mTwoGateInputsShortcut;
    QShortcut* mThreeGateInputsShortcut;
    QShortcut* mFourGateInputsShortcut;
    QShortcut* mFiveGateInputsShortcut;
    QShortcut* mSixGateInputsShortcut;
    QShortcut* mSevenGateInputsShortcut;
    QShortcut* mEightGateInputsShortcut;
    QShortcut* mNineGateInputsShortcut;

    QShortcut* mComponentDirectionRightShortcut;
    QShortcut* mComponentDirectionDownShortcut;
    QShortcut* mComponentDirectionLeftShortcut;
    QShortcut* mComponentDirectionUpShortcut;
};

#endif // MAINWINDOW_H
