#ifndef VIEW_H
#define VIEW_H

#include "CoreLogic.h"
#include "Configuration.h"

#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QLabel;
class QToolButton;
QT_END_NAMESPACE

class View;
class CoreLogic;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View &pView, CoreLogic &pCoreLogic);

protected:
    void wheelEvent(QWheelEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void mouseReleaseEvent(QMouseEvent *pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent *pEvent) override;
    void keyPressEvent(QKeyEvent *pEvent) override;

    void AddSelectedComponent(QPointF pPosition);

protected:
    View &mView;
    CoreLogic &mCoreLogic;

    bool mIsLeftMousePressed = false;
    QPoint mPanStart;
};

class View : public QFrame
{
    Q_OBJECT
public:
    View(CoreLogic &pCoreLogic);

    void Init(void);

    void Scene(QGraphicsScene &pScene);
    QGraphicsScene* Scene(void);

    /// \brief Returns a list of pointers to all scene items
    /// \return A QList of all items in mScene
    QList<QGraphicsItem*> Components(void);

public slots:
    void ZoomIn(int32_t pLevel);
    void ZoomOut(int32_t pLevel);

    /// \brief Performs all GUI adjustments to enter the new control mode
    /// \param pNewMode: The newly entered control mode
    void OnControlModeChanged(ControlMode pNewMode);

    /// \brief Checks the button associated with pNewType
    /// \param pNewType: The type of components to be added
    void OnComponentTypeChanged(ComponentType pNewType);

protected slots:
    void SetupMatrix(void);

protected:
    /// \brief Creates all GUI components of the main window
    void CreateGui(void);

    /// \brief Invokes connectors for all GUI components
    void ConnectGuiSignalsAndSlots(void);

    /// \brief Creates a grid pattern for the canvas background
    /// \param pZoomLevel: The zoom level decides whether to draw the grid or not
    /// \return A pixel map containing a background grid tile
    QPixmap DrawGridPattern(int32_t pZoomLevel);

protected:
    GraphicsView mGraphicsView;
    QGraphicsScene *mScene;
    CoreLogic &mCoreLogic;

    QToolButton *mEditButton;
    QToolButton *mDeleteButton;
    QToolButton *mCopyButton;
    QToolButton *mAddAndGateButton;
    QToolButton *mAddOrGateButton;
    QToolButton *mAddXorGateButton;
    QToolButton *mUndoButton;
    QToolButton *mRedoButton;

    int32_t mZoomLevel = canvas::DEFAULT_ZOOM_LEVEL;
};

#endif // VIEW_H
