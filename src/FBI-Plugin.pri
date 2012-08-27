CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(fluxusbrosinterfaceplugin)
TEMPLATE    = lib
QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/designer

HEADERS     = knobplugin.h knobcontrolplugin.h fadercontrolplugin.h crossfadercontrolplugin.h buttoncontrolplugin.h fluxusbrosinterface.h \

SOURCES     = knobplugin.cpp knobcontrolplugin.cpp fadercontrolplugin.cpp crossfadercontrolplugin.cpp buttoncontrolplugin.cpp fluxusbrosinterface.cpp \

#RESOURCES   = icons.qrc
LIBS        += -L. 

INCLUDEPATH *= :/

include(knob.pri)
include(knobcontrol.pri)
include(fadercontrol.pri)
include(crossfadercontrol.pri)
include(buttoncontrol.pri)

target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$ HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/FBI-Plugin
INSTALLS    += target sources


