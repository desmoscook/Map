#include "timer.h"

Timer::Timer(int id, QObject *parent)
    : QObject{parent}, m_id{id}
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Timer::on_timeout);
    m_timer->start(5000); // TODO:改成5秒
}

void Timer::reset()
{
    m_timer->start(5000); // TODO:改成5秒
}

void Timer::start(int msec)
{
    m_timer->start(msec);
}

void Timer::stop()
{
    m_timer->stop();
}


