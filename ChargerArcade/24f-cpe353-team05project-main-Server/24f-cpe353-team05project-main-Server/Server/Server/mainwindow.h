#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QObject>
#include <QtDebug>
#include <QNetworkInterface>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);//commit comment
    ~MainWindow();
    QUdpSocket* sendS=nullptr;

signals:
    void finish();
private:
    Ui::MainWindow *ui;
    QString greetings[4];
    QUdpSocket* sockServer=nullptr;


    QDialog* gameWindow;
    Game* activeGame;
    QString ServerIp;
    QString Players[4];
    QStringList ipList;
    int portList[4];
    int listSize = 0;
    QByteArray* PlayerMsg;
    int playcnt = 0;
    int playersReady = 0;
    QString PlayerColors[4] = {"#0099FF", "#FF3333", "#00CC33", "#FFCC00"}; //temporary colors (blue, red, green, and yellow) I decided to use, doesn't have to be final
    void SendSQL(QString ip);
    QString SQLDatabaseName;
    QSqlDatabase db;

private slots:
    void postPlayer(QString name);
    void readFromPlayer();
    void openServer();
    void readSQL();
    void ChangeSQLScore(int playerid, int newscore);
    void closes();

};

#endif // MAINWINDOW_H


