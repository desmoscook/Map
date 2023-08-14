#include "track.h"

Track::Track(QGraphicsScene *scene, int type, int x, int y, QObject *parent)
    : QObject{parent}, m_scene{scene}
{
    // 创建Track并加入sence
    QBrush brush(Qt::SolidPattern);
    if (type == 0) {
        brush.setColor(QColor(55, 192, 255));
    } else {
        brush.setColor(QColor(252, 202, 0));
    }
    track = new QGraphicsEllipseItem(x - 2, y - 2, 4, 4);
    track->setBrush(brush);
    m_scene->addItem(track);

    // 启动定时器
    QTimer::singleShot(8000, this, [=](){
        m_scene->removeItem(track);
        delete track;
    });
}
