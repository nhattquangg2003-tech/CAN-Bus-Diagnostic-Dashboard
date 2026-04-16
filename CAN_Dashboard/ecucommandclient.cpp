#include "ecucommandclient.h"

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <QDebug>

static const char* CMD_SOCKET = "/tmp/ecu_cmd.sock";

EcuCommandClient::EcuCommandClient(QObject *parent) : QObject(parent) {}

void EcuCommandClient::sendCmd(const char *cmd) {
    int fd = ::socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
        qWarning() << "EcuCommandClient: socket(AF_UNIX) failed";
        return;
    }

    struct sockaddr_un addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, CMD_SOCKET, sizeof(addr.sun_path) - 1);

    const int n = (int)::sendto(fd, cmd, (int)std::strlen(cmd), 0,
                                 (struct sockaddr*)&addr, sizeof(addr));
    if (n < 0) {
        qWarning() << "EcuCommandClient: sendto failed. Is ecu_sim running?";
    }
    ::close(fd);
}

void EcuCommandClient::throttleOn()  { sendCmd("THROTTLE_ON"); }
void EcuCommandClient::throttleOff() { sendCmd("THROTTLE_OFF"); }
void EcuCommandClient::brakeOn()     { sendCmd("BRAKE_ON"); }
void EcuCommandClient::brakeOff()    { sendCmd("BRAKE_OFF"); }
void EcuCommandClient::resetEcu()    { sendCmd("RESET"); }
