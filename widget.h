#ifndef WIDGET_H
#define WIDGET_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <array>

#include <QWidget>
#include <QThread>
#include <QDebug>
#include <QTimer>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

#include "httphandler.h"
#include "iconitem.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void HandlePosition(std::vector<std::pair<int, int>> peoples,
                        std::vector<std::pair<int, int>> cars);
    void HandleFences(int id, std::vector<std::pair<int, int>> points);
    void HandleTrack(int id, int type, int x, int y);
    void HandlePosTimeout();

private:
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QGraphicsPixmapItem* m_icon;

    QTimer PosTimer; // 据上次接收到报文定时，超时删除所有position
    size_t usingPeoples = 0, usingCars = 0; // 记录当前使用的icon
    std::array<IconItem*, 50> m_peoples;
    std::array<IconItem*, 30> m_cars;

    std::unordered_map<int, std::vector<QGraphicsLineItem*>> m_fences;

    HttpHandler *m_httpHandler;
};
#endif // WIDGET_H
