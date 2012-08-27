#include "knobcontrol.h"
#include "classicstyle.h"
#include "QDebug"

KnobControl::KnobControl ( QWidget *Parent )
    : MultiControl(Parent)
{
    setupUi(this);
    KnobDial->setValue(getValue());
    setMinimum(getMinimum());
    setMaximum(getMaximum());
//    QStyle* dialStyle;
//    dialStyle = new ClassicStyle();
//    this->KnobDial->setStyle(dialStyle);
    connect(this, SIGNAL(WidgetUpdate(int)), KnobDial, SLOT(setValue(int)));
    connect(KnobDial, SIGNAL(sliderMoved(int)), this, SLOT(WidgetMoved(int)));
}

void KnobControl::setMinimum(int Min)
{
    MultiControl::setMinimum(Min);
    KnobDial->setMinimum(Min);
}

void KnobControl::setMaximum(int Max)
{
    MultiControl::setMaximum(Max);
    KnobDial->setMaximum(Max);
}

void KnobControl::setStyle(QStyle *style)
{
    KnobDial->setStyle(style);
}
