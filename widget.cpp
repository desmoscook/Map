#include "widget.h"
#include "track.h"

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

    // 创建icon池
    for (size_t i = 0; i < 50; ++i) {
        QPixmap map(":/icon/people");
        IconItem* people = new IconItem(map);
        people->setZValue(1);
        m_peoples[i] = people;
    }
    for (size_t i = 0; i < 30; ++i) {
        QPixmap map(":/icon/car");
        IconItem* car = new IconItem(map);
        car->setZValue(1);
        m_cars[i] = car;
    }


    // 开启新线程处理连接
//    QThread* t = new QThread;
    m_httpHandler = new HttpHandler;
//    m_httpHandler->moveToThread(t);
//    t->start();
    m_httpHandler->listenRequest(8990);

    // 处理pos超时
    connect(&PosTimer, &QTimer::timeout, this, &Widget::HandlePosTimeout);

    // 处理接收到的数据
    connect(m_httpHandler, &HttpHandler::positionInfo,
            this, &Widget::HandlePosition, Qt::QueuedConnection);
    connect(m_httpHandler, &HttpHandler::fenceInfo,
            this, &Widget::HandleFences, Qt::QueuedConnection);
    connect(m_httpHandler, &HttpHandler::trackInfo,
            this, &Widget::HandleTrack, Qt::QueuedConnection);
}

Widget::~Widget()
{
    for (size_t i = 0; i < 50; ++i) {
        delete m_peoples[i];
    }
    for (size_t i = 0; i < 30; ++i) {
        delete m_cars[i];
    }
}

void Widget::HandlePosition(std::vector<std::pair<int, int>> rev_peoples,
                            std::vector<std::pair<int, int>> rev_cars)
{
    // 将当前报文的所有人员的位置更新
    for (size_t i = 0; i < rev_peoples.size(); ++i) {
        m_peoples[i]->updatePos(rev_peoples[i].first, rev_peoples[i].second);
    }
    // 如果小于当前人数，则删除多余;如果大于当前人数，则添加到场景中
    if (rev_peoples.size() < usingPeoples) {
        for (size_t i = rev_peoples.size(); i < usingPeoples; ++i)
            m_scene->removeItem(m_peoples[i]);
    } else {
        for (size_t i = usingPeoples; i < rev_peoples.size(); ++i) {
            m_scene->addItem(m_peoples[i]);
        }
    }
    // 将当前报文的所有车辆的位置更新
    for (size_t i = 0; i < rev_cars.size(); ++i) {
        m_cars[i]->updatePos(rev_cars[i].first, rev_cars[i].second);
    }
    if (rev_cars.size() < usingCars) {
        for (size_t i = rev_cars.size(); i < usingCars; ++i)
            m_scene->removeItem(m_cars[i]);
    } else {
        for (size_t i = usingCars; i < rev_cars.size(); ++i) {
            m_scene->addItem(m_cars[i]);
        }
    }
    usingPeoples = rev_peoples.size();
    usingCars = rev_cars.size();

    // 重置接收报文定时器
    PosTimer.start(5000);
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
    new Track(m_scene, type, x, y);
}

void Widget::HandlePosTimeout()
{
    for (size_t i = 0; i < usingPeoples; ++i) {
        m_scene->removeItem(m_peoples[i]);
    }
    for (size_t i = 0; i < usingCars; ++i) {
        m_scene->removeItem(m_cars[i]);
    }
    usingPeoples = 0;
    usingCars = 0;
}
