#-------------------------------------------------
#
# Project created by QtCreator 2012-06-21T02:52:12
#
#-------------------------------------------------

QT       += core gui svg network script
QT_PLUGIN_PATH *= src/
TARGET = FluxusBros-Qinterface
TEMPLATE = app
CONFIG += designer uitools
SOURCES +=\
    src/mainwindow.cpp \
    src/main.cpp \
    src/knob.cpp \
    src/classicstyle.cpp \
    src/udpmidi.cpp \
    src/RtMidi.cpp \
    src/qosctypes.cpp \
    src/qoscserver.cpp \
    src/qoscclient.cpp \
    src/qmidiserver.cpp \
    src/qmidiclient.cpp \
    src/qmiditypes.cpp \
    src/multicontrol.cpp \
    src/knobcontrol.cpp \
    src/fadercontrol.cpp \
    src/crossfadercontrol.cpp \
    src/buttoncontrol.cpp \
    src/mediacontrol.cpp \
    src/mediawidget.cpp \
    src/droptabwidget.cpp \
    src/configdialogpages.cpp \
    src/configdialog.cpp \
    src/configfluxus.cpp \
    src/heads.cpp \
    src/fluxushead.cpp

HEADERS  += \
    src/mainwindow.h \
    src/constants.h \
    src/knob.h \
    src/classicstyle.h \
    src/RtMidi.h \
    src/RtError.h \
    src/mididefs.h \
    src/qosctypes.h \
    src/qoscserver.h \
    src/qoscclient.h \
    src/qmidiserver.h \
    src/qmidiclient.h \
    src/qmiditypes.h \
    src/multicontrolinterface.h \
    src/multicontrol.h \
    src/knobcontrol.h \
    src/fadercontrol.h \
    src/crossfadercontrol.h \
    src/buttoncontrol.h \
    src/mediacontrol.h \
    src/mediawidget.h \
    src/droptabwidget.h \
    src/configdialogpages.h \
    src/configdialog.h \
    src/configfluxus.h \
    src/heads.h \
    src/fluxushead.h

FORMS    += \
    src/mainwindow.ui \
    controllers/KorgMicrocontrol.ui \
    controllers/Bitstream3X.ui \
    controllers/VmxVJ.ui \
    src/knobcontrol.ui \
    src/fadercontrol.ui \
    src/crossfadercontrol.ui \
    src/buttoncontrol.ui \
    controllers/FingerPlay.ui \
    controllers/KorgMicrocontrolCCN2.ui \
    controllers/KorgMicrocontrolCCN1.ui

linux*:!simulator {
    DEFINES += __LINUX_ALSA__
    DEFINES += AVOID_TIMESTAMPING
    DEFINES += RAWKBD_SUPPORT
    CONFIG += link_pkgconfig x11
    PKGCONFIG += alsa
    LIBS += -lpthread -lasound
    INCLUDEPATH += /usr/include/qjson/
}
macx {
    CONFIG += x86 \
        ppc
    ICON = data/vmpk.icns
    DEFINES += __MACOSX_CORE__
    DEFINES += RAWKBD_SUPPORT
    BUNDLE_RES.files = data/help.html \
        data/help_de.html \
        data/help_es.html \
        data/help_fr.html \
        data/help_nl.html \
        data/help_ru.html \
        data/gmgsxg.ins \
        data/spanish.xml \
        data/german.xml \
        data/azerty.xml \
        data/it-qwerty.xml \
        data/vkeybd-default.xml \
        data/pc102mac.xml \
        qt.conf \
        $$[QT_INSTALL_TRANSLATIONS]/qt_cs.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_es.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_fr.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_ru.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_sv.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt_zh_CN.qm \
        $$OUT_PWD/vmpk_cs.qm \
        $$OUT_PWD/vmpk_de.qm \
        $$OUT_PWD/vmpk_es.qm \
        $$OUT_PWD/vmpk_fr.qm \
        $$OUT_PWD/vmpk_nl.qm \
        $$OUT_PWD/vmpk_ru.qm \
        $$OUT_PWD/vmpk_sv.qm \
        $$OUT_PWD/vmpk_zh_CN.qm
    BUNDLE_RES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += BUNDLE_RES
    LIBS += -framework \
        CoreMidi \
        -framework \
        CoreAudio \
        -framework \
        CoreFoundation \
        -framework \
        Carbon
}

debug:DEFINES += __RTMIDI_DEBUG__
//LIBS += -fluxusbrosinterfaceplugin
LIBS += -lqjson
INCLUDEPATH *= ./src

RESOURCES += \
    FBI.qrc

#include(FBI-Plugin.pro)

OTHER_FILES += \
    README.md
