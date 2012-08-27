#include "knobcontrol.h"
#include "knobcontrolplugin.h"

#include <QtCore/QtPlugin>

KnobControlPlugin::KnobControlPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void KnobControlPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;


    m_initialized = true;
}

bool KnobControlPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *KnobControlPlugin::createWidget(QWidget *parent)
{
    return new KnobControl(parent);
}

QString KnobControlPlugin::name() const
{
    return QLatin1String("KnobControl");
}

QString KnobControlPlugin::group() const
{
    return QLatin1String("");
}

QIcon KnobControlPlugin::icon() const
{
    return QIcon();
}

QString KnobControlPlugin::toolTip() const
{
    return QLatin1String("");
}

QString KnobControlPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool KnobControlPlugin::isContainer() const
{
    return false;
}

QString KnobControlPlugin::domXml() const
{
    return
        "<widget class=\"KnobControl\" name=\"KnobMultiControl\">\n"
        "</widget>\n";
}

QString KnobControlPlugin::includeFile() const
{
    return QLatin1String("knobcontrol.h");
}
