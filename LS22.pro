QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

HEADERS += \
    Components/Gates/AbstractGate.h \
    Components/Gates/AndGate.h \
    Components/Gates/OrGate.h \
    Components/Gates/XorGate.h \
    Components/RectComponent.h \
    CoreLogic.h \
    HelperFunctions.h \
    HelperStructures.h \
    MainWindow.h \
    View.h \
    Configuration.h

SOURCES += \
    Components/Gates/AbstractGate.cpp \
    Components/Gates/AndGate.cpp \
    Components/Gates/OrGate.cpp \
    Components/Gates/XorGate.cpp \
    Components/RectComponent.cpp \
    CoreLogic.cpp \
    EventHandling.cpp \
    GuiHandling.cpp \
    MainWindow.cpp \
    View.cpp \
    main.cpp

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
