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
    mCoreLogic(pCoreLogic)
{}

View* GraphicsView::GetView() const
{
    return &mView;
}

View::View(CoreLogic &pCoreLogic):
    mGraphicsView(*this, pCoreLogic),
    mCoreLogic(pCoreLogic)
{}

void View::Init()
{
    setFrameStyle(QFrame::Plain | QFrame::NoFrame);

    mGraphicsView.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
    mGraphicsView.setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mGraphicsView.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mGraphicsView.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    mGraphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setStyleSheet("QGraphicsView {selection-background-color: #00272b;}");

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

QList<QGraphicsItem*> View::Components(void)
{
    return mScene->items();
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
        default:
        {
            break;
        }
    }
    mScene->clearSelection();
}

void View::OnComponentTypeChanged(ComponentType pNewType)
{
    switch (pNewType)
    {
        case ComponentType::AND_GATE:
        {
            mAddAndGateButton->setChecked(true);
            break;
        }
        case ComponentType::OR_GATE:
        {
            mAddOrGateButton->setChecked(true);
            break;
        }
        case ComponentType::XOR_GATE:
        {
            mAddXorGateButton->setChecked(true);
            break;
        }
        case ComponentType::NOT_GATE:
        {
            mAddNotGateButton->setChecked(true);
            break;
        }
        case ComponentType::INPUT:
        {
            mAddInputButton->setChecked(true);
            break;
        }
        case ComponentType::OUTPUT:
        {
            mAddOutputButton->setChecked(true);
            break;
        }
        default:
        {
            mEditButton->setChecked(true);
            break;
        }
    }
}

void View::OnComponentPositionChanged(QPointF pOffset)
{
    mCoreLogic.OnSelectedComponentsMoved(pOffset);
}

void View::SetupMatrix()
{
    double scale = std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f);

    QTransform matrix;
    matrix.scale(scale, scale);

    mGraphicsView.setTransform(matrix);
    mGraphicsView.setBackgroundBrush(DrawGridPattern(mZoomLevel));
}

void View::ZoomIn(int32_t pLevel)
{
    mZoomLevel += pLevel;
    mZoomLevel = std::min(mZoomLevel, canvas::MAX_ZOOM_LEVEL);
    UpdateZoomLabel(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100);
    SetupMatrix();
}

void View::ZoomOut(int32_t pLevel)
{
    mZoomLevel -= pLevel;
    mZoomLevel = std::max(mZoomLevel, canvas::MIN_ZOOM_LEVEL);
    UpdateZoomLabel(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100);
    SetupMatrix();
}

void View::UpdateZoomLabel(uint8_t pZoomPercentage)
{
    mZoomLabel->setText(QString::fromStdString(std::to_string(pZoomPercentage) + "%"));
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
