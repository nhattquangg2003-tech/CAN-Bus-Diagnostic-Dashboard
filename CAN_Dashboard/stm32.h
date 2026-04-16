#ifndef STM32_H
#define STM32_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
class Stm32 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dataFromStm32 READ dataFromStm32 NOTIFY dataFromStm32Changed);

    Q_PROPERTY(bool led1 READ led1 NOTIFY ledChanged)
    Q_PROPERTY(bool led2 READ led2 NOTIFY ledChanged)
    Q_PROPERTY(bool led3 READ led3 NOTIFY ledChanged)
    Q_PROPERTY(bool led4 READ led4 NOTIFY ledChanged)
    Q_PROPERTY(bool led5  READ led5  NOTIFY ledChanged)
    Q_PROPERTY(double speed READ speed NOTIFY speedChanged)
    //rpm
    Q_PROPERTY(double rpm READ rpm NOTIFY rpmChanged) // Thêm thuộc tính rpm
    // xăng
    Q_PROPERTY(double fuelLevel READ fuelLevel NOTIFY fuelLevelChanged) // Thêm thuộc tính fuelLevel
public:
    explicit Stm32(QObject *parent = nullptr);

    QString dataFromStm32() const;

    Q_INVOKABLE void getDataFromStm32(const QString &portName);
    bool led1() const;

    bool led2() const;

    bool led3() const;

    bool led4() const;

    bool led5() const;

    double speed() const;

    double rpm() const;

    double fuelLevel() const;

signals:
    void dataFromStm32Changed();
    void ledChanged(); // Signal chung cho cả 4 đèn

    void speedChanged();

    void rpmChanged();

    void fuelLevelChanged();

private slots:
    void onReadyReadDataFromStm32();
private:
    QString m_dataFromStm32;
    QByteArray m_buffer; // Buffer tạm để lưu dữ liệu chưa hoàn chỉnh
    QSerialPort *m_serialPort;

    bool m_led1 = false, m_led2 = false, m_led3 = false, m_led4 = false;
    bool m_led5;
    double m_speed;
    double m_rpm;
    double m_fuelLevel;
};

#endif // STM32_H
