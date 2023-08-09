#ifndef WIDGET_H
#define WIDGET_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <QWidget>
#include <QThread>
#include <QDebug>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

#include "httphandler.h"
#include "IconItem.h"
#include "timer.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void insertPoint(const QString &fileName, int id, int x, int y);
    void updatePoint(int id, int x, int y);

public slots:
    void HandlePosition(int id, int type, int x, int y);
    void HandleFences(int id, std::vector<std::pair<int, int>> points);
    void HandleTrack(int id, int type, int x, int y);
    void DeletePoint(int id);

private:
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QGraphicsPixmapItem* m_icon;
    std::unordered_map<int, Timer*> m_timers;
    // TODO: 上一次展示的icon在下一个报文接收到之后就销毁
    std::unordered_map<int, IconItem*> m_iconItems;
    // TODO: track不需要记录id，需要用一个类封装，其中有一个定时器，和一个点
    std::unordered_map<int, std::vector<QGraphicsEllipseItem*>> m_tracks;
    std::unordered_map<int, std::vector<QGraphicsLineItem*>> m_fences;

    HttpHandler *m_httpHandler;
};
#endif // WIDGET_H
