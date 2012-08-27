#include "buttoncontrol.h"
#include "buttoncontrolplugin.h"

#include <QtCore/QtPlugin>

ButtonControlPlugin::ButtonControlPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void ButtonControlPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool ButtonControlPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *ButtonControlPlugin::createWidget(QWidget *parent)
{
    return new ButtonControl(parent);
}

QString ButtonControlPlugin::name() const
{
    return QLatin1String("ButtonControl");
}

QString ButtonControlPlugin::group() const
{
    return QLatin1String("");
}

QIcon ButtonControlPlugin::icon() const
{
    return QIcon();
}

QString ButtonControlPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ButtonControlPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ButtonControlPlugin::isContainer() const
{
    return false;
}

QString ButtonControlPlugin::domXml() const
{
    return QLatin1String("<widget class=\"ButtonControl\" name=\"ButtonMultiControl\">\n </widget>\n");
}

QString ButtonControlPlugin::includeFile() const
{
    return QLatin1String("buttoncontrol.h");
}
