#include "View.h"
#include "HelperFunctions.h"

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
        if (mView.GetPieMenu()->isVisible())
        {
            mView.GetPieMenu()->HideIfNotPinned();
        }

        mIsLeftMousePressed = true;
        if (pEvent->modifiers() & Qt::ControlModifier)
        {
            // Start panning
            mPanStart = pEvent->pos();
            pEvent->accept();
        }
        else if (pEvent->modifiers() & Qt::ShiftModifier)
        {
            if (!mCoreLogic.IsSimulationRunning())
            {
                // Add single components to selection via shift key
                auto&& item = scene()->itemAt(mapToScene(pEvent->pos()), QTransform());
                if (nullptr != item) {
                    item->setSelected(!item->isSelected());
                }
            }
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
    else if (pEvent->button() == Qt::RightButton)
    {
        if (!mCoreLogic.IsSimulationRunning())
        {
            mView.ShowPieMenu(mapFromGlobal(QCursor::pos()));
        }
    }

    QGraphicsView::mouseReleaseEvent(pEvent);

    if (pEvent->button() == Qt::LeftButton)
    {
        if (mCoreLogic.GetControlMode() == ControlMode::COPY)
        {
            mCoreLogic.FinishPaste();
            mCoreLogic.EnterControlMode(ControlMode::EDIT);
        }
    }

    //mView.ZoomIn(0); // To remove buggy line left by the selection rectangle
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
        if (mView.GetPieMenu()->isVisible())
        {
            mView.GetPieMenu()->update();
        }

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
    Q_UNUSED(pEvent);
    return; // Prevent interaction while not in edit mode
}

View::View(QtAwesome &pAwesome, CoreLogic &pCoreLogic):
    mGraphicsView(*this, pCoreLogic),
    mCoreLogic(pCoreLogic),
    mAwesome(pAwesome)
{
    mStandardPieMenuIconVariant.insert("color", QColor(0, 0, 0));
    mDisabledPieMenuIconVariant.insert("color", QColor(180, 180, 180));
}

void View::Init()
{
    setFrameStyle(QFrame::Plain | QFrame::NoFrame);

    //mGraphicsView.setCacheMode(QGraphicsView::CacheBackground);
#warning assess if SmoothPixmapTransform is neccessary / impacts performance
    mGraphicsView.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
    mGraphicsView.setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mGraphicsView.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mGraphicsView.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    mGraphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setFrameStyle(QGraphicsView::NoFrame);

#warning evaluate if this fixes issue #31
    // mGraphicsView.setMouseTracking(true);

    mProcessingOverlay = new QWidget(&mGraphicsView);
    mProcessingOverlay->setObjectName("mProcessingOverlay");
    mProcessingOverlay->setStyleSheet("#mProcessingOverlay {background: rgba(0, 0, 0, 70);}");
    mProcessingOverlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mProcessingImage = new QLabel();
    mProcessingImage->setPixmap(QPixmap(":/images/logo_processing.png").scaled(518, 200, Qt::KeepAspectRatio));

    mProcessingLayout = new QGridLayout();
    mProcessingLayout->addWidget(mProcessingImage, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    mProcessingOverlay->setLayout(mProcessingLayout);

    mProcessingOverlay->hide();

    // Initialize pie menu
    mPieMenu = new PieMenu(&mGraphicsView);
    //mPieMenu->hide();

    QObject::connect(mPieMenu, &PieMenu::ButtonClickedSignal, this, &View::OnPieMenuButtonClicked);

    mMainLayout = new QGridLayout();
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);

    mMainLayout->addWidget(&mGraphicsView, 1, 0);
    mMainLayout->addWidget(mProcessingOverlay, 1, 0);

    setLayout(mMainLayout);

    mGraphicsView.stackUnder(mProcessingOverlay);

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

int32_t View::GetZoomLevel() const
{
    return mZoomLevel;
}

void View::UpdatePieMenuIcons()
{
    mPieMenu->SetButtonIcon(0, mCoreLogic.GetControlMode() == ControlMode::WIRE ? QImage(":/images/icons/material_symbols/arrow_selector_tool_FILL1_wght400_GRAD0_opsz24.svg") :
                                                                                  QImage(":/images/icons/material_symbols/edit_FILL1_wght400_GRAD0_opsz24.svg"));
    mPieMenu->SetButtonIcon(1, QImage(":/images/icons/material_symbols/redo_FILL0_wght400_GRAD0_opsz24.svg"));
    mPieMenu->SetButtonIcon(2, QImage(":/images/icons/material_symbols/delete_FILL0_wght400_GRAD0_opsz24.svg"));
    mPieMenu->SetButtonIcon(3, QImage(":/images/icons/material_symbols/undo_FILL0_wght400_GRAD0_opsz24.svg"));
    mPieMenu->SetCloseButtonIcon(QIcon(":/images/icons/material_symbols/close_FILL1_wght400_GRAD0_opsz24.svg"));
    mPieMenu->SetPinButtonIcon(QIcon(":/images/icons/pushpin-icon.png"));
}

void View::OnPieMenuButtonClicked(int8_t pButtonIndex)
{
    switch (pButtonIndex)
    {
        case 0:
        {
            if (mCoreLogic.GetControlMode() == ControlMode::WIRE)
            {
                mCoreLogic.EnterControlMode(ControlMode::EDIT);
            }
            else
            {
                mCoreLogic.EnterControlMode(ControlMode::WIRE);
            }
            break;
        }
        case 1:
        {
            emit RedoFromPieMenuSignal();
            break;
        }
        case 2:
        {
            if (mCoreLogic.GetControlMode() != ControlMode::COPY)
            {
                mCoreLogic.DeleteSelectedComponents();
            }
            else
            {
                mCoreLogic.AbortPastingIfInCopy();
            }
            break;
        }
        case 3:
        {
            emit UndoFromPieMenuSignal();
            break;
        }
        default:
        {
            break;
        }
    }
}

void View::ShowPieMenu(const QPoint &pPos)
{
    auto geometry = mPieMenu->geometry();
    geometry.setTopLeft(pPos - QPoint(81, 81));

    mPieMenu->setGeometry(geometry);

    UpdatePieMenuIcons();

    FadeInWidget(*mPieMenu, gui::PIE_MENU_ANIMATION_DURATION);
    mPieMenu->setFocus();
}

PieMenu* View::GetPieMenu()
{
    return mPieMenu;
}

void View::ResetViewport()
{
    SetZoom(canvas::DEFAULT_ZOOM_LEVEL);

    mGraphicsView.horizontalScrollBar()->setValue((mGraphicsView.horizontalScrollBar()->maximum() + mGraphicsView.horizontalScrollBar()->minimum()) / 2);
    mGraphicsView.verticalScrollBar()->setValue((mGraphicsView.verticalScrollBar()->maximum() + mGraphicsView.verticalScrollBar()->minimum()) / 2);
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

void View::FadeInProcessingOverlay()
{
    bool stoppedCurrentAnimation = false;
    if (mProcessingOverlay->graphicsEffect() != nullptr)
    {
        delete mProcessingOverlay->graphicsEffect();
        stoppedCurrentAnimation = true;
    }

    if (stoppedCurrentAnimation || !mProcessingOverlay->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();

        mProcessingOverlay->setGraphicsEffect(effect);
        mProcessingOverlay->show();

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(200);
        anim->setStartValue(0.0f);
        anim->setEndValue(1.0f);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QObject::connect(anim, &QPropertyAnimation::finished, this, [&]()
        {
            delete mProcessingOverlay->graphicsEffect();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void View::FadeOutProcessingOverlay()
{
    bool stoppedCurrentAnimation = false;
    if (mProcessingOverlay->graphicsEffect() != nullptr)
    {
        delete mProcessingOverlay->graphicsEffect();
        stoppedCurrentAnimation = true;
    }

    if (stoppedCurrentAnimation || mProcessingOverlay->isVisible())
    {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        mProcessingOverlay->setGraphicsEffect(effect);

        QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(200);
        anim->setStartValue(1.0f);
        anim->setEndValue(0.0f);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QObject::connect(anim, &QPropertyAnimation::finished, this, [&]()
        {
            delete mProcessingOverlay->graphicsEffect();
            mProcessingOverlay->hide();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
