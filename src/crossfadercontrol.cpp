#include "crossfadercontrol.h"

CrossFaderControl::CrossFaderControl ( QWidget *Parent )
    : MultiControl(Parent)
{
    setupUi(this);
    horizontalSlider->setValue(getValue());
    setMinimum(getMinimum());
    setMaximum(getMaximum());
    connect(this, SIGNAL(WidgetUpdate(int)), horizontalSlider, SLOT(setValue(int)));
    connect(horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(WidgetMoved(int)));
}

void CrossFaderControl::setMinimum(int Min)
{
    MultiControl::setMinimum(Min);
    horizontalSlider->setMinimum(Min);
}

void CrossFaderControl::setMaximum(int Max)
{
    MultiControl::setMaximum(Max);
    horizontalSlider->setMaximum(Max);
}

void CrossFaderControl::setStyle(QStyle *style)
{
    horizontalSlider->setStyle(style);
}
