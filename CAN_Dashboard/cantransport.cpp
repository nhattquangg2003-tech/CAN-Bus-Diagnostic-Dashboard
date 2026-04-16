#include "cantransport.h"
#include <QSocketNotifier>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

CanTransport::CanTransport(QObject *parent) : QObject(parent)
{
    m_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    ifreq ifr{};
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(m_fd, SIOCGIFINDEX, &ifr);

    sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(m_fd, (sockaddr *)&addr, sizeof(addr));

    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated, this, [=] {
        can_frame frame;
        if (read(m_fd, &frame, sizeof(frame)) > 0) {
            emit frameReceived(QByteArray(
                reinterpret_cast<char*>(&frame),
                sizeof(frame)));
        }
    });
}
