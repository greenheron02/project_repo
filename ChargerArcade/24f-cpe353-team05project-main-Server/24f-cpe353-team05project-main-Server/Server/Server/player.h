#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QtDebug>
#include <QGraphicsScene>
#include <QTimer>
#include <cmath>


class player : public QGraphicsObject
{
    Q_OBJECT
public:
    player(QGraphicsItem *parent = nullptr);
    ~player() override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void advance(int phase) override;

    void RotateLeft();
    void RotateRight();
    void MoveForward();
    void changeSpeed();
    void SlowDown();

    QPainter* playerPaint;
    QRect* playRect;
    QString name="du0lkwkops/1/2e";
    int xpos;
    int ypos;
    QString Ip="0";
    QBrush brush;
    qreal dx = 0;
    qreal dy = 0;
    qreal x = 0;
    qreal y = 0;
    int score=0;
    bool dead=0;


signals:
    void hit(player*);

private:
    QPixmap object;
    int index;
    int offset;
    qreal playerDx = 2;
    qreal playerDy = 2;
    qreal speed=1;
    qreal maxplayerDy = 5;
    qreal maxplayerDx = 5;
    qreal minplayerDy = 0;
    qreal minplayerDx = 0;
    QList<QGraphicsItem*> list;

    QTimer slowDown;
    QTimer SpeedUp;
};

#endif // PLAYER_H
