#include "configfluxus.h"
#include <QtGui>
#include <QSettings>
#include <QBool>
#include <QLibraryInfo>

configFluxus::configFluxus(QWidget *parent) :
    QWidget(parent),
    changed(false)
{
    //Collect FluxusHeads Settings
    QSettings settings;
    settings.beginGroup("FluxusConfig");

    int FlxNum = settings.beginReadArray("FluxusHeads");
    for (int i = 0; i < FlxNum; i++) {
        settings.setArrayIndex(i);
        FluxusHead* FlxList = new FluxusHead;
        FlxList->activated = settings.value("activated").toBool();
        FlxList->name = settings.value("name").toString();
        FlxList->address = settings.value("address").toString();
        FlxList->interface = settings.value("interface").toBool();
        FluxusHeads << FlxList;
    }
    settings.endArray();
    settings.endGroup();

    //Draw FluxusHeads List

    QGroupBox *FlxGroup = new QGroupBox(tr("FluxusBros Heads"));
    FlxTable = new QTableWidget;
    FlxTable->setColumnCount(4);
    FlxTable->setRowCount(FlxNum);
    FlxTable->setHorizontalHeaderLabels(QStringList() << tr("Activated") << tr("Name") << tr("Address") << tr("Interface"));
    //FlxTable->horizontalHeader()->setStretchLastSection(true);
    FlxTable->horizontalHeader()->setResizeMode(0, QHeaderView::Custom);
    FlxTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    FlxTable->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    FlxTable->horizontalHeader()->setResizeMode(3, QHeaderView::Custom);
    FlxTable->horizontalHeader()->resizeSection(0, 100);
    FlxTable->horizontalHeader()->resizeSection(3, 100);
    for (int j=0; j < FlxNum; j++){
        QTableWidgetItem *ActivatedItem = new QTableWidgetItem();
        ActivatedItem->setCheckState(FluxusHeads.value(j)->activated ? Qt::Checked : Qt::Unchecked);
        QTableWidgetItem *AddressItem = new QTableWidgetItem(FluxusHeads.value(j)->address);
        QTableWidgetItem *NameItem = new QTableWidgetItem(FluxusHeads.value(j)->name);
        QTableWidgetItem *InterfaceItem = new QTableWidgetItem();
        InterfaceItem->setCheckState(FluxusHeads.value(j)->interface ? Qt::Checked : Qt::Unchecked);
        FlxTable->setItem(j, 0, ActivatedItem);
        FlxTable->setItem(j, 1, NameItem);
        FlxTable->setItem(j, 2, AddressItem);
        FlxTable->setItem(j, 3, InterfaceItem);
    }

    QPushButton* RemoveFlx = new QPushButton("Remove", FlxGroup);
    RemoveFlx->hide();
    connect(FlxTable, SIGNAL(itemSelectionChanged()), RemoveFlx, SLOT(show()));
    QPushButton* AddFlx = new QPushButton("Add", FlxGroup);
    connect(AddFlx, SIGNAL(clicked()), this, SLOT(FlxWizard()));
    QPushButton* EditFlx = new QPushButton("Edit", FlxGroup);
    EditFlx->hide();
    connect(FlxTable, SIGNAL(itemSelectionChanged()), EditFlx, SLOT(show()));

    QHBoxLayout *FlxButtons = new QHBoxLayout;
    FlxButtons->addWidget(AddFlx);
    FlxButtons->addWidget(EditFlx);
    FlxButtons->addWidget(RemoveFlx);

    QVBoxLayout *FluxusLayout = new QVBoxLayout;
    FlxGroup->setLayout(FluxusLayout);
    FluxusLayout->addWidget(FlxTable);
    FluxusLayout->addLayout(FlxButtons);

    QVBoxLayout *MainLayout = new QVBoxLayout;
    MainLayout->addWidget(FlxGroup);
    setLayout(MainLayout);
}

void configFluxus::FlxWizard()
{
    FluxusWizard* wiz = new FluxusWizard ;
    //connect(wiz, SIGNAL(accepted()), this, SLOT(WizardResult()));
    int returnCode = wiz->exec();
    if (returnCode){
        AddHead(wiz->getFluxusHead());
    }
}

void configFluxus::WizardResult()
{
    FluxusWizard *FlxWidget = qobject_cast<FluxusWizard *>(QObject::sender());
    if (FlxWidget){
        AddHead(FlxWidget->getFluxusHead());
    }
}

void configFluxus::AddHead(FluxusHead* FlxHead, int Row)
{
    int registerRow = Row;
    if (Row == false){
        Row = registerRow;
        registerRow = FlxTable->rowCount();
        Row = registerRow;
        FlxTable->insertRow(registerRow);
    }
    QTableWidgetItem *ActivatedItem = new QTableWidgetItem();
    ActivatedItem->setCheckState(FlxHead->activated ? Qt::Checked : Qt::Unchecked);
    QTableWidgetItem *AddressItem = new QTableWidgetItem(FlxHead->address);
    QTableWidgetItem *NameItem = new QTableWidgetItem(FlxHead->name);
    QTableWidgetItem *InterfaceItem = new QTableWidgetItem();
    InterfaceItem->setCheckState(FlxHead->interface ? Qt::Checked : Qt::Unchecked);
    FlxTable->setItem(registerRow, 0, ActivatedItem);
    FlxTable->setItem(registerRow, 1, NameItem);
    FlxTable->setItem(registerRow, 2, AddressItem);
    FlxTable->setItem(registerRow, 3, InterfaceItem);
    if (Row == false){
        FluxusHeads << FlxHead;
        QSettings settings;
        settings.beginGroup("FluxusConfig");
        settings.beginWriteArray("FluxusHeads");
        for (int i = 0; i < FluxusHeads.length(); i++) {
            settings.setArrayIndex(i);
            FluxusHead* FlxList = FluxusHeads[i];
            settings.setValue("activated", FlxList->activated);
            settings.setValue("name", FlxList->name);
            settings.setValue("address", FlxList->address);
            settings.setValue("directory", FlxList->directory);
            settings.setValue("interface", FlxList->interface);
        }
        settings.endArray();
        settings.endGroup();
        changed = true;
    }
}

FluxusWizard::FluxusWizard(QWidget *parent) : QWizard(parent)
{
    FlxInfo = new FluxusInfoPage;
    setPage(Page_Info, FlxInfo);
    setStartId(Page_Info);
    setOptions(QWizard::NoBackButtonOnStartPage);
    setWindowTitle(tr("FluxusBros Head Wizard"));
}

FluxusHead* FluxusWizard::getFluxusHead()
{
    return FlxInfo->getFluxusHead();
}

FluxusInfoPage::FluxusInfoPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("FluxusBros Head Infos"));

    QString Octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp rxiprange = QRegExp();
    rxiprange.setPattern("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "(?:-" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + ")?$"); // i.e. the octets can be from 0-199 or 200-249 or 250-255
    QRegExpValidator* ValIPRange = new QRegExpValidator(rxiprange);

    DirectoryValidator* DirValid = new DirectoryValidator(this);

    QLabel *nameLabel = new QLabel(tr("FluxusBros Head Name"), this);
    headName = new QLineEdit(this);

    QLabel *ipLabel = new QLabel("Ip Address", this);
    ipAddress = new QLineEdit(this);
    ipAddress->setValidator(ValIPRange);

    QLabel *DirectoryLabel = new QLabel(tr("FluxusBros Directory"), this);
    flxDir = new QLineEdit(this);
    flxDir->setValidator(DirValid);
    QPushButton *DirectorySearch = new QPushButton("...", this);
    QFileDialog *DirectoryDialog = new QFileDialog(this, tr("Open FluxusBros Directory"), "../");
    DirectoryDialog->setOptions(QFileDialog::DontUseNativeDialog);
    DirectoryDialog->setFileMode(QFileDialog::Directory);
    connect(DirectorySearch, SIGNAL(clicked()), DirectoryDialog, SLOT(exec()));
    connect(DirectoryDialog, SIGNAL(fileSelected(QString)), flxDir, SLOT(setText(QString)));

    QLabel *interfaceLabel = new QLabel(tr("Connect to remote interface"), this);
    QCheckBox *interfaceCheck = new QCheckBox(this);

    registerField(QString("FlxName*"), headName);
    registerField(QString("FlxAddress*"), ipAddress);
    registerField(QString("FlxDirectory*"), flxDir);
    registerField(QString("FlxInterface"), interfaceCheck);

    QHBoxLayout *NameLayout = new QHBoxLayout;
    NameLayout->addWidget(nameLabel);
    NameLayout->addWidget(headName);

    QHBoxLayout *ipLayout = new QHBoxLayout;
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipAddress);

    QHBoxLayout *DirectoryLayout = new QHBoxLayout;
    DirectoryLayout->addWidget(DirectoryLabel);
    DirectoryLayout->addWidget(flxDir);
    DirectoryLayout->addWidget(DirectorySearch);

    QHBoxLayout *interfaceCheckLayout = new QHBoxLayout;
    interfaceCheckLayout->addWidget(interfaceLabel);
    interfaceCheckLayout->addWidget(interfaceCheck);
    interfaceCheckLayout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QVBoxLayout *FlxInfosLayout = new QVBoxLayout;
    FlxInfosLayout->addLayout(NameLayout);
    FlxInfosLayout->addLayout(ipLayout);
    FlxInfosLayout->addLayout(DirectoryLayout);
    FlxInfosLayout->addLayout(interfaceCheckLayout);

    setLayout(FlxInfosLayout);
}

int FluxusInfoPage::nextId() const
{
    return -1;
}

FluxusHead* FluxusInfoPage::getFluxusHead()
{
    FluxusHead *FlxH = new FluxusHead;
    FlxH->activated = true;
    FlxH->name = field(QString("FlxName")).toString();
    FlxH->address = field(QString("FlxAddress")).toString();
    FlxH->directory = field(QString("FlxDirectory")).toString();
    FlxH->interface = field(QString("FlxInterface")).toBool();
    return FlxH;
}

DirectoryValidator::DirectoryValidator(QObject *parent) : QValidator(parent)
{
}

QValidator::State DirectoryValidator::validate(QString &input, int &pos) const
{
    QFileInfo fileInfo(input.right(pos));
    if (fileInfo.isDir() && fileInfo.isWritable()) {
        return QValidator::Acceptable;
    } else {
        return QValidator::Invalid;
    }
}

