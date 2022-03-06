HEADERS += \
    $$PWD/Components/ConPoint.h \
    $$PWD/Components/Gates/LogicCells/LogicAndGateCell.h \
    $$PWD/Components/Inputs/LogicButton.h \
    $$PWD/Components/Inputs/LogicCells/LogicButtonCell.h \
    $$PWD/Components/LogicBaseCell.h \
    $$PWD/Components/Inputs/LogicCells/LogicInputCell.h \
    $$PWD/Components/Gates/LogicCells/LogicNotGateCell.h \
    $$PWD/Components/Gates/LogicCells/LogicOrGateCell.h \
    $$PWD/Components/Outputs/LogicCells/LogicOutputCell.h \
    $$PWD/Components/LogicWireCell.h \
    $$PWD/Components/Gates/LogicCells/LogicXorGateCell.h \
    $$PWD/Components/TextLabel.h \
    $${PWD}/Components/IBaseComponent.h \
    $${PWD}/Components/Gates/AbstractGate.h \
    $${PWD}/Components/Gates/AndGate.h \
    $${PWD}/Components/Gates/NotGate.h \
    $${PWD}/Components/Gates/OrGate.h \
    $${PWD}/Components/Gates/XorGate.h \
    $${PWD}/Components/Inputs/LogicInput.h \
    $${PWD}/Components/Outputs/LogicOutput.h \
    $${PWD}/Components/LogicWire.h \
    $${PWD}/CoreLogic.h \
    $${PWD}/HelperFunctions.h \
    $${PWD}/HelperStructures.h \
    $${PWD}/MainWindow.h \
    $${PWD}/Undo/UndoAddType.h \
    $${PWD}/Undo/UndoBaseType.h \
    $${PWD}/Undo/UndoConfigureType.h \
    $${PWD}/Undo/UndoDeleteType.h \
    $${PWD}/Undo/UndoMoveType.h \
    $${PWD}/View/View.h \
    $${PWD}/Configuration.h

SOURCES += \
    $$PWD/Components/ConPoint.cpp \
    $$PWD/Components/Gates/LogicCells/LogicAndGateCell.cpp \
    $$PWD/Components/Inputs/LogicButton.cpp \
    $$PWD/Components/Inputs/LogicCells/LogicButtonCell.cpp \
    $$PWD/Components/LogicBaseCell.cpp \
    $$PWD/Components/Inputs/LogicCells/LogicInputCell.cpp \
    $$PWD/Components/Gates/LogicCells/LogicNotGateCell.cpp \
    $$PWD/Components/Gates/LogicCells/LogicOrGateCell.cpp \
    $$PWD/Components/Outputs/LogicCells/LogicOutputCell.cpp \
    $$PWD/Components/LogicWireCell.cpp \
    $$PWD/Components/Gates/LogicCells/LogicXorGateCell.cpp \
    $$PWD/Components/TextLabel.cpp \
    $${PWD}/Components/IBaseComponent.cpp \
    $${PWD}/Components/Gates/AbstractGate.cpp \
    $${PWD}/Components/Gates/AndGate.cpp \
    $${PWD}/Components/Gates/NotGate.cpp \
    $${PWD}/Components/Gates/OrGate.cpp \
    $${PWD}/Components/Gates/XorGate.cpp \
    $${PWD}/Components/Inputs/LogicInput.cpp \
    $${PWD}/Components/Outputs/LogicOutput.cpp \
    $${PWD}/Components/LogicWire.cpp \
    $${PWD}/CoreLogic.cpp \
    $${PWD}/Undo/UndoAddType.cpp \
    $${PWD}/Undo/UndoBaseType.cpp \
    $${PWD}/Undo/UndoConfigureType.cpp \
    $${PWD}/Undo/UndoDeleteType.cpp \
    $${PWD}/Undo/UndoMoveType.cpp \
    $${PWD}/View/EventHandling.cpp \
    $${PWD}/View/GuiHandling.cpp \
    $${PWD}/MainWindow.cpp \
    $${PWD}/View/View.cpp

INCLUDEPATH += $${PWD}
