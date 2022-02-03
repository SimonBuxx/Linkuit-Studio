#include "View.h"
#include "HelperFunctions.h"

#include <QtWidgets>

void GraphicsView::wheelEvent(QWheelEvent *pEvent)
{
    if (pEvent->modifiers() & Qt::ControlModifier)
    {
        if (pEvent->angleDelta().y() > 0)
        {
            mView.ZoomIn(canvas::ZOOM_SPEED);
        }
        else
        {
            mView.ZoomOut(canvas::ZOOM_SPEED);
        }
        pEvent->accept();
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::LeftButton && pEvent->modifiers() & Qt::ControlModifier)
    {
        // Start panning
        mIsLeftMousePressed = true;
        mPanStart = pEvent->pos();
        setCursor(Qt::ClosedHandCursor);
        pEvent->accept();
        return;
    }

    // Add component at the current position
    if (pEvent->button() == Qt::LeftButton && mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.AddCurrentTypeComponent(mapToScene(pEvent->pos()));
        return;
    }

    QGraphicsView::mousePressEvent(pEvent);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        mIsLeftMousePressed = false;
        setCursor(Qt::ArrowCursor);

        if (pEvent->modifiers() & Qt::ControlModifier)
        {
            pEvent->accept();
            return;
        }

        for (auto& comp : scene()->selectedItems())
        {
            comp->setPos(SnapToGrid(comp->pos()));
        }
    }

    QGraphicsView::mouseReleaseEvent(pEvent);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *pEvent)
{
    if (mIsLeftMousePressed && pEvent->modifiers() & Qt::ControlModifier)
    {
        // Pan the scene
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (pEvent->position().x() - mPanStart.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (pEvent->position().y() - mPanStart.y()));

        mPanStart = pEvent->pos();

        pEvent->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(pEvent);
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Prevent interaction while not in edit mode
    Q_UNUSED(pEvent);
    return;
}

void GraphicsView::keyPressEvent(QKeyEvent *pEvent)
{
    switch (pEvent->key())
    {
        case Qt::Key_Escape:
        {
            // Enter edit mode
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
            break;
        }
        default:
        {
            break;
        }
    }
}
