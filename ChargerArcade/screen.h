#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsView>
#include <QColor>
#include <QtWidgets>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QFontMetrics>
#include <QTcpSocket>


#include <windows.h>

namespace Ui {
class Screen;
}

class Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Screen(QWidget *parent = nullptr);
    ~Screen();
    void testFunction();
    void DrawFrame(QColor col, int id);
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
    void clear(QLayout* layout);
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
    void adjustFontSize(QLabel* label);
    void flash(QString);
    void runGame();
    void resizeWindowWithAHK(const QString &windowTitle, int width, int height);
    void sendToAHK(const QString &key);
    void makeControlArray();
    void endScript();

    protected:
    bool eventFilter(QObject *obj, QEvent *event)override;
private:
    QGridLayout* grid = new QGridLayout;
    QProcess *process = new QProcess(this);
    QScrollArea Scroller;
    QWidget selectscreen, startscreen, screen, gamescreen, managescreen, ScrollArea;
    QVBoxLayout startLayout;
    Ui::Screen *ui;
    QStackedLayout stack;
    QScreen &monitor = *QGuiApplication::primaryScreen(); //MAY NOT NEED TO BE IN PRIVATE

    int screenw, screenh, gridw=2, x=1, y=1, spot=0, total=0, gridwidth=600, gridheight=300, borderThickness=12, boundaryWidth=5;
    double  diff=1.2;
    QColor UAHBlue=QColor("#003DA5"), UAHYellow=QColor("#FDDA24"), UAHDBlue=QColor("#002D72"), UAHLBlue=QColor("#009FDF"), UAHLGrey=QColor("#E5E5E5"), UAHGrey=QColor("#BBB1A7"), UAHDGrey=QColor("#757575");
    QVector<QPixmap*>images;
    QVector<QLabel*> holders;
    QVector<QString> programs;
    QVector<QDialog*> entries;

    QString keyPhrase = "HelloIAmKey";
    int buffer = 300;
    bool editMode=false;

    int prevSpot=1;
    QTcpSocket ahksock;
    QMap<QString, QString> con;

};
#endif // SCREEN_H
