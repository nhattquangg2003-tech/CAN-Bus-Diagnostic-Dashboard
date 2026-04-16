#ifndef CAN_TRANSPORT_H
#define CAN_TRANSPORT_H

#endif // CAN_TRANSPORT_H
#pragma once
#include <QObject>
#include <QByteArray>

class QSocketNotifier;

class CanTransport : public QObject {
    Q_OBJECT
public:
    explicit CanTransport(QObject *parent = nullptr);

signals:
    void frameReceived(const QByteArray &frame);

private:
    int m_fd = -1;
    QSocketNotifier *m_notifier = nullptr;
};
