#include "knobplugin.h"
#include "knobcontrolplugin.h"
#include "fadercontrolplugin.h"
#include "crossfadercontrolplugin.h"
#include "buttoncontrolplugin.h"
#include "fluxusbrosinterface.h"

FluxusBrosInterface::FluxusBrosInterface(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new KnobPlugin(this));
    m_widgets.append(new KnobControlPlugin(this));
    m_widgets.append(new FaderControlPlugin(this));
    m_widgets.append(new CrossFaderControlPlugin(this));
    m_widgets.append(new ButtonControlPlugin(this));
    
}

QList<QDesignerCustomWidgetInterface*> FluxusBrosInterface::customWidgets() const
{
    return m_widgets;
}

Q_EXPORT_PLUGIN2(fluxusbrosinterfaceplugin, FluxusBrosInterface)
