#include "knob.h"
#include "knobplugin.h"

#include <QtCore/QtPlugin>

KnobPlugin::KnobPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void KnobPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;


    m_initialized = true;
}

bool KnobPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *KnobPlugin::createWidget(QWidget *parent)
{
    return new Knob(parent);
}

QString KnobPlugin::name() const
{
    return QLatin1String("Knob");
}

QString KnobPlugin::group() const
{
    return QLatin1String("");
}

QIcon KnobPlugin::icon() const
{
    return QIcon();
}

QString KnobPlugin::toolTip() const
{
    return QLatin1String("");
}

QString KnobPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool KnobPlugin::isContainer() const
{
    return false;
}

QString KnobPlugin::domXml() const
{
    return
        "<widget class=\"Knob\" name=\"Knob\">\n"
        "</widget>\n";
}

QString KnobPlugin::includeFile() const
{
    return QLatin1String("knob.h");
}
