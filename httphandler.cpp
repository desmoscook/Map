#include "httphandler.h"

HttpHandler::HttpHandler(QObject *parent)
    : QObject{parent}
{

}

void HttpHandler::listenRequest(unsigned short port)
{
    if (!m_server.listen(QHostAddress::Any, port)) {
        qWarning() << "Failed to start server!";
        return;
    }

    qDebug() << "Server Started, listening on port" << port;

    connect(&m_server, &QTcpServer::newConnection, this, &HttpHandler::handleNewConnection);
}

void HttpHandler::handleNewConnection()
{
    if (!m_client) {
        m_client = m_server.nextPendingConnection();
        connect(m_client, &QTcpSocket::readyRead, this, &HttpHandler::readFromClient);
        connect(m_client, &QTcpSocket::disconnected, this, &HttpHandler::disconnectClient);
    }
}

void HttpHandler::parsePosition(const QByteArray jsonString)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj["flag"].toDouble() == 1) {
            QJsonArray position = jsonObj["position"].toArray();
            for (int i = 0; i < position.size(); i++) {
                QJsonObject data = position[i].toObject();
                int id = data["id"].toInt();
                int type = data["type"].toInt();
                QJsonArray pos = data["point"].toArray();
                int x = pos[0].toInt();
                int y = pos[1].toInt();
                if (x > 928) x = 928;
                if (y > 568) y = 568;
                emit pointInfo(id, type, x, y);
            }
        }
    }
}

void HttpHandler::parseFences(const QByteArray jsonString)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj["flag"].toDouble() == 1) {
			QJsonArray fences = jsonObj["fences"].toArray();
            for (int i = 0; i < fences.size(); i++) { // 遍历每一个围栏
                std::vector<std::pair<int, int>> points;
				QJsonObject data = fences[i].toObject();
				int id = data["id"].toInt();
                QJsonArray point_x = data["point_x"].toArray();
                QJsonArray point_y = data["point_y"].toArray();
                if (point_x.size() != point_y.size()) {
                    qDebug() << "fences data error";
					continue;
				}
                for (int i = 0; i < point_x.size(); i++) {
                    int x = point_x[i].toInt();
                    int y = point_y[i].toInt();
                    if (x > 928) x = 928;
                    if (y > 558) y = 568;
                    points.push_back({x, y});
				}
				emit fenceInfo(id, points);
			}
		}
    }
}

void HttpHandler::parseTrack(const QByteArray jsonString)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString);
    if (jsonDoc.isObject()) {
        QJsonObject track = jsonDoc.object();
        int id = track["id"].toInt();
        int type = track["type"].toInt();
        int security_type = track["security_type"].toInt();
        int x = track["point_x"].toArray()[0].toInt();
        int y = track["point_y"].toArray()[0].toInt();
        if (x > 928) x = 928;
        if (y > 568) y = 568;
        qDebug() << "id:" << id;
        qDebug() << "type:" << type;
        qDebug() << "security_type:" << security_type;
        qDebug() << "x:" << x;
        qDebug() << "y:" << y;
        emit trackInfo(id, type, x, y);
    }
}

void HttpHandler::readFromClient()
{
    int data_type = -1;
    while (m_client->bytesAvailable() > 0) {
        QByteArray line = m_client->readLine().trimmed();
        if (line.startsWith("sihan_type")) {
            if (line.contains("fences")) {
                qDebug() << "--------------------fences-----------------------";
                data_type = 0;
            } else if (line.contains("track")) {
                qDebug() << "--------------------track------------------------";
                data_type = 1;
            } else if (line.contains("position")) {
                qDebug() << "-------------------position----------------------";
                data_type = 2;
            }
        }
        if (line[0] == '{') {
            switch (data_type) {
            case 0:
                parseFences(line);
                break;
            case 1:
                parseTrack(line);
                break;
            case 2:
                parsePosition(line);
                break;
            default:
                break;
            }
        }
    }
    m_client->deleteLater();
    m_client = nullptr;
}

void HttpHandler::disconnectClient()
{
    if (m_client != nullptr) {
        m_client->deleteLater();
        m_client = nullptr;
    }
}
