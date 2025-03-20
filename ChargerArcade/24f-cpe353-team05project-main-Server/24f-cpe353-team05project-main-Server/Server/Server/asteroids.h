#ifndef ASTEROIDS_H
#define ASTEROIDS_H


#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QtDebug>
#include <cmath>

class Asteroids : public QGraphicsObject
{
    Q_OBJECT

public:
    Asteroids(QGraphicsItem *parent = nullptr);
    Asteroids(QGraphicsItem *parent, int l);
    ~Asteroids() override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void advance(int phase) override;

    QPainter* playerPaint;
    QRect* playRect;
    QString* Name;
    int xpos;
    int ypos;
    QString ip;
    QList<QGraphicsItem*> list;
    bool destroy;
    qreal dx = 2;
    qreal dy = 2;
    int size = 4;
    int vs;

private:
    QPixmap object;
    int index;
    int offset;
    qreal x = 0;
    qreal y = 0;

signals:
    void hit(Asteroids*);
};

#endif // PROJECTILE_H
