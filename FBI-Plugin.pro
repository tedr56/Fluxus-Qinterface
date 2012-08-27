CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(fluxusbrosinterfaceplugin)
TEMPLATE    = lib
QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/designer

HEADERS     = src/knobplugin.h src/knobcontrolplugin.h src/fadercontrolplugin.h src/crossfadercontrolplugin.h src/buttoncontrolplugin.h src/fluxusbrosinterface.h \

SOURCES     = src/knobplugin.cpp src/knobcontrolplugin.cpp src/fadercontrolplugin.cpp src/crossfadercontrolplugin.cpp src/buttoncontrolplugin.cpp src/fluxusbrosinterface.cpp \

#RESOURCES   = icons.qrc
LIBS        += -L. 

INCLUDEPATH *= src/
SUBDIRS *= src/

include(src/knob.pri)
include(src/knobcontrol.pri)
include(src/fadercontrol.pri)
include(src/crossfadercontrol.pri)
include(src/buttoncontrol.pri)

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$ HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/FBI-Plugin
INSTALLS    += target sources


