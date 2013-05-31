/*
    MIDI Virtual Piano Keyboard
    Copyright (C) 2008-2011, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    Based on an original design by Thorsten Wilms.
    Implemented as a widget for the Rosegarden MIDI and audio sequencer
    and notation editor by Chris Cannam.
    Extracted into a standalone Qt3 widget by Pedro Lopez-Cabanillas
    and adapted for use in QSynth.
    Ported to Qt4 by Chris Cannam.
    Adapted as a QStyle by Pedro Lopez-Cabanillas.

    For this file, the following copyright notices are also applicable:
    This file copyright 2003-2006 Chris Cannam,
    copyright 2005,2008 Pedro Lopez-Cabanillas,
    copyright 2006 Queen Mary, University of London.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; If not, see <http://www.gnu.org/licenses/>.
*/

#include "classicstyle.h"

#include <QtGui/QPainter>
#include <QtGui/QStyleOptionSlider>
#include <QtSvg/QSvgRenderer>
#include <qmath.h>
#include <QDebug>

#define DIAL_MIN (0.25 * M_PI)
#define DIAL_MAX (1.75 * M_PI)
#define DIAL_RANGE (DIAL_MAX - DIAL_MIN)

static void drawTick(QPainter *p, float angle, int size, bool internal)
{
	float hyp = float(size) / 2.0;
	float x0 = hyp - (hyp - 1) * qSin(angle);
	float y0 = hyp + (hyp - 1) * qCos(angle);
	if (internal) {
		float len = hyp / 4;
		float x1 = hyp - (hyp - len) * qSin(angle);
		float y1 = hyp + (hyp - len) * qCos(angle);
		p->drawLine(int(x0), int(y0), int(x1), int(y1));
	} else {
		float len = hyp / 4;
		float x1 = hyp - (hyp + len) * qSin(angle);
		float y1 = hyp + (hyp + len) * qCos(angle);
		p->drawLine(int(x0), int(y0), int(x1), int(y1));
	}
}

void ClassicStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const
{
//    if (cc != QStyle::CC_Dial) {
//        QCommonStyle::drawComplexControl(cc, opt, p, widget);
//        return;
//    }
    if (cc == QStyle::CC_Dial){
        drawDial(opt, p);
//    } else if (cc == QStyle::CC_Slider) {
//        drawFader(opt, p);
//        QCommonStyle::drawComplexControl(cc,opt,p,widget);
    } else {
        QCommonStyle::drawComplexControl(cc, opt, p, widget);
    }
}

void ClassicStyle::drawControl ( ControlElement element, const QStyleOption * opt, QPainter * p, const QWidget * widget) const
{
    QCommonStyle::drawControl(element, opt, p, widget);
}

void ClassicStyle::drawDial(const QStyleOptionComplex *opt, QPainter *p) const
{
    const QStyleOptionSlider *dial = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (dial == NULL)
        return;

    float angle = DIAL_MIN + (DIAL_RANGE * (float(dial->sliderValue - dial->minimum) /
                   (float(dial->maximum - dial->minimum))));
    int degrees = int(angle * 180.0 / M_PI);
    int ns = dial->tickInterval;
    int numTicks = 1 + (dial->maximum + ns - dial->minimum) / ns;
    int size = dial->rect.width() < dial->rect.height() ? dial->rect.width() : dial->rect.height();
    int scale = 1;
    int width = size * scale;
    int indent = (int)(width * 0.15 + 1);

    QPalette pal = opt->palette;
    QColor knobColor = pal.mid().color(); //pal.background().color();
    QColor meterColor = (dial->state & State_Enabled) ? pal.highlight().color() : pal.mid().color();
    QPen pen;
    QColor c;

    p->save();
    p->setRenderHint(QPainter::Antialiasing, true);
    p->translate(1+(dial->rect.width()-size)/2, 1+(dial->rect.height()-size)/2);

    // Knob body and face...

    pen.setColor(knobColor);
    pen.setWidth(scale * 2);
    pen.setCapStyle(Qt::FlatCap);
    p->setPen(pen);

//    QRadialGradient gradient(size/2, size/2, size-indent, size/2-indent, size/2-indent);
//    gradient.setColorAt(0, knobColor.light());
//    gradient.setColorAt(1, knobColor.dark());
//    p->setBrush(gradient);
//    p->drawEllipse(indent, indent, width-2*indent, width-2*indent);

    // The bright metering bit...

    c = meterColor;
    pen.setColor(QColor(161,255,52));
    pen.setWidth(indent);
    p->setPen(pen);

    int arcLen = -(degrees - 45) * 16;
    if (arcLen == 0) arcLen = -16;
    p->drawArc(indent/2, indent/2, width-indent, width-indent, (180 + 45) * 16, arcLen);
    p->setBrush(Qt::NoBrush);

    // Tick notches...

    if (dial->subControls & QStyle::SC_DialTickmarks) {
        //	std::cerr << "Notches visible" << std::endl;
        pen.setColor(pal.dark().color());
        pen.setWidth(scale);
        p->setPen(pen);
        for (int i = 0; i < numTicks; ++i) {
            int div = numTicks;
            if (div > 1) --div;
            drawTick(p, DIAL_MIN + (DIAL_MAX - DIAL_MIN) * i / div, width, true);
        }
    }

    // Shadowing...

    pen.setWidth(scale);
    p->setPen(pen);

    // Knob shadow...

    int shadowAngle = -720;
//    c = knobColor.dark();
//    for (int arc = 120; arc < 2880; arc += 240)
//    {
//        pen.setColor(c);
//        p->setPen(pen);
//        p->drawArc(indent, indent, width-2*indent, width-2*indent, shadowAngle
//                        + arc, 240);
//        p->drawArc(indent, indent, width-2*indent, width-2*indent, shadowAngle
//                        - arc, 240);
//        c = c.light(110);
//    }

    // Scale shadow...

    shadowAngle = 2160;
    c = pal.dark().color();
    for (int arc = 120; arc < 2880; arc += 240) {
        pen.setColor(QColor(161,255,52));
        p->setPen(pen);
        p->drawArc(scale/2, scale/2,
                width-scale, width-scale, shadowAngle + arc, 240);
        p->drawArc(scale/2, scale/2,
                width-scale, width-scale, shadowAngle - arc, 240);
        c = c.light(108);
    }

    // Undraw the bottom part...

    pen.setColor(pal.background().color());
    pen.setWidth(scale * 4);
    p->setPen(pen);
    p->drawArc(scale/2, scale/2,
          width-scale, width-scale, -45 * 16, -90 * 16);

    // Pointer notch...

    float hyp = float(width) / 2.0;
    float len = hyp - indent;
    --len;

    float x0 = hyp;
    float y0 = hyp;

    float x = hyp - len * qSin(angle);
    float y = hyp + len * qCos(angle);

    c = pal.dark().color();
    pen.setColor((dial->state & State_HasFocus) ? QColor(161,255,52) : c);
    pen.setWidth(scale * 2);
    p->setPen(pen);
    p->drawLine(int(x0), int(y0), int(x), int(y));

    // done
    p->restore();

}

void ClassicStyle::drawFader(const QStyleOptionComplex *opt, QPainter *p) const
{
    const QStyleOptionSlider *fader = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (fader == NULL)
        return;
    int Value = fader->sliderValue;
    int Size = (fader->rect.height() > fader->rect.width() ? fader->rect.height() : fader->rect.width());
    float CursorRatio = float(Size) / float(fader->maximum);
    if (fader->orientation & Qt::Vertical)
        CursorRatio *= -1;
    int CursorPosition = Value * CursorRatio;
    QPen pen;
    QColor color(161,255,52);
    color.setAlpha(100);
    QBrush brush(color, Qt::SolidPattern);
    pen.setBrush(brush);
    p->save();
    p->setRenderHint(QPainter::Antialiasing, true);
    p->translate(0, fader->rect.height());
    p->setPen(pen);
    p->setBrush(brush);
    //p->drawRoundedRect(0, 0, 10, 30, 3, 3);
    //p->drawRect(1,1,10,10);
    if (fader->orientation & Qt::Vertical)
        p->drawRect(0,0, fader->rect.width(), CursorPosition);
    else
        p->drawRect(0,0, CursorPosition, fader->rect.height());
    color.setAlpha(255);
    brush.setColor(color);
    p->setBrush(brush);
    if (fader->orientation & Qt::Vertical)
        p->drawRect(0,CursorPosition - 1 , fader->rect.width(), 3);
    else
        p->drawRect(CursorPosition - 1, 0 , 3, fader->rect.height());
    qDebug() << "";
    qDebug() << ((fader->orientation & Qt::Vertical) ? fader->rect.height() : fader->rect.width());
    qDebug() << Value;
    qDebug() << fader->maximum;
    qDebug() << CursorRatio;
    qDebug() << CursorPosition;
    p->restore();
}

void ClassicStyle::drawPrimitive( PrimitiveElement pe, const QStyleOption *opt,
                                  QPainter *p, const QWidget *w ) const
{
    static QSvgRenderer onRenderer(QString(":/data/led_green.svg"));
    static QSvgRenderer offRenderer(QString(":/data/led_grey.svg"));
    if (pe == PE_IndicatorCheckBox) {
        QRect r = opt->rect;
        if (opt->state & State_On)
            onRenderer.render(p, r);
        else if (opt->state & State_Off)
            offRenderer.render(p, r);
        return;
    } else if (pe == PE_FrameFocusRect)
        return;
    QCommonStyle::drawPrimitive(pe, opt, p, w);
}

int ClassicStyle::pixelMetric( PixelMetric metric, const QStyleOption *option,
                               const QWidget *widget ) const
{
    if ( metric == PM_IndicatorWidth || metric == PM_IndicatorHeight )
        return 24;
    return QCommonStyle::pixelMetric( metric, option, widget );
}
