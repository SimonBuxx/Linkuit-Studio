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
    if (pEvent->button() == Qt::LeftButton)
    {
        mIsLeftMousePressed = true;
        if (pEvent->modifiers() & Qt::ControlModifier)
        {
            // Start panning
            mPanStart = pEvent->pos();
            setCursor(Qt::ClosedHandCursor);
            pEvent->accept();
            return;
        }
    }

    // Add component at the current position
    if (pEvent->button() == Qt::LeftButton && mCoreLogic.GetControlMode() == ControlMode::ADD)
    {
        mCoreLogic.AddCurrentTypeComponent(mapToScene(pEvent->pos()));
        return;
    }

    // Start the preview wire at the current position
    if (pEvent->button() == Qt::LeftButton && mCoreLogic.GetControlMode() == ControlMode::WIRE)
    {
        mCoreLogic.SetPreviewWireStart(mapToScene(pEvent->pos()));
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

        // Add the new wires at the current position
        if (mCoreLogic.GetControlMode() == ControlMode::WIRE)
        {
            mCoreLogic.AddWires(mapToScene(pEvent->pos()));
            return;
        }

        // Snap all potentially moved components to grid
        for (auto& comp : scene()->selectedItems())
        {
            comp->setPos(SnapToGrid(comp->pos()));
        }
    }

    QGraphicsView::mouseReleaseEvent(pEvent);
    mCoreLogic.ResetSelectionCopied();
}

void GraphicsView::mouseMoveEvent(QMouseEvent *pEvent)
{
    if (mIsLeftMousePressed)
    {
        mCoreLogic.ShowPreviewWires(mapToScene(pEvent->pos()));
    }

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
#warning Check that hotkey actions are permitted (simulation is not running etc.)
    switch (pEvent->key())
    {
        case Qt::Key_Escape:
        {
            // Enter edit mode
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
            break;
        }
        case Qt::Key_Delete:
        {
            mCoreLogic.DeleteSelectedComponents();
            break;
        }
        default:
        {
            break;
        }
    }
}
