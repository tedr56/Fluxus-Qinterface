#include <QSpacerItem>
#include <QMimeData>
#include <QFile>
#include <QRegExp>
#include <QMessageBox>
#include "mediacontrol.h"

MediaControl::MediaControl(QWidget *parent) : ButtonControl(parent),
    m_mediaName("Media"),
    m_nameLabel(new QLabel(m_mediaName, this)),
    m_dragging(false)
{
    verticalLayout->addWidget(m_nameLabel);
    setMediaName("Media");
    MultiButton->setMinimumSize(70, 70);
    QSizePolicy ButtonSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    MultiButton->setSizePolicy(ButtonSizePolicy);
    setMinimumSize(70,70);
    QSizePolicy WidgetSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setSizePolicy(WidgetSizePolicy);
    verticalLayout->setAlignment(m_nameLabel, Qt::AlignHCenter);
}

QString MediaControl::mediaName()
{
    return m_mediaName;
}

void MediaControl::setMediaName(QString name)
{
    //setStyleSheet("QWidget { background-color: blue;} QPushButton { colour: red }");
    m_mediaName = name;
    m_nameLabel->setText(name);
}

void MediaControl::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        startMousePoint = event->pos();
}

void MediaControl::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        int distance = (event->pos() - startMousePoint).manhattanLength();
        if(distance >= QApplication::startDragDistance())
            emit startDragging();
    }
}

void MediaControl::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        if(!m_dragging)
            emit clicking();
    }
}

void MediaControl::dragging()
{
    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    mimeData->setData(mimeType(), itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    hide();
    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        close();
    else
        show();
}

FluxusVisual::FluxusVisual(QFileInfo fileInfo, QWidget *parent, int defValues) : MediaControl(parent),
    m_fileInfo(QFileInfo(fileInfo)),
    m_controlList(new QStringList),
    m_defaultsValues(defValues)
{
    initMedia();
}

FluxusVisual::FluxusVisual(QString pathInfo, QWidget *parent, int defValues) : MediaControl(parent),
    m_fileInfo(QFileInfo(pathInfo)),
    m_controlList(new QStringList),
    m_defaultsValues(defValues)
{
    initMedia();
}

void FluxusVisual::initMedia()
{
    if (!checkControls(m_fileInfo)){
        QMessageBox msgBox;
        msgBox.setText("No control found!");
        QString infoText;
        infoText.append("Do you want to keep ");
        infoText.append(m_fileInfo.baseName());
        msgBox.setInformativeText(infoText);
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No)
            deleteLater();
    }
    verticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    setMimeType("fluxus/visual");
    setMediaName(m_fileInfo.baseName());
    connect(this, SIGNAL(startDragging()), this, SLOT(dragging()));
}

int FluxusVisual::checkControls(QFileInfo fileInfo)
{
    QFile visualFile(fileInfo.filePath());
    if (!visualFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QRegExp controlReg, controlNameReg;
    controlReg.setPattern("\\(c \"[A-Za-z0-9\\-\\_]*\" id(\\s\\#\\:coeff [0-9]*)?\\)");
    controlNameReg.setPattern("\\\"[A-Za-z0-9\\-\\_]*\\\"");
    QTextStream in(&visualFile);
    int count;
    int pos;
    while (!in.atEnd()) {
        QString line = in.readLine();
        count = 0;
        pos = 0;
        while ((pos = controlReg.indexIn(line, pos)) != -1) {
            ++count;
            pos += controlReg.matchedLength();
            controlNameReg.indexIn(controlReg.cap());
            m_controlList->append( controlNameReg.cap().remove(QChar('\"')) );
        }
    }
    visualFile.close();
    return m_controlList->count();
}

void FluxusVisual::dragging()
{
    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << m_fileInfo.filePath() << m_defaultsValues;
    mimeData->setData(mimeType(), itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    hide();

    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        close();
    else
        show();
}
