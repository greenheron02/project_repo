#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtDebug>
#include <QString>
#include <QList>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <string>
#include <cstring>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlError>

namespace Ui {
class MainWindow;
}
//commit comments
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;

    QByteArray *A;
    QByteArray *D;
    QByteArray *S;
    QString W_Key = "Move Forward";
    QString A_Key = "Rotate Left";
    QString D_Key = "Rotate Right";
    QString S_Key = "Slow Down";
    QString Proj_Key = "Fire";
    QUdpSocket* clientSocket;
    QUdpSocket* sqlr;
    QGraphicsScene* game = nullptr;
    QGraphicsView* view = nullptr;
    QString myIP;
    QSqlQuery qprep;

    QString serverIP="1";

    bool dontFire=0;
    QTimer* projWait;

private slots:
    void connectToServer();
    void findSIP();
    void SendControl(QString str);
    void endt();
    void retrieveSQL();

};

#endif // MAINWINDOW_H
