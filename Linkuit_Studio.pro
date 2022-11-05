QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#build_nr.commands = $${PWD}/build_inc.bat
#build_nr.depends = FORCE
#QMAKE_EXTRA_TARGETS += build_nr
#PRE_TARGETDEPS += build_nr

FULL_VERSION = "$$cat($${OUT_PWD}/version.txt)"
DEFINES += FULL_VERSION='\\"$$FULL_VERSION\\"'

VERSION = "$$FULL_VERSION"

SOURCES += \
   $${PWD}/main.cpp

RESOURCES += \
    Gui/resources.qrc

RC_ICONS = images/icons/icon_default.ico \
           images/icons/icon_file.ico

include(Common.pri)
include(QtAwesome/QtAwesome.pri)
