#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QHostAddress>
#include <game.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sockServer=new QUdpSocket(this);
    connect(ui->openB, SIGNAL(clicked()), this, SLOT(openServer()));
    connect(sockServer,SIGNAL(readyRead()),this,SLOT(readFromPlayer()));

    ///////////////////////////////////////////SQL SETUP////////////////////////////////////////////////
    db = QSqlDatabase::addDatabase("QSQLITE"); // Using SQLite
    /*db.setDatabaseName("tracker.db");

    if (!db.open())
        qDebug() << "Database error: " << db.lastError().text();

    QSqlQuery query;
    if (!query.exec("DROP TABLE IF EXISTS players"))
        qDebug() << "Error dropping table: " << query.lastError().text();

    query.exec("CREATE TABLE IF NOT EXISTS players (name TEXT NOT NULL,score INTEGER NOT NULL)");
*/

}

MainWindow::~MainWindow()
{
    QApplication::quit();
    delete ui;
}

void MainWindow::closes()
{
    QApplication::quit();
}
void MainWindow::SendSQL(QString ip)
{
     QString Mul = "BIGMAN";
     QString relativePath = "players.db";

         QFileInfo fileInfo(relativePath);
         QString absolutePath = fileInfo.absoluteFilePath();

         qDebug() << "Absolute path:" << absolutePath;

     QByteArray name;
     name.append(absolutePath);
     sendS = new QUdpSocket(this);
     sendS->connectToHost(ip, (ui->portEdit->text()).toInt()+1, QIODevice::ReadWrite); //Connect to server
     sendS->write(name);
}

void MainWindow::postPlayer(QString name) {
    if(playcnt<4) {//player count less than 4
        ui->textBrowser->append(name);
        ui->textBrowser->append("Added PLayer");
        ui->textBrowser->append("Waiting for more Players");//s
        activeGame->makePlayer(playcnt, ipList.at(playcnt), name);

        playcnt++;//limits player count




    }
}

void MainWindow::openServer() {
    ui->textBrowser->append("Dont Try to Open a second Game after first is done");
    int port = (ui->portEdit->text()).toInt();//retrieve port number
    SQLDatabaseName = ui->SQLdbName->text();
    QString tablename = SQLDatabaseName;
    SQLDatabaseName.append(".db");
    db.setDatabaseName(SQLDatabaseName);

    sockServer->bind(QHostAddress(this->windowTitle()), port);  //bind ip and port
    ui->textBrowser->append("ServerOpen Waiting for players");
    activeGame = new Game();//NEEDS TO BE DONE create new game then add player object too that new game. This will allow us too test movement without actually starting the game yet
    QObject::connect(activeGame, SIGNAL(end()), this, SLOT(closes()));
    activeGame->makeTable(db, tablename);
    delete ui->openB;
}

void MainWindow::readFromPlayer() {
    while (sockServer->hasPendingDatagrams()) {//process multiple messages one at a time
        PlayerMsg = new QByteArray;
        PlayerMsg->resize(sockServer->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        sockServer->readDatagram(PlayerMsg->data(),PlayerMsg->size(),&sender,&senderPort);
        QString SMsg(PlayerMsg->data());
        QStringList PlayerString = SMsg.split('/');
        QString PlayerIp=PlayerString.at(0);
        QString AuthenticationClient=PlayerString.at(1);//retrieves ip
        int Ipin=4;
        for(int i=0; i<ipList.length(); i++)
        {
            if(ipList.at(i)==PlayerIp)
            {
                Ipin=i;
                break;
            }
        }
        if(Ipin!=4)
        {
            if(!activeGame->playerList.at(Ipin)->dead)
            {
                if(AuthenticationClient == "Move Forward")
                {
                    activeGame->playerList.at(Ipin)->MoveForward();
                }
                else if(AuthenticationClient=="Rotate Left")
                {
                    activeGame->playerList.at(Ipin)->RotateLeft();
                }
                else if(AuthenticationClient=="Rotate Right")
                {
                    activeGame->playerList.at(Ipin)->RotateRight();
                }
                else if(AuthenticationClient=="Fire")
                {
                    activeGame->spawnPro(Ipin);
                }
            }
        }
        if(AuthenticationClient == "Multiplayer Spacerocks?" && ipList.length()<4 && Ipin==4)//account for first element
        {
            ipList.append(PlayerIp);//how we will track players
            postPlayer(PlayerString.at(2));//addplayer too game
            Players->append(PlayerString.at(2));
            SendSQL(PlayerIp);
        }


    }
}


void MainWindow::readSQL()                                       //this function reads the table (to the debug output for now)
{
    QSqlQuery query;
    if (!query.exec("SELECT name, color, score FROM players"))
    {
        qDebug() << "Error reading from database:" << query.lastError().text();
        return;
    }

    qDebug() << "Players in database:";

    while (query.next())
    {
        QString name = query.value(0).toString();
        QString color = query.value(1).toString();
        int score = query.value(2).toInt();

        qDebug()<<"Name:"<<name<<", Color:"<<color<<", Score:"<<score;
    }
}

void MainWindow::ChangeSQLScore(int playerid, int newscore)
{
    QSqlQuery query;
    query.prepare("UPDATE players SET score = :newScore WHERE id = :id");
    query.bindValue(":newScore", newscore);
    query.bindValue(":id", playerid);

    if (!query.exec())
        qDebug() << "Error updating player score:" << query.lastError().text();

}
