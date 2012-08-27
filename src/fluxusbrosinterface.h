#ifndef FLUXUSBROSINTERFACE_H
#define FLUXUSBROSINTERFACE_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class FluxusBrosInterface : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
    
public:
    explicit FluxusBrosInterface(QObject *parent = 0);
    
    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;
    
private:
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};

#endif
