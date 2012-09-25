#include <QFileInfo>
#include <QSpacerItem>
#include "mediawidget.h"
#include "mediacontrol.h"
#include "multicontrol.h"
#include "buttoncontrol.h"

StackHBoxLayout::StackHBoxLayout(QWidget *parent) : QBoxLayout(QBoxLayout::LeftToRight, parent)
{
    setSizeConstraint(QLayout::SetMaximumSize);
    setContentsMargins(0,0,0,0);
    setSpacing(2);
    addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
}

void StackHBoxLayout::addWidget(QWidget *w, int stretch = 0 , Qt::Alignment align = Qt::AlignLeft)
{
    insertWidget(count() - 1, w, stretch, align);
}

MediaWidget::MediaWidget(QWidget *parent) : QWidget(parent)
{
    m_mimeTypes = QStringList("fluxus/visual");
    m_layout = new StackHBoxLayout(this);
    setAcceptDrops(true);
    setBackgroundRole(QPalette::Dark);
    setLayout(m_layout);
}

void MediaWidget::setMimeType(QStringList Type)
{
    m_mimeTypes = Type;
}

QStringList MediaWidget::MimeType()
{
    return m_mimeTypes + m_externalMimeTypes;
}

void MediaWidget::setParametersDock(QWidget *paramDock)
{
    m_parametersDock = paramDock;
}

bool MediaWidget::checkMimeFormat(QStringList mimeFormats)
{
    for (int i = 0; i < mimeFormats.size(); i++){
        if (MimeType().contains(mimeFormats.at(i)))
            return true;
    }
    return false;
}

QStringList MediaWidget::getValidMimeFormats(QStringList mimeFormats)
{
    QStringList validFormats;
    foreach(const QString &str, mimeFormats){
        if (MimeType().contains(str))
            validFormats.append(str);
    }
    return validFormats;
}

void MediaWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (checkMimeFormat(event->mimeData()->formats())) {
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
    if (checkMimeFormat(event->mimeData()->formats())) {
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
    if (checkMimeFormat(event->mimeData()->formats())) {
        if (event->source()->parent() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            dropAction(event);
        } else {
            event->acceptProposedAction();
            dropAction(event);
        }
    } else {
        event->ignore();
    }
}

void MediaWidget::dropAction(QDropEvent *event){
    const QMimeData *mime = event->mimeData();
    if (mime->formats().contains("fluxus/visual")){
        QByteArray itemData = mime->data("fluxus/visual");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QString filePath;
        int defValues;
        dataStream >> filePath >> defValues;
        FluxusVisual* Target = new FluxusVisual(filePath, this, defValues);
        m_layout->addWidget(Target);
    }
}

void MediaWidget::addWidget(QWidget *w)
{
    m_layout->insertWidget(m_layout->count() - 1, w, 1, Qt::AlignLeft);
}

DeckWidget::DeckWidget(QWidget *parent) : MediaWidget(parent)
{
    m_externalMimeTypes.append("text/plain");
    setAcceptDrops(true);
}

QStringList DeckWidget::getFilesPath(QList<QUrl> urlList)
{
    QStringList pathList;
    for (int i = 0; i < urlList.size() && i < 32; ++i)
    {
        pathList.append(urlList.at(i).toLocalFile());
    }
    return pathList;
}

void DeckWidget::dropEvent(QDropEvent *event)
{
    if (checkMimeFormat(event->mimeData()->formats())) {
        if (event->source()){
            if (event->source()->parent() == this) {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            }
        } else {
            event->acceptProposedAction();
            dropAction(event);
        }
    } else {
        event->ignore();
    }
}

void DeckWidget::dropAction(QDropEvent *event)
{
    QStringList validMime = getValidMimeFormats(event->mimeData()->formats());
    int externalMedia = 0;
    externalMedia += addFluxusMedia(event);
    if (!externalMedia)
        MediaWidget::dropEvent(event);
}

int DeckWidget::addFluxusMedia(QDropEvent *event)
{
    int addedMedia = 0;
    if ((event->mimeData()->formats().contains("text/plain")) && (event->mimeData()->hasUrls())){
        QStringList filesPath = getFilesPath(event->mimeData()->urls());
        foreach(const QString &path, filesPath){
            QFileInfo fileInfo(path);
            if (fileInfo.completeSuffix() == "scm"){
                FluxusVisual* FlxsVisual = new FluxusVisual(fileInfo, this);
                m_layout->addWidget(FlxsVisual);
                //connect()
                addedMedia++;
            }
        }
    }
    return addedMedia;
}
