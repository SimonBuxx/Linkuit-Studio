QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET_SUBDIR =
CONFIG(debug, debug|release) {
    TARGET_SUBDIR = debug
} else {
    TARGET_SUBDIR = release
}

CONF_FILE = $$PWD/qt.conf
DEST_DIR = $$OUT_PWD/$$TARGET_SUBDIR

win32: QMAKE_POST_LINK += copy /Y $$shell_path($$CONF_FILE) $$shell_path($$DEST_DIR)
unix:  QMAKE_POST_LINK += cp $$shell_path($$CONF_FILE) $$shell_path($$DEST_DIR)

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
