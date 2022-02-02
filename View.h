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

    void Scene(QGraphicsScene &pScene);
    QGraphicsScene* Scene(void);

public slots:
    void ZoomIn(int32_t pLevel);
    void ZoomOut(int32_t pLevel);
    void OnControlModeChanged(ControlMode pNewMode);

protected slots:
    void SetupMatrix(void);

protected:
    void CreateGui(void);
    void ConnectGuiSignalsAndSlots(void);

    QPixmap DrawGridPattern(int32_t pZoomLevel);

protected:
    GraphicsView mGraphicsView;
    QGraphicsScene *mScene;
    CoreLogic &mCoreLogic;

    QToolButton *mEditButton;
    QToolButton *mAddAndGateButton;
    QToolButton *mAddOrGateButton;
    QToolButton *mAddXorGateButton;
    QToolButton *mUndoButton;
    QToolButton *mRedoButton;

    int32_t mZoomLevel = canvas::DEFAULT_ZOOM_LEVEL;
};

#endif // VIEW_H
