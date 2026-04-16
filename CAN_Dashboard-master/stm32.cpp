#include "stm32.h"
#include <QDateTime>
#include <QDebug>

Stm32::Stm32(QObject *parent) : QObject(parent) {
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &Stm32::onReadyReadDataFromStm32);
}

void Stm32::getDataFromStm32(const QString &portName) {
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        m_dataFromStm32 = "Connected to " + portName;
        emit dataFromStm32Changed();
    } else {
        m_dataFromStm32 = "Connect failed!";
        emit dataFromStm32Changed();
    }
}



void Stm32::onReadyReadDataFromStm32() {
    m_buffer += m_serialPort->readAll();

    while (m_buffer.contains('\n')) {
        QString line = QString::fromUtf8(m_buffer).section('\n', 0, 0).trimmed();
        int index = m_buffer.indexOf('\n');
        m_buffer.remove(0, index + 1);

        // Xử lý LED theo bitmask
        if (line.contains("LED=")) {
            bool ok;
            int ledMask = line.split('=')[1].toInt(&ok, 16);
            if (!ok) {
                qDebug() << "Invalid LED mask:" << line;
                continue;
            }
            m_led1 = (ledMask & 0x01); // bit0
            m_led2 = (ledMask & 0x02); // bit1
            m_led3 = (ledMask & 0x04); // bit2
            m_led4 = (ledMask & 0x08); // bit3
            m_led5  = (ledMask & 0x10 ); // bit4
            emit ledChanged();

        }
        // xử lí SPEED
        if (line.contains("SPEED=")) {
            bool ok;
            double speedValue = line.split('=')[1].toDouble(&ok);
            if (!ok) {
                qDebug() << "Invalid SPEED value:" << line;
                continue;
            }
            m_speed = speedValue;
            m_rpm = speedValue * 20.33; // Giả sử RPM = SPEED * 33.33, bạn có thể điều chỉnh hệ số này theo thực tế
            emit speedChanged();
            emit rpmChanged();
        }

        // xử lí fuel level
        if (line.contains("FUEL=")) {
            bool ok;
            double fuelValue = line.split('=')[1].toDouble(&ok);
            if (!ok) {
                qDebug() << "Invalid FUEL value:" << line;
                continue;
            }
            m_fuelLevel = fuelValue;
            emit fuelLevelChanged();
        }
        m_dataFromStm32 = "[" + QDateTime::currentDateTime().toString("ss.zzz") + "] " + line;

        emit dataFromStm32Changed();
    }
}


QString Stm32::dataFromStm32() const {
    return m_dataFromStm32;
}

bool Stm32::led1() const
{
    return m_led1;
}

bool Stm32::led2() const
{
    return m_led2;
}

bool Stm32::led3() const
{
    return m_led3;
}

bool Stm32::led4() const
{
    return m_led4;
}

bool Stm32::led5() const
{
    return m_led5;
}

double Stm32::speed() const
{
    return m_speed;
}

double Stm32::rpm() const
{
    return m_rpm;
}

double Stm32::fuelLevel() const
{
    return m_fuelLevel;
}
