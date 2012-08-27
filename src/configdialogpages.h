#ifndef CONFIGDIALOGPAGES_H
#define CONFIGDIALOGPAGES_H

#include <QWidget>
#include <QTableWidget>

//class MidiConnections : public QWidget
//{
//public:
//    MidiConnections(QWidget *parent = 0);
//};

class ConfigDialog;

class OscConnections : public QWidget
{
    Q_OBJECT
    friend class ConfigDialog;
public:
    OscConnections(QWidget *parent = 0);
public slots:
    void OscInputChanged();
    void OscOutputItemChanged(QTableWidgetItem *);
private:
    bool changed;
};

class MimeTypes : public QWidget
{
    Q_OBJECT
    friend class ConfigDialog;
public:
    MimeTypes(QWidget *parent = 0);
public slots:
    void MimeTypeChange(QTableWidgetItem *);
    void ScriptChange(QTableWidgetItem *);
private:
    void SaveSettings(QTableWidget *);
    bool changed;
};

#endif
