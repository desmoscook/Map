#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 设置场景和视图
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(this);
    m_view->setScene(m_scene);
    m_view->setFixedSize(933, 573); // 设置视图大小等于图片
    this->setFixedSize(931, 571);

    // 设置场景的背景图
    QPixmap map(":/icon/map");
    QGraphicsPixmapItem* background = m_scene->addPixmap(map);
    background->setZValue(-1); // 设置ZValue使其一直置于最底层


    // 开启新线程处理连接
//    QThread* t = new QThread;
    m_httpHandler = new HttpHandler;
//    m_httpHandler->moveToThread(t);
//    t->start();
    m_httpHandler->listenRequest(8989);

    // 处理接收到的数据
    connect(m_httpHandler, &HttpHandler::pointInfo,
            this, &Widget::HandlePosition, Qt::QueuedConnection);
    connect(m_httpHandler, &HttpHandler::fenceInfo,
            this, &Widget::HandleFences, Qt::QueuedConnection);
    connect(m_httpHandler, &HttpHandler::trackInfo,
            this, &Widget::HandleTrack, Qt::QueuedConnection);
}

Widget::~Widget()
{
}


void Widget::insertPoint(const QString &fileName, int id, int x, int y)
{
    // 创建对象并插入
    QPixmap iconPixmap(fileName);
    IconItem* iconItem = new IconItem(iconPixmap);
    iconItem->updatePos(x, y);
    m_scene->addItem(iconItem);
    m_iconItems[id] = iconItem;

    // 创建对应的定时器
    Timer* timer = new Timer(id);
    connect(timer, &Timer::timeout, this, &Widget::DeletePoint);
    m_timers[id] = timer;
}

void Widget::updatePoint(int id, int x, int y)
{
    m_iconItems[id]->updatePos(x, y);
    m_timers[id]->reset();
}

void Widget::HandlePosition(int id, int type, int x, int y)
{
    // 如果没有则创建，如果有则更新
    if (m_iconItems.find(id) == m_iconItems.end()) {
        if (type == 0) {
            insertPoint(":/icon/people", id, x, y);
        } else {  // TODO:确认type == 1是否为车，以及是否还有其他type
            insertPoint(":/icon/car", id, x, y);
        }
    } else {
        updatePoint(id, x, y);
    }
}

void Widget::HandleFences(int id, std::vector<std::pair<int, int>> points)
{
    QPen linePen(Qt::yellow);
    linePen.setWidth(2);
    std::vector<QGraphicsLineItem*> lines;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t next = (i + 1 + points.size()) % points.size();
        auto line = new QGraphicsLineItem(points[i].first, points[i].second,
            points[next].first, points[next].second);
        line->setPen(linePen);
        m_scene->addItem(line);
        lines.push_back(line);
    }
    m_fences[id] = lines;
}

void Widget::HandleTrack(int id, int type, int x, int y)
{
    // TODO: 同id的轨迹使用相同的颜色
    // TODO: 设置定时器
    QBrush brush(Qt::yellow);
    auto track = new QGraphicsEllipseItem(x - 2, y - 2, 4, 4);
    track->setBrush(brush);
    m_scene->addItem(track);
    // 如果没有则创建，如果有则插入
    if (m_tracks.find(id) == m_tracks.end()) {
        std::vector<QGraphicsEllipseItem*> tracks;
        tracks.push_back(track);
        m_tracks[id] = tracks;
    }
    else {
        m_tracks[id].push_back(track);
    }
}

void Widget::DeletePoint(int id)
{
    m_scene->removeItem(m_iconItems[id]);
    delete m_iconItems[id];
    m_iconItems.erase(id);

    delete m_timers[id];
    m_timers.erase(id);
}
