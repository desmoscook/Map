#include "iconitem.h"
#include <QDebug>


IconItem::IconItem(const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap)
{
    width = pixmap.width();
	height = pixmap.height();
}

void IconItem::updatePos(int x, int y)
{
    setPos(x - width / 2, y - height);
}
