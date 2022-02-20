#include "CoreLogic.h"

#include "Components/Gates/AndGate.h"
#include "Components/Gates/OrGate.h"
#include "Components/Gates/XorGate.h"
#include "Components/Gates/NotGate.h"
#include "Components/LogicInput.h"
#include "Components/LogicOutput.h"

#include "Undo/UndoAddType.h"
#include "Undo/UndoDeleteType.h"
#include "Undo/UndoMoveType.h"
#include "Undo/UndoConfigureType.h"

#include "HelperFunctions.h"

#include <QElapsedTimer>

CoreLogic::CoreLogic(View &pView):
    mView(pView),
    mHorizontalPreviewWire(this, WireDirection::HORIZONTAL, 0),
    mVerticalPreviewWire(this, WireDirection::VERTICAL, 0)
{
    ConnectToView();
    mView.Init();
}

void CoreLogic::ConnectToView()
{
    QObject::connect(this, &CoreLogic::ControlModeChangedSignal, &mView, &View::OnControlModeChanged);
    QObject::connect(this, &CoreLogic::ComponentTypeChangedSignal, &mView, &View::OnComponentTypeChanged);
}

void CoreLogic::EnterControlMode(ControlMode pMode)
{
    if (pMode != mControlMode)
    {
        if (mControlMode == ControlMode::SIMULATION)
        {
            mControlMode = pMode;
            emit SimulationStopSignal();
        }

        mControlMode = pMode;
        emit ControlModeChangedSignal(pMode);

        if (pMode == ControlMode::ADD)
        {
            emit ComponentTypeChangedSignal(mComponentType);
        }

        if (pMode == ControlMode::SIMULATION)
        {
            emit SimulationStartSignal();
        }
    }
}

void CoreLogic::EnterAddControlMode(ComponentType pComponentType)
{
    EnterControlMode(ControlMode::ADD);
    SetComponentType(pComponentType);
}

ComponentType CoreLogic::GetComponentType() const
{
    return mComponentType;
}

bool CoreLogic::IsSimulationRunning(void) const
{
    return (mControlMode == ControlMode::SIMULATION);
}

bool CoreLogic::IsUndoQueueEmpty(void) const
{
    return mUndoQueue.empty();
}

bool CoreLogic::IsRedoQueueEmpty(void) const
{
    return mRedoQueue.empty();
}

void CoreLogic::SetComponentType(ComponentType pComponentType)
{
    mComponentType = pComponentType;
    emit ComponentTypeChangedSignal(mComponentType);
}

QGraphicsItem* CoreLogic::GetItem()
{
    QGraphicsItem* item;

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
        case ComponentType::INPUT:
        {
            item = new LogicInput(this);
            break;
        }
        case ComponentType::OUTPUT:
        {
            item = new LogicOutput(this);
            break;
        }
        default:
        {
            item = nullptr;
            break;
        }
    }

    return item;
}

ControlMode CoreLogic::GetControlMode()
{
    return mControlMode;
}

void CoreLogic::AddCurrentTypeComponent(QPointF pPosition)
{
    auto item = GetItem();
    Q_ASSERT(item);

    item->setPos(SnapToGrid(pPosition));

    if (GetCollidingComponents(item).empty())
    {
        mView.Scene()->addItem(item);

        auto addedComponents = std::vector<BaseComponent*>{static_cast<BaseComponent*>(item)};
        AppendUndo(new UndoAddType(addedComponents));
    }
    else
    {
        delete item;
    }
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
    std::vector<BaseComponent*> addedComponents;
    std::vector<BaseComponent*> deletedComponents;

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
            endAdjacent = nullptr;
        }

        if (startAdjacent)
        {
            deletedComponents.push_back(static_cast<BaseComponent*>(startAdjacent));
            mView.Scene()->removeItem(startAdjacent);
        }

        if (endAdjacent)
        {
            deletedComponents.push_back(static_cast<BaseComponent*>(endAdjacent));
            mView.Scene()->removeItem(endAdjacent);
        }

        mView.Scene()->addItem(horizontalWire);
        addedComponents.push_back(static_cast<BaseComponent*>(horizontalWire));
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
            endAdjacent = nullptr;
        }

        if (startAdjacent)
        {
            deletedComponents.push_back(static_cast<BaseComponent*>(startAdjacent));
            mView.Scene()->removeItem(startAdjacent);
        }

        if (endAdjacent)
        {
            deletedComponents.push_back(static_cast<BaseComponent*>(endAdjacent));
            mView.Scene()->removeItem(endAdjacent);
        }

        mView.Scene()->addItem(verticalWire);
        addedComponents.push_back(static_cast<BaseComponent*>(verticalWire));
    }

    std::vector<BaseComponent*> addedConPoints;

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

void CoreLogic::MergeWiresAfterMove(QList<QGraphicsItem*> &pComponents, std::vector<BaseComponent*> &pAddedWires, std::vector<BaseComponent*> &pDeletedWires)
{
    auto wires = FilterForWires(pComponents);

    for (auto &w : wires)
    {
        const auto containedWires = DeleteContainedWires(static_cast<LogicWire*>(w));
        pDeletedWires.insert(pDeletedWires.end(), containedWires.begin(), containedWires.end());

        LogicWire* startAdjacent = nullptr;
        LogicWire* endAdjacent = nullptr;

        if (static_cast<LogicWire*>(w)->GetDirection() == WireDirection::HORIZONTAL)
        {
            startAdjacent = GetAdjacentWire(QPointF(w->x() - 2, w->y()), WireDirection::HORIZONTAL);
            endAdjacent = GetAdjacentWire(QPointF(w->x() + static_cast<LogicWire*>(w)->GetLength() + 2, w->y()), WireDirection::HORIZONTAL);

            auto horizontalWire = MergeWires(static_cast<LogicWire*>(w), startAdjacent, endAdjacent);
            horizontalWire->setSelected(w->isSelected());

            if (startAdjacent == endAdjacent)
            {
                endAdjacent = nullptr;
            }

            mView.Scene()->addItem(horizontalWire);
            pAddedWires.push_back(static_cast<BaseComponent*>(horizontalWire));
        }
        else
        {
            startAdjacent = GetAdjacentWire(QPointF(w->x(), w->y() - 2), WireDirection::VERTICAL);
            endAdjacent = GetAdjacentWire(QPointF(w->x(), w->y() + static_cast<LogicWire*>(w)->GetLength() + 2), WireDirection::VERTICAL);

            auto verticalWire = MergeWires(static_cast<LogicWire*>(w), startAdjacent, endAdjacent);
            verticalWire->setSelected(w->isSelected());

            if (startAdjacent == endAdjacent)
            {
                endAdjacent = nullptr;
            }

            mView.Scene()->addItem(verticalWire);
            pAddedWires.push_back(static_cast<BaseComponent*>(verticalWire));
        }

        pDeletedWires.push_back(w);
        mView.Scene()->removeItem(w);

        if (startAdjacent)
        {
            pDeletedWires.push_back(static_cast<BaseComponent*>(startAdjacent));
            mView.Scene()->removeItem(startAdjacent);
        }

        if (endAdjacent)
        {
            pDeletedWires.push_back(static_cast<BaseComponent*>(endAdjacent));
            mView.Scene()->removeItem(endAdjacent);
        }
    }
}

std::vector<BaseComponent*> CoreLogic::DeleteContainedWires(LogicWire* pWire)
{
    std::vector<BaseComponent*> deletedComponents;

    QList<QGraphicsItem*> containedComponents;

    if (pWire->GetDirection() == WireDirection::HORIZONTAL)
    {
        containedComponents = mView.Scene()->items(pWire->x() - 2, pWire->y() - components::wires::BOUNDING_RECT_SIZE / 2 - 2,
                                                   pWire->GetLength() + 4, components::wires::BOUNDING_RECT_SIZE + 4, Qt::ContainsItemShape, Qt::DescendingOrder);
    }
    else
    {
        containedComponents = mView.Scene()->items(pWire->x() - components::wires::BOUNDING_RECT_SIZE / 2 - 2, pWire->y() - 2,
                                                   components::wires::BOUNDING_RECT_SIZE + 4, pWire->GetLength() + 4, Qt::ContainsItemShape, Qt::DescendingOrder);
    }
    const auto containedWires = FilterForWires(containedComponents, pWire->GetDirection());

    for (auto &wire : containedWires)
    {
        if (wire != pWire)
        {
            deletedComponents.push_back(wire);
            mView.Scene()->removeItem(wire);
        }
    }

    return deletedComponents;
}

LogicWire* CoreLogic::GetAdjacentWire(QPointF pCheckPosition, WireDirection pDirection) const
{
    const auto startAdjacentComponents = mView.Scene()->items(pCheckPosition.x(), pCheckPosition.y(), 1, 1, Qt::IntersectsItemShape, Qt::DescendingOrder);
    const auto startAdjacentWires = FilterForWires(startAdjacentComponents, pDirection);

    if (startAdjacentWires.size() > 0)
    {
        return static_cast<LogicWire*>(startAdjacentWires.at(0));
    }

    return nullptr;
}

std::vector<BaseComponent*> CoreLogic::FilterForWires(const QList<QGraphicsItem*> &pComponents, WireDirection pDirection) const
{
    std::vector<BaseComponent*> wires;

    for (auto &comp : pComponents)
    {
        if (dynamic_cast<LogicWire*>(comp) != nullptr
                && (static_cast<LogicWire*>(comp)->GetDirection() == pDirection || pDirection == WireDirection::UNSET))
        {
            wires.push_back(static_cast<BaseComponent*>(comp));
        }
    }

    return wires;
}

std::vector<BaseComponent*> CoreLogic::GetCollidingComponents(QGraphicsItem* &pComponent)
{
    std::vector<BaseComponent*> collidingComponents;

    for (auto &comp : mView.Scene()->collidingItems(pComponent))
    {
        if (dynamic_cast<LogicWire*>(comp) == nullptr)
        {
            collidingComponents.push_back(static_cast<BaseComponent*>(comp));
        }
    }

    return collidingComponents;
}

bool CoreLogic::IsCollidingComponent(QGraphicsItem* pComponent) const
{
    return (dynamic_cast<LogicWire*>(pComponent) == nullptr && dynamic_cast<ConPoint*>(pComponent) == nullptr);
}

bool CoreLogic::IsTCrossing(const LogicWire* pWire1, const LogicWire* pWire2) const
{
    const LogicWire* a;
    const LogicWire* b;

    if (pWire1->GetDirection() == WireDirection::VERTICAL && pWire2->GetDirection() == WireDirection::HORIZONTAL)
    {
        a = pWire1;
        b = pWire2;
    }
    else if (pWire1->GetDirection() == WireDirection::HORIZONTAL && pWire2->GetDirection() == WireDirection::VERTICAL)
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
    const auto& wires = FilterForWires(mView.Scene()->items(pConPoint->pos(), Qt::IntersectsItemBoundingRect));

    if (wires.size() <= 1)
    {
        return true;
    }
    else
    {
        for (const auto& wire : wires)
        {
            if (!static_cast<LogicWire*>(wire)->StartsOrEndsIn(pConPoint->pos()))
            {
                // T-Crossing wire found, this is no L or I crossing
                return false;
            }
        }
        return true;
    }
}

bool CoreLogic::IsXCrossingPoint(QPointF pPoint) const
{
    const auto& wires = FilterForWires(mView.Scene()->items(pPoint, Qt::IntersectsItemBoundingRect));

    if (wires.size() <= 1)
    {
        return false;
    }
    else
    {
        for (const auto& wire : wires)
        {
            if (static_cast<LogicWire*>(wire)->StartsOrEndsIn(pPoint))
            {
                // L-Crossing type wire found, this is no X crossing
                return false;
            }
        }
        return true;
    }
}

LogicWire* CoreLogic::MergeWires(LogicWire* pNewWire, LogicWire* pStartAdjacent, LogicWire* pEndAdjacent) const
{
    QPointF newStart(pNewWire->pos());

    if (pNewWire->GetDirection() == WireDirection::HORIZONTAL)
    {
        QPointF newEnd(pNewWire->x() + pNewWire->GetLength(), pNewWire->y());

        if (pStartAdjacent && pStartAdjacent->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->y() == pStartAdjacent->y());
            newStart = QPointF(pStartAdjacent->x(), pNewWire->y());
        }
        if (pEndAdjacent && pEndAdjacent->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->y() == pEndAdjacent->y());
            newEnd = QPoint(pEndAdjacent->x() + pEndAdjacent->GetLength(), pNewWire->y());
        }

        auto newWire = new LogicWire(this, WireDirection::HORIZONTAL, newEnd.x() - newStart.x());
        newWire->setPos(newStart);
        return newWire;
    }
    else
    {
        QPointF newEnd(pNewWire->x(), pNewWire->y() + pNewWire->GetLength());

        if (pStartAdjacent && pStartAdjacent->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->x() == pStartAdjacent->x());
            newStart = QPointF(pNewWire->x(), pStartAdjacent->y());
        }
        if (pEndAdjacent && pEndAdjacent->GetDirection() == pNewWire->GetDirection())
        {
            Q_ASSERT(pNewWire->x() == pEndAdjacent->x());
            newEnd = QPoint(pNewWire->x(), pEndAdjacent->y() + pEndAdjacent->GetLength());
        }

        auto newWire = new LogicWire(this, WireDirection::VERTICAL, newEnd.y() - newStart.y());
        newWire->setPos(newStart);
        return newWire;
    }
}

void CoreLogic::OnSelectedComponentsMoved(QPointF pOffset)
{
    if (pOffset.manhattanLength() <= 0)
    {
        // No effective movement
        return;
    }

    QList<QGraphicsItem*> movedGraphicsItems = mView.Scene()->selectedItems();

    std::vector<BaseComponent*> movedComponents;
    std::vector<BaseComponent*> addedComponents;
    std::vector<BaseComponent*> deletedComponents;

    MergeWiresAfterMove(movedGraphicsItems, addedComponents, deletedComponents);

    for (auto& comp : movedGraphicsItems)
    {
        // Delete all invalid ConPoints the original position colliding with the selection
        QRectF oldCollisionRect(comp->pos() + comp->boundingRect().topLeft() - pOffset,
                                           comp->pos() + comp->boundingRect().bottomRight() - pOffset);

        for (const auto& collidingComp : mView.Scene()->items(oldCollisionRect, Qt::IntersectsItemShape))
        {
            if (dynamic_cast<ConPoint*>(collidingComp) != nullptr && !collidingComp->isSelected() && IsNoCrossingPoint(static_cast<ConPoint*>(collidingComp)))
            {
                mView.Scene()->removeItem(collidingComp);
                deletedComponents.push_back(static_cast<BaseComponent*>(collidingComp));
            }
        }

        // Delete all ConPoints of the moved components that are not valid anymore
        if (dynamic_cast<ConPoint*>(comp) && IsNoCrossingPoint(static_cast<ConPoint*>(comp)))
        {
            mView.Scene()->removeItem(comp);
            deletedComponents.push_back(static_cast<BaseComponent*>(comp));
        }

        // Add ConPoints to all T Crossings
        if (dynamic_cast<LogicWire*>(comp) != nullptr)
        {
            for (const auto& collidingComp : mView.Scene()->collidingItems(comp, Qt::IntersectsItemShape))
            {
                if (dynamic_cast<LogicWire*>(collidingComp) != nullptr && IsTCrossing(static_cast<LogicWire*>(comp), static_cast<LogicWire*>(collidingComp)))
                {
                    QPointF conPointPos;
                    if (static_cast<LogicWire*>(comp)->GetDirection() == WireDirection::HORIZONTAL)
                    {
                        conPointPos.setX(collidingComp->x());
                        conPointPos.setY(comp->y());
                    }
                    else
                    {
                        conPointPos.setX(comp->x());
                        conPointPos.setY(collidingComp->y());
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
        }

        // If collision was detected
        if (IsCollidingComponent(comp) && !GetCollidingComponents(comp).empty())
        {
            // Collision, abort
            for (const auto& comp : movedGraphicsItems) // Revert moving
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
            mView.Scene()->clearSelection();
            return;
        }
        movedComponents.push_back(static_cast<BaseComponent*>(comp));
    }

    mView.Scene()->clearSelection();

    if (movedComponents.size() > 0)
    {
        AppendUndo(new UndoMoveType(movedComponents, addedComponents, deletedComponents, pOffset));
    }
}

void CoreLogic::OnLeftMouseButtonPressed(QPointF pMappedPos, QMouseEvent &pEvent)
{
    auto snappedPos = SnapToGrid(pMappedPos);

    if (mControlMode != ControlMode::SIMULATION)
    {
        // Add ConPoint on X crossing
        if (mControlMode == ControlMode::EDIT && IsXCrossingPoint(snappedPos)
                && !IsComponentAtPosition<ConPoint>(snappedPos)
                && mView.Scene()->selectedItems().empty())
        {
            // Create a new ConPoint (removing will be handled by OnConnectionTypeChanged)
            auto item = new ConPoint(this);
            item->setPos(snappedPos);
            std::vector<BaseComponent*> addedComponents{item};
            mView.Scene()->addItem(item);
            AppendUndo(new UndoAddType(addedComponents));
            return;
        }

        // Add component at the current position
        if (mControlMode == ControlMode::ADD)
        {
            AddCurrentTypeComponent(snappedPos);
            return;
        }

        // Start the preview wire at the current position
        if (mControlMode == ControlMode::WIRE)
        {
            SetPreviewWireStart(snappedPos);
            return;
        }
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

        auto deleted = std::vector<BaseComponent*>{pConPoint};
        AppendUndo(new UndoDeleteType(deleted));
    }
    else
    {
        auto data = std::make_shared<Undo::ConnectionTypeChangedData>(pConPoint, pPreviousType, pCurrentType);
        AppendUndo(new UndoConfigureType(data));
    }
}

void CoreLogic::CopySelectedComponents()
{
    QList<QGraphicsItem*> componentsToCopy = mView.Scene()->selectedItems();
    std::vector<BaseComponent*> addedComponents{};

    mView.Scene()->clearSelection();

    for (auto& orig : componentsToCopy)
    {
        // Create a copy of the original component
        BaseComponent* copy = static_cast<BaseComponent*>(orig)->CloneBaseComponent(this);
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
    std::vector<BaseComponent*> deletedComponents{};
    for (auto& comp : componentsToDelete)
    {
        // Do not allow deleting of ConPoints on T crossings
        if (dynamic_cast<ConPoint*>(comp) == nullptr || IsXCrossingPoint(comp->pos()))
        {
            mView.Scene()->removeItem(comp);
            deletedComponents.push_back(static_cast<BaseComponent*>(comp));
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
                deletedComponents.push_back(static_cast<BaseComponent*>(collidingComp));
            }
        }
    }

    if (deletedComponents.size() > 0)
    {
        AppendUndo(new UndoDeleteType(deletedComponents));
    }
}

void CoreLogic::AppendUndo(UndoBaseType* pUndoObject)
{
    AppendToUndoQueue(pUndoObject, mUndoQueue);
    mRedoQueue.clear();

    mView.SetUndoRedoButtonsEnableState();
}

void CoreLogic::AppendToUndoQueue(UndoBaseType* pUndoObject, std::deque<UndoBaseType*> &pQueue)
{
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
            case Undo::Type::ADD:
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
            case Undo::Type::DEL:
            {
                for (const auto& comp : static_cast<UndoDeleteType*>(undoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->addItem(comp);
                }
                AppendToUndoQueue(undoObject, mRedoQueue);
                break;
            }
            case Undo::Type::MOVE:
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
            case Undo::Type::CONFIGURE:
            {
                const auto undoConfigureObject = static_cast<UndoConfigureType*>(undoObject);
                switch (undoConfigureObject->Data()->Type())
                {
                    case Undo::ConfigType::CONNECTION_TYPE:
                    {
                        auto data = std::static_pointer_cast<Undo::ConnectionTypeChangedData>(undoConfigureObject->Data());
                        Q_ASSERT(data->conPoint);
                        data->conPoint->SetConnectionType(data->previousType);
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
    }
    mView.Scene()->clearSelection();
    mView.SetUndoRedoButtonsEnableState();
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
            case Undo::Type::ADD:
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
            case Undo::Type::DEL:
            {
                for (const auto& comp : static_cast<UndoDeleteType*>(redoObject)->Components())
                {
                    Q_ASSERT(comp);
                    mView.Scene()->removeItem(comp);
                }
                AppendToUndoQueue(redoObject, mUndoQueue);
                break;
            }
            case Undo::Type::MOVE:
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
            case Undo::Type::CONFIGURE:
            {
                const auto undoConfigureObject = static_cast<UndoConfigureType*>(redoObject);
                switch (undoConfigureObject->Data()->Type())
                {
                    case Undo::ConfigType::CONNECTION_TYPE:
                    {
                        auto data = std::static_pointer_cast<Undo::ConnectionTypeChangedData>(undoConfigureObject->Data());
                        Q_ASSERT(data->conPoint);
                        data->conPoint->SetConnectionType(data->currentType);
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
    }
    mView.Scene()->clearSelection();
    mView.SetUndoRedoButtonsEnableState();
}
