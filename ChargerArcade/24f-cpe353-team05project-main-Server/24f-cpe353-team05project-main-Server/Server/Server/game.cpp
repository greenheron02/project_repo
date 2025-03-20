#include "game.h"
#include <QTimer>
#include "mainwindow.h"
#include <QObject>
#include "player.h"


Game::Game()
{
    gameDialog = new QDialog;
    gameDialog->resize(1800, 1000);

    player* p = new player;
    p->brush=QBrush(Qt::red);
    p->setRotation(135);
    p->setPos(100,100);
    playerList.append(p);
    connect(p, SIGNAL(hit(player*)), this, SLOT(playCol(player*)));
    player* q = new player;
    q->brush=QBrush(Qt::blue);
    playerList.append(q);
    q->setRotation(45);
    q->setPos(100,-100);
    connect(q, SIGNAL(hit(player*)), this, SLOT(playCol(player*)));
    player* r = new player;
    r->brush=QBrush(Qt::green);
    r->setPos(-100,-100);
    r->setRotation(315);
    playerList.append(r);
    connect(r, SIGNAL(hit(player*)), this, SLOT(playCol(player*)));
    player* s = new player;
    s->brush=QBrush(Qt::yellow);
    s->setPos(-100,100);
    s->setRotation(225);
    playerList.append(s);
    connect(s, SIGNAL(hit(player*)), this, SLOT(playCol(player*)));


    // Allocate and configure scene
    gameScene = new QGraphicsScene(-SCENE_WIDTH/2, -SCENE_HEIGHT/2,
                                SCENE_WIDTH, SCENE_HEIGHT);

    gameScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    gameScene->setBackgroundBrush(Qt::black);

    gameView= new QGraphicsView;
    gameView->setScene(gameScene);
    gameTimer = new QLCDNumber();
    gameTimer->setSegmentStyle(QLCDNumber::Filled);
    gameTimer->setDigitCount(3);
    tracker = new QTableView;

    tracker->setMaximumSize(400,400);
    //allocate layout and add it too Dialog
    lay = new QVBoxLayout();
    largeLay = new QHBoxLayout();
    lay->addWidget(gameTimer);
    lay->addWidget(gameView);
    largeLay->addLayout(lay);
    largeLay->addWidget(tracker);
    gameDialog->setLayout(largeLay);
    GameTimer = new QTimer(this);
    GameTimeDisplay = 150;   connect(GameTimer, SIGNAL(timeout()), this, SLOT(tick()));
    GameTimer->start(1000);


    /*for(int i=0; i<5; i++)
    {
        spawnAsteroid();
    }
    */

    gameDialog->show();
    QTimer* timer = new QTimer;
    QObject::connect(timer,SIGNAL(timeout()), gameScene, SLOT(advance()));
    timer->setInterval(24);
    timer->start();
    asSpawner = new QTimer;
    asSpawner->setInterval(5000);
    asSpawner->start();
    QObject::connect(asSpawner,SIGNAL(timeout()), this, SLOT(spawnAsteroid()));

}

Game::~Game() {
    emit end();
    delete gameDialog;
}

void Game::endGame()//remove all objects
{

    asSpawner->stop();
    int pL=projList.length();
    for(int i=0; i<pL; i++)
    {
        gameScene->removeItem(projList.at(i));
        delete projList.at(pL-i-1);
    }
    int aL=asList.length();
    for(int i=0; i<aL; i++)
    {
        gameScene->removeItem(asList.at(i));
        delete asList.at(aL-i-1);
    }
    int HighScore=0;
    QString name="du0lkwkops/1/2e";
    for(int i=0; i<4; i++)
    {
        playerList.at(i)->dead=1;
        if(playerList.at(i)->score>=HighScore)
        {
            HighScore=playerList.at(i)->score;
            if(playerList.at(i)->name=="du0lkwkops/1/2e")
            {
                break;
            }
            name=playerList.at(i)->name;
        }
    }
    if(name=="du0lkwkops/1/2e")
    {
        QString hs = QString::number(HighScore);
        QGraphicsTextItem* winner= new QGraphicsTextItem;
        winner->setDefaultTextColor(Qt::white);

        QString vicMsg = QString("No players joined");
        winner->setPlainText(vicMsg);
        gameScene->addItem(winner);
        return;
    }
    QString hs = QString::number(HighScore);
    QGraphicsTextItem* winner= new QGraphicsTextItem;
    winner->setDefaultTextColor(Qt::white);

    QString vicMsg = QString("The Winner is ");
    vicMsg.append(name);
    winner->setPlainText(vicMsg);
    gameScene->addItem(winner);
}

void Game::tick() {
    GameTimeDisplay--;
    if(GameTimeDisplay>0)
    {
        gameTimer->display(GameTimeDisplay);
    }
    else if(GameTimeDisplay==0)
    {
        gameTimer->display(GameTimeDisplay);
        endGame();
    }
    else if(GameTimeDisplay==-6)
    {
        delete this;
    }
}
void Game::getPlayers(QString *Players, int *playercnt) {
    return;
}

void Game::makePlayer(int playerNum, QString ip, QString name)
{
    qDebug() << playerNum;
    playerList.at(playerNum)->Ip=ip;
    playercnt++;
    gameScene->addItem(playerList.at(playerNum));
    playerList.at(playerNum)->name=name;
    GameStart = 1;
    addrow(playerNum, playerList.at(playerNum));
    gameScene->advance();
}

void Game::spawnPro(int i)
{

            Projectile* p = new Projectile;
            player* pl = playerList.at(i);
            p->ip=pl->Ip;
            p->setRotation(pl->rotation());
            //projectile velocity independant of player velocity
            double angleInRads = p->rotation() * (3.1415926/180);
            p->dx=10*sin(angleInRads);
            p->dy=10*-cos(angleInRads);
            p->setPos(pl->pos());
            p->advance(1);
            p->advance(1);
            p->advance(1);
            p->advance(1);

            projList.append(p);
            gameScene->addItem(p);
            //connect hits too each graphics item so we can make game level changes such as score
            connect(p, SIGNAL(hit(Projectile*)), this, SLOT(ProCol(Projectile*)));
            connect(p, SIGNAL(done(Projectile*)), this, SLOT(proTimeout(Projectile*)));

}

void Game::ProCol(Projectile* pro)
{
    player* play;
    //player collision
    int forSQL=5;
    for(int i=0; i<4; i++)
    {
        if(pro->ip==playerList.at(i)->Ip)
        {
            play=playerList.at(i);
            forSQL=i;
        }
    }
    if(play != nullptr)
    {
        for(int i=0; i<4; i++)
        {
            if(pro->list.at(0)->pos()==playerList.at(i)->pos())//add extra points for hitting a player
            {
                killPlayer(playerList.at(i));
                play->score+=50;
                UPDATETABLE(forSQL, play, play->score);
                return;
            }
        }//player collision end
        //other collisions
        QList<QGraphicsItem*> li = pro->list;
        for(int i=0; i<li.length(); i++)//account for unlikely scenario that the projectile hits mutliple items at once
        {
            gameScene->removeItem(li.at(i));
            for(int j=0; j<asList.length(); j++)
            {
                Asteroids* asHit = asList.at(j);
                if(asHit->pos()==li.at(i)->pos())
                {
                    if(asHit->size>1)
                        astmake(asHit);
                    asList.removeOne(asHit);
                    delete asHit;
                    projList.removeOne(pro);
                    play->score+=10;
                    UPDATETABLE(forSQL, play, play->score);
                    return;
                }
            }
            for(int j=0; j<projList.length(); j++)
            {
                Projectile* prohit = projList.at(j);
                if(prohit->pos()==li.at(i)->pos())
                {
                    projList.removeOne(prohit);
                    delete prohit;
                    projList.removeOne(pro);
                    play->score+=20;
                    UPDATETABLE(forSQL, play, play->score);
                    return;
                }
            }
        }
    }
}

void Game::playCol(player* killed)
{
    QList<QGraphicsItem*> li = killed->collidingItems();
    for(int i=0; i<li.length(); i++)//account for unlikely scenario that the projectile hits mutliple items at once
    {
        gameScene->removeItem(li.at(i));
        for(int j=0; j<asList.length(); j++)
        {
            Asteroids* asHit = asList.at(j);
            if(asHit->pos()==li.at(i)->pos())
            {
                if(asHit->size>1)
                    astmake(asHit);
                asList.removeOne(asHit);
                delete asHit;
                killPlayer(killed);
                return;
            }
        }
        //add collision with other players here you can use projCol above too help     
    }
}

void Game::killPlayer(player* killed)
{
    killed->dead=true;
    gameScene->removeItem(killed);
    playercnt--;
    qDebug() << playercnt;
    if(playercnt==0)
    {
        GameTimeDisplay=1;
    }
}

void Game::spawnAsteroid()
{
    if (GameStart == 1) {
    Asteroids* a = new Asteroids;
    int randx=rand() % 1000;
    int randy=rand() % 1000;
    a->setPos(randx,randy);
    int randrot=rand() % 359;
    a->setRotation(randrot);

    double angleInRads = a->rotation() * (3.1415926/180);
    a->dx=2*sin(angleInRads);
    a->dy=2*-cos(angleInRads);

    asList.append(a);
    gameScene->addItem(a);
    }
}
void Game::astmake(Asteroids* origin)
{
    int rotchange=90;
    for(int i=0; i<2; i++)
    {
        Asteroids* a = new Asteroids(nullptr, origin->size/2);
        a->setPos(origin->pos());
        int rot=origin->rotation()+rotchange;
        a->setRotation(rot);

        double angleInRads = a->rotation() * (3.1415926/180);
        a->dx=2*sin(angleInRads);
        a->dy=2*-cos(angleInRads);
        a->dx=a->dx*1.5;
        a->dy=a->dy*1.5;
        if(a->size==1)
        {
            a->dx=a->dx*1.5;
            a->dy=a->dy*1.5;
        }
        qDebug() << a->dx;
        qDebug() << a->dy;
        asList.append(a);
        gameScene->addItem(a);
        rotchange+=180;
    }
}
void Game::proTimeout(Projectile*p)
{
    projList.removeOne(p);
    delete p;
}

void Game::makeTable(QSqlDatabase d, QString Tablename)
{
    db=d;

    if (!db.open())
        qDebug() << "Database error: " << db.lastError().text();

    if (!query.exec("DROP TABLE IF EXISTS players"))
        qDebug() << "Error dropping table: " << query.lastError().text();

    query.exec("CREATE TABLE IF NOT EXISTS players (id INTEGER NOT NULL, name TEXT NOT NULL, COLOR TEXT NOT NULL, score INTEGER NOT NULL)");

    tm = new QSqlTableModel;
    tm->setTable("players");
    tm->setEditStrategy(QSqlTableModel::OnFieldChange);
    tm->select();
    tracker->setModel(tm);

}

void Game::addrow(int id, player* p)
{
    QString name = p->name;
    QString color=p->brush.color().name();
    if(color=="#ff0000")
        color="red";
    else if(color=="#0000ff")
        color="blue";
    else if(color=="#00ff00")
        color="green";
    else if(color=="#ffff00")
        color="yellow";

    query.prepare("INSERT INTO players (id, name, color, score)" "VALUES (:id, :name, :color, :score)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":color", color);
    query.bindValue(":score", 0);
     query.exec();


    tm->select();


}

void Game::UPDATETABLE(int id, player* p, int score)
{
    query.prepare("UPDATE players SET score=:score WHERE id=:id");
    query.bindValue(":score", score);
    query.bindValue(":id", id);
    query.exec();
    tm->select();
}
