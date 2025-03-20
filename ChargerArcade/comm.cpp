#include "Charcade.h"
#include <QTcpServer>
#include <QTcpSocket>

void Screen::ConnectToPeers()
{
    for(int i=ID; i < TerminalCount; i++)
    {
        //qDebug() << "connectiong to " << i;
        if(connectToPeer(i))
        {
            //qDebug() << qPrintable(Sign) << qPrintable(">") << qPrintable(ip) << ":" << port;
            qDebug() << qPrintable(Sign) << qPrintable(">") << qPrintable(QString::number(i+1));
            //socket->write("Hello from peer");
            registerConnection(i, peerSockets[i]);
        }
        else
            qDebug() << qPrintable(Sign) << qPrintable("}") << qPrintable(QString::number(i+1));
    }
}

bool Screen::connectToPeer(int id)
{
    const QString ip = addresses[id];
    QStringList address = ip.split(":");
    QTcpSocket *socket = peerSockets[id]/*new QTcpSocket(this)*/;

    socket->connectToHost(address[0], address[1].toInt());
    socket->write(QString::number(ID-1).toUtf8());

    return socket->waitForConnected(2000);

}




void Screen::handleServerConnection() // Handles new incoming connections
{
    QTcpSocket *newClient = server->nextPendingConnection();
    connect(newClient, &QTcpSocket::readyRead, this, [this, newClient]() {handleClientMessage(newClient, -1);});





    /*QString add = newClient->peerAddress().toString()+":"+QString::number(newClient->peerPort());
    add.replace("::ffff:","");
    /*for(int i=0; i < TerminalCount; i++)
    {
        if(addresses[i] == add)
        {
            qDebug() << " FOUND ID";
            id=i;
        }
        else
        {
            qDebug() << addresses[i] << " is not " << add;
        }
    }*/

}


void Screen::handleClientMessage(QTcpSocket *client, int id) // Reads messages from connected peers
{
    //QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
if (!client) return;
    QString ip = client->peerAddress().toString();
    QByteArray dataa = client->readAll();
    QString data = dataa;
    data.split("=:=");
    QString act=data[0];
    QString comm=data[1];
    if(id < 0)
    {
        disconnect(client);
        id = std::atoi(dataa);
        //qDebug() << Sign << qPrintable("id is " + QString::number(id) + " from string " + data);
        registerConnection(id, client);
        QString ip = client->peerAddress().toString();
        ip = ip.replace("::ffff:", "");
        //QString a = ip+":"+QString::number(newClient->peerPort());
        //qDebug() << qPrintable(Sign) << qPrintable("<") << qPrintable(a);
        qDebug() << " " << qPrintable(Sign) << qPrintable("<") << qPrintable(QString::number(id+1));
    }
    if(act == "startGame")
    {

    }
    else
        qDebug() << qPrintable(Sign) << qPrintable("-") << qPrintable(QString::number(id)) << qPrintable(""+data);
    //client->write("Message received");
}


void Screen::SetUpServer() // Initializes the server and starts listening for connections
{
    QString ip = addresses[ID-1];
    QStringList address = ip.split(":");
    int port = address[1].toInt();
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Screen::handleServerConnection);
    server->listen(QHostAddress::Any, port);
    qDebug() << qPrintable(Sign) << qPrintable("Server started");
}


void Screen::registerConnection(int id, QTcpSocket* zokket)
{
    if(zokket != peerSockets[id])
    {
        delete peerSockets[id];
        peerSockets[id] = zokket;
    }
    if(DoStatus)
    {
        StatusIcons[id]->hide();
        bar.removeWidget(StatusIcons[id]);
    }

    connect(zokket, &QTcpSocket::readyRead, this, [this, zokket, id]() {handleClientMessage(zokket, id);});
    connect(zokket, &QTcpSocket::disconnected, this, [this, zokket, id]() {
        handleDisconnect(zokket, id);
    });



    int spot=-1;
    spot = disconnectedPeers.indexOf(id);
    if(spot!=-1)
    {
    disconnectedPeers.remove(spot);
    if(disconnectedPeers.size()==0)
        reconnectTimer.stop();
    }
    connections[id] = true;
    //qDebug() << "New connection made to " << addresses[id] << ":" << ports[id];
}




/*int findIndexOfString(const QVector<QString>& vec, const QString& target) {
    // Use std::find to search for the target string in the QVector
    auto it = std::find(vec.begin(), vec.end(), target);

    // If found, return the index; otherwise, return -1 to indicate not found
    if (it != vec.end()) {
        return std::distance(vec.begin(), it);  // Calculate index from iterator
    }

    return -1;  // Return -1 if the string is not found
}*/


void Screen::handleDisconnect(QTcpSocket *zokket, int id)
{
    //QString ip = (zokket->peerAddress().toString())+":"+QString::number(zokket->peerPort());
    //ip = ip.replace("::ffff:", "");
    //qDebug() << "disconnected address is " << ip;
    //int id = findIndexOfString(addresses, ip);
    qDebug() << qPrintable(Sign) << qPrintable("X ") << qPrintable(QString::number(id));
    if(DoStatus)
    {
        StatusIcons[id]->show();
        bar.addWidget(StatusIcons[id]);
        bar.setAlignment(StatusIcons[id],Qt::AlignLeft);
    }
    connections[id] = false;

    if(id > ID-1)
    {
        disconnectedPeers.append(id);
        if(disconnectedPeers.size()==1)
            reconnectTimer.start();
    }
}






void Screen::Reconnect()
{
    for(int id:disconnectedPeers)
    {
        qDebug() << "reconnecting to " << id;
        connectToPeer(id);
    }
}














void Screen::sendMessage(const QString &message, int id)
{
    QTcpSocket* socket = peerSockets[id];
    if (connections[id])
    {
        socket->write("8");
        socket->flush();
        qDebug() << qPrintable(Sign) << qPrintable("Sent message:") << message << qPrintable("to "+QString::number(id));
    }
   // else
        //qDebug() << "Error: Not connected to the selected peer.";
}

// Sends a message to all connected peers
void Screen::broadcastMessage(const QString &message)
{
    for (int i=0; i < TerminalCount-1; i++)
        sendMessage(message, IdReference[i]);
}

void Screen::Connect()
{
    if(ID != TerminalCount)
        ConnectToPeers();
}

void Screen::DisconnectAll()
{
    for(int i=0; i < TerminalCount; i++)
        delete(peerSockets[i]);
    delete(server);
}
