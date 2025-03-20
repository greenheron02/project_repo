#include "asteroids.h"

Asteroids::Asteroids(QGraphicsItem *parent)
{
    vs=size*25;
}
Asteroids::Asteroids(QGraphicsItem *parent, int l)
{
    size=l;
    vs=size*25;
    destroy=0;
}
Asteroids::~Asteroids()
{
    return;
}

QRectF Asteroids::boundingRect() const
{
    return QRectF(-vs/2, -vs/2, vs, vs);
}

QPainterPath Asteroids::shape() const
{
    QPainterPath path;
    path.addRect(-vs/2, -vs/2, vs, vs);
    return path;
}

void Asteroids::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::white));
    painter->drawEllipse(-vs/2, -vs/2, vs, vs);
}

void Asteroids::advance(int phase)
{
    if (phase == 0)
        return;

    qreal x = this->pos().rx();
    qreal y = this->pos().ry();
    x = x + dx;
    y = y + dy;


    if ( x <= -650) {x = 700;}    //
    if ( x > 700) {x = -650;}    //
    if ( y <= -500) {y = 500;}    //   screen-wrapping the projectile
    if ( y > 500) {y = -500;}    //

    this->setPos(x,y);

    // qDebug() << x << "   " << y << "   " << this->rotation();  //display coordinates and rotation angle to console for debugging
}
