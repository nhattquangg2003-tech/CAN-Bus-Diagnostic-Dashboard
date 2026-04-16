#include "clustercontroller.h"
#include "cantransport.h"
#include <linux/can.h>

static QString hx(quint8 v) {
    return QString("%1").arg(v, 2, 16, QChar('0')).toUpper();
}

ClusterController::ClusterController(QObject *parent)
    : QObject(parent)
{
    auto can = new CanTransport(this);

    connect(can, &CanTransport::frameReceived,
            this, [=](const QByteArray &raw) {
                auto f = reinterpret_cast<const can_frame *>(raw.constData());
                if (f->can_id != 0x100)
                    return;

                m_rpm = ((f->data[0] << 8) | f->data[1]) / 4;
                m_speed = f->data[2];
                m_coolant = f->data[3] - 40;

                m_raw = QString("ID:0x%1 [%2] %3 %4 %5 %6 %7 %8 %9 %10")
                            .arg(f->can_id,3,16,QChar('0'))
                            .arg(f->can_dlc)
                            .arg(hx(f->data[0]))
                            .arg(hx(f->data[1]))
                            .arg(hx(f->data[2]))
                            .arg(hx(f->data[3]))
                            .arg(hx(f->data[4]))
                            .arg(hx(f->data[5]))
                            .arg(hx(f->data[6]))
                            .arg(hx(f->data[7]));

                emit updated();
            });
}
