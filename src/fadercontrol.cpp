#include "fadercontrol.h"
#include <QStyle>
FaderControl::FaderControl ( QWidget *Parent )
    : MultiControl(Parent)
{
    setupUi(this);
    verticalSlider->setValue(getValue());
    setMinimum(getMinimum());
    connect(this, SIGNAL(WidgetUpdate(int)), verticalSlider, SLOT(setValue(int)));
    connect(verticalSlider, SIGNAL(sliderMoved(int)), this, SLOT(WidgetMoved(int)));
    setMaximum(getMaximum());
}

void FaderControl::setMinimum(int Min)
{
    MultiControl::setMinimum(Min);
    verticalSlider->setMinimum(Min);
}

void FaderControl::setMaximum(int Max)
{
    MultiControl::setMaximum(Max);
    verticalSlider->setMaximum(Max);
}

void FaderControl::setStyle(QStyle *style)
{
    verticalSlider->setStyle(style);
}
