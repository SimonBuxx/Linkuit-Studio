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

    mGraphicsView.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
    mGraphicsView.setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mGraphicsView.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mGraphicsView.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    mGraphicsView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView.setFrameStyle(QGraphicsView::NoFrame);
    mGraphicsView.setStyleSheet("QGraphicsView {selection-background-color: #00272b;}");

    CreateGui();
    ConnectGuiSignalsAndSlots();

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

    QObject::connect(mScene, &QGraphicsScene::selectionChanged, &mCoreLogic, &CoreLogic::SelectionChanged);

    QTimer::singleShot(0, &mGraphicsView, [&](){
        mGraphicsView.setFocus();
    });
}

QGraphicsScene* View::Scene(void) const
{
    return mScene;
}

QList<QGraphicsItem*> View::Components(void) const
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
        case ComponentType::BUTTON:
        {
            mAddButtonButton->setChecked(true);
            break;
        }
        case ComponentType::CLOCK:
        {
            mAddClockButton->setChecked(true);
            break;
        }
        case ComponentType::OUTPUT:
        {
            mAddOutputButton->setChecked(true);
            break;
        }
        case ComponentType::TEXT_LABEL:
        {
            mAddTextLabelButton->setChecked(true);
            break;
        }
        case ComponentType::HALF_ADDER:
        {
            mAddHalfAdderButton->setChecked(true);
            break;
        }
        case ComponentType::FULL_ADDER:
        {
            mAddFullAdderButton->setChecked(true);
            break;
        }
        case ComponentType::RS_FLIPFLOP:
        {
            mAddRsFlipFlopButton->setChecked(true);
            break;
        }
        case ComponentType::D_FLIPFLOP:
        {
            mAddDFlipFlopButton->setChecked(true);
            break;
        }
        default:
        {
            mEditButton->setChecked(true);
            break;
        }
    }
}

void View::OnSimulationStart()
{
    PrepareGuiForSimulation();
    mGraphicsView.setDragMode(QGraphicsView::NoDrag);
}

void View::OnSimulationStop()
{
    PrepareGuiForEditing();
    mGraphicsView.setDragMode(QGraphicsView::RubberBandDrag);
}

void View::ShowSpecialTab(gui::MenuTab mTab)
{
    switch(mTab)
    {
        case gui::MenuTab::CLOCK:
        {
            if (!mClockPage->isVisible())
            {
                mRibbonMenu->setTabVisible(mRibbonMenu->indexOf(mClockPage), true);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    mRibbonMenu->tabBar()->setStyleSheet("QTabBar::tab {border-radius: 1px; background: rgb(0, 31, 34); font-family: 'Quicksand Medium'; font-size: 16px; color: rgb(255, 255, 255);" \
    "height: 30px; width: 120px;} QTabBar::tab:hover {background: rgb(0, 45, 50);} QTabBar::tab:selected {background: rgb(0, 45, 50);} QTabBar::tab:last {color: rgb(0, 204, 143);}");
    mRibbonMenu->setStyleSheet("QWidget {background: rgb(0, 45, 50); color: white;} QTabWidget {border: none;} QTabWidget::pane {border: none;}");
}

void View::HideSpecialTab()
{
    mRibbonMenu->setTabVisible(mRibbonMenu->indexOf(mClockPage), false);

    mRibbonMenu->tabBar()->setStyleSheet("QTabBar::tab {border-radius: 1px; background: rgb(0, 31, 34); font-family: 'Quicksand Medium'; font-size: 16px; color: rgb(255, 255, 255);" \
    "height: 30px; width: 120px;} QTabBar::tab:hover {background: rgb(0, 45, 50);} QTabBar::tab:selected {background: rgb(0, 45, 50);}");
    mRibbonMenu->setStyleSheet("QWidget {background: rgb(0, 45, 50); color: white;} QTabWidget {border: none;} QTabWidget::pane {border: none;}");
}

void View::SetupMatrix()
{
    double scale = std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f);

    QTransform matrix;
    matrix.scale(scale, scale);

    mGraphicsView.setTransform(matrix);
    mGraphicsView.setBackgroundBrush(DrawGridPattern(mZoomLevel));
}

void View::ZoomIn(int32_t pAmount)
{
    mZoomLevel += pAmount;
    mZoomLevel = std::min(mZoomLevel, canvas::MAX_ZOOM_LEVEL);
    UpdateZoomLabel(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100);
    SetupMatrix();
}

void View::ZoomOut(int32_t pAmount)
{
    mZoomLevel -= pAmount;
    mZoomLevel = std::max(mZoomLevel, canvas::MIN_ZOOM_LEVEL);
    UpdateZoomLabel(std::pow(2, (mZoomLevel - canvas::DEFAULT_ZOOM_LEVEL) / 50.0f) * 100);
    SetupMatrix();
}

void View::UpdateZoomLabel(uint8_t pZoomPercentage)
{
    Q_ASSERT(mZoomLabel);
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
