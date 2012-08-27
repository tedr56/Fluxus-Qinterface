#include "mediawidget.h"
#include "multicontrol.h"
#include "buttoncontrol.h"

MediaWidget::MediaWidget(QWidget *parent) : QWidget(parent)
{
    m_mimeType = "multicontrol/media";
    QHBoxLayout* MediaLayout = new QHBoxLayout(this);
    MediaLayout->setSizeConstraint(QLayout::SetMinimumSize);
    MediaLayout->setContentsMargins(0,0,0,0);
    MediaLayout->setSpacing(0);
    setAcceptDrops(true);
    setBackgroundRole(QPalette::Dark);
    setLayout(MediaLayout);
}

void MediaWidget::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug() << "DragEnter Debug " << event->mimeData()->formats() << " " << m_mimeType;
    if (event->mimeData()->hasFormat(m_mimeType)) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MediaWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "DragMove Debug ";
    qDebug() << event->mimeData()->formats() << " " << m_mimeType;
    qDebug() << event->source();
    if (event->mimeData()->hasFormat(m_mimeType)) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MediaWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop Debug " << event->mimeData()->formats() << " " << m_mimeType;
    if (event->mimeData()->hasFormat(m_mimeType)) {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data(m_mimeType);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        int channel;
        int control;
        dataStream >> channel >> control;
        qDebug() << channel << " " << control;
        ButtonControl* Target = new ButtonControl(this);
//        MediaLayout->addWidget(Target);
        Target->setChannel(channel);
        Target->setControl(control);
        Target->setOutputMode(MultiControl::OutMidi);
        Target->setInputMode(MultiControl::MidiCC);
        Target->setMinimumSize(60,60);
        layout()->addWidget(Target);
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MediaWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        MultiControl *child = static_cast<MultiControl *>(childAt(event->pos()));
        if (!child)
            return;
        QPoint hotspot = event->pos() - child->pos();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << child->getChannel() << child->getControl();

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(m_mimeType, itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setHotSpot(hotspot);

        child->hide();

        if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
            child->close();
        else
            child->show();
    }
}
