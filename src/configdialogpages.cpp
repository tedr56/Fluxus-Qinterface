#include <QtGui>
#include <QSettings>
#include <QRegExpValidator>
#include "QIntValidator"
#include "QDebug"
#include "configdialogpages.h"

OscConnections::OscConnections(QWidget *parent) : QWidget(parent),
    changed(false)
{
    //Collect Osc Settings
    QSettings settings;
    settings.beginGroup("OscConfig");
    int OscServerPort = settings.value("serverport", "3333").toInt();
    int OscClientNum = settings.beginReadArray("oscclients");
    settings.endArray();
    QList<QPair<QString, QString> > OscClients;
    if (OscClientNum == 0) {
        settings.setValue("oscclientsnum","1");
        settings.beginWriteArray("oscclients");
        settings.setArrayIndex(0);
        settings.setValue("address", "127.0.0.1");
        settings.setValue("port", "4444");
        OscClientNum = 1;
        settings.endArray();
    }
    OscClientNum = settings.beginReadArray("oscclients");
    for (int i = 0; i < OscClientNum; i++) {
        settings.setArrayIndex(i);
        OscClients << QPair<QString,QString>(settings.value("address").toString(), settings.value("port").toString());
    }
    settings.endArray();
    settings.endGroup();

    QGroupBox *OscInput = new QGroupBox(tr("Osc Input"));
    QLabel *OscInputPortLabel = new QLabel(tr("Input Port"));
    QSpinBox *OscInputPort = new QSpinBox;
    OscInputPort->setMaximum(65536);
    OscInputPort->setValue(OscServerPort);

    QGroupBox *OscOutput = new QGroupBox(tr("Osc Output"));
    QTableWidget *OscOutputTable = new QTableWidget;
    OscOutputTable->setColumnCount(2);
    OscOutputTable->setRowCount(OscClientNum + 1);
    OscOutputTable->setHorizontalHeaderLabels(QStringList() << "Address" << "Port");
    for (int j=0; j < OscClientNum; j++){
        QTableWidgetItem *AddressItem = new QTableWidgetItem(OscClients.value(j).first);
        QTableWidgetItem *PortItem = new QTableWidgetItem(OscClients.value(j).second);
        OscOutputTable->setItem(j, 0, AddressItem);
        OscOutputTable->setItem(j, 1, PortItem);
    }
    QHBoxLayout *OscInputPortLayout = new QHBoxLayout;
    OscInputPortLayout->addWidget(OscInputPortLabel);
    OscInputPortLayout->addWidget(OscInputPort);

    QVBoxLayout *OscInputLayout = new QVBoxLayout;
    OscInputLayout->addLayout(OscInputPortLayout);
    OscInput->setLayout(OscInputLayout);

    QVBoxLayout *OscOutputAddressLayout = new QVBoxLayout;
    OscOutputAddressLayout->addWidget(OscOutputTable);

    QVBoxLayout *OscOutputLayout = new QVBoxLayout;
    OscOutputLayout->addLayout(OscOutputAddressLayout);
    OscOutput->setLayout(OscOutputLayout);

    QVBoxLayout *MainLayout = new QVBoxLayout;
    MainLayout->addWidget(OscInput);
    MainLayout->addWidget(OscOutput);
    setLayout(MainLayout);

    connect(OscInputPort, SIGNAL(editingFinished()), this, SLOT(OscInputChanged()));
    connect(OscOutputTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(OscOutputItemChanged(QTableWidgetItem *)));
}

void OscConnections::OscInputChanged()
{
    QSpinBox *sender = qobject_cast<QSpinBox *>(QObject::sender());
    if (!sender)
        return;
    QSettings settings;
    settings.beginGroup("OscConfig");
    settings.setValue("serverport", sender->value());
    settings.endGroup();
}

void OscConnections::OscOutputItemChanged(QTableWidgetItem * item)
{
    if (!item->text().count()){
        if (item->row() != item->tableWidget()->rowCount() - 1){
            QTableWidget *ItemTable = item->tableWidget();
            ItemTable->removeRow(item->row());
            QSettings settings;
            settings.beginGroup("OscConfig");
            int RowCount = (ItemTable->rowCount() - 1);
            settings.beginWriteArray("oscclients");
            for (int i = 0; i < RowCount; i++){
                settings.setArrayIndex(i);
                settings.setValue("address", ItemTable->item(i, 0)->text());
                settings.setValue("port", ItemTable->item(i,1)->text());
            }
            settings.endArray();
            settings.endGroup();
        }
    } else {
        if (item->row() == item->tableWidget()->rowCount() - 1){
            item->tableWidget()->insertRow(item->tableWidget()->rowCount());
        }
        if ((item->tableWidget()->item(item->row(), 0)) && (item->tableWidget()->item(item->row(), 1))){
            if ((item->tableWidget()->item(item->row(), 0)->text().count()) && item->tableWidget()->item(item->row(), 1)->text().count()){
                QString Octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
                QRegExp rxip = QRegExp();
                rxip.setPattern("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$"); // i.e. the octets can be from 0-199 or 200-249 or 250-255
                QRegExpValidator ValIPAddr(rxip, 0);
                QRegExp rxiprange = QRegExp();
                rxiprange.setPattern("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "(?:-" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + ")?$"); // i.e. the octets can be from 0-199 or 200-249 or 250-255
                QRegExpValidator ValIPRange(rxiprange, 0);
                QString ipText;
                if (item->tableWidget()->item(item->row(),0))
                    ipText = item->tableWidget()->item(item->row(),0)->text();
                int pos = 0;
                int portVal = 0;
                if (item->tableWidget()->item(item->row(),1))
                    if (item->tableWidget()->item(item->row(),1)->text().count())
                        portVal = item->tableWidget()->item(item->row(),1)->text().toInt();
                if (portVal){
                    if ((ValIPAddr.validate(ipText, pos) == QValidator::Acceptable) && (ValIPRange.validate(ipText, pos) == QValidator::Acceptable) && portVal <= 65535 && portVal> 0){
                        QSettings settings;
                        settings.beginGroup("OscConfig");
                        QTableWidget *ItemTable = item->tableWidget();
                        int RowCount = (ItemTable->rowCount() - 1);
                        settings.beginWriteArray("oscclients");
                        for (int i = 0; i < RowCount; i++){
                            settings.setArrayIndex(i);
                            settings.setValue("address", ItemTable->item(i, 0)->text());
                            settings.setValue("port", ItemTable->item(i,1)->text());
                        }
                        settings.endArray();
                        settings.endGroup();
                    }
                }
            }
        }
    }
}


MimeTypes::MimeTypes(QWidget *parent) : QWidget(parent),
    changed(false)
{
    //Colletcts Mime settings
    QSettings settings;
    settings.beginGroup("MimeTypes");
    int entries = settings.beginReadArray("MimeEntry");
    QList< QPair <QString, QString> > MimeEntries;
    for(int i=0; i < entries; i++){
        settings.setArrayIndex(i);
        QString mimeEntryType = settings.value("mimeType", 0).toString();
        QString mimeScript = settings.value("mimeScript", 0).toString();
        QFile *mimeScriptFile = new QFile(mimeScript);
        if ((mimeEntryType.count()) && (mimeScriptFile->exists())) {
            MimeEntries.append(QPair<QString, QString>(mimeEntryType, mimeScript));
        }
    }
    settings.endArray();
    settings.endGroup();

    QGroupBox *MimeGroup = new QGroupBox("Mime Scripts", this);
    QTableWidget *MimeTable = new QTableWidget(this);
    MimeTable->setColumnCount(2);
    MimeTable->setRowCount(MimeEntries.count() + 1);
    MimeTable->setHorizontalHeaderLabels(QStringList() << "Mime" << "Script");
    MimeTable->setItem(MimeTable->rowCount() - 1, 0, new QTableWidgetItem());
    MimeTable->setItem(MimeTable->rowCount() - 1, 1, new QTableWidgetItem());
    for (int i = 0; i < MimeEntries.count(); i++){
        QTableWidgetItem *mime = new QTableWidgetItem(MimeEntries.at(i).first);
        QTableWidgetItem *script = new QTableWidgetItem(MimeEntries.at(i).second);
        MimeTable->setItem(i, 0, mime);
        MimeTable->setItem(i, 1, script);
    }
    MimeTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    QHBoxLayout *MimeEntriesLayout = new QHBoxLayout();
    MimeEntriesLayout->addWidget(MimeTable);

    QVBoxLayout *MimeLayout = new QVBoxLayout();
    MimeLayout->addLayout(MimeEntriesLayout);
    MimeGroup->setLayout(MimeLayout);

    QVBoxLayout *MainLayout = new QVBoxLayout();
    MainLayout->addWidget(MimeGroup);
    setLayout(MainLayout);

    connect(MimeTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(MimeTypeChange(QTableWidgetItem*)));
    connect(MimeTable, SIGNAL(itemPressed(QTableWidgetItem*)), this, SLOT(ScriptChange(QTableWidgetItem*)));
}

void MimeTypes::MimeTypeChange(QTableWidgetItem * item)
{
    if (item->text().count()) {
        changed = true;
        SaveSettings(item->tableWidget());
        if (item->row() == item->tableWidget()->rowCount() - 1){
            item->tableWidget()->blockSignals(true);
            item->tableWidget()->insertRow(item->tableWidget()->rowCount());
            item->tableWidget()->setItem(item->tableWidget()->rowCount() - 1, 0, new QTableWidgetItem());
            item->tableWidget()->setItem(item->tableWidget()->rowCount() - 1, 1, new QTableWidgetItem());
            item->tableWidget()->blockSignals(false);
        }
    } else  if (item->tableWidget()->rowCount() > 1){
        item->tableWidget()->removeRow(item->row());
    }
}

void MimeTypes::ScriptChange(QTableWidgetItem * item)
{
    QTableWidget *TableWidget = item->tableWidget();
    if (item->column() == 1){
        //QFileDialog::Options script;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mime Script"));
        TableWidget->blockSignals(true);
        if (!fileName.isEmpty()){
            item->setText(fileName);
        } else {
            item->setText("");
        }

        if (!fileName.isEmpty()) {
            changed = true;
            SaveSettings(TableWidget);
            if (item->row() == (TableWidget->rowCount() - 1)){
                TableWidget->insertRow(item->tableWidget()->rowCount());
                TableWidget->setItem(item->tableWidget()->rowCount() - 1, 0, new QTableWidgetItem());
                TableWidget->setItem(item->tableWidget()->rowCount() - 1, 1, new QTableWidgetItem());
            }
        }
        TableWidget->blockSignals(false);
    }
}

void MimeTypes::SaveSettings(QTableWidget *Table)
{
    QSettings settings;
    settings.beginGroup("MimeTypes");
    settings.beginWriteArray("MimeEntry");
    for(int i = 0; i < Table->rowCount(); i++){
        if ((!Table->item(i,0)->text().isEmpty()) && (!Table->item(i,1)->text().isEmpty())){
            settings.setArrayIndex(i);
            settings.setValue("mimeType", Table->item(i,0)->text());
            settings.setValue("mimeScript", Table->item(i,1)->text());
        }
    }
    settings.endArray();
    settings.endGroup();
}
