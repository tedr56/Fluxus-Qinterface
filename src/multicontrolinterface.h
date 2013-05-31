#ifndef MULTICONTROLINTERFACE_H
#define MULTICONTROLINTERFACE_H

#include <QApplication>
#include <QStyle>
#include <QtPlugin>
#include "qmidiserver.h"
#include "qmidiclient.h"
#include "qoscserver.h"
#include "qoscclient.h"

class MultiControlInterface
{
public:
    virtual void setStyle( QStyle *) = 0;
    virtual QStyle *style() { return 0; } ;
    virtual void setMidiObject(QMidiServer*) = 0;
    virtual void setMidiObject(QMidiClient*) = 0;
    virtual void setOscObject(QOscServer*) = 0;
    //virtual void setOscObject(QOscClient*) = 0;
    virtual void setOscObject(QList<QOscClient*>) = 0;
    virtual ~MultiControlInterface() {};
};

Q_DECLARE_INTERFACE(MultiControlInterface, "Ted.MultiControlInterface/1.0")

#endif
