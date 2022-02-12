HEADERS += \
    $$PWD/Components/ConPoint.h \
    $${PWD}/Components/BaseComponent.h \
    $${PWD}/Components/Gates/AbstractGate.h \
    $${PWD}/Components/Gates/AndGate.h \
    $${PWD}/Components/Gates/NotGate.h \
    $${PWD}/Components/Gates/OrGate.h \
    $${PWD}/Components/Gates/XorGate.h \
    $${PWD}/Components/LogicInput.h \
    $${PWD}/Components/LogicOutput.h \
    $${PWD}/Components/LogicWire.h \
    $${PWD}/Components/RectComponent.h \
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
    $${PWD}/Components/BaseComponent.cpp \
    $${PWD}/Components/Gates/AbstractGate.cpp \
    $${PWD}/Components/Gates/AndGate.cpp \
    $${PWD}/Components/Gates/NotGate.cpp \
    $${PWD}/Components/Gates/OrGate.cpp \
    $${PWD}/Components/Gates/XorGate.cpp \
    $${PWD}/Components/LogicInput.cpp \
    $${PWD}/Components/LogicOutput.cpp \
    $${PWD}/Components/LogicWire.cpp \
    $${PWD}/Components/RectComponent.cpp \
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
