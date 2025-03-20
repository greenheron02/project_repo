#ifndef CHARCADE_H
#define CHARCADE_H

#include <QColor>
#include <QDebug>
#include <QDialog>
#include <QFontMetrics>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QScreen>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <QtMath>
#include <QtWidgets>

#include <QTcpServer>

#include <windows.h>

namespace Ui {
class Screen;
}

class Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Screen(int id);
    explicit Screen() {}
    ~Screen();
    bool splitScreenVal, doCommsVal;
    void build();
    void testFunction();
    QPixmap* DrawFrame(QPixmap *image, QColor col, int thickness);
    //void RestoreFrame();
    //void LeftKeyPress();
    //void UpdateXY();
    void UpdateSpot();
    void MakeSelectScreen();
    void PopulateCell(QString line, int id);
    void MakeTitleScreen();
    void MakeGameScreen();
    void MakeEditScreen();
    void CheckForUSB();
    void temp();
    void insert();
    void extract(int num);
    void clear(QLayout *layout);
    //static QVector<QString> imageFiles, programs;
    //static QVector<QPixmap*> images;
    //static QVector<QLabel*> holders;
    //static int screenw, screenh, gridw,gridh,x,y,spot,count,total,gridwidth,gridheight,borderThickness,boundaryWidth;
    //static double diff;
    static QFontDatabase CustomFontDatabase;
    static int CustomFontId;
    static QString UAH;
    //static QColor UAHBlue, UAHYellow, UAHDBlue, UAHLBlue, UAHLGrey, UAHGrey, UAHDGrey;
    static bool stopscroll;
    void adjustFontSize(QLabel *label, int max);
    void flash(QString);
    void runGame();
    void resizeWindowWithAHK(const QString &windowTitle, int width, int height);
    void sendToAHK(const QString &key);
    void makeAHKFile(int programid);
    void endScript();

    void SetUpServer();
    void handleServerConnection();
    void handleClientMessage(QTcpSocket *client, int id);
    void readMessage();
    void handleConnection();
    bool connectToPeer(int id);
    void sendMessage(const QString &message, int id);
    void Overlay();
    void MakeStatusBar();
    void ShowStatus();

    void broadcastMessage(const QString &message);
    void ConnectToPeers();
    void registerConnection(int id, QTcpSocket *zokket);
    void handleDisconnect(QTcpSocket *zokket, int id);
    void Connect();
    void DisconnectAll();
    void Reconnect();

    QPixmap colorizePixmap(const QString &filePath, const QColor &color);

    void MakeNoti();
    void summonNoti(QString, QString);
    void dismissNoti();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void changeEvent(QEvent *event)
    {
        if (event->type() == QEvent::WindowStateChange) {
            if (windowState() == Qt::WindowMinimized) {
                noti->hide();
                status->hide();
            }
            if (windowState() != Qt::WindowMinimized) {
                noti->show();
                status->show();
            }
        }
    }
    void closeEvent(QCloseEvent *event) override
    {
        qDebug() << "adios!";

        if (game->state() != QProcess::NotRunning) {
            game->terminate();
            if (!game->waitForFinished(2000)) {
                game->kill();
                qDebug() << "Had to kill";
            }
        }
        if (AHKControl)
            ahksock.disconnectFromHost();
        if (AHKActive)
            endScript();
        if (DoComms)
            DisconnectAll();
        status->close();
        noti->close();
        event->accept(); // Accept the event, causing the window to close
    }

private:
    ////////////////////////////////////////SETTINGS////////////////////////////////////////////
    int screenw, screenh, gridw = 2, x = 1, y = 1, spot = 0, total = 0, gridwidth = 600,
                          gridheight = 300, borderThickness = 12, boundaryWidth = 5;

    QString keyPhrase = "HelloIAmKey";

    QString localpath = "C:/Users/wesri/School/ChargerArcade/";
    QVector<QString> addresses = {"192.168.1.147:12345",
                                  "192.168.1.147:12346",
                                  "192.168.1.147:12347",
                                  "192.168.1.147:12348"};
    QVector<int> ports = {12345, 12346, 12347, 12348};
    int ID = 1;
    QString title = "Mario";

    /////////////////////////////////CONSTANTS//////////////////////////////
    QColor UAHBlue = QColor("#003DA5"), UAHYellow = QColor("#FDDA24"), UAHDBlue = QColor("#002D72"),
           UAHLBlue = QColor("#009FDF"), UAHLGrey = QColor("#E5E5E5"), UAHGrey = QColor("#BBB1A7"),
           UAHDGrey = QColor("#757575");
    QWidget *notitimebar = new QWidget;
    QWidget *notitimebar2 = new QWidget;

    QLabel notititle;
    QLabel notitext;

    QGridLayout *grid = new QGridLayout;
    QProcess *game = new QProcess(this);
    QScrollArea Scroller;
    QWidget selectscreen, startscreen, screen, gamescreen, managescreen, ScrollArea;
    QVBoxLayout selectLayout;
    Ui::Screen *ui;
    QStackedLayout stack;
    QScreen &monitor = *QGuiApplication::primaryScreen(); //MAY NOT NEED TO BE IN PRIVATE

    double diff = 1.2;

    QVector<QPixmap *> images;
    QVector<QLabel *> imageholders;
    QVector<QLabel *> holders;
    QVector<QString> programs;
    QVector<QDialog *> entries;

    int buffer = 300;
    bool editMode = false;

    int prevSpot = 0;
    QTcpSocket ahksock;
    QMap<QString, QString> con;

    QLabel *header;
    QLabel Mario, Luigi, Wario, Waluigi;
    QWidget *status = new QWidget(this, Qt::Window);
    QWidget *noti = new QWidget();
    QTcpServer *server;
    QVector<QTcpSocket *> peerSockets = {new QTcpSocket(this),
                                         new QTcpSocket(this),
                                         new QTcpSocket(this),
                                         new QTcpSocket(this)};
    int TerminalCount = 4;
    QString Sign;

    QVector<bool> connections = {false, false, false, false};
    //QVector<QString> addresses = {"10.0.0.1", "10.0.0.2", "10.0.0.3", "10.0.0.4"};

    QVector<QLabel *> StatusIcons = {new QLabel, new QLabel, new QLabel, new QLabel};
    QVector<QPixmap *> StatusImgs = {new QPixmap, new QPixmap, new QPixmap, new QPixmap};
    QPixmap OffImg;
    QVector<int> IdReference = {1, 2, 3, 4};
    QVector<QMetaObject::Connection> conn;
    int connid = 0;
    QHBoxLayout bar;
    QTimer reconnectTimer;
    QVector<int> disconnectedPeers;

    int notiwid = 400;
    int xspot, yspot;
    QString filePath = "C:/Users/bryan/Documents/ChargerArcade/";

    bool splitScreen = false;
    bool DoStatus = true;
    bool DoComms = true;
    bool AHKActive
        = false; ////Set this to false if the computer does not have AutoHotKey installed. This will disable the usability of the controls, but the computer's keyboard can always be used instead.
    bool AHKControl = false;
};
#endif
