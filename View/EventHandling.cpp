#include "View.h"
#include "HelperFunctions.h"
#include "Components/TextLabel.h"

#include <QtWidgets>

void GraphicsView::wheelEvent(QWheelEvent *pEvent)
{
    // Disable zooming while moving a selection
    if (!mView.Scene()->selectedItems().empty() && mIsLeftMousePressed)
    {
        return;
    }

    Q_ASSERT(pEvent);

    if (pEvent->modifiers() & Qt::ControlModifier && !mCoreLogic.IsProcessing())
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
    // Disable canvas interaction while processing
    if (mCoreLogic.IsProcessing())
    {
        return;
    }

    mView.HideSpecialTab();

    Q_ASSERT(pEvent);

    if (pEvent->button() == Qt::LeftButton) // RMB ignored
    {
        mIsLeftMousePressed = true;
        if (pEvent->modifiers() & Qt::ControlModifier)
        {
            // Start panning
            mPanStart = pEvent->pos();
            pEvent->accept();
        }
        else
        {
            mIsDragging = true;
            if (mCoreLogic.GetControlMode() == ControlMode::WIRE)
            {
                mIsAddingWire = true;
            }
            emit LeftMouseButtonPressedWithoutCtrlEvent(mapToScene(pEvent->pos()), *pEvent);
        }
    }
}

void GraphicsView::OnMousePressedEventDefault(QMouseEvent &pEvent)
{
    QGraphicsView::mousePressEvent(&pEvent);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *pEvent)
{
    Q_ASSERT(pEvent);

    mIsDragging = false;
    if (mCoreLogic.IsProcessing())
    {
        return;
    }

    if (pEvent->button() == Qt::LeftButton)
    {
        mIsLeftMousePressed = false;

        if (!mCoreLogic.IsSimulationRunning())
        {
            // Snap all potentially moved components to grid
            for (auto& comp : scene()->selectedItems())
            {
                comp->setPos(SnapToGrid(comp->pos()));
            }
        }

        // Add the new wires at the current position
        if (mIsAddingWire)
        {
            mCoreLogic.AddWires(mapToScene(pEvent->pos()));
            mIsAddingWire = false;
            return;
        }

        if (pEvent->modifiers() & Qt::ControlModifier)
        {
            pEvent->accept();
            return;
        }
    }

    QGraphicsView::mouseReleaseEvent(pEvent);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Disable canvas interaction while processing
    if (mCoreLogic.IsProcessing())
    {
        return;
    }

    Q_ASSERT(pEvent);

    if (mIsLeftMousePressed)
    {
        if (mIsAddingWire)
        {
            mCoreLogic.ShowPreviewWires(mapToScene(pEvent->pos()));
        }
        else if (pEvent->modifiers() & Qt::ControlModifier && !mIsDragging)
        {
            // Pan the scene
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (pEvent->position().x() - mPanStart.x()));
            verticalScrollBar()->setValue(verticalScrollBar()->value() - (pEvent->position().y() - mPanStart.y()));

            mPanStart = pEvent->pos();

            pEvent->accept();
            return;
        }
        else if (mCoreLogic.GetControlMode() == ControlMode::ADD && mView.Scene()->selectedItems().size() != 1)
        {
            /* Only allow dragging of exactly one component (selectedItems().size() == 1),
               not selecting (selectedItems().size() == 0) or dragging multiple components */
            return;
        }
    }

    QGraphicsView::mouseMoveEvent(pEvent);
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Prevent interaction while not in edit mode
    Q_UNUSED(pEvent);
    return;
}

#warning implement remaining keyboard shortcuts via QShortcut
void GraphicsView::keyPressEvent(QKeyEvent *pEvent)
{
    Q_ASSERT(pEvent);
    if (mCoreLogic.IsProcessing() || mIsAddingWire)
    {
        return;
    }

    switch (pEvent->key())
    {
        case Qt::Key_Escape:
        {
            // Entering EDIT mode is also enabled during label editing
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
            return;
        }
        case Qt::Key_Delete:
        {
            if (!mCoreLogic.IsSimulationRunning())
            {
                mCoreLogic.DeleteSelectedComponents();
            }
            return;
        }
        default:
        {
            break;
        }
    }

    QList<QGraphicsItem*>&& selected = mView.Scene()->selectedItems();

    if (selected.size() != 1 || dynamic_cast<TextLabel*>(selected[0]) == nullptr)
    {
        // Key actions that are disabled during label editing

        switch (pEvent->key())
        {
            case Qt::Key_Return:
            {
                if (mCoreLogic.IsSimulationRunning())
                {
                    mCoreLogic.EnterControlMode(ControlMode::EDIT);
                }
                else
                {
                    mCoreLogic.EnterControlMode(ControlMode::SIMULATION);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    QGraphicsView::keyPressEvent(pEvent);
}
