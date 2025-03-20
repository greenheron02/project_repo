#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
    sqlr=new QUdpSocket(this);
    connect(sqlr,SIGNAL(readyRead()),this,SLOT(retrieveSQL()));

    QList<QHostAddress> list = QNetworkInterface::allAddresses(); //Creats a list of ip addresses on the machine
    for(int index = 0; index < list.count(); index++) {  //For loop to determine your ip address
        if( !list[index].isLoopback() ) {
            if ( list[index].protocol() == QAbstractSocket::IPv4Protocol ) {
                myIP = list[index].toString();
            }
        }
    }
    this->setWindowTitle(myIP);    // Places local IP into title bar of program window
    projWait= new QTimer;
    connect(projWait, SIGNAL(timeout()), this, SLOT(endt()));
    projWait->setInterval(480);


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::retrieveSQL()
{
    QByteArray* PlayerMsg = new QByteArray;
    PlayerMsg->resize(sqlr->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    sqlr->readDatagram(PlayerMsg->data(),PlayerMsg->size(),&sender,&senderPort);
    qDebug() << PlayerMsg;
    QString SMsg(PlayerMsg->data());
    qDebug() << SMsg;


}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if(e) {
        switch (e->key()) { //switch case for user inputs
            case Qt::Key_W:
            case Qt::Key_I:
                SendControl(W_Key);
                break;
            case Qt::Key_S:
            case Qt::Key_K:
                SendControl(S_Key);
            break;
            case Qt::Key_A:
            case Qt::Key_J:
                SendControl(A_Key);
                break;
            case Qt::Key_D:
            case Qt::Key_L:
                SendControl(D_Key);
                break;
            case Qt::Key_Q:
            case Qt::Key_P:
                if(dontFire==0)
                {
                    SendControl(Proj_Key);
                }
                break;

            default:    qDebug() <<"ignore";
                break;
        }
    }
}

void MainWindow::connectToServer() {
    sqlr->bind(QHostAddress(this->windowTitle()), (ui->portEdit->text()).toInt()+1);  //bind ip and port
    QString Mul = "Multiplayer Spacerocks?";
    QString divider = "/";    //divider between different strings for list
    QByteArray name; //Wrap message in correct format
    name.append(myIP.toUtf8());
    name.append(divider.toUtf8());
    name.append(Mul.toUtf8());
    name.append(divider.toUtf8());
    findSIP();
    name.append((ui->lineEdit->text()).toUtf8());   //Append player name
    qDebug() << name;
    clientSocket->write(name);  //Send message
    ui->infoBrowser->append("Player Name =" + ui->lineEdit->text());    //Write to textBrowser

    //connect(clientSocket, SIGNAL(readyRead()), this, SLOT(foundServer()));

}


void MainWindow::findSIP()
{
    int port = (ui->portEdit->text()).toInt();//retrieve port number
    serverIP = ui->IpEdit->text();

//insert Ip code here. Must iterate through machine and room numbers and if a server is found let the user know

    clientSocket = new QUdpSocket(this);    //Socket for client
    clientSocket->connectToHost(serverIP, port, QIODevice::ReadWrite); //Connect to server

}

void MainWindow::SendControl(QString str)
{
    if(str==Proj_Key)
    {
        if(dontFire==0)
        {
        dontFire=1;
        projWait->start();
        }
        else {
            return;
        }
    }
    qDebug() << str;
    if(serverIP!="1")
    {
    QString divider = "/";
    QByteArray ba;
    ba.append(myIP.toUtf8());
    ba.append(divider.toUtf8());
    ba.append(str.toUtf8());
    clientSocket->write(ba);
    }
}
void MainWindow::endt()
{
    projWait->stop();
    dontFire=0;
}
