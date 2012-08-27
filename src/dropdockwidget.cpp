#include <QtGui>
#include "dropdockwidget.h"

DropDockWidget::DropDockWidget(QWidget* parent) : QDockWidget(parent),
    tabTimer(0), m_mimeType("multicontrol/media")
{
    setAcceptDrops(true);
}

void DropDockWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(m_mimeType)) {
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

void DropDockWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(m_mimeType)) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void DropDockWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    tabTimer->stop();
    delete tabTimer;
    event->ignore();
}

void DropDockWidget::dropEvent(QDropEvent *event)
{
    event->ignore();
}

void DropDockWidget::DropTabTimeOut()
{
    this->raise();
}
