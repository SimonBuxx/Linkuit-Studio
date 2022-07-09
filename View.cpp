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

void View::CreateGui()
{
    QMovie *procImage = new QMovie(QString(":/images/loading.gif"));
    mProcessingOverlay = new QLabel();
    mProcessingOverlay->setMovie(procImage);
    mProcessingOverlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    procImage->start();
    mProcessingOverlay->hide();

    mMainLayout = new QGridLayout();
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);

    mMainLayout->addWidget(&mGraphicsView, 1, 0);
    mMainLayout->addWidget(mProcessingOverlay, 1, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    setLayout(mMainLayout);

    mGraphicsView.stackUnder(mProcessingOverlay);
}

void View::FadeInProcessingOverlay()
{
    if (!mProcessingOverlay->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        mProcessingOverlay->setGraphicsEffect(effect);
        mProcessingOverlay->show();

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(500);
        anim->setStartValue(0.0f);
        anim->setEndValue(1.0f);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QObject::connect(anim, &QPropertyAnimation::finished, [&]()
        {
            delete mProcessingOverlay->graphicsEffect();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void View::FadeOutProcessingOverlay()
{
    if (mProcessingOverlay->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        mProcessingOverlay->setGraphicsEffect(effect);

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(500);
        anim->setStartValue(1.0f);
        anim->setEndValue(0.0f);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QObject::connect(anim, &QPropertyAnimation::finished, [&]()
        {
            delete mProcessingOverlay->graphicsEffect();
            mProcessingOverlay->hide();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
