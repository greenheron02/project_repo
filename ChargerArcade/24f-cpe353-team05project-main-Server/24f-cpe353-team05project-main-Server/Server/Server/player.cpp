 #include "player.h"
#include <QtDebug>
player::player(QGraphicsItem *parent)
{
    object = QPixmap(100, 100);
}

player::~player()
{
    return;
}

QRectF player::boundingRect() const
{
    return QRectF(-25/2, -25/2, 25, 25);
}

QPainterPath player::shape() const {
    QPainterPath path;
    path.addRect(-25/2, -25/2, 25, 25);
    return path;
}

void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::white));
    painter->setBrush(brush);

    QPainterPath path;
    path.moveTo(this->boundingRect().left() + (this->boundingRect().width() / 2), this->boundingRect().top());      //
    path.lineTo(this->boundingRect().bottomLeft());                                                                 //   makes triangle to see where player is pointing
    path.lineTo(this->boundingRect().bottomRight());                                                                //
    path.lineTo(this->boundingRect().left() + (this->boundingRect().width() / 2), this->boundingRect().top());      //

    painter->fillPath(path, brush);
}

void player::advance(int phase) {
    if (phase == 0)
        return;

    qreal x = this->pos().rx();
    qreal y = this->pos().ry();
    x = x + dx;
    y = y + dy;
    SlowDown();

    if ( x <= -650) {x = 700;}    //
    if ( x > 700) {x = -650;}    //
    if ( y <= -500) {y = 500;}    //   screen-wrapping the player
    if ( y > 500) {y = -500;}    //

    this->setPos(x,y);
    list = collidingItems();
    if (!list.isEmpty())
    {
        qDebug() << "Hit";
        emit hit(this);

    }

}

void player::RotateLeft() {
    double rotation = this->rotation();
    rotation = rotation - 9;
    if (rotation < 0){rotation = rotation + 360; this->setRotation(rotation);}  // resets rotation to between 0-360 when you do a full circle
    else {this->setRotation(rotation);}
}

void player::RotateRight() {
    double rotation = this->rotation();
    rotation = rotation + 9;
    if (rotation > 360){rotation = rotation - 360; this->setRotation(rotation);}  // resets rotation to between 0-360 when you do a full circle
    else {this->setRotation(rotation);}
}

void player::MoveForward() {
    double rotation = this->rotation();
    double angleInRads = rotation * (3.1415926/180);   // convert the angle in degrees to rads

    dx = 0 + sin(angleInRads) * 4;
    dy = 0 + -cos(angleInRads) * 4;
    slowDown.start(1);

}
void player::SlowDown()
{
    if(slowDown.remainingTime() <= 0) {
        return;
    }
    if (dx > 0.0005) {
        dx = dx/1.03;
    }
    else if (dx > 0){
        dx = 0;
    }

    if (dy > 0.0005) {
        dy = dy/1.03;
    }
    else if (dy > 0){
        dy = 0;
    }

    if (dx < -0.0005) {
        dx = dx*0.97;
    }
    else if (dx < 0){
        dx = 0;
    }

    if (dy < -0.0005) {
        dy = dy*0.97;
    }
    else if (dy < 0){
        dy = 0;
    }

}
