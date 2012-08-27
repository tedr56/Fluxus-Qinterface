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
    style->signalsBlocked();
    const QString classicStyle = QString("QSlider::groove:vertical { background: red; position: absolute; left: 4px; right: 4px;}  QSlider::handle:vertical {height: 10px; background: darkGrey; margin: 0 -4px;} QSlider::add-page:vertical {background: darkCyan;} QSlider::sub-page:vertical {background: white;}");
    verticalSlider->setStyleSheet(classicStyle);
}
