#ifndef CORELOGIC_H
#define CORELOGIC_H

#include "View.h"
#include "Undo/UndoBaseType.h"
#include "Undo/UndoCopyType.h"
#include "Components/LogicWire.h"
#include "Components/ConPoint.h"
#include "Components/TextLabel.h"
#include "Components/LogicWireCell.h"
#include "Configuration.h"
#include "RuntimeConfigParser.h"
#include "CircuitFileParser.h"

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

    /// \brief Getter for the runtime config parser object
    /// \return Reference to the runtime config parser
    const RuntimeConfigParser& GetRuntimeConfigParser(void) const;

    /// \brief Getter for the circuit file parser object
    /// \return Reference to the circuit file parser
    CircuitFileParser& GetCircuitFileParser(void);

    /// \brief Updates whether the welcome dialog should be shown on software startup
    /// \param pShowOnStartup: If true, the welcome dialog will be shown on next startup
    void SetShowWelcomeDialogOnStartup(bool pShowOnStartup);

    /// \brief Selects all items in the scene
    void SelectAll(void);

    /// \brief Creates a new scene component
    /// \return Optional pointer to the new component
    std::optional<IBaseComponent*> GetItem(void) const;

    /// \brief Changes the control mode to the given mode and kicks off all
    /// neccessary steps to enter this mode
    /// \param pNewMode: The new control mode
    void EnterControlMode(ControlMode pNewMode);

    /// \brief Getter for the current control mode (EDIT, ADD, etc.)
    /// \return the current control mode
    ControlMode GetControlMode(void) const;

    /// \brief Getter for the current simulation mode (RUNNING or STOPPED)
    /// \return the current simulation mode
    SimulationMode GetSimulationMode(void) const;

    /// \brief Changes the type of component to add to the given type
    /// \param pComponentType: The component type of which to add new instances
    void SelectComponentType(ComponentType pComponentType);

    /// \brief Getter for the currently selected component type
    /// \return the currently selected component type
    ComponentType GetSelectedComponentType(void) const;

    /// \brief Changes the current control mode to ADD and selects the given component type
    /// \param pComponentType: The component type of which to add new instances
    void EnterAddControlMode(ComponentType pComponentType);

    /// \brief Adds a component of the currently selected type to the scene at position pPosition
    /// if there is no collision with existing components
    /// \param pPosition: The position to add the component at
    /// \return False, if the action has been aborted
    bool AddCurrentTypeComponent(QPointF pPosition);

    /// \brief Setter for the amount of inputs for gates with variable input count
    /// \param pCount: The amount of inputs to use for new gates
    void SetGateInputCount(uint8_t pCount);

    /// \brief Setter for the amount of inputs for en-/decoders
    /// \param pCount: The amount of inputs to use for new en-/decoders
    void SetEncoderDecoderInputCount(uint8_t pCount);

    /// \brief Setter for the component direction for components with variable direction
    /// \param pDirection: The new component direction (output-side of the component)
    void SetComponentDirection(Direction pDirection);

    /// \brief Setter for the (de-)multiplexer bit width
    /// \param pBitWidth: The new (de-)multiplexer bit width
    void SetMultiplexerBitWidth(uint8_t pBitWidth);

    /// \brief Setter for the shift register bit width
    /// \param pBitWidth: The new shift register bit width
    void SetShiftRegisterBitWidth(uint8_t pBitWidth);

    /// \brief Setter for the constant input state
    /// \param pState: The new constant input state
    void SetConstantState(LogicState pState);

    /// \brief Setter for the counter bit width
    /// \param pBitWidth: The new counter bit width
    void SetCounterBitWidth(uint8_t pBitWidth);

    /// \brief Setter for the flip-flop style
    /// \param pStyle: The new flip-flop style
    void SetFlipFlopStyle(FlipFlopStyle pStyle);

    /// \brief Sets the starting point of the preview wires to the given point and adds them to the scene
    /// \param pStartPoint: The point where the new wire drawing action started
    void SetPreviewWireStart(QPointF pStartPoint);

    /// \brief Draws preview wires to the current mouse position
    /// \param pCurrentPoint: The current mouse position
    void ShowPreviewWires(QPointF pCurrentPoint);

    /// \brief Adds wires from mPreviewWireStart to pEndPoint, with the first wire being
    /// added in mWireStartDirection direction, if two wires are added
    /// \param pEndPoint: The end point the last drawn wire should reach
    void AddWires(QPointF pEndPoint);

    /// \brief Copies the currently selected components
    void CopySelectedComponents(void);

    /// \brief Pastes the currently copied components
    void PasteCopiedComponents(void);

    /// \brief Cuts the currently selected components
    void CutSelectedComponents(void);

    /// \brief Deletes the currently selected components
    void DeleteSelectedComponents(void);

    /// \brief Either aborts or completes the current copy/paste action, depending on collision
    void FinishPaste(void);

    /// \brief Aborts the current copy/paste action
    void AbortPastingIfInCopy(void);

    /// \brief Returns true, if the core logic is in simulation mode
    /// \return True, if in simulation mode
    bool IsSimulationRunning(void) const;

    /// \brief Returns whether the software is currently processing or loading
    /// \return True, if the software is currently processing or loading
    bool IsProcessing(void) const;

    // Functions for simulation control

    /// \brief Starts the simulation if in simulation mode and the simulation is not yet started
    void RunSimulation(void);

    /// \brief Pauses the simulation if it is currently running
    void PauseSimulation(void);

    /// \brief Resets the simulation if in simulation mode
    void ResetSimulation(void);

    /// \brief Advances the simulation by one tick if in simulation mode
    void StepSimulation(void);

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

    // Functions for item configuration
    /// \brief Sets the toggle value of the selected clock to the new value
    /// \param pValue: The new toggle value
    void OnToggleValueChanged(uint32_t pValue);

    /// \brief Sets the pulse value of the selected clock to the new value
    /// \param pValue: The new pulse value
    void OnPulseValueChanged(uint32_t pValue);

    /// \brief Sets the clock mode of the selected clock to the new mode
    /// \param pMode: The new clock mode
    void OnClockModeChanged(ClockMode pMode);

    // Functions for saving and loading

    /// \brief Returns a JSON representation of the current circuit
    /// \return A JSON object containing the circuit information
    QJsonObject GetJson(void) const;

    /// \brief Resets the canvas
    void NewCircuit(void);

    // ////////////////////////////

signals:
    /// \brief Emitted when the current control mode changes
    /// \param pNewMode: The newly entered control mode
    void ControlModeChangedSignal(ControlMode pNewMode);

    /// \brief Emitted when the current simulation mode changes
    /// \param pNewMode: The newly entered simulation mode
    void SimulationModeChangedSignal(SimulationMode pNewMode);

    /// \brief Emitted when the selected component type for new components changes
    /// \param pNewType: The newly selected component type
    void ComponentTypeChangedSignal(ComponentType pNewType);

    /// \brief Emitted when a longer process has been started and the GUI needs to be disabled
    void ProcessingStartedSignal(void);

    /// \brief Emitted when a longer process has been ended and the GUI needs to be re-enabled
    void ProcessingEndedSignal(void);

    /// \brief Emitted to finish the given QMouseEvent in the GraphicsView
    /// \param pEvent: The mouse press event to finish
    void MousePressedEventDefaultSignal(QMouseEvent &pEvent);

    /// \brief Emitted when the core logic has started the simulation
    void SimulationStartSignal(void);

    /// \brief Emitted when the simulation should advance by one step
    void SimulationAdvanceSignal(void);

    /// \brief Emitted when the core logic has stopped the simulation
    void SimulationStopSignal(void);

    /// \brief Emitted when the undo/redo button enabled state should be updated
    void UpdateUndoRedoEnabledSignal(void);

    /// \brief Emitted when the clock configurator should be displayed with the given settings
    /// \param pMode: The clock mode (either toggle or pulse)
    /// \param pToggle: The toggle speed
    /// \param pPulse: The pulse duration
    void ShowClockConfiguratorSignal(ClockMode pMode, uint32_t pToggle, uint32_t pPulse);

    /// \brief Emitted when the clock configurator should be hidden
    void HideClockConfiguratorSignal(void);

public slots:
    /// \brief Checks for collisions, merges moved or copied wires and brings the ConPoints in a valid state
    /// \param pOffset: The relative offset by which the selected components have been moved
    void OnSelectedComponentsMovedOrPasted(QPointF pOffset);

    /// \brief Performs operations such as adding ConPoints, inversion circles and components on click
    /// May emit MousePressedEventDefaultSignal to pass the press event back to the GraphicsView
    /// (e.g. to selecting, dragging, etc. if no action was executed instead)
    /// \param pMappedPos: The position in the scene where the mouse press happened
    /// \param pEvent: The mouse press event to pass back to MousePressedEventDefaultSignal
    void OnLeftMouseButtonPressedWithoutCtrl(QPointF pMappedPos, QMouseEvent &pEvent);

    /// \brief Forwards the request to show the clock configurator GUI overlay
    /// \param pMode: The clock mode (either toggle or pulse)
    /// \param pToggle: The toggle speed
    /// \param pPulse: The pulse duration
    void OnShowClockConfiguratorRequest(ClockMode pMode, uint32_t pToggle, uint32_t pPulse);

    // Slots for configuration events

    /// \brief Invoked by pConPoint when its connection type changed; creates undo/redo object
    /// \param pConPoint: Pointer to the affected ConPoint
    /// \param pPreviousType: The previous connection type
    /// \param pCurrentType: The new connection type
    void OnConnectionTypeChanged(ConPoint* pConPoint, ConnectionType pPreviousType, ConnectionType pCurrentType);

    /// \brief Invoked by pTextLabel when its text content changed; creates undo/redo object
    /// \param pTextLabel: Pointer to the affected TextLabel
    /// \param pPreviousText: The previous text content
    /// \param pCurrentText: The new text content
    void OnTextLabelContentChanged(TextLabel* pTextLabel, const QString& pPreviousText, const QString& pCurrentText);

    // ///////////////////////////////

    /// \brief Advances the simulation by one step; invoked by mPropagationTimer
    void OnPropagationTimeout(void);

    /// \brief Displays the processing overlay (loading screen); invoked by mProcessingTimer
    void OnProcessingTimeout(void);

protected:
    /// \brief Performs all neccessary steps to enter simulation mode
    void EnterSimulation(void);

    /// \brief Performs all neccessary steps to leave simulation mode
    void LeaveSimulation(void);

    /// \brief Sets the current simulation mode to the given mode and emits the change signal
    /// \param pNewMode: The new simulation mode to go into
    void SetSimulationMode(SimulationMode pNewMode);

    /// \brief Removes all components added in the current copy/paste action
    void RemoveCurrentPaste(void);

    // Functions for wire processing

    /// \brief Returns a vector of components that only contains the wires found in pComponents, facing in pDirection
    /// \param pComponents: The list of components to filter for logic wires
    /// \param pDirection: The wire direction to filter for, or UNSET for all wires
    /// \return A vector containing all logic wires from pComponents
    std::vector<IBaseComponent*> FilterForWires(const QList<QGraphicsItem*> &pComponents, WireDirection pDirection = WireDirection::UNSET) const;

    /// \brief Merges pNewWire and the given adjacent wires into a new wire
    /// \param pNewWire: The new wire that has to be merged with adjacent wires
    /// \param pLeftTopAdjacent: The first (left or top) adjacent wire
    /// \param pRightBottomAdjacent: The second (right or bottom) adjacent wire
    /// \return A new LogicWire to replace all of the given wires
    LogicWire* MergeWires(LogicWire* pNewWire, std::optional<LogicWire*> pLeftTopAdjacent, std::optional<LogicWire*> pRightBottomAdjacent) const;

    /// \brief Removes all wires from the scene that are completely contained in the given wire
    /// \param pWire: The wire to look for contained wires below
    /// \return A vector containing pointers to all deleted wires
    std::vector<LogicWire*> DeleteContainedWires(const LogicWire* pWire);

    /// \brief Searches a wire at the given position that has the given direction
    /// \param pCheckPosition: The position to check at
    /// \param pDirection: The direction to look for
    /// \return A pointer to the wire, if it has been found, or std::nullopt
    std::optional<LogicWire*> GetAdjacentWire(QPointF pCheckPosition, WireDirection pDirection) const;

    /// \brief Merges all wires that have been moved if they are now adjacent with or overlapping others
    /// \param pWires: The wires that have been moved
    /// \param pAddedComponents: Vector to add newly created wires to
    /// \param pDeletedComponents: Vector to add deleted wires to
    void MergeWiresAfterMove(const std::vector<LogicWire*> &pWires, std::vector<IBaseComponent*> &pAddedComponents, std::vector<IBaseComponent*> &pDeletedComponents);

    /// \brief One step of the algorithm to create a valid ConPoint state in OnSelectedComponentsMovedOrPasted
    /// \param pComponent: The component that currently processed
    /// \param pOffset: The offset by that the component has been moved
    /// \param pMovedComponents: Vector to add moved components to
    /// \param pAddedComponents: Vector to add newly added components to
    /// \param pDeletedComponents: Vector to add deleted components to
    /// \return False, if the move operation has to be aborted
    bool ManageConPointsOneStep(IBaseComponent* pComponent, QPointF& pOffset, std::vector<IBaseComponent*>& pMovedComponents,
                                           std::vector<IBaseComponent*>& pAddedComponents, std::vector<IBaseComponent*>& pDeletedComponents);

    /// \brief Adds ConPoints on T-crossings that include the given wire
    /// \param pWire: The wire to add ConPoints to
    /// \param addedComponents: Reference to the vector to add the ConPoints to
    void AddConPointsToTCrossings(LogicWire* pWire, std::vector<IBaseComponent*>& addedComponents);

    // Functions to check for wire crossings and ConPoint positions

    /// \brief Checks whether the two given wires form a T-crossing
    /// \param pWire1: Pointer to the first wire
    /// \param pWire2: Pointer to the second wire
    /// \return True, if both wires form a T-crossing
    bool IsTCrossing(const LogicWire* pWire1, const LogicWire* pWire2) const;

    /// \brief Determines whether there is a T- or X-crossing below pConPoint
    /// \param pConPoint: The ConPoint to take the position from
    /// \return True, if there is no T- or X-crossing of wires below pConPoint
    bool IsNoCrossingPoint(const ConPoint* pConPoint) const;

    /// \brief Determines whether there is an X-crossing at the given point
    /// \param pPoint: The point to check
    /// \return True, if there are two wires in pPoint crossing each other
    bool IsXCrossingPoint(QPointF pPoint) const;

    /// \brief Checks if the two given wires form an L-type crossing
    /// \param pWireA: The first wire
    /// \param pWireB: The second wire
    /// \return True, if the wires form an L-type crossing
    bool IsLCrossing(LogicWire* pWireA, LogicWire* pWireB) const;

    /// \brief Gets the collision point of two wires of different direction, assuming they do in fact collide
    /// \param pWireA: The first wire
    /// \param pWireB: The second wire
    /// \return The collision point of the wires (optional)
    std::optional<QPointF> GetWireCollisionPoint(const LogicWire* pWireA, const LogicWire* pWireB) const;

    /// \brief Checks if a ConPoint of the given type collides with the given position
    /// \param pPos: The position to check
    /// \param pType: The connection type to look for
    /// \return A pointer to the ConPoint (optional)
    std::optional<ConPoint*> GetConPointAtPosition(QPointF pPos, ConnectionType pType) const;

    // Functions for component retreival

    /// \brief Checks if a component of the given type T collides with the given position
    /// \param pPos: The position to check
    /// \return True, if a component of type T is found at pPos
    template<typename T>
    bool IsComponentAtPosition(QPointF pPos);

    /// \brief Checks if there are two ConPoints at the given position pPos
    /// \param pPos: The position to check
    /// \return True, if there are exactly two ConPoints at pPos
    bool TwoConPointsAtPosition(QPointF pPos);

    /// \brief Returns all components that are "colliding" (may not be before or behind others) with the given component
    /// \param pComponent: The component to check for colliding components
    /// \param pOnlyUnselected: If true, selected colliding components are ignored
    /// \return A vector of components that are colliding with pComponent
    std::vector<IBaseComponent*> GetCollidingComponents(IBaseComponent* pComponent, bool pOnlyUnselected) const;

    /// \brief Checks if the given item is a "colliding" component (may not be before or behind others)
    /// \param pComponent: The item to check
    /// \return True, if pComponent is a "colliding" component
    bool IsCollidingComponent(QGraphicsItem* pComponent) const;

    // Functions for entering simulation mode (parsing, setting up logic cells etc.)

    /// \brief Groups all existing wires into groups that are connected, meaning they always have the same logic state
    void ParseWireGroups(void);

    /// \brief Looks for wires connecting to pWire and recursively includes them in the group
    /// \param pWire: The root wire of that the group should be explored
    /// \param pGroupIndex: The index of a group in mWireGroups
    void ExploreGroup(LogicWire* pWire, int32_t pGroupIndex);

    /// \brief Creates logic cells for wire groups, including full ConPoints
    void CreateWireLogicCells(void);

    /// \brief Connects all logic cells based on their connector positions
    void ConnectLogicCells(void);

    // Functions for undo and redo

    /// \brief Appends the given undo/redo object to the undo queue and clears the redo queue
    /// \param pUndoObject: The object to append
    void AppendUndo(UndoBaseType* pUndoObject);

    /// \brief Appends the given undo/redo object to the given queue
    /// \param pUndoObject: The object to append
    /// \param pQueue: The queue to append to object to
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

    /// \brief Unselects all items in the scene
    void ClearSelection(void);

    // Functions for loading and saving
    /// \brief Loads a circuit from the given JSON object
    /// \param pJson: The JSON data of the circuit to load
    void ReadJson(const QJsonObject& pJson);

    /// \brief Creates a circuit component using the provided JSON data
    /// \param pJson: The JSON data of the circuit component
    /// \return True, if the component has been added
    bool CreateComponent(const QJsonObject& pJson);

protected:
    View &mView;

    // Variables for general states and modes
    ControlMode mControlMode = ControlMode::EDIT;
    SimulationMode mSimulationMode = SimulationMode::STOPPED;
    ComponentType mComponentType = ComponentType::NONE;
    Direction mComponentDirection = components::DEFAULT_DIRECTION;
    uint8_t mGateInputCount = components::gates::DEFAULT_INPUT_COUNT;
    uint8_t mEncoderDecoderInputCount = components::encoder_decoder::DEFAULT_INPUT_COUNT;
    uint8_t mMultiplexerBitWidth = components::multiplexer::DEFAULT_BIT_WIDTH;
    uint8_t mShiftRegisterBitWidth = components::shift_register::DEFAULT_BIT_WIDTH;
    LogicState mConstantState = components::inputs::constant::DEFAULT_STATE;
    uint8_t mCounterBitWidth = components::counter::DEFAULT_BIT_WIDTH;
    FlipFlopStyle mFlipFlopStyle = FlipFlopStyle::LATCH;

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

    std::vector<IBaseComponent*> mCopiedComponents;
    std::vector<IBaseComponent*> mCurrentPaste;
    std::optional<UndoCopyType*> mCurrentCopyUndoType;

    CircuitFileParser mCircuitFileParser;
    RuntimeConfigParser mRuntimeConfigParser;
};

#endif // CORELOGIC_H
