#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QFile>
#include <QHash>
#include "classicstyle.h"
#include "multicontrolinterface.h"
#include "qmidiserver.h"
#include "qmidiclient.h"
#include "qoscserver.h"
#include "qoscclient.h"
#include "configdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setOpenTable();
    void setPreferences();
    void setMultipleTable();
    void openRecentFile();
    void on_actionAdd_Media_Deck_activated();

protected:

private:
    Ui::MainWindow *ui;

    bool MultipleTable;

    QMidiServer *m_midiserver;
    QMidiClient *m_midiclient;
    QOscServer *m_oscserver;
    QOscClient *m_oscclient;
    QList<QOscClient*> m_oscclients;

    int m_currentOut;
    int m_currentIn;
    bool m_inputActive;
    bool m_midiThru;
    bool m_initialized;
    bool m_osc_server_initialized;

    ConfigDialog *m_configDialog;

    QStyle* m_dialStyle;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];

    void initConnections();
    void initMidiConnections();
    void initOscConnections();
    void initUi();
    void initActions();
    void initMimeConf();

    void setScrollDock(QWidget *);

    void OpenTable(QFile &file);
    void OpenTable(QString fileString);
    void AddController(MultiControlInterface *Widget);
    void RemoveController(MultiControlInterface *Widget);

    void read_settings();
    void write_settings();
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
};

#endif // MAINWINDOW_H
