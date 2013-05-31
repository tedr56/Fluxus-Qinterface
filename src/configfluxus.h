#ifndef CONFIGFLUXUS_H
#define CONFIGFLUXUS_H

#include <QWidget>
#include <QTableWidget>
#include <QWizard>
#include <QValidator>

class ConfigDialog;
class QLabel;
class QLineEdit;
class QFileInfo;

struct FluxusHead {
    bool activated;
    QString name;
    QString address;
    QString directory;
    bool interface;
};
Q_DECLARE_METATYPE(FluxusHead)

class configFluxus : public QWidget
{
    Q_OBJECT
    friend class ConfigDialog;
public:
    explicit configFluxus(QWidget *parent = 0);
    
signals:
    
public slots:
    void FlxWizard();
    void WizardResult();
protected:
    void AddHead(FluxusHead *FlxHead, int Row);
    void AddHead(FluxusHead *FlxHead) {AddHead(FlxHead, false);};
private:
    QTableWidget *FlxTable;
    bool changed;
    QList<FluxusHead*> FluxusHeads;
};

class FluxusInfoPage : public QWizardPage
{
    Q_OBJECT
public:
    FluxusInfoPage(QWidget *parent = 0);
    int nextId() const;
    FluxusHead *getFluxusHead();
private:
    QLineEdit *headName;
    QLineEdit *ipAddress;
    QLineEdit *flxDir;
};

class FluxusWizard : public QWizard
{
    Q_OBJECT
public:
    enum { Page_Info };
    FluxusWizard(QWidget *parent = 0);
    FluxusHead *getFluxusHead();
private:
    FluxusInfoPage *FlxInfo;
};

class DirectoryValidator : public QValidator
{
    Q_OBJECT
public:
    DirectoryValidator( QObject * parent = 0 );
    State validate ( QString & input, int & pos ) const;
};

#endif // CONFIGFLUXUS_H
