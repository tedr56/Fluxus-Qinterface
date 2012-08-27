#include "visualwidget.h"
#include "visualwidgetplugin.h"

#include <QtCore/QtPlugin>

VisualWidgetPlugin::VisualWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void VisualWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool VisualWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *VisualWidgetPlugin::createWidget(QWidget *parent)
{
    return new VisualWidget(parent);
}

QString VisualWidgetPlugin::name() const
{
    return QLatin1String("VisualWidget");
}

QString VisualWidgetPlugin::group() const
{
    return QLatin1String("");
}

QIcon VisualWidgetPlugin::icon() const
{
    return QIcon();
}

QString VisualWidgetPlugin::toolTip() const
{
    return QLatin1String("");
}

QString VisualWidgetPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool VisualWidgetPlugin::isContainer() const
{
    return false;
}

QString VisualWidgetPlugin::domXml() const
{
    return QLatin1String("<widget class=\"VisualWidget\" name=\"visualWidget\">\n</widget>\n");
}

QString VisualWidgetPlugin::includeFile() const
{
    return QLatin1String("visualwidget.h");
}

