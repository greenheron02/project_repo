#include "projectile.h"

Projectile::Projectile(QGraphicsItem *parent)
{
    destroy=0;
    lifeTime=new QTimer;
    QObject::connect(lifeTime,SIGNAL(timeout()), this, SLOT(ProjDelete()));
    lifeTime->setInterval(2500);
    lifeTime->start();

}
Projectile::~Projectile()
{
    return;
}

QRectF Projectile::boundingRect() const
{
    return QRectF(-20/2, -20/2, 20, 20);
}

QPainterPath Projectile::shape() const
{
    QPainterPath path;
    path.addRect(-20/2, -20/2, 20, 20);
    return path;
}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::white));
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(-20/2, -20/2, 20, 20);
}

void Projectile::advance(int phase) {
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

    list = collidingItems();
    if (!list.isEmpty())
    {

        emit hit(this);
        this->scene()->removeItem(this);
        delete this;
    }
    // qDebug() << x << "   " << y << "   " << this->rotation();  //display coordinates and rotation angle to console for debugging
}
void Projectile::ProjDelete()
{
    lifeTime->stop();
    this->scene()->removeItem(this);
    emit done(this);
}
