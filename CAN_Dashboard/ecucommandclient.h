#ifndef ECUCOMMANDCLIENT_H
#define ECUCOMMANDCLIENT_H

#endif // ECUCOMMANDCLIENT_H
#pragma once
#include <QObject>

class EcuCommandClient : public QObject {
    Q_OBJECT
public:
    explicit EcuCommandClient(QObject *parent = nullptr);

    Q_INVOKABLE void throttleOn();
    Q_INVOKABLE void throttleOff();
    Q_INVOKABLE void brakeOn();
    Q_INVOKABLE void brakeOff();
    Q_INVOKABLE void resetEcu();

private:
    void sendCmd(const char *cmd);
};
