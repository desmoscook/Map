#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(int id, QObject *parent = nullptr);

    void reset();
    void start(int msec);
    void stop();

signals:
    void timeout(int m_id);

public slots:
    void on_timeout() {
        qDebug() << "timeout" << m_id;
        emit timeout(m_id);
    }

private:
    int m_id;
    QTimer* m_timer;
};

#endif // TIMER_H
