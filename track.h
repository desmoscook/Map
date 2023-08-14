#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QBrush>
#include <QDebug>

class Track : public QObject
{
    Q_OBJECT
public:
    explicit Track(QGraphicsScene* scene, int type, int x, int y, QObject *parent = nullptr);

signals:

private:
    QGraphicsScene* m_scene;
    QGraphicsEllipseItem* track;
};

#endif // TRACK_H
