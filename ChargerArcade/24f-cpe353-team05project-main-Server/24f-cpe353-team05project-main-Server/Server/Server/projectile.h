#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QtDebug>
#include <QTimer>
#include <cmath>

class Projectile : public QGraphicsObject
{
    Q_OBJECT

public:
    Projectile(QGraphicsItem *parent = nullptr);
    ~Projectile() override;

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
    qreal dx = 10;
    qreal dy = 10;


private:
    QPixmap object;
    int index;
    int offset;
    qreal x = 0;
    qreal y = 0;

    QTimer* lifeTime;
signals:
    void hit(Projectile*);
    void done(Projectile*);

private slots:
    void ProjDelete();
};

#endif // PROJECTILE_H
