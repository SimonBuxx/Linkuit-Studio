#include "View.h"
#include "GridBuilder.h"
#include "HelperFunctions.h"

#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"

#include <QtWidgets>
#include <QtMath>

GraphicsView::GraphicsView(View &pView, CoreLogic &pCoreLogic):
    QGraphicsView(),
    mView(pView),
    mCoreLogic(pCoreLogic)
{}

void GraphicsView::AddSelectedComponent(QPointF pPosition)
{
    auto item = mCoreLogic.GetItem();
    Q_ASSERT(item);

    item->setPos(SnapToGrid(pPosition));

    if (mView.Scene()->collidingItems(item).empty())
    {
        mView.Scene()->addItem(item);
    }
}

View::View(CoreLogic &pCoreLogic):
    mGraphicsView(*this, pCoreLogic),
    mCoreLogic(pCoreLogic)
{
    setFrameStyle(Plain | NoFrame);

    mGraphicsView.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
    mGraphicsView.setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mGraphicsView.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mGraphicsView.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    mGraphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    CreateGui();
    ConnectGuiSignalsAndSlots();

    SetupMatrix();
}

void View::Scene(QGraphicsScene &pScene)
{
    mScene = &pScene;
    mGraphicsView.setScene(&pScene);
    mGraphicsView.centerOn(0, 0);
}

QGraphicsScene* View::Scene(void)
{
    return mScene;
}

void View::OnControlModeChanged(ControlMode pNewMode)
{
    switch (pNewMode)
    {
        case ControlMode::EDIT:
        {
            mEditButton->setChecked(true);
            break;
        }
        case ControlMode::ADD_AND_GATE:
        {
            mAddAndGateButton->setChecked(true);
            break;
        }
        case ControlMode::ADD_OR_GATE:
        {
            mAddOrGateButton->setChecked(true);
            break;
        }
        case ControlMode::ADD_XOR_GATE:
        {
            mAddXorGateButton->setChecked(true);
            break;
        }
        default:
        {
            return;
        }
    }
}

void View::SetupMatrix()
{
    double scale = std::pow(2, (mZoomLevel - 250) / 50.0f);

    QTransform matrix;
    matrix.scale(scale, scale);

    mGraphicsView.setTransform(matrix);
    mGraphicsView.setBackgroundBrush(DrawGridPattern(mZoomLevel));
}

void View::ZoomIn(int32_t pLevel)
{
    mZoomLevel += pLevel;
    mZoomLevel = std::min(mZoomLevel, canvas::MAX_ZOOM_LEVEL);
    SetupMatrix();
}

void View::ZoomOut(int32_t pLevel)
{
    mZoomLevel -= pLevel;
    mZoomLevel = std::max(mZoomLevel, canvas::MIN_ZOOM_LEVEL);
    SetupMatrix();
}

QPixmap View::DrawGridPattern(int32_t pZoomLevel)
{
    QPixmap pixmap(canvas::GRID_SIZE, canvas::GRID_SIZE);
    QPainter painter;

    pixmap.fill(canvas::BACKGROUND_COLOR);

    if (pZoomLevel < canvas::MIN_GRID_ZOOM_LEVEL)
    {
        return pixmap;
    }

    painter.begin(&pixmap);

    painter.setPen(QPen(canvas::GRID_COLOR, canvas::GRID_STROKE_WIDTH));
    painter.drawLine(0, 0, canvas::GRID_SIZE - 1, 0);
    painter.drawLine(0, 0, 0, canvas::GRID_SIZE - 1);

    return pixmap;
}
