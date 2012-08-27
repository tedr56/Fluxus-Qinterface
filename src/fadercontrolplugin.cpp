#include "fadercontrol.h"
#include "fadercontrolplugin.h"

#include <QtCore/QtPlugin>

FaderControlPlugin::FaderControlPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void FaderControlPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool FaderControlPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *FaderControlPlugin::createWidget(QWidget *parent)
{
    return new FaderControl(parent);
}

QString FaderControlPlugin::name() const
{
    return QLatin1String("FaderControl");
}

QString FaderControlPlugin::group() const
{
    return QLatin1String("");
}

QIcon FaderControlPlugin::icon() const
{
    return QIcon();
}

QString FaderControlPlugin::toolTip() const
{
    return QLatin1String("");
}

QString FaderControlPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool FaderControlPlugin::isContainer() const
{
    return false;
}

QString FaderControlPlugin::domXml() const
{
    return QLatin1String("<widget class=\"FaderControl\" name=\"FaderMultiControl\">\n </widget>\n");
}

QString FaderControlPlugin::includeFile() const
{
    return QLatin1String("fadercontrol.h");
}
