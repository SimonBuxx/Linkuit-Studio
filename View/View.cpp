#include "View.h"
#include "HelperFunctions.h"

#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"

#include <QtWidgets>
#include <QtMath>

GraphicsView::GraphicsView(View &pView, CoreLogic &pCoreLogic):
    QGraphicsView(),
    mView(pView),
    mCoreLogic(pCoreLogic),
    mIsLeftMousePressed(false),
    mIsAddingWire(false),
    mIsDragging(false)
{}

View::View(CoreLogic &pCoreLogic):
    mGraphicsView(*this, pCoreLogic),
    mCoreLogic(pCoreLogic)
{}

void View::Init()
{
    setFrameStyle(QFrame::Plain | QFrame::NoFrame);

    //mGraphicsView.setCacheMode(QGraphicsView::CacheBackground);
    mGraphicsView.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
    mGraphicsView.setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mGraphicsView.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mGraphicsView.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    mGraphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setFrameStyle(QGraphicsView::NoFrame);

    CreateGui();

    QObject::connect(&mGraphicsView, &GraphicsView::LeftMouseButtonPressedWithoutCtrlEvent, &mCoreLogic, &CoreLogic::OnLeftMouseButtonPressedWithoutCtrl);
    QObject::connect(&mCoreLogic, &CoreLogic::MousePressedEventDefaultSignal, &mGraphicsView, &GraphicsView::OnMousePressedEventDefault);

    QObject::connect(&mCoreLogic, &CoreLogic::SimulationStartSignal, this, &View::OnSimulationStart);
    QObject::connect(&mCoreLogic, &CoreLogic::SimulationStopSignal, this, &View::OnSimulationStop);

    SetupMatrix();
}

void View::SetScene(QGraphicsScene &pScene)
{
    mScene = &pScene;
    mGraphicsView.setScene(&pScene);
    mGraphicsView.centerOn(0, 0);

    QTimer::singleShot(0, &mGraphicsView, [&]()
    {
        mGraphicsView.setFocus();
    });
}

QGraphicsScene* View::Scene(void) const
{
    Q_ASSERT(mScene);
    return mScene;
}

QList<QGraphicsItem*> View::Components(void) const
{
    Q_ASSERT(mScene);
    return mScene->items();
}

void View::OnSimulationStart()
{
    mGraphicsView.setDragMode(QGraphicsView::NoDrag);
}

void View::OnSimulationStop()
{
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
}

void View::SetupMatrix()
{
    double scale = std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f);

    QTransform matrix;
    matrix.scale(scale, scale);

    mGraphicsView.setTransform(matrix);
    mGraphicsView.setBackgroundBrush(DrawGridPattern(mZoomLevel));

    emit ZoomLevelChangedSignal(scale * 100, mZoomLevel);
}

void View::SetZoom(int32_t pZoomLevel)
{
    Q_ASSERT(pZoomLevel >= canvas::MIN_ZOOM_LEVEL);
    Q_ASSERT(pZoomLevel <= canvas::MAX_ZOOM_LEVEL);

    if (!mCoreLogic.IsProcessing())
    {
        mZoomLevel = pZoomLevel;
        SetupMatrix();

        emit ZoomLevelChangedSignal(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100, mZoomLevel);
    }
}

void View::ZoomIn(int32_t pAmount)
{
    mZoomLevel += pAmount;
    mZoomLevel = std::min(mZoomLevel, canvas::MAX_ZOOM_LEVEL);
    SetupMatrix();

    emit ZoomLevelChangedSignal(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100, mZoomLevel);
}

void View::ZoomOut(int32_t pAmount)
{
    mZoomLevel -= pAmount;
    mZoomLevel = std::max(mZoomLevel, canvas::MIN_ZOOM_LEVEL);
    SetupMatrix();

    emit ZoomLevelChangedSignal(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100, mZoomLevel);
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

    if (pZoomLevel < canvas::MIN_GRID_ZOOM_LEVEL + 20)
    {
        QColor color(canvas::GRID_COLOR);
        color.setAlpha(((pZoomLevel - canvas::MIN_GRID_ZOOM_LEVEL) * 255) / 20.0f);

        painter.setPen(QPen(color, 2 / std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f)));
    }
    else
    {
        painter.setPen(QPen(canvas::GRID_COLOR, 2 / std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f)));
    }
    painter.drawLine(0, 0, canvas::GRID_SIZE - 1, 0);
    painter.drawLine(0, 0, 0, canvas::GRID_SIZE - 1);

    return pixmap;
}
