#include <QtGui>
#include "mediawidget.h"
#include "droptabwidget.h"

DropTabBar::DropTabBar(QWidget* parent) : QTabBar(parent),
    tabTimer(0), m_overTabIndex(-1), m_mimeType("multicontrol/media")
{
    setAcceptDrops(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

void DropTabBar::showContextMenu(const QPoint &point)
{
    if (point.isNull())
        return;
    int tabIndex = tabAt(point);
    QTabWidget *tabWidget = qobject_cast<QTabWidget *>(this->parent());
    if (!tabWidget)
        return;
    m_overTabIndex = tabIndex;
    QWidget *tabDeck = tabWidget->widget(tabIndex);
    QMenu menu(this);
    if (tabDeck->property("tabType").toString() == "Deck"){
        QAction *renameAct = menu.addAction(tr("Rename"));
        connect(renameAct, SIGNAL(triggered()), this, SLOT(renameTab()));
    }
    else if (tabDeck->property("tabType").toString() == "Mime")
        menu.addAction(tr("Mime Tab"));
    QAction *deleteAct = menu.addAction("Delete");
    connect(deleteAct, SIGNAL(triggered()), tabDeck, SLOT(deleteLater()));
    menu.exec(mapToGlobal(point));
}

void DropTabBar::renameTab()
{
    bool ok = false;
    QString label = QInputDialog::getText(this, tabText(m_overTabIndex) , "Enter Deck's name :", QLineEdit::Normal, tabText(m_overTabIndex), &ok);
    if (ok && !label.isEmpty()){
        setTabText(m_overTabIndex, label);
    }
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

int DropTabWidget::addDeckTab()
{
    bool ok = false;
    QString label = QInputDialog::getText(this, "New Media Deck", "Enter Deck's name :", QLineEdit::Normal, "Deck", &ok);
    if (ok && !label.isEmpty()){
        int tabNum = addTab(label);
        //QWidget *tabWidget = widget(tabNum);
        return tabNum;
    } else {
        return -1;
    }
}

int DropTabWidget::addTab(const QString &label)
{
    int tabNum = QTabWidget::addTab(new QWidget(this), label);
//    setScrollbar(tabNum);
    return tabNum;
}

int DropTabWidget::addTab(QWidget *widget, const QString &tabName)
{
    int tabNum = QTabWidget::addTab(widget, tabName);
//    setScrollbar(tabNum);
    return tabNum;
}

int DropTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    int tabNum = QTabWidget::addTab(widget, icon, label);
//    setScrollbar(tabNum);
    return tabNum;
}

MediaWidget *DropTabWidget::getScrollWidget(int tabNum)
{
    QWidget *tabWidget = widget(tabNum);
    MediaWidget *mediaWidget = tabWidget->findChild<MediaWidget*>();
    if (mediaWidget)
        return mediaWidget;
    else
        return false;
}

void DropTabWidget::setScrollbar(int tabNum)
{
    QWidget *tabWidget = widget(tabNum);
    if (!tabWidget)
        return;
    if (! tabWidget->layout())
    {
        QHBoxLayout* tabLayout = new QHBoxLayout(tabWidget);
        tabLayout->setContentsMargins(0,0,0,0);
        tabLayout->setSpacing(0);
        tabWidget->setLayout(tabLayout);
    }
    QScrollArea* Scroll = new QScrollArea(tabWidget);
    tabWidget->layout()->addWidget(Scroll);
    Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    DeckWidget* wrapper = new DeckWidget(Scroll);
    Scroll->setWidget(wrapper);
    Scroll->setWidgetResizable(true);
}

