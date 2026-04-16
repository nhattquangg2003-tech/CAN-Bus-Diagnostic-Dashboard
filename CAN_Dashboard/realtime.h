#ifndef REALTIME_H
#define REALTIME_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QString>

class RealTime : public QObject
{
    Q_OBJECT
public:
    explicit RealTime(QObject *parent = nullptr);
    Q_INVOKABLE void startTimer();
    Q_PROPERTY(QString datetime READ datetime WRITE setdatetime NOTIFY datetimeChanged FINAL)

    QString datetime() const;
    void setdatetime(const QString &newDatetime);
public slots:
    void printDateTime();
signals:
    void datetimeChanged();
private:
    QString m_datetime;
    QTimer m_timer;
};

#endif // REALTIME_H
