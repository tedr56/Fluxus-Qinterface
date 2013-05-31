#include <QtGui/QApplication>
#include "mainwindow.h"
#include "src/constants.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(QSTR_DOMAIN);
    QCoreApplication::setOrganizationDomain(QSTR_DOMAIN);
    QCoreApplication::setApplicationName(QSTR_APPNAME);

    QApplication a(argc, argv);
    iVjServer w;
    w.show();
    
    return a.exec();
}
