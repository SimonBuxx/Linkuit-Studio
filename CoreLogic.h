#ifndef CORELOGIC_H
#define CORELOGIC_H

#include "View/View.h"
#include "Undo/UndoBaseType.h"
#include "Components/LogicWire.h"
#include "Components/ConPoint.h"
#include "Components/TextLabel.h"
#include "Components/LogicWireCell.h"
#include "Configuration.h"

#include <QGraphicsItem>
#include <QTimer>
#include <deque>

class View;

class CoreLogic : public QObject
{
    Q_OBJECT
public:
    CoreLogic(View &pView);

    IBaseComponent* GetItem(void);

    void EnterControlMode(ControlMode pMode);
    ControlMode GetControlMode(void);

    void SetComponentType(ComponentType pComponentType);
    ComponentType GetComponentType(void) const;

    void EnterAddControlMode(ComponentType pComponentType);

    void AddCurrentTypeComponent(QPointF pPosition);

    void SetPreviewWireStart(QPointF pStartPoint);

    /// \brief Draws preview wires to the current mouse position
    /// \param pCurrentPoint: The current mouse position
    void ShowPreviewWires(QPointF pCurrentPoint);

    /// \brief Adds wires from mPreviewWireStart to pEndPoint, with the first wire being
    /// added in mWireStartDirection direction, if two wires are added
    /// \param pEndPoint: The end point the last drawn wire should reach
    void AddWires(QPointF pEndPoint);

    void CopySelectedComponents(void);
    void DeleteSelectedComponents(void);

    bool IsSimulationRunning(void) const;

    bool IsUndoQueueEmpty(void) const;
    bool IsRedoQueueEmpty(void) const;

    void Undo(void);
    void Redo(void);

signals:
    void ControlModeChangedSignal(ControlMode pNewMode);
    void ComponentTypeChangedSignal(ComponentType pNewType);
    void MousePressedEventDefaultSignal(QMouseEvent &pEvent);

    void SimulationStartSignal(void);
    void SimulationAdvanceSignal(void);
    void SimulationStopSignal(void);

public slots:
    void OnSelectedComponentsMoved(QPointF pOffset);
    void OnLeftMouseButtonPressed(QPointF pMappedPos, QMouseEvent &pEvent);

    // Slots for configuration events
    void OnConnectionTypeChanged(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType);
    void OnTextLabelContentChanged(TextLabel* pTextLabel, QString pPreviousText, QString pCurrentText);

    void OnPropagationTimeout(void);

protected:
    void ConnectToView(void);

    // Functions for wire processing

    std::vector<IBaseComponent*> FilterForWires(const QList<QGraphicsItem*> &pComponents, WireDirection pDirection = WireDirection::UNSET) const;

    LogicWire* MergeWires(LogicWire* pNewWire, LogicWire* pLeftTopAdjacent, LogicWire* pRightBottomAdjacent) const;
    std::vector<IBaseComponent*> DeleteContainedWires(LogicWire* pWire);
    LogicWire* GetAdjacentWire(QPointF pCheckPosition, WireDirection pDirection) const;
    void MergeWiresAfterMove(std::vector<LogicWire*> &pComponents, std::vector<IBaseComponent*> &pAddedWires, std::vector<IBaseComponent*> &pDeletedWires);

    bool IsTCrossing(const LogicWire* pWire1, const LogicWire* pWire2) const;
    bool IsNoCrossingPoint(const ConPoint* pConPoint) const;
    bool IsXCrossingPoint(QPointF pPoint) const;

    // Functions for component retreival

    /// \brief Checks if a component of the given type T collides with the given position
    /// \param pPos: The position to check
    /// \return True, if a component of type T is found at pPos
    template<typename T>
    bool IsComponentAtPosition(QPointF pPos);

    /// \brief Returns all components that are "colliding" (may not be before or behind others) with the given component
    /// \param pComponent: The component to check for colliding components
    /// \return A vector of components that are colliding with pComponent
    std::vector<IBaseComponent*> GetCollidingComponents(IBaseComponent* pComponent) const;

    /// \brief Checks if the given component is a "colliding" component (may not be before or behind others)
    /// \param pComponent: The component to check
    /// \return True, if pComponent is a "colliding" component
    bool IsCollidingComponent(IBaseComponent* pComponent) const;

    /// \brief Groups all existing wires into groups that are connected, meaning they always have the same logic state
    void ParseWireGroups(void);

    /// \brief Looks for wires connecting to pWire and recursively includes them in the group
    /// \param pWire: The root wire of that the group should be explored
    /// \param pGroupIndex: The index of a group in mWireGroups
    void ExploreGroup(LogicWire* pWire, int32_t pGroupIndex);

    /// \brief Checks if the two given wires form an L-type crossing
    /// \param pWireA: The first wire
    /// \param pWireB: The second wire
    /// \return True, if the wires form an L-type crossing
    bool IsLCrossing(LogicWire* pWireA, LogicWire* pWireB) const;

    /// \brief Gets the collision point of two wires of different direction, assuming they do in fact collide
    /// \param pWireA: The first wire
    /// \param pWireB: The second wire
    /// \return The collision point of the wires
    QPointF GetWireCollisionPoint(const LogicWire* pWireA, const LogicWire* pWireB) const;

    /// \brief Checks if a ConPoint of the given type collides with the given position
    /// \param pPos: The position to check
    /// \param pType: The connection type to look for
    /// \return A pointer to the ConPoint, if a ConPoint is found at pPos with connection type pType, nullptr otherwise
    ConPoint* GetConPointAtPosition(QPointF pPos, ConnectionType pType) const;

    void CreateWireLogicCells(void);
    void ConnectLogicCells(void);

    // Functions for undo and redo

    void AppendUndo(UndoBaseType* pUndoObject);
    void AppendToUndoQueue(UndoBaseType* pUndoObject, std::deque<UndoBaseType*> &pQueue);

protected:
    View &mView;

    // Variables for general states and modes
    ControlMode mControlMode = ControlMode::EDIT;
    ComponentType mComponentType = ComponentType::NONE;
    Direction mComponentDirection = components::DEFAULT_DIRECTION;
    uint8_t mComponentInputCount = components::gates::DEFAULT_INPUT_COUNT;

    // Variables for wire insertion
    QPointF mPreviewWireStart;
    WireDirection mWireStartDirection = WireDirection::HORIZONTAL;
    LogicWire mHorizontalPreviewWire;
    LogicWire mVerticalPreviewWire;

    // Variables for wire parsing (grouping)
    std::vector<std::vector<IBaseComponent*>> mWireGroups;
    std::map<LogicWire*, int32_t> mWireMap; // Contains wire pointers as keys and group indizes as values

    std::vector<std::shared_ptr<LogicWireCell>> mLogicWireCells;

    QTimer mPropagationTimer;

    // Undo and redo queues
    std::deque<UndoBaseType*> mUndoQueue;
    std::deque<UndoBaseType*> mRedoQueue;
};

#endif // CORELOGIC_H
