#include "CoreLogic.h"

#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"
#include "Components/Gates/NotGate.h"
#include "Components/Gates/BufferGate.h"
#include "Components/Inputs/LogicInput.h"
#include "Components/Inputs/LogicButton.h"
#include "Components/Inputs/LogicClock.h"
#include "Components/Outputs/LogicOutput.h"
#include "Components/TextLabel.h"
#include "Components/ComplexLogic/HalfAdder.h"
#include "Components/ComplexLogic/FullAdder.h"
#include "Components/ComplexLogic/RsFlipFlop.h"
#include "Components/ComplexLogic/DFlipFlop.h"
#include "Components/ComplexLogic/TFlipFlop.h"
#include "Components/ComplexLogic/JKFlipFlop.h"
#include "Components/ComplexLogic/Multiplexer.h"
#include "Components/ComplexLogic/Demultiplexer.h"

#include "Undo/UndoAddType.h"
#include "Undo/UndoDeleteType.h"
#include "Undo/UndoMoveType.h"
#include "Undo/UndoConfigureType.h"

#include "HelperFunctions.h"

#include <QElapsedTimer>
#include <QCoreApplication>

CoreLogic::CoreLogic(View &pView):
    mView(pView),
    mHorizontalPreviewWire(this, WireDirection::HORIZONTAL, 0),
    mVerticalPreviewWire(this, WireDirection::VERTICAL, 0),
    mPropagationTimer(this),
    mProcessingTimer(this),
    mFilePath(std::nullopt)
{
    mView.Init();

    QObject::connect(&mPropagationTimer, &QTimer::timeout, this, &CoreLogic::OnPropagationTimeout);
    QObject::connect(&mProcessingTimer, &QTimer::timeout, this, &CoreLogic::OnProcessingTimeout);
}

void CoreLogic::SelectAll()
{
    if (mControlMode == ControlMode::COPY || mControlMode == ControlMode::SIMULATION)
    {
        return;
    }

    EnterControlMode(ControlMode::EDIT);

    QPainterPath path;
    path.addRect(mView.Scene()->sceneRect());
    mView.Scene()->setSelectionArea(path);
}

void CoreLogic::EnterControlMode(ControlMode pNewMode)
{
    mView.Scene()->clearFocus();
    emit HideClockConfiguratorSignal();

    if (pNewMode == mControlMode)
    {
        return;
    }

    if (mControlMode == ControlMode::SIMULATION)
    {
        mControlMode = pNewMode;
        LeaveSimulation();
    }

    mControlMode = pNewMode;
    emit ControlModeChangedSignal(pNewMode);

    if (pNewMode == ControlMode::ADD)
    {
        emit ComponentTypeChangedSignal(mComponentType);
    }

    if (pNewMode == ControlMode::SIMULATION)
    {
        EnterSimulation();
        RunSimulation();
    }

    Q_ASSERT(mControlMode == pNewMode);
}

void CoreLogic::SetSimulationMode(SimulationMode pNewMode)
{
    if (mSimulationMode != pNewMode)
    {
        mSimulationMode = pNewMode;
        emit SimulationModeChangedSignal(mSimulationMode);
    }
}

void CoreLogic::EnterSimulation()
{
    StartProcessing();
    ParseWireGroups();
    CreateWireLogicCells();
    ConnectLogicCells();
    //qDebug() << "Found " << mWireGroups.size() << " groups";
    EndProcessing();
    SetSimulationMode(SimulationMode::STOPPED);
    emit SimulationStartSignal();
    StepSimulation();
}

void CoreLogic::RunSimulation()
{
    if (mControlMode == ControlMode::SIMULATION && mSimulationMode == SimulationMode::STOPPED)
    {
        mPropagationTimer.start(simulation::PROPAGATION_DELAY);
        SetSimulationMode(SimulationMode::RUNNING);
    }
}

void CoreLogic::StepSimulation()
{
    if (mControlMode == ControlMode::SIMULATION)
    {
        OnPropagationTimeout();
    }
}

void CoreLogic::ResetSimulation()
{
    if (mControlMode == ControlMode::SIMULATION)
    {
        LeaveSimulation();
        EnterSimulation();
    }
}

void CoreLogic::PauseSimulation()
{
    if (mControlMode == ControlMode::SIMULATION && mSimulationMode == SimulationMode::RUNNING)
    {
        mPropagationTimer.stop();
        SetSimulationMode(SimulationMode::STOPPED);
    }
}

void CoreLogic::LeaveSimulation()
{
    mPropagationTimer.stop();
    SetSimulationMode(SimulationMode::STOPPED);
    emit SimulationStopSignal();
}

void CoreLogic::OnToggleValueChanged(uint32_t pValue)
{
    if (mView.Scene()->selectedItems().size() == 1 && nullptr != dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0]))
    {
        if (nullptr != std::dynamic_pointer_cast<LogicClockCell>(dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0])->GetLogicCell()))
        {
            std::dynamic_pointer_cast<LogicClockCell>(dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0])->GetLogicCell())->SetToggleTicks(pValue);
            CircuitModified();
        }
    }
}

void CoreLogic::OnPulseValueChanged(uint32_t pValue)
{
    if (mView.Scene()->selectedItems().size() == 1 && nullptr != dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0]))
    {
        if (nullptr != std::dynamic_pointer_cast<LogicClockCell>(dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0])->GetLogicCell()))
        {
            std::dynamic_pointer_cast<LogicClockCell>(dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0])->GetLogicCell())->SetPulseTicks(pValue);
            CircuitModified();
        }
    }
}

void CoreLogic::OnClockModeChanged(ClockMode pMode)
{
    if (mView.Scene()->selectedItems().size() == 1 && nullptr != dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0]))
    {
        if (nullptr != std::dynamic_pointer_cast<LogicClockCell>(dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0])->GetLogicCell()))
        {
            std::dynamic_pointer_cast<LogicClockCell>(dynamic_cast<LogicClock*>(mView.Scene()->selectedItems()[0])->GetLogicCell())->SetClockMode(pMode);
            CircuitModified();
        }
    }
}

void CoreLogic::EnterAddControlMode(ComponentType pComponentType)
{
    EnterControlMode(ControlMode::ADD);
    SelectComponentType(pComponentType);
}

ComponentType CoreLogic::GetSelectedComponentType() const
{
    return mComponentType;
}

bool CoreLogic::IsSimulationRunning() const
{
    return (mControlMode == ControlMode::SIMULATION);
}

void CoreLogic::OnPropagationTimeout()
{
    emit SimulationAdvanceSignal();
}

bool CoreLogic::IsUndoQueueEmpty() const
{
    return mUndoQueue.empty();
}

bool CoreLogic::IsRedoQueueEmpty() const
{
    return mRedoQueue.empty();
}

void CoreLogic::SelectComponentType(ComponentType pComponentType)
{
    Q_ASSERT(mControlMode == ControlMode::ADD);
    mComponentType = pComponentType;
    emit ComponentTypeChangedSignal(mComponentType);
}

std::optional<IBaseComponent*> CoreLogic::GetItem() const
{
    IBaseComponent* item = nullptr;

    switch(mComponentType)
    {
        case ComponentType::AND_GATE:
        {
            item = new AndGate(this, mComponentInputCount, mComponentDirection);
            break;
        }
        case ComponentType::OR_GATE:
        {
            item = new OrGate(this, mComponentInputCount, mComponentDirection);
            break;
        }
        case ComponentType::XOR_GATE:
        {
            item = new XorGate(this, mComponentInputCount, mComponentDirection);
            break;
        }
        case ComponentType::NOT_GATE:
        {
            item = new NotGate(this, mComponentDirection);
            break;
        }
        case ComponentType::BUFFER_GATE:
        {
            item = new BufferGate(this, mComponentDirection);
            break;
        }
        case ComponentType::INPUT:
        {
            item = new LogicInput(this);
            break;
        }
        case ComponentType::BUTTON:
        {
            item = new LogicButton(this);
            break;
        }
        case ComponentType::CLOCK:
        {
            item = new LogicClock(this, mComponentDirection);
            break;
        }
        case ComponentType::OUTPUT:
        {
            item = new LogicOutput(this);
            break;
        }
        case ComponentType::TEXT_LABEL:
        {
            item = new TextLabel(this);
            break;
        }
        case ComponentType::HALF_ADDER:
        {
            item = new HalfAdder(this, mComponentDirection);
            break;
        }
        case ComponentType::FULL_ADDER:
        {
            item = new FullAdder(this, mComponentDirection);
            break;
        }
        case ComponentType::RS_FLIPFLOP:
        {
            item = new RsFlipFlop(this, mComponentDirection);
            break;
        }
        case ComponentType::D_FLIPFLOP:
        {
            item = new DFlipFlop(this, mComponentDirection);
            break;
        }
        case ComponentType::T_FLIPFLOP:
        {
            item = new TFlipFlop(this, mComponentDirection);
            break;
        }
        case ComponentType::JK_FLIPFLOP:
        {
            item = new JKFlipFlop(this, mComponentDirection);
            break;
        }
        case ComponentType::MULTIPLEXER:
        {
            item = new Multiplexer(this, mComponentDirection, mMultiplexerBitWidth);
            break;
        }
        case ComponentType::DEMULTIPLEXER:
        {
            item = new Demultiplexer(this, mComponentDirection, mMultiplexerBitWidth);
            break;
        }
        default:
        {
            return std::nullopt;
        }
    }

    Q_ASSERT(item);
    return item;
}

ControlMode CoreLogic::GetControlMode() const
{
    return mControlMode;
}

bool CoreLogic::AddCurrentTypeComponent(QPointF pPosition)
{
    if (mView.Scene()->selectedItems().size() > 0)
    {
        return false;
    }

    auto item = GetItem();
    Q_ASSERT(item.has_value());

    item.value()->setPos(SnapToGrid(pPosition));

    if (!GetCollidingComponents(item.value(), false).empty())
    {
        delete item.value();
        return false;
    }

    mView.Scene()->clearFocus(); // Remove focus from components like labels that can be edited while in ADD mode
    mView.Scene()->addItem(item.value());

    auto addedComponents = std::vector<IBaseComponent*>{static_cast<IBaseComponent*>(item.value())};
    AppendUndo(new UndoAddType(addedComponents));

    return true;
}

void CoreLogic::SetComponentInputCount(uint8_t pCount)
{
    Q_ASSERT(pCount >= components::gates::MIN_INPUT_COUNT && pCount <= components::gates::MAX_INPUT_COUNT);
    mComponentInputCount = pCount;
}

void CoreLogic::SetComponentDirection(Direction pDirection)
{
    mComponentDirection = pDirection;
}

void CoreLogic::SetMultiplexerBitWidth(uint8_t pBitWidth)
{
    Q_ASSERT(pBitWidth >= components::multiplexer::MIN_BIT_WIDTH && pBitWidth <= components::multiplexer::MAX_BIT_WIDTH);
    mMultiplexerBitWidth = pBitWidth;
}

void CoreLogic::SetPreviewWireStart(QPointF pPoint)
{
    mPreviewWireStart = SnapToGrid(pPoint);

    mHorizontalPreviewWire.SetLength(0);
    mVerticalPreviewWire.SetLength(0);

    mView.Scene()->addItem(&mHorizontalPreviewWire);
    mView.Scene()->addItem(&mVerticalPreviewWire);
}

void CoreLogic::ShowPreviewWires(QPointF pCurrentPoint)
{
    QPointF snappedCurrentPoint = SnapToGrid(pCurrentPoint);

    // Set the start direction (which wire is drawn starting at the start position)
    if (mWireStartDirection == WireDirection::UNSET)
    {
        if (snappedCurrentPoint.x() != mPreviewWireStart.x())
        {
            mWireStartDirection = WireDirection::HORIZONTAL;
        }
        else if (snappedCurrentPoint.y() != mPreviewWireStart.y())
        {
            mWireStartDirection = WireDirection::VERTICAL;
        }
    }

    // Trigger a redraw of the area where the wires were before
    mHorizontalPreviewWire.setVisible(false);
    mVerticalPreviewWire.setVisible(false);

    mHorizontalPreviewWire.SetLength(std::abs(mPreviewWireStart.x() - snappedCurrentPoint.x()));
    mVerticalPreviewWire.SetLength(std::abs(mPreviewWireStart.y() - snappedCurrentPoint.y()));

    if (mWireStartDirection == WireDirection::HORIZONTAL)
    {
        mHorizontalPreviewWire.setPos(std::min(mPreviewWireStart.x(), snappedCurrentPoint.x()), mPreviewWireStart.y());
        mVerticalPreviewWire.setPos(snappedCurrentPoint.x(), std::min(mPreviewWireStart.y(), snappedCurrentPoint.y()));
    }
    else
    {
        mVerticalPreviewWire.setPos(mPreviewWireStart.x(), std::min(mPreviewWireStart.y(), snappedCurrentPoint.y()));
        mHorizontalPreviewWire.setPos(std::min(mPreviewWireStart.x(), snappedCurrentPoint.x()), snappedCurrentPoint.y());
    }

    mHorizontalPreviewWire.setVisible(true);
    mVerticalPreviewWire.setVisible(true);
}

void CoreLogic::AddWires(QPointF pEndPoint)
{
    mView.Scene()->removeItem(&mHorizontalPreviewWire);
    mView.Scene()->removeItem(&mVerticalPreviewWire);

    if (mWireStartDirection == WireDirection::UNSET)
    {
        return; // Return if no wire must be drawn anyways
    }

    QPointF snappedEndPoint = SnapToGrid(pEndPoint);
    std::vector<IBaseComponent*> addedComponents;
    std::vector<IBaseComponent*> deletedComponents;

    // Add horizontal wire
    if (mPreviewWireStart.x() != snappedEndPoint.x())
    {
        auto item = new LogicWire(this, WireDirection::HORIZONTAL, std::abs(mPreviewWireStart.x() - snappedEndPoint.x()));

        if (mWireStartDirection == WireDirection::HORIZONTAL)
        {
            item->setPos(std::min(mPreviewWireStart.x(), snappedEndPoint.x()), mPreviewWireStart.y());
        }
        else
        {
            item->setPos(std::min(mPreviewWireStart.x(), snappedEndPoint.x()), snappedEndPoint.y());
        }

        // Delete wires that are completely behind the new wire
        const auto containedWires = DeleteContainedWires(item);
        deletedComponents.insert(deletedComponents.end(), containedWires.begin(), containedWires.end());

        // Find wires left or right of the new wire (those may be partly behind the new wire)
        auto startAdjacent = GetAdjacentWire(QPointF(item->x() - 2, item->y()), WireDirection::HORIZONTAL);
        auto endAdjacent = GetAdjacentWire(QPointF(item->x() + item->GetLength() + 2, item->y()), WireDirection::HORIZONTAL);

        auto horizontalWire = MergeWires(item, startAdjacent, endAdjacent);

        delete item;

        if (startAdjacent == endAdjacent)
        {
            endAdjacent = std::nullopt;
        }

        if (startAdjacent.has_value())
        {
            deletedComponents.push_back(static_cast<IBaseComponent*>(startAdjacent.value()));
            Q_ASSERT(startAdjacent.value());
            mView.Scene()->removeItem(startAdjacent.value());
        }

        if (endAdjacent.has_value())
        {
            deletedComponents.push_back(static_cast<IBaseComponent*>(endAdjacent.value()));
            Q_ASSERT(endAdjacent.value());
            mView.Scene()->removeItem(endAdjacent.value());
        }

        mView.Scene()->addItem(horizontalWire);
        addedComponents.push_back(static_cast<IBaseComponent*>(horizontalWire));
    }

    // Add vertical wire
    if (mPreviewWireStart.y() != snappedEndPoint.y())
    {
        auto item = new LogicWire(this, WireDirection::VERTICAL, std::abs(mPreviewWireStart.y() - snappedEndPoint.y()));

        if (mWireStartDirection == WireDirection::VERTICAL)
        {
            item->setPos(mPreviewWireStart.x(), std::min(mPreviewWireStart.y(), snappedEndPoint.y()));
        }
        else
        {
            item->setPos(snappedEndPoint.x(), std::min(mPreviewWireStart.y(), snappedEndPoint.y()));
        }

        // Delete wires that are completely behind the new wire
        const auto containedWires = DeleteContainedWires(item);
        deletedComponents.insert(deletedComponents.end(), containedWires.begin(), containedWires.end());

        // Find wires above or below of the new wire (those may be partly behind the new wire)
        auto startAdjacent = GetAdjacentWire(QPointF(item->x(), item->y() - 2), WireDirection::VERTICAL);
        auto endAdjacent = GetAdjacentWire(QPointF(item->x(), item->y() + item->GetLength() + 2), WireDirection::VERTICAL);

        auto verticalWire = MergeWires(item, startAdjacent, endAdjacent);

        delete item;

        if (startAdjacent == endAdjacent)
        {
            endAdjacent = std::nullopt;
        }

        if (startAdjacent.has_value())
        {
            deletedComponents.push_back(static_cast<IBaseComponent*>(startAdjacent.value()));
            mView.Scene()->removeItem(startAdjacent.value());
        }

        if (endAdjacent.has_value())
        {
            deletedComponents.push_back(static_cast<IBaseComponent*>(endAdjacent.value()));
            mView.Scene()->removeItem(endAdjacent.value());
        }

        mView.Scene()->addItem(verticalWire);
        addedComponents.push_back(static_cast<IBaseComponent*>(verticalWire));
    }

    std::vector<IBaseComponent*> addedConPoints;

    for (const auto& wire : addedComponents)
    {
        for (const auto& collidingComp : mView.Scene()->collidingItems(wire, Qt::IntersectsItemShape))
        {
            if (dynamic_cast<LogicWire*>(collidingComp) != nullptr && IsTCrossing(static_cast<LogicWire*>(wire), static_cast<LogicWire*>(collidingComp)))
            {
                QPointF conPointPos;
                if (static_cast<LogicWire*>(wire)->GetDirection() == WireDirection::HORIZONTAL)
                {
                    conPointPos.setX(collidingComp->x());
                    conPointPos.setY(wire->y());
                }
                else
                {
                    conPointPos.setX(wire->x());
                    conPointPos.setY(collidingComp->y());
                }

                if (!IsComponentAtPosition<ConPoint>(conPointPos))
                {
                    auto item = new ConPoint(this);
                    item->setPos(conPointPos);
                    addedConPoints.push_back(item);
                    mView.Scene()->addItem(item);
                }
            }
        }
    }

    addedComponents.insert(addedComponents.end(), addedConPoints.begin(), addedConPoints.end());
    AppendUndo(new UndoAddType(addedComponents, deletedComponents));
    mWireStartDirection = WireDirection::UNSET;
}

template<typename T>
bool CoreLogic::IsComponentAtPosition(QPointF pPos)
{
    for (const auto& comp : mView.Scene()->items(pPos, Qt::IntersectsItemShape))
    {
        if (dynamic_cast<T*>(comp) != nullptr)
        {
            return true;
        }
    }
    return false;
}

void CoreLogic::MergeWiresAfterMove(const std::vector<LogicWire*> &pWires, std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents)
{
    for (const auto& w : pWires)
    {
        const auto&& containedWires = DeleteContainedWires(w);
        pDeletedComponents.insert(pDeletedComponents.end(), containedWires.begin(), containedWires.end());

        std::optional<LogicWire*> startAdjacent = std::nullopt;
        std::optional<LogicWire*> endAdjacent = std::nullopt;

        if (w->GetDirection() == WireDirection::HORIZONTAL)
        {
            startAdjacent = GetAdjacentWire(QPointF(w->x() - 4, w->y()), WireDirection::HORIZONTAL);
            endAdjacent = GetAdjacentWire(QPointF(w->x() + w->GetLength() + 4, w->y()), WireDirection::HORIZONTAL);

            auto horizontalWire = MergeWires(w, startAdjacent, endAdjacent);
            horizontalWire->setSelected(w->isSelected());

            if (startAdjacent == endAdjacent)
            {
                endAdjacent = std::nullopt;
            }

            mView.Scene()->addItem(horizontalWire);
            pAddedComponents.push_back(static_cast<LogicWire*>(horizontalWire));
        }
        else
        {
            startAdjacent = GetAdjacentWire(QPointF(w->x(), w->y() - 4), WireDirection::VERTICAL);
            endAdjacent = GetAdjacentWire(QPointF(w->x(), w->y() + w->GetLength() + 4), WireDirection::VERTICAL);

            auto verticalWire = MergeWires(w, startAdjacent, endAdjacent);
            verticalWire->setSelected(w->isSelected());

            if (startAdjacent == endAdjacent)
            {
                endAdjacent = std::nullopt;
            }

            mView.Scene()->addItem(verticalWire);
            pAddedComponents.push_back(static_cast<LogicWire*>(verticalWire));
        }

        Q_ASSERT(w->scene() == mView.Scene());
        pDeletedComponents.push_back(w);
        mView.Scene()->removeItem(w);

        if (startAdjacent.has_value() && std::find(pAddedComponents.begin(), pAddedComponents.end(), startAdjacent) == pAddedComponents.end())
        {
            pDeletedComponents.push_back(static_cast<LogicWire*>(startAdjacent.value()));
            mView.Scene()->removeItem(startAdjacent.value());
        }

        if (endAdjacent.has_value() && std::find(pAddedComponents.begin(), pAddedComponents.end(), endAdjacent) == pAddedComponents.end())
        {
            pDeletedComponents.push_back(static_cast<LogicWire*>(endAdjacent.value()));
            mView.Scene()->removeItem(endAdjacent.value());
        }
    }
}

std::vector<LogicWire*> CoreLogic::DeleteContainedWires(const LogicWire* pWire)
{
    std::vector<LogicWire*> deletedComponents;

    QRectF collisionRect;
    if (pWire->GetDirection() == WireDirection::HORIZONTAL)
    {
        collisionRect = QRectF(pWire->x() - 2, pWire->y() - components::wires::BOUNDING_RECT_SIZE / 2.0f - 2,
                               pWire->GetLength() + 4, components::wires::BOUNDING_RECT_SIZE + 4);
    }
    else
    {
        collisionRect = QRectF(pWire->x() - components::wires::BOUNDING_RECT_SIZE / 2.0f - 2, pWire->y() - 2,
                               components::wires::BOUNDING_RECT_SIZE + 4, pWire->GetLength() + 4);
    }

    const auto&& containedComponents = mView.Scene()->items(collisionRect, Qt::ContainsItemShape, Qt::DescendingOrder);

    for (const auto &wire : containedComponents)
    {
        if (dynamic_cast<LogicWire*>(wire) != nullptr && static_cast<LogicWire*>(wire)->GetDirection() == pWire->GetDirection() && wire != pWire)
        {
            deletedComponents.push_back(static_cast<LogicWire*>(wire));
            mView.Scene()->removeItem(wire);
        }
    }

    return deletedComponents;
}

std::optional<LogicWire*> CoreLogic::GetAdjacentWire(QPointF pCheckPosition, WireDirection pDirection) const
{
    const auto&& componentsAtPosition = mView.Scene()->items(pCheckPosition.x(), pCheckPosition.y(), 1, 1, Qt::IntersectsItemShape, Qt::DescendingOrder);
    const auto&& wiresAtPosition = FilterForWires(componentsAtPosition, pDirection);

    if (wiresAtPosition.size() > 0)
    {
        Q_ASSERT(static_cast<LogicWire*>(wiresAtPosition.at(0)));
        return std::optional(static_cast<LogicWire*>(wiresAtPosition.at(0)));
    }

    return std::nullopt;
}

// Remember that using (dynamic_cast<LogicWire*>(comp) != nullptr) directly is more efficient than iterating over filtered components
std::vector<IBaseComponent*> CoreLogic::FilterForWires(const QList<QGraphicsItem*> &pComponents, WireDirection pDirection) const
{
    std::vector<IBaseComponent*> wires;

    for (auto &comp : pComponents)
    {
        if (dynamic_cast<LogicWire*>(comp) != nullptr
                && (static_cast<LogicWire*>(comp)->GetDirection() == pDirection || pDirection == WireDirection::UNSET))
        {
            wires.push_back(static_cast<IBaseComponent*>(comp));
        }
    }

    return wires;
}

std::vector<IBaseComponent*> CoreLogic::GetCollidingComponents(IBaseComponent* pComponent, bool pOnlyUnselected) const
{
    std::vector<IBaseComponent*> collidingComponents;

    for (auto &comp : mView.Scene()->collidingItems(pComponent))
    {
        Q_ASSERT(comp);

        if (IsCollidingComponent(comp) && (!pOnlyUnselected || !comp->isSelected()))
        {
            // comp must be IBaseComponent at this point
            collidingComponents.push_back(static_cast<IBaseComponent*>(comp));
        }
    }

    return collidingComponents;
}

bool CoreLogic::IsCollidingComponent(QGraphicsItem* pComponent) const
{
    return (dynamic_cast<IBaseComponent*>(pComponent) != nullptr
            && dynamic_cast<LogicWire*>(pComponent) == nullptr
            && dynamic_cast<ConPoint*>(pComponent) == nullptr);
}

bool CoreLogic::IsTCrossing(const LogicWire* pWire1, const LogicWire* pWire2) const
{
    const LogicWire* a = nullptr;
    const LogicWire* b = nullptr;

    if (pWire1->GetDirection() == WireDirection::VERTICAL && pWire2->GetDirection() == WireDirection::HORIZONTAL)
    {
        a = pWire1;
        b = pWire2;
    }
    else if (pWire1->GetDirection() != pWire2->GetDirection()) // pWire1 horizontal, pWire2 vertical
    {
        a = pWire2;
        b = pWire1;
    }
    else
    {
        return false;
    }

    return ((a->y() < b->y() && a->x() == b->x() && a->y() + a->GetLength() > b->y())
        || (a->y() < b->y() && a->x() == b->x() + b->GetLength() && a->y() + a->GetLength() > b->y())
        || (a->x() > b->x() && a->y() + a->GetLength() == b->y() && a->x() < b->x() + b->GetLength())
        || (a->x() > b->x() && a->y() == b->y() && a->x() < b->x() + b->GetLength()));
}

bool CoreLogic::IsNoCrossingPoint(const ConPoint* pConPoint) const
{
    const auto&& components = mView.Scene()->items(pConPoint->pos(), Qt::IntersectsItemBoundingRect);

    if (components.size() <= 2)
    {
        // Including the ConPoint at the position, there can be at max the ConPoint and one wire
        return true;
    }

    bool foundOne = false;
    bool firstGoesTrough = false;
    for (const auto& comp : components)
    {
        if (dynamic_cast<LogicWire*>(comp) != nullptr)
        {
            if (!foundOne)
            {
                foundOne = true; // Found a crossing wire (either ends in pConPoint or doesn't)
                firstGoesTrough = !static_cast<LogicWire*>(comp)->StartsOrEndsIn(pConPoint->pos()); // True, if this wire doesn't end in pConPoint
            }
            else if ((foundOne && firstGoesTrough) || (foundOne && !static_cast<LogicWire*>(comp)->StartsOrEndsIn(pConPoint->pos())))
            {
                // T-Crossing wire found (first or second one) and two wires total, this is no L or I crossing
                return false;
            }
        }
    }
    return true;
}

bool CoreLogic::IsXCrossingPoint(QPointF pPoint) const
{
    const auto& wires = FilterForWires(mView.Scene()->items(pPoint, Qt::IntersectsItemBoundingRect));

    if (wires.size() <= 1)
    {
        return false;
    }

    for (const auto& wire : wires)
    {
        if (static_cast<LogicWire*>(wire)->StartsOrEndsIn(pPoint))
        {
            return false; // L-Crossing type wire found, this is no X crossing
        }
    }
    return true;
}

LogicWire* CoreLogic::MergeWires(LogicWire* pNewWire, std::optional<LogicWire*> pLeftTopAdjacent, std::optional<LogicWire*> pRightBottomAdjacent) const
{
    Q_ASSERT(pNewWire);
    Q_ASSERT(!pLeftTopAdjacent.has_value() || pLeftTopAdjacent.value());
    Q_ASSERT(!pRightBottomAdjacent.has_value() || pRightBottomAdjacent.value());

    QPointF newStart(pNewWire->pos());

    if (pNewWire->GetDirection() == WireDirection::HORIZONTAL)
    {
        QPointF newEnd(pNewWire->x() + pNewWire->GetLength(), pNewWire->y());

        if (pLeftTopAdjacent.has_value() && pLeftTopAdjacent.value()->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->y() == pLeftTopAdjacent.value()->y());
            newStart = QPointF(pLeftTopAdjacent.value()->x(), pNewWire->y());
        }
        if (pRightBottomAdjacent.has_value()  && pRightBottomAdjacent.value()->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->y() == pRightBottomAdjacent.value()->y());
            newEnd = QPoint(pRightBottomAdjacent.value()->x() + pRightBottomAdjacent.value()->GetLength(), pNewWire->y());
        }

        auto newWire = new LogicWire(this, WireDirection::HORIZONTAL, newEnd.x() - newStart.x());
        newWire->setPos(newStart);

        Q_ASSERT(newWire);
        return newWire;
    }
    else
    {
        QPointF newEnd(pNewWire->x(), pNewWire->y() + pNewWire->GetLength());

        if (pLeftTopAdjacent.has_value()  && pLeftTopAdjacent.value()->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->x() == pLeftTopAdjacent.value()->x());
            newStart = QPointF(pNewWire->x(), pLeftTopAdjacent.value()->y());
        }
        if (pRightBottomAdjacent.has_value()  && pRightBottomAdjacent.value()->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->x() == pRightBottomAdjacent.value()->x());
            newEnd = QPoint(pNewWire->x(), pRightBottomAdjacent.value()->y() + pRightBottomAdjacent.value()->GetLength());
        }

        auto newWire = new LogicWire(this, WireDirection::VERTICAL, newEnd.y() - newStart.y());
        newWire->setPos(newStart);
        return newWire;
    }
}

void CoreLogic::ParseWireGroups(void)
{
    mWireGroups.clear();
    mWireMap.clear();

    for (auto& comp : mView.Scene()->items())
    {
        // If the component is a wire that is not yet part of a group
        if (dynamic_cast<LogicWire*>(comp) && mWireMap.find(static_cast<LogicWire*>(comp)) == mWireMap.end())
        {
            mWireGroups.push_back(std::vector<IBaseComponent*>());
            ExploreGroup(static_cast<LogicWire*>(comp), mWireGroups.size() - 1);
        }
        ProcessingHeartbeat();
    }
}

void CoreLogic::ExploreGroup(LogicWire* pWire, int32_t pGroupIndex)
{
    Q_ASSERT(pWire);
    Q_ASSERT(pGroupIndex >= 0);

    mWireMap[pWire] = pGroupIndex;
    mWireGroups[pGroupIndex].push_back(pWire); // Note: pWire must not be part of group pGroupIndex before the call to ExploreGroup

    for (auto& coll : mView.Scene()->collidingItems(pWire, Qt::IntersectsItemShape)) // Item shape is sufficient for wire collision
    {
        if (dynamic_cast<LogicWire*>(coll) != nullptr && mWireMap.find(static_cast<LogicWire*>(coll)) == mWireMap.end())
        {
            auto collisionPoint = GetWireCollisionPoint(pWire, static_cast<LogicWire*>(coll));
            if (collisionPoint.has_value())
            {
                auto conPoint = GetConPointAtPosition(collisionPoint.value(), ConnectionType::FULL);
                if (conPoint.has_value())
                {
                    mWireGroups[pGroupIndex].push_back(conPoint.value());
                }
                if (conPoint.has_value() || IsLCrossing(pWire, static_cast<LogicWire*>(coll)))
                {
                    ExploreGroup(static_cast<LogicWire*>(coll), pGroupIndex); // Recursive call
                }
            }
        }
        ProcessingHeartbeat();
    }
}

std::optional<QPointF> CoreLogic::GetWireCollisionPoint(const LogicWire* pWireA, const LogicWire* pWireB) const
{
    Q_ASSERT(pWireA);
    Q_ASSERT(pWireB);

    if (pWireA->GetDirection() == WireDirection::HORIZONTAL && pWireB->GetDirection() == WireDirection::VERTICAL)
    {
        return QPointF(pWireB->x(), pWireA->y());
    }
    else if (pWireA->GetDirection() == WireDirection::VERTICAL && pWireB->GetDirection() == WireDirection::HORIZONTAL)
    {
        return QPointF(pWireA->x(), pWireB->y());
    }

    return std::nullopt;
}

bool CoreLogic::IsLCrossing(LogicWire* pWireA, LogicWire* pWireB) const
{
    Q_ASSERT(pWireA && pWireB);

    const LogicWire* a = nullptr;
    const LogicWire* b = nullptr;

    if (pWireA->GetDirection() == pWireB->GetDirection())
    {
        return false;
    }

    if (pWireA->GetDirection() == WireDirection::VERTICAL && pWireB->GetDirection() == WireDirection::HORIZONTAL)
    {
        a = pWireB;
        b = pWireA;
    }
    else // must be pWireA horizontal, pWireB vertical
    {
        a = pWireA;
        b = pWireB;
    }

    return ((a->y() == b->y() && a->x() == b->x()) || (a->y() == b->y() && a->x() + a->GetLength() == b->x())
        || (a->x() == b->x() && b->y() + b->GetLength() == a->y()) || (a->x() + a->GetLength() == b->x() && a->y() == b->y() + b->GetLength()));
}

std::optional<ConPoint*> CoreLogic::GetConPointAtPosition(QPointF pPos, ConnectionType pType) const
{
    for (const auto& comp : mView.Scene()->items(pPos, Qt::IntersectsItemShape))
    {
        if ((nullptr != dynamic_cast<ConPoint*>(comp)) && (pType == static_cast<ConPoint*>(comp)->GetConnectionType()))
        {
            return static_cast<ConPoint*>(comp);
        }
    }

    return std::nullopt;
}

void CoreLogic::CreateWireLogicCells()
{
    mLogicWireCells.clear();

    for (const auto& group : mWireGroups)
    {
        auto logicCell = std::make_shared<LogicWireCell>(this);
        mLogicWireCells.emplace_back(logicCell);
        for (auto& comp : group)
        {
            if (dynamic_cast<LogicWire*>(comp) != nullptr)
            {
                static_cast<LogicWire*>(comp)->SetLogicCell(logicCell);
            }
            else if (dynamic_cast<ConPoint*>(comp) != nullptr) // Full crossing because sorted into group
            {
                static_cast<ConPoint*>(comp)->SetLogicCell(logicCell);
            }
            ProcessingHeartbeat();
        }
    }
}

void CoreLogic::ConnectLogicCells()
{
    for (auto& comp : mView.Scene()->items())
    {
        if (nullptr == dynamic_cast<IBaseComponent*>(comp) || nullptr != dynamic_cast<LogicWire*>(comp))
        {
            continue; //  Skip if no non-wire component
        }

        auto compBase = static_cast<IBaseComponent*>(comp);

        for (auto& coll : mView.Scene()->collidingItems(comp, Qt::IntersectsItemBoundingRect))
        {
            if (nullptr == dynamic_cast<LogicWire*>(coll))
            {
                continue; // Skip if no wire
            }

            // Component <-> Wire connection
            auto wire = static_cast<LogicWire*>(coll);

            if (nullptr != dynamic_cast<ConPoint*>(comp))
            {
                const auto& conPoint = static_cast<ConPoint*>(comp);
                if (conPoint->GetConnectionType() != ConnectionType::FULL) // Diode <-> Wire connection
                {
                    Q_ASSERT(compBase->GetLogicCell());
                    auto outputDirection = (conPoint->GetConnectionType() == ConnectionType::DIODE_X ? WireDirection::HORIZONTAL : WireDirection::VERTICAL);
                    auto inputDirection = (conPoint->GetConnectionType() == ConnectionType::DIODE_X ? WireDirection::VERTICAL : WireDirection::HORIZONTAL);

                    if ((wire->GetDirection() == outputDirection)
                            && wire->contains(wire->mapFromScene(compBase->pos() + compBase->GetOutConnectors()[0].pos)))
                    {
                        std::static_pointer_cast<LogicWireCell>(wire->GetLogicCell())->AddInputSlot();
                        compBase->GetLogicCell()->ConnectOutput(wire->GetLogicCell(), std::static_pointer_cast<LogicWireCell>(wire->GetLogicCell())->GetInputSize() - 1, 0);
                    }
                    else if ((wire->GetDirection() == inputDirection)
                            && wire->contains(wire->mapFromScene(compBase->pos() + compBase->GetInConnectors()[0].pos)))
                    {
                        std::static_pointer_cast<LogicWireCell>(wire->GetLogicCell())->AppendOutput(compBase->GetLogicCell(), 0);
                    }
                }
            }
            else // Other component <-> Wire connection
            {
                for (size_t out = 0; out < compBase->GetOutConnectorCount(); out++)
                {
                    if (wire->contains(wire->mapFromScene(compBase->pos() + compBase->GetOutConnectors()[out].pos)))
                    {
                        std::static_pointer_cast<LogicWireCell>(wire->GetLogicCell())->AddInputSlot();
                        compBase->GetLogicCell()->ConnectOutput(wire->GetLogicCell(), std::static_pointer_cast<LogicWireCell>(wire->GetLogicCell())->GetInputSize() - 1, out);
                    }
                }

                for (size_t in = 0; in < compBase->GetInConnectorCount(); in++)
                {
                    if (wire->contains(wire->mapFromScene(compBase->pos() + compBase->GetInConnectors()[in].pos)))
                    {
                        std::static_pointer_cast<LogicWireCell>(wire->GetLogicCell())->AppendOutput(compBase->GetLogicCell(), in);
                    }
                }
            }
            ProcessingHeartbeat();
        }
    }
}

void CoreLogic::StartProcessing()
{
    mProcessingTimer.start(gui::PROCESSING_OVERLAY_TIMEOUT);
    mIsProcessing = true;
}

void CoreLogic::ProcessingHeartbeat()
{
    QCoreApplication::processEvents(); // User input during processing will be handled but ignored
}

void CoreLogic::OnProcessingTimeout()
{
    mView.FadeInProcessingOverlay();
}

void CoreLogic::EndProcessing()
{
    mProcessingTimer.stop();
    mView.FadeOutProcessingOverlay();
    mIsProcessing = false;
}

bool CoreLogic::IsProcessing() const
{
    return mIsProcessing;
}

void CoreLogic::ClearSelection()
{
    mView.Scene()->clearSelection();
    emit HideClockConfiguratorSignal();
}

void CoreLogic::OnSelectedComponentsMoved(QPointF pOffset)
{   
    StartProcessing();

    if (pOffset.manhattanLength() <= 0) // No effective movement
    {
        EndProcessing();
        return;
    }

    std::vector<LogicWire*> affectedWires;
    std::vector<IBaseComponent*> affectedComponents;

    for (const auto& comp : mView.Scene()->selectedItems())
    {
        if (nullptr == dynamic_cast<IBaseComponent*>(comp))
        {
            continue;
        }

        affectedComponents.push_back(static_cast<IBaseComponent*>(comp));

        if (nullptr != dynamic_cast<LogicWire*>(comp))
        {
            affectedWires.push_back(static_cast<LogicWire*>(comp));
        }

        ProcessingHeartbeat();
    }

    std::vector<IBaseComponent*> movedComponents;
    std::vector<IBaseComponent*> addedComponents;
    std::vector<IBaseComponent*> deletedComponents;

    MergeWiresAfterMove(affectedWires, addedComponents, deletedComponents); // Ca. 25% of total cost

    // Insert merged wires to recognize T-crossings
    affectedComponents.insert(affectedComponents.end(), addedComponents.begin(), addedComponents.end());
    // In theory, we should remove deletedComponents from movedComponents here, but that would be costly and
    // should not make any difference because old wires behind the merged ones cannot generate new ConPoints

    for (const auto& comp : affectedComponents) // Ca. 75% of total cost
    {                   
        if (ManageConPointsOneStep(comp, pOffset, movedComponents, addedComponents, deletedComponents))
        {
            ProcessingHeartbeat();
            continue;
        }

        // Collision, abort
        for (const auto& comp : affectedComponents) // Revert moving
        {
            comp->moveBy(-pOffset.x(), -pOffset.y());
        }
        for (const auto& comp : addedComponents) // Revert adding
        {
            delete comp;
        }
        for (const auto& comp : deletedComponents) // Revert deleting
        {
            mView.Scene()->addItem(comp);
        }

        ClearSelection();
        EndProcessing();
        return;
    }

    ClearSelection();

    if (movedComponents.size() > 0)
    {
        AppendUndo(new UndoMoveType(movedComponents, addedComponents, deletedComponents, pOffset));
    }

    EndProcessing();
}

bool CoreLogic::ManageConPointsOneStep(IBaseComponent* pComponent, QPointF& pOffset, std::vector<IBaseComponent*>& movedComponents,
                                       std::vector<IBaseComponent*>& addedComponents, std::vector<IBaseComponent*>& deletedComponents)
{
    if (IsCollidingComponent(pComponent) && !GetCollidingComponents(pComponent, true).empty()) // Abort if collision with unselected component
    {
        return false;
    }

    // Delete all invalid ConPoints at the original position colliding with the selection
    QRectF oldCollisionRect(pComponent->pos() + pComponent->boundingRect().topLeft() - pOffset, pComponent->pos() + pComponent->boundingRect().bottomRight() - pOffset);

    const auto&& abandonedComponents = mView.Scene()->items(oldCollisionRect, Qt::IntersectsItemShape);

    for (const auto& collidingComp : abandonedComponents)
    {
        if ((nullptr != dynamic_cast<ConPoint*>(collidingComp)) && !collidingComp->isSelected() && IsNoCrossingPoint(static_cast<ConPoint*>(collidingComp)))
        {
            Q_ASSERT(collidingComp->scene() == mView.Scene());
            mView.Scene()->removeItem(collidingComp);
            deletedComponents.push_back(static_cast<IBaseComponent*>(collidingComp));
        }
        ProcessingHeartbeat();
    }

    // Delete all ConPoints of the moved components that are not valid anymore
    if ((nullptr != dynamic_cast<ConPoint*>(pComponent)) && IsNoCrossingPoint(static_cast<ConPoint*>(pComponent)))
    {
        Q_ASSERT(pComponent->scene() == mView.Scene());
        mView.Scene()->removeItem(pComponent);
        deletedComponents.push_back(pComponent);
    }

    // Add ConPoints to all T Crossings
    if (nullptr != dynamic_cast<LogicWire*>(pComponent))
    {
        AddConPointsToTCrossings(static_cast<LogicWire*>(pComponent), addedComponents);
    }

    movedComponents.push_back(pComponent);
    return true;
}

void CoreLogic::AddConPointsToTCrossings(LogicWire* pWire, std::vector<IBaseComponent*>& addedComponents)
{
    const auto&& collidingComponents = mView.Scene()->collidingItems(pWire, Qt::IntersectsItemShape);

    for (const auto& collidingComp : collidingComponents)
    {
        ProcessingHeartbeat();

        if (dynamic_cast<LogicWire*>(collidingComp) == nullptr)
        {
            continue;
        }

        if (!IsTCrossing(pWire, static_cast<LogicWire*>(collidingComp)))
        {
            continue;
        }

        QPointF conPointPos;
        if (pWire->GetDirection() == WireDirection::HORIZONTAL)
        {
            conPointPos = QPointF(collidingComp->x(), pWire->y());
        }
        else
        {
            conPointPos = QPointF(pWire->x(), collidingComp->y());
        }

        if (!IsComponentAtPosition<ConPoint>(conPointPos))
        {
            auto item = new ConPoint(this);
            item->setPos(conPointPos);
            addedComponents.push_back(item);
            mView.Scene()->addItem(item);
        }
    }
}

void CoreLogic::OnShowClockConfiguratorRequest(ClockMode pMode, uint32_t pToggle, uint32_t pPulse)
{
    emit ShowClockConfiguratorSignal(pMode, pToggle, pPulse);
}

void CoreLogic::OnLeftMouseButtonPressedWithoutCtrl(QPointF pMappedPos, QMouseEvent &pEvent)
{
    auto snappedPos = SnapToGrid(pMappedPos);

    emit HideClockConfiguratorSignal();

    // Add ConPoint on X crossing
    if (mControlMode == ControlMode::EDIT
            && mView.Scene()->selectedItems().empty()                                               // Scene must be empty (select of clicked item did not yet happen)
            && dynamic_cast<LogicWire*>(mView.Scene()->itemAt(pMappedPos, QTransform())) != nullptr // Wire is clicked (not crossing below other component)
            && IsXCrossingPoint(snappedPos)                                                         // There is an X-crossing at that position
            && !IsComponentAtPosition<ConPoint>(snappedPos))                                        // There is no ConPoint at that position yet
    {
        auto item = new ConPoint(this); // Create a new ConPoint (removing will be handled by OnConnectionTypeChanged)
        item->setPos(snappedPos);
        std::vector<IBaseComponent*> addedComponents{item};
        mView.Scene()->addItem(item);
        AppendUndo(new UndoAddType(addedComponents));
        return;
    }

    if (mControlMode == ControlMode::EDIT
            && mView.Scene()->selectedItems().empty()) // Invert in/output connectors
    {
        for (const auto& item : mView.Scene()->items(pMappedPos, Qt::IntersectsItemBoundingRect))
        {
            if ((nullptr != dynamic_cast<AbstractGate*>(item)) || (nullptr != dynamic_cast<AbstractComplexLogic*>(item)) || (nullptr != dynamic_cast<LogicClock*>(item)))
            {
                const auto& connector = static_cast<IBaseComponent*>(item)->InvertConnectorByPoint(pMappedPos);
                if (connector.has_value())
                {
                    auto data = std::make_shared<undo::ConnectorInversionChangedData>(static_cast<IBaseComponent*>(item), connector.value());
                    AppendUndo(new UndoConfigureType(data));
                    return;
                }
            }
        }
    }

    if (mControlMode == ControlMode::ADD) // Add component at the current position
    {
        const auto&& success = AddCurrentTypeComponent(snappedPos);
        if (success)
        {
            // A new component has been added => clear selection if it wasn't a text label
            if (mView.Scene()->selectedItems().size() != 1 || dynamic_cast<TextLabel*>(mView.Scene()->selectedItems()[0]) == nullptr)
            {
                ClearSelection();
            }
            return;
        }
    }

    if (mControlMode == ControlMode::WIRE) // Start the preview wire at the current position
    {
        SetPreviewWireStart(snappedPos);
        return;
    }

    if (mControlMode == ControlMode::COPY)
    {
        // TODO: Delete selection if press was not on selected item
        return;
    }

    emit MousePressedEventDefaultSignal(pEvent);
}

void CoreLogic::OnConnectionTypeChanged(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType)
{
    Q_ASSERT(pConPoint);

    if (IsXCrossingPoint(pConPoint->pos()) && pPreviousType == ConnectionType::DIODE_X)
    {
        pConPoint->setSelected(false);
        pConPoint->SetConnectionType(pPreviousType); // Restore old connection type in case delete is undone
        mView.Scene()->removeItem(pConPoint);

        auto deleted = std::vector<IBaseComponent*>{pConPoint};
        AppendUndo(new UndoDeleteType(deleted));
    }
    else
    {
        auto data = std::make_shared<undo::ConnectionTypeChangedData>(pConPoint, pPreviousType, pCurrentType);
        AppendUndo(new UndoConfigureType(data));
    }
}

void CoreLogic::OnTextLabelContentChanged(TextLabel* pTextLabel, const QString& pPreviousText, const QString& pCurrentText)
{
    Q_ASSERT(pTextLabel);

    auto data = std::make_shared<undo::TextLabelContentChangedData>(pTextLabel, pPreviousText, pCurrentText);
    AppendUndo(new UndoConfigureType(data));
}

void CoreLogic::CopySelectedComponents()
{
    QList<QGraphicsItem*> componentsToCopy = mView.Scene()->selectedItems();
    std::vector<IBaseComponent*> addedComponents{};

    ClearSelection();

    for (auto& orig : componentsToCopy)
    {
        // Create a copy of the original component
        IBaseComponent* copy = static_cast<IBaseComponent*>(orig)->CloneBaseComponent(this);
        Q_ASSERT(copy);

        // Paste the copied component one grid cell below and to the right
        copy->setPos(SnapToGrid(orig->pos() + QPointF(canvas::GRID_SIZE, canvas::GRID_SIZE)));
        copy->setSelected(true);
        copy->ResetZValue();
        copy->setZValue(copy->zValue() + 100); // Bring copied components to front
        mView.Scene()->addItem(copy);
        addedComponents.push_back(copy);
    }

    if (addedComponents.size() > 0)
    {
        AppendUndo(new UndoAddType(addedComponents));
    }
}

void CoreLogic::DeleteSelectedComponents()
{
    QList<QGraphicsItem*> componentsToDelete = mView.Scene()->selectedItems();
    std::vector<IBaseComponent*> deletedComponents{};
    for (auto& comp : componentsToDelete)
    {
        // Do not allow deleting of ConPoints on T crossings
        if (dynamic_cast<ConPoint*>(comp) == nullptr || IsXCrossingPoint(comp->pos()))
        {
            mView.Scene()->removeItem(comp);
            deletedComponents.push_back(static_cast<IBaseComponent*>(comp));
        }
    }

    // Delete all colliding ConPoints that are not over a crossing anymore
    for (auto& comp : FilterForWires(componentsToDelete))
    {
        for (const auto& collidingComp : mView.Scene()->collidingItems(comp))
        {
            if (dynamic_cast<ConPoint*>(collidingComp) != nullptr && IsNoCrossingPoint(static_cast<ConPoint*>(collidingComp)))
            {
                mView.Scene()->removeItem(collidingComp);
                deletedComponents.push_back(static_cast<IBaseComponent*>(collidingComp));
            }
        }
    }

    if (deletedComponents.size() > 0)
    {
        AppendUndo(new UndoDeleteType(deletedComponents));
    }
    ClearSelection();
}

QJsonObject CoreLogic::GetJson() const
{
    QJsonObject json;

    QJsonArray components;

    for (const auto& item : mView.Scene()->items())
    {
        if (nullptr != dynamic_cast<IBaseComponent*>(item))
        {
            components.append(static_cast<IBaseComponent*>(item)->GetJson());
        }
    }

    json[file::JSON_COMPONENTS_IDENTIFIER] = components;

    return json;
}

void CoreLogic::NewCircuit()
{
    EnterControlMode(ControlMode::EDIT); // Always start in edit mode after loading

    // Delete all components
    for (const auto& item : mView.Scene()->items())
    {
        mView.Scene()->removeItem(item);
    }

    mView.ResetViewport();

    // Clear undo and redo stacks
    mUndoQueue.clear();
    mRedoQueue.clear();

    emit UpdateUndoRedoEnabledSignal();

    mCircuitModified = false;
    mFilePath = std::nullopt;
}

void CoreLogic::ReadJson(const QJsonObject& pJson)
{
    EnterControlMode(ControlMode::EDIT); // Always start in edit mode after loading

    // Delete all components
    for (const auto& item : mView.Scene()->items())
    {
        mView.Scene()->removeItem(item);
    }

    mView.ResetViewport();

    // Create components
    if (pJson.contains(file::JSON_COMPONENTS_IDENTIFIER) && pJson[file::JSON_COMPONENTS_IDENTIFIER].isArray())
    {
        auto components = pJson[file::JSON_COMPONENTS_IDENTIFIER].toArray();

        for (uint32_t compIndex = 0; compIndex < components.size(); compIndex++)
        {
            auto component = components[compIndex].toObject();

            if (!CreateComponent(component))
            {
                qDebug() << "Component unknown";
            }
        }
    }

    // Clear undo and redo stacks
    mUndoQueue.clear();
    mRedoQueue.clear();

    emit UpdateUndoRedoEnabledSignal();
}

bool CoreLogic::CreateComponent(const QJsonObject &pJson)
{
    if (pJson.contains(file::JSON_TYPE_IDENTIFIER) && pJson[file::JSON_TYPE_IDENTIFIER].isDouble())
    {
        IBaseComponent* item = nullptr;
        switch (pJson[file::JSON_TYPE_IDENTIFIER].toInt())
        {
            case file::ComponentId::AND_GATE:
            {
                item = new AndGate(this, pJson);
                break;
            }
            case file::ComponentId::OR_GATE:
            {
                item = new OrGate(this, pJson);
                break;
            }
            case file::ComponentId::XOR_GATE:
            {
                item = new XorGate(this, pJson);
                break;
            }
            case file::ComponentId::NOT_GATE:
            {
                item = new NotGate(this, pJson);
                break;
            }
            case file::ComponentId::BUFFER_GATE:
            {
                item = new BufferGate(this, pJson);
                break;
            }
            case file::ComponentId::WIRE:
            {
                item = new LogicWire(this, pJson);
                break;
            }
            case file::ComponentId::CONPOINT:
            {
                item = new ConPoint(this, pJson);
                break;
            }
            case file::ComponentId::TEXT_LABEL:
            {
                item = new TextLabel(this, pJson);
                break;
            }
            case file::ComponentId::INPUT:
            {
                item = new LogicInput(this, pJson);
                break;
            }
            case file::ComponentId::BUTTON:
            {
                item = new LogicButton(this, pJson);
                break;
            }
            case file::ComponentId::CLOCK:
            {
                item = new LogicClock(this, pJson);
                break;
            }
            case file::ComponentId::OUTPUT:
            {
                item = new LogicOutput(this, pJson);
                break;
            }
            case file::ComponentId::HALF_ADDER:
            {
                item = new HalfAdder(this, pJson);
                break;
            }
            case file::ComponentId::FULL_ADDER:
            {
                item = new FullAdder(this, pJson);
                break;
            }
            case file::ComponentId::RS_FLIPFLOP:
            {
                item = new RsFlipFlop(this, pJson);
                break;
            }
            case file::ComponentId::D_FLIPFLOP:
            {
                item = new DFlipFlop(this, pJson);
                break;
            }
            case file::ComponentId::T_FLIPFLOP:
            {
                item = new TFlipFlop(this, pJson);
                break;
            }
            case file::ComponentId::JK_FLIPFLOP:
            {
                item = new JKFlipFlop(this, pJson);
                break;
            }
            case file::ComponentId::MULTIPLEXER:
            {
                item = new Multiplexer(this, pJson);
                break;
            }
            case file::ComponentId::DEMULTIPLEXER:
            {
                item = new Demultiplexer(this, pJson);
                break;
            }
            default:
            {
                // component unknown by this SW version
                return false;
                break;
            }
        }

        if (nullptr != item)
        {
            mView.Scene()->addItem(item);
            return true;
        }
    }

    // JSON array does not contain a type
    return false;
}

void CoreLogic::CircuitModified()
{
    if (!mCircuitModified)
    {
        mCircuitModified = true;
        emit CircuitModifiedSignal();
    }
}

bool CoreLogic::IsFileOpen() const
{
    return mFilePath.has_value();
}

bool CoreLogic::IsCircuitModified() const
{
    return mCircuitModified;
}

std::optional<QString> CoreLogic::GetFilePath() const
{
    return mFilePath;
}

bool CoreLogic::SaveJson()
{
    if (!mFilePath.has_value())
    {
        return false;
    }

    QFile saveFile(mFilePath.value());

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    auto jsonObject = GetJson();

    saveFile.write(file::SAVE_FORMAT == file::SaveFormat::JSON
                   ? QJsonDocument(jsonObject).toJson()
                   : QCborValue::fromJsonValue(jsonObject).toCbor());

    mCircuitModified = false;

    return true;
}

bool CoreLogic::SaveJsonAs(const QString& pPath)
{
    QFile saveFile(pPath);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    auto jsonObject = GetJson();

    saveFile.write(file::SAVE_FORMAT == file::SaveFormat::JSON
                   ? QJsonDocument(jsonObject).toJson()
                   : QCborValue::fromJsonValue(jsonObject).toCbor());

    mFilePath = pPath;
    mCircuitModified = false;

    return true;
}

bool CoreLogic::LoadJson(const QString& pPath)
{
    QFile loadFile(pPath);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        mFilePath = std::nullopt;
        return false;
    }

    QByteArray rawData = loadFile.readAll();

    QJsonDocument jsonDoc(file::SAVE_FORMAT == file::SaveFormat::JSON
                    ? QJsonDocument::fromJson(rawData)
                    : QJsonDocument(QCborValue::fromCbor(rawData).toMap().toJsonObject()));

    ReadJson(jsonDoc.object());

    mFilePath = pPath;

    return true;
}

void CoreLogic::AppendUndo(UndoBaseType* pUndoObject)
{
    Q_ASSERT(pUndoObject);

    CircuitModified();
    AppendToUndoQueue(pUndoObject, mUndoQueue);
    mRedoQueue.clear();

    emit UpdateUndoRedoEnabledSignal();
}

void CoreLogic::AppendToUndoQueue(UndoBaseType* pUndoObject, std::deque<UndoBaseType*> &pQueue)
{
    Q_ASSERT(pUndoObject);

    pQueue.push_back(pUndoObject);
    if (pQueue.size() > MAX_UNDO_STACK_SIZE)
    {
        delete pQueue.front();
        pQueue.pop_front();
    }
}

void CoreLogic::Undo()
{
    if (mUndoQueue.size() > 0)
    {
        UndoBaseType* undoObject = mUndoQueue.back();
        mUndoQueue.pop_back();
        Q_ASSERT(undoObject);

        switch (undoObject->Type())
        {
            case undo::Type::ADD:
            {
                for (const auto& comp : static_cast<UndoAddType*>(undoObject)->AddedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                for (const auto& comp : static_cast<UndoAddType*>(undoObject)->DeletedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case undo::Type::DEL:
            {
                for (const auto& comp : static_cast<UndoDeleteType*>(undoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case undo::Type::MOVE:
            {
                const auto undoMoveObject = static_cast<UndoMoveType*>(undoObject);
                for (const auto& comp : static_cast<UndoMoveType*>(undoObject)->DeletedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                for (const auto& comp : static_cast<UndoMoveType*>(undoObject)->AddedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                for (const auto& comp : undoMoveObject->MovedComponents())
                {
                    Q_ASSERT(comp);
                    comp->moveBy(-undoMoveObject->Offset().x(), -undoMoveObject->Offset().y());
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case undo::Type::CONFIGURE:
            {
                const auto undoConfigureObject = static_cast<UndoConfigureType*>(undoObject);
                switch (undoConfigureObject->Data()->Type())
                {
                    case undo::ConfigType::CONNECTION_TYPE:
                    {
                        auto data = std::static_pointer_cast<undo::ConnectionTypeChangedData>(undoConfigureObject->Data());
                        Q_ASSERT(data->conPoint);
                        data->conPoint->SetConnectionType(data->previousType);
                        AppendToUndoQueue(undoObject, mRedoQueue);
                        break;
                    }
                case undo::ConfigType::TEXTLABEL_CONTENT:
                {
                    auto data = std::static_pointer_cast<undo::TextLabelContentChangedData>(undoConfigureObject->Data());
                    Q_ASSERT(data->textLabel);
                    data->textLabel->SetTextContent(data->previousText);
                    AppendToUndoQueue(undoObject, mRedoQueue);
                    break;
                }
                case undo::ConfigType::CONNECTOR_INVERSION:
                {
                    auto data = std::static_pointer_cast<undo::ConnectorInversionChangedData>(undoConfigureObject->Data());
                    Q_ASSERT(data->component);
                    Q_ASSERT(data->logicConnector);
                    data->component->InvertConnectorByPoint(data->component->pos() + data->logicConnector->pos);
                    AppendToUndoQueue(undoObject, mRedoQueue);
                    break;
                }
                }
                break;
            }
            default:
            {
                break;
            }
        }
        CircuitModified();
    }
    ClearSelection();
}

void CoreLogic::Redo()
{
    if (mRedoQueue.size() > 0)
    {
        UndoBaseType* redoObject = mRedoQueue.back();
        mRedoQueue.pop_back();
        Q_ASSERT(redoObject);

        switch (redoObject->Type())
        {
            case undo::Type::ADD:
            {
                for (const auto& comp : static_cast<UndoAddType*>(redoObject)->AddedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                for (const auto& comp : static_cast<UndoAddType*>(redoObject)->DeletedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case undo::Type::DEL:
            {
                for (const auto& comp : static_cast<UndoDeleteType*>(redoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case undo::Type::MOVE:
            {
                const auto redoMoveObject = static_cast<UndoMoveType*>(redoObject);
                for (const auto& comp : redoMoveObject->MovedComponents())
                {
                    Q_ASSERT(comp);
                    comp->moveBy(redoMoveObject->Offset().x(), redoMoveObject->Offset().y());
                }
                for (const auto& comp : static_cast<UndoMoveType*>(redoObject)->AddedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                for (const auto& comp : static_cast<UndoMoveType*>(redoObject)->DeletedComponents())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case undo::Type::CONFIGURE:
            {
                const auto undoConfigureObject = static_cast<UndoConfigureType*>(redoObject);
                switch (undoConfigureObject->Data()->Type())
                {
                    case undo::ConfigType::CONNECTION_TYPE:
                    {
                        auto data = std::static_pointer_cast<undo::ConnectionTypeChangedData>(undoConfigureObject->Data());
                        Q_ASSERT(data->conPoint);
                        data->conPoint->SetConnectionType(data->currentType);
                        AppendToUndoQueue(redoObject, mUndoQueue);
                        break;
                    }
                    case undo::ConfigType::TEXTLABEL_CONTENT:
                    {
                        auto data = std::static_pointer_cast<undo::TextLabelContentChangedData>(undoConfigureObject->Data());
                        Q_ASSERT(data->textLabel);
                        data->textLabel->SetTextContent(data->currentText);
                        AppendToUndoQueue(redoObject, mUndoQueue);
                        break;
                    }
                    case undo::ConfigType::CONNECTOR_INVERSION:
                    {
                        auto data = std::static_pointer_cast<undo::ConnectorInversionChangedData>(undoConfigureObject->Data());
                        Q_ASSERT(data->component);
                        Q_ASSERT(data->logicConnector);
                        data->component->InvertConnectorByPoint(data->component->pos() + data->logicConnector->pos);
                        AppendToUndoQueue(redoObject, mUndoQueue);
                        break;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }
        CircuitModified();
    }
    ClearSelection();
}
