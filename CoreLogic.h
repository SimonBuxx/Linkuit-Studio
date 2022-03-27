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

///
/// \brief The CoreLogic class contains all business logic
///
class CoreLogic : public QObject
{
    Q_OBJECT
public:
    /// \brief Constructor for CoreLogic
    /// \param pView: The view that contains the main scene
    CoreLogic(View &pView);

    /// \brief Creates a new scene component
    /// \return Pointer to the new component
    IBaseComponent* GetItem(void);

    void EnterControlMode(ControlMode pMode);

    ControlMode GetControlMode(void);

    void SelectComponentType(ComponentType pComponentType);

    ComponentType GetSelectedComponentType(void) const;

    void EnterAddControlMode(ComponentType pComponentType);

    /// \brief Adds a component of the currently selected type to the scene at position pPosition
    /// if there is no collision with existing components
    /// \param pPosition: The position to add the component at
    /// \return False, if the action has been aborted
    bool AddCurrentTypeComponent(QPointF pPosition);

    void SetComponentInputCount(uint8_t pCount);

    void SetComponentInputDirection(Direction pDirection);

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

    /// \brief Returns true, if the core logic is in simulation mode
    /// \return True, if in simulation mode
    bool IsSimulationRunning(void) const;

    // Functions for undo and redo

    /// \brief Returns true, if the undo queue has no elements
    /// \return True, if the undo queue is empty
    bool IsUndoQueueEmpty(void) const;

    /// \brief Returns true, if the redo queue has no elements
    /// \return True, if the redo queue is empty
    bool IsRedoQueueEmpty(void) const;

    /// \brief Undos the last logged undo action if existant
    void Undo(void);

    /// \brief Redos the last undone undo action if existant
    void Redo(void);

    bool IsProcessing(void);

signals:
    void ControlModeChangedSignal(ControlMode pNewMode);

    void ComponentTypeChangedSignal(ComponentType pNewType);

    void MousePressedEventDefaultSignal(QMouseEvent &pEvent);

    void SimulationStartSignal(void);

    void SimulationAdvanceSignal(void);

    void SimulationStopSignal(void);

public slots:
    void OnSelectedComponentsMoved(QPointF pOffset);

    void OnLeftMouseButtonPressedWithoutCtrl(QPointF pMappedPos, QMouseEvent &pEvent);

    // Slots for configuration events
    void OnConnectionTypeChanged(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType);

    void OnTextLabelContentChanged(TextLabel* pTextLabel, QString pPreviousText, QString pCurrentText);

    /// \brief Advances the simulation by one step
    void OnPropagationTimeout(void);

    void OnProcessingTimeout(void);

    /// \brief Checks if a special tab should be displayed
    void SelectionChanged(void);

protected:
    /// \brief Connects to the View object via signals and slots
    void ConnectToView(void);

    /// \brief Performs all neccessary steps to enter simulation mode
    void StartSimulation(void);

    // Functions for wire processing

    /// \brief Returns a vector of components that only contains the wires found in pComponents, facing in pDirection
    /// \param pComponents: The list of components to filter for logic wires
    /// \param pDirection: The wire direction to filter for, or UNSET for all wires
    /// \return A vector containing all logic wires from pComponents
    std::vector<IBaseComponent*> FilterForWires(const QList<QGraphicsItem*> &pComponents, WireDirection pDirection = WireDirection::UNSET) const;

    LogicWire* MergeWires(LogicWire* pNewWire, LogicWire* pLeftTopAdjacent, LogicWire* pRightBottomAdjacent) const;

    std::vector<IBaseComponent*> DeleteContainedWires(LogicWire* pWire);

    LogicWire* GetAdjacentWire(QPointF pCheckPosition, WireDirection pDirection) const;

    void MergeWiresAfterMove(std::vector<LogicWire*> &pComponents, std::vector<IBaseComponent*> &pAddedWires, std::vector<IBaseComponent*> &pDeletedWires);

    bool ManageConPointsOneStep(IBaseComponent* comp, QPointF& pOffset, std::vector<IBaseComponent*>& movedComponents,
                                           std::vector<IBaseComponent*>& addedComponents, std::vector<IBaseComponent*>& deletedComponents);

    bool IsTCrossing(const LogicWire* pWire1, const LogicWire* pWire2) const;

    /// \brief Determines whether there is a T- or X-crossing below pConPoint
    /// \param pConPoint: The ConPoint to take the position from
    /// \return True, if there is no T- or X-crossing of wires below pConPoint
    bool IsNoCrossingPoint(const ConPoint* pConPoint) const;

    /// \brief Determines whether there is an X-crossing at the given point
    /// \param pPoint: The point to check
    /// \return True, if there are two wires in pPoint crossing each other
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

    /// \brief Checks if the given item is a "colliding" component (may not be before or behind others)
    /// \param pComponent: The item to check
    /// \return True, if pComponent is a "colliding" component
    bool IsCollidingComponent(QGraphicsItem* pComponent) const;

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

    // Functions for handling long processes (copy of large selections, etc.)
    // These keep the GUI responsive, display a loading screen and block all user interaction
    // Cheap alternative to multi-threading, because QGraphicsItems cannot be accessed in other threads

    /// \brief Called at the start of a longer process to keep the GUI responsive
    void StartProcessing(void);

    /// \brief Handles events while processing, call periodically during longer processes
    void ProcessingHeartbeat(void);

    /// \brief Called when the longer process has been finished
    void EndProcessing(void);

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

    QTimer mPropagationTimer; // Main timer to set the propagation delay

    // Undo and redo queues
    std::deque<UndoBaseType*> mUndoQueue;
    std::deque<UndoBaseType*> mRedoQueue;

    QTimer mProcessingTimer;

    bool mIsProcessing = false;
};

#endif // CORELOGIC_H
