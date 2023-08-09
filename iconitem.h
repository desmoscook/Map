#ifndef ICONITEM_H
#define ICONITEM_H

#include <QPixmap>
#include <QGraphicsPixmapItem>

class IconItem : public QGraphicsPixmapItem
{
public:
    IconItem(const QPixmap& pixmap);

    void updatePos(int x, int y);
private:
    int width;
    int height;
};

//class TrackItem : public QGraphics

#endif // ICONITEM_H
