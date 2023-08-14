#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <vector>
#include <utility>
#include <QTimer>

#include <QObject>
#include <QByteArray>

#include <QTcpServer>
#include <QTcpSocket>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



class HttpHandler : public QObject
{
    Q_OBJECT
public:
    explicit HttpHandler(QObject *parent = nullptr);

    void listenRequest(unsigned short port);
    void handleNewConnection();

    void parsePosition(const QByteArray jsonString);
    void parseFences(const QByteArray jsonString);
    void parseTrack(const QByteArray jsonString);

signals:
    void positionInfo(std::vector<std::pair<int, int>> rev_peoples,
                      std::vector<std::pair<int, int>> rev_cars);
    void fenceInfo(int id, std::vector<std::pair<int, int>> points);
    void trackInfo(int id, int type, int x, int y);

public slots:
    void readFromClient();
    void disconnectClient();

private:
    QTcpServer m_server;
    QTcpSocket* m_client = nullptr;
    QTimer connect_timer; // 长连接超时定时器
};

#endif // HTTPHANDLER_H
