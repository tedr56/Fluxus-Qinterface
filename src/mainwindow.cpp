#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtGui>
#include <QFormBuilder>
#include <QtDebug>
#include <QtUiTools/QtUiTools>
#include <QDesktopWidget>
#include <QScriptEngine>
#include "constants.h"
#include "RtMidi.h"
#include "mididefs.h"
#include "configdialog.h"
#include "mediawidget.h"
#include "buttoncontrol.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    MultipleTable(0),
    m_midiserver(0),
    m_midiclient(0),
    m_oscserver(0),
    m_oscclient(0),
//    m_oscclients(0),
    m_currentOut(0),
    m_currentIn(0),
    m_inputActive(0),
    m_midiThru(0),
    m_initialized(0),
    m_osc_server_initialized(0),
    m_configDialog(0)
{
    initConnections();
    initUi();
    initActions();
    read_settings();
    OpenTable(":/controllers/KorgMicrocontrol.ui");
}

void MainWindow::initConnections()
{
    initMidiConnections();
    initOscConnections();
}

void MainWindow::initMidiConnections()
{
    m_midiserver = new QMidiServer(this, QSTR_FBIPORT, QSTR_FBIINPUT_PORT);
    m_midiclient = new QMidiClient(this, QSTR_FBIPORT, QSTR_FBIOUTPUT_PORT);
}

void MainWindow::initOscConnections(){
    QSettings settings;
    settings.beginGroup("OscConfig");
    if (m_oscserver){
        delete m_oscserver;
        m_oscserver = new QOscServer( settings.value("serverport", "3333").toInt() , this);
    } else {
        m_oscserver = new QOscServer( settings.value("serverport", "3333").toInt() , this);
    }
    if (m_oscclients.count()){

    } else {
        int clientsum = settings.beginReadArray("oscclients");
        for (int i = 0; i < clientsum; i++){
            settings.setArrayIndex(i);
            m_oscclients.append(new QOscClient(QHostAddress(settings.value("address","127.0.0.1").toString()), settings.value("port", 4444).toInt(), this ));
        }
        settings.endArray();
    }
    settings.endGroup();
}

void MainWindow::initUi()
{
    ui->setupUi(this);
    // Set max frame size to primary screen available area
    QRect PrimScreenSize = QApplication::desktop()->availableGeometry();
    this->setMaximumSize(PrimScreenSize.size());
    //Set style for widgets
    m_dialStyle = new ClassicStyle();
    m_dialStyle->setParent(this);

    setScrollDock(ui->Deck1);
    setScrollDock(ui->Deck2);
    setScrollDock(ui->Visuals);
    setScrollDock(ui->FXs);
    setScrollDock(ui->SideADockContents);
    setScrollDock(ui->SideBDockContents);
    //Apply settings
    MultipleTable = 0;
}

void MainWindow::initActions()
{
    //Connect menu specific actions
    connect(ui->actionOpen_Table, SIGNAL(triggered()), this, SLOT(setOpenTable()));
    connect(ui->actionAllow_multiple_tables, SIGNAL(toggled(bool)), this, SLOT(setMultipleTable()));
    //Add recent table files actions
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(ui->menuOpen_Recent_Table);
        recentFileActs[i]->setVisible(false);
        ui->menuOpen_Recent_Table->addAction(recentFileActs[i]);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
    }
    updateRecentFileActions();
    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(setPreferences()));
}

void MainWindow::initMimeConf()
{
    QScriptEngine engine;
    QSettings settings;
    settings.beginGroup("MimeTypes");
    int MimeNum = settings.beginReadArray("MimeEntry");
    for(int i = 0; i < MimeNum; i++){
        settings.setArrayIndex(i);
        QString Mime = settings.value("mimeType").toString();
        QString Script = settings.value("mimeScript").toString();
        QFile ScriptFile(Script);
        if (!ScriptFile.open(QIODevice::ReadOnly))
            break;
        QString ScriptContent = ScriptFile.readAll();
        QScriptSyntaxCheckResult CheckResult = engine.checkSyntax(ScriptContent);
        if (CheckResult.state() == QScriptSyntaxCheckResult::Valid){
            qDebug() << engine.evaluate(ScriptContent).toString();
        } else {
            qDebug() << CheckResult.state();
        }
    }
}

void MainWindow::setScrollDock(QWidget *Dock)
{
    if (! Dock->layout())
    {
        QHBoxLayout* DockLayout = new QHBoxLayout(Dock);
        DockLayout->setContentsMargins(0,0,0,0);
        DockLayout->setSpacing(0);
        Dock->setLayout(DockLayout);
    }
    QScrollArea* Scroll = new QScrollArea(Dock);
    Dock->layout()->addWidget(Scroll);
    Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    MediaWidget* wrapper = new MediaWidget(Scroll);
    Scroll->setWidget(wrapper);
    Scroll->setWidgetResizable(true);
    for(int i = 0; i < 2; i++)
    {
        ButtonControl* Button = new ButtonControl(wrapper);
        Button->setMinimumSize(60,60);
        wrapper->layout()->addWidget(Button);
    }
}

void MainWindow::setPreferences()
{
    if (!m_configDialog)
        m_configDialog = new ConfigDialog(this);
    m_configDialog->exec();
    if (m_configDialog->OscChanged())
        initOscConnections();
    if (m_configDialog->MidiChanged())
        initMidiConnections();
    if (m_configDialog->MimeChanged())
        initMimeConf();
    m_configDialog->ResetChanged();
}

void MainWindow::setOpenTable()
{
     QFileDialog::Options options;
     QString selectedFilter;
     QString fileName = QFileDialog::getOpenFileName(this,
                                                     tr("Open Table Interface"),
                                                     tr("controllers/"),
                                                     tr("UI Files (*.ui)"),
                                                     &selectedFilter,
                                                     options);
     if (!fileName.isEmpty())
         OpenTable(fileName);
}

void MainWindow::OpenTable(QString fileString)
{
    QFile file(fileString);
    OpenTable(file);
}

void MainWindow::OpenTable(QFile &file)
{
    //Load Controller Ui
    if (file.exists())
    {
        QUiLoader loader;
        file.open(QFile::ReadOnly);
        QWidget *LoadedController = loader.load(&file, this);
        file.close();
        if ((!MultipleTable) && (ui->TableDockWidget->layout()->count()))
        {
            QLayoutItem* item;
            while(( item = ui->TableDockWidget->layout()->takeAt(0)) != NULL)
            {
                delete item->widget();
                delete item;
                ui->TableDockWidget->layout()->removeItem(item);
            }
        }

        ui->TableDockWidget->layout()->addWidget(LoadedController);
        foreach(QObject *W, LoadedController->findChildren<QObject *>())
        {
            MultiControlInterface *widget = qobject_cast<MultiControlInterface *>(W);
            if ( widget != NULL )
            {
                //widget->setStyle(m_dialStyle);
                AddController(widget);
            }
        }
        QFileInfo fileInfo(file);
        setCurrentFile(fileInfo.absoluteFilePath());
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    ui->menuOpen_Recent_Table->setProperty("enabled", true);
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        OpenTable(action->data().toString());
}

void MainWindow::AddController(MultiControlInterface *Widget)
{
    Widget->setMidiObject(m_midiserver);
    Widget->setMidiObject(m_midiclient);
    Widget->setOscObject(m_oscserver);
    Widget->setOscObject(m_oscclients);
}

void MainWindow::RemoveController(MultiControlInterface *Widget)
{
    delete Widget;
}

void MainWindow::setMultipleTable()
{
    MultipleTable = ui->actionAllow_multiple_tables->isChecked();
}

void MainWindow::read_settings()
{
  QSettings settings;
  settings.beginGroup( "MainWindow" );
  restoreState( settings.value( "wm_state" ).toByteArray() );
  resize( settings.value( "size", QSize( 400, 400 ) ).toSize() );
  move( settings.value( "pos" ).toPoint() );
  settings.endGroup();
}

void MainWindow::write_settings()
{
  QSettings settings;
  settings.beginGroup( "MainWindow" );
  settings.setValue( "wm_state", saveState() );
  settings.setValue( "size", size() );
  settings.setValue( "pos", pos() );
  settings.endGroup();
}

MainWindow::~MainWindow()
{
    write_settings();
    delete ui;
}

void MainWindow::on_DeckWidget_currentChanged(QWidget *SelectedTab)
{
    if (SelectedTab == ui->DeckAddTab)
    {
        QWidget *newTab = new QWidget(ui->DeckWidget);
        QString newTabName = QString("Deck %1").arg(ui->DeckWidget->count());
        ui->DeckWidget->insertTab(ui->DeckWidget->count() - 1, newTab, newTabName);
        ui->DeckWidget->setCurrentIndex(ui->DeckWidget->count() - 2);
        setScrollDock(newTab);
    }
}
