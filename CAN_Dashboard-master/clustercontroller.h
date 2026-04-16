#pragma once
#include <QObject>

class CanTransport;

class ClusterController : public QObject {
    Q_OBJECT
    Q_PROPERTY(int rpm READ rpm NOTIFY updated)
    Q_PROPERTY(int speed READ speed NOTIFY updated)
    Q_PROPERTY(int coolant READ coolant NOTIFY updated)
    Q_PROPERTY(QString rawFrame READ rawFrame NOTIFY updated)

public:
    explicit ClusterController(QObject *parent = nullptr);

    int rpm() const { return m_rpm; }
    int speed() const { return m_speed; }
    int coolant() const { return m_coolant; }
    QString rawFrame() const { return m_raw; }

signals:
    void updated();

private:
    int m_rpm = 0;
    int m_speed = 0;
    int m_coolant = 0;
    QString m_raw;
};
