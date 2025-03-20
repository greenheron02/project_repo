#include "mainwindow.h"
#include <QApplication>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QObject>
int main(int argc, char *argv[])
{
    QString ServerIp;
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow;
    w->show();
    QList<QHostAddress> list = QNetworkInterface::allAddresses(); //reads ip address
    for(int index = 0; index < list.count(); index++) {   //For loop to determine your ip address.commit
        if( !list[index].isLoopback() ) {
            if ( list[index].protocol() == QAbstractSocket::IPv4Protocol ) {
                ServerIp = list[index].toString();
            }
        }
    }
    w->setWindowTitle(ServerIp);
    return a.exec();
}
