QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

HEADERS += \
    Components/BaseComponent.h \
    Components/Gates/AbstractGate.h \
    Components/Gates/AndGate.h \
    Components/Gates/NotGate.h \
    Components/Gates/OrGate.h \
    Components/Gates/XorGate.h \
    Components/LogicWire.h \
    Components/RectComponent.h \
    CoreLogic.h \
    HelperFunctions.h \
    HelperStructures.h \
    MainWindow.h \
    Undo/UndoAddType.h \
    Undo/UndoBaseType.h \
    Undo/UndoConfigureType.h \
    Undo/UndoDeleteType.h \
    Undo/UndoMoveType.h \
    View/View.h \
    Configuration.h

SOURCES += \
    Components/BaseComponent.cpp \
    Components/Gates/AbstractGate.cpp \
    Components/Gates/AndGate.cpp \
    Components/Gates/NotGate.cpp \
    Components/Gates/OrGate.cpp \
    Components/Gates/XorGate.cpp \
    Components/LogicWire.cpp \
    Components/RectComponent.cpp \
    CoreLogic.cpp \
    Undo/UndoAddType.cpp \
    Undo/UndoBaseType.cpp \
    Undo/UndoConfigureType.cpp \
    Undo/UndoDeleteType.cpp \
    Undo/UndoMoveType.cpp \
    View/EventHandling.cpp \
    View/GuiHandling.cpp \
    MainWindow.cpp \
    View/View.cpp \
    main.cpp

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
