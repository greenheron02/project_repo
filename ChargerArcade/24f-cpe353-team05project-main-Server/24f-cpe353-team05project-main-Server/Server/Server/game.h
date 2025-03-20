#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <QGraphicsScene>
#include <QtSql>
#include <QLayout>
#include <QGraphicsView>
#include <QPainter>
#include <QTimer>
#include <QGraphicsItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlError>
#include <cstdlib>
#include <list>
#include "player.h"
#include "projectile.h"
#include "asteroids.h"
#include <QTextBrowser>
#include <QLCDNumber>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game();
    ~Game();
    QDialog* gameDialog;
    QString players[4];
    int playercnt=0;
    void getPlayers(QString Players[4], int* playercnt);
    void makePlayer(int, QString, QString);
    void spawnPro(int i);
    void killPlayer(player* killed);
    void astmake(Asteroids*);
    void endGame();
    QList<player*> playerList;
    QList<Asteroids*> asList;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlQueryModel* model;
    QSqlTableModel* tm;
    QTableView* tracker;
    void makeTable( QSqlDatabase d, QString Tablename);
signals:
    void score();
    void end();

private:
    QVBoxLayout* lay = nullptr;
    QHBoxLayout* largeLay = nullptr;
    QGraphicsView* gameView = nullptr;
    QGraphicsScene* gameScene = nullptr;
    const int SCENE_WIDTH = 640;
    const int SCENE_HEIGHT = 420;
    int FRAME_RATE = 24;
    QString IpClient;
    QByteArray* PlayerMsg;
    QList<Projectile*> projList;
    QTextBrowser* PlayerScores;
    int portList[4];
    QTimer* asSpawner;
    QLCDNumber* gameTimer;
    QTimer *GameTimer;
    int GameTimeDisplay;
    bool GameStart = 0;
    void UPDATETABLE(int id, player* p, int score);
    void addrow(int id, player* p);

private slots:
    //collision for any interaction which requires interactions out of the respective object's scope like score.
    void ProCol(Projectile* p);
    void playCol(player *p);
    void spawnAsteroid();
    void proTimeout(Projectile* p);
    void tick();
};

#endif // GAME_H
