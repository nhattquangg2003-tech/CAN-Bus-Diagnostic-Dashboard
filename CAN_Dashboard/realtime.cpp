#include "realtime.h"

RealTime::RealTime(QObject *parent)
    : QObject{parent}
{}

void RealTime::startTimer()
{
    printDateTime(); // Gọi ngay lập tức để không phải chờ 1 giây đầu tiên
    connect(&m_timer, &QTimer::timeout, this, &RealTime::printDateTime);
    m_timer.start(1000);// 1000ms = 1s.
}

QString RealTime::datetime() const
{
    return m_datetime;
}

void RealTime::setdatetime(const QString &newDatetime)
{
    if (m_datetime == newDatetime)
        return;
    m_datetime = newDatetime;
    emit datetimeChanged();
}

void RealTime::printDateTime()
{
    setdatetime(QDateTime::currentDateTime().toString("HH:mm"));
}
