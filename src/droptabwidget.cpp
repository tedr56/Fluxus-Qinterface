#include <QtGui>
#include "droptabwidget.h"

DropTabBar::DropTabBar(QWidget* parent) : QTabBar(parent),
    tabTimer(0), m_overTabIndex(-1), m_mimeType("multicontrol/media")
{
    setAcceptDrops(true);
}

void DropTabBar::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(m_mimeType)) {
        m_overTabIndex = tabAt(event->pos());
        tabTimer = new QTimer(this);
        connect(tabTimer, SIGNAL(timeout()), this, SLOT(DropTabTimeOut()));
        tabTimer->setSingleShot(true);
        tabTimer->setInterval(1000);
        tabTimer->start();
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void DropTabBar::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(m_mimeType)) {
        if (tabAt(event->pos()) != m_overTabIndex) {
            m_overTabIndex = tabAt(event->pos());
            tabTimer->stop();
            tabTimer->start();
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DropTabBar::dragLeaveEvent(QDragLeaveEvent *event)
{
    tabTimer->stop();
    delete tabTimer;
    event->ignore();
}

void DropTabBar::dropEvent(QDropEvent *event)
{
    event->ignore();
}

void DropTabBar::DropTabTimeOut()
{
    emit TabDropActivate(m_overTabIndex);
}


DropTabWidget::DropTabWidget(QWidget *parent) : QTabWidget(parent)
{
    DropTabBar* DropBar = new DropTabBar(this);
    connect(DropBar, SIGNAL(TabDropActivate(int)), this,  SLOT(setCurrentIndex(int)));
    setTabBar(DropBar);
}
