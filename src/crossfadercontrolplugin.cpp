#include "crossfadercontrol.h"
#include "crossfadercontrolplugin.h"

#include <QtCore/QtPlugin>

CrossFaderControlPlugin::CrossFaderControlPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void CrossFaderControlPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool CrossFaderControlPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *CrossFaderControlPlugin::createWidget(QWidget *parent)
{
    return new CrossFaderControl(parent);
}

QString CrossFaderControlPlugin::name() const
{
    return QLatin1String("CrossFaderControl");
}

QString CrossFaderControlPlugin::group() const
{
    return QLatin1String("");
}

QIcon CrossFaderControlPlugin::icon() const
{
    return QIcon();
}

QString CrossFaderControlPlugin::toolTip() const
{
    return QLatin1String("");
}

QString CrossFaderControlPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool CrossFaderControlPlugin::isContainer() const
{
    return false;
}

QString CrossFaderControlPlugin::domXml() const
{
    return QLatin1String("<widget class=\"CrossFaderControl\" name=\"CrossFaderMultiControl\">\n </widget>\n");
}

QString CrossFaderControlPlugin::includeFile() const
{
    return QLatin1String("crossfadercontrol.h");
}
