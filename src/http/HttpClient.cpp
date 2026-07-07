#include "WiFi.h"

extern "C" {
#include "wl_definitions.h"
#include "wl_types.h"
#include "string.h"
#include "errno.h"
#include "update.h"
}

#include "HttpClient.h"
#include "HttpServer.h"
#include "server_drv.h"

HttpClient::HttpClient():
    _sock(MAX_SOCK_NUM)
{
    _is_connected = false;
    recvTimeout = 3000;
}

HttpClient::HttpClient(uint8_t sock)
{
    _sock = sock;
    if ((sock >= 0) && (sock != 0xFF)) {
        _is_connected = true;
    }
    recvTimeout = 3000;
}

HttpClient::HttpClient(tPortMode portMode):
    _sock(MAX_SOCK_NUM)
{
    _is_connected = false;
    recvTimeout = 3000;
    _portMode = portMode;
}

HttpClient::HttpClient(tBlockingMode blockMode):
    _sock(MAX_SOCK_NUM)
{
    _is_connected = false;
    recvTimeout = 3000;
    _is_blocked = blockMode;
}

HttpClient::HttpClient(uint8_t sock, tPortMode portMode)
{
    _sock = sock;
    if ((sock >= 0) && (sock != 0xFF)) {
        _is_connected = true;
    }
    recvTimeout = 3000;
    _portMode = portMode;
}

HttpClient::HttpClient(uint8_t sock, tPortMode portMode, tBlockingMode blockMode)
{
    _sock = sock;
    if ((sock >= 0) && (sock != 0xFF)) {
        _is_connected = true;
    }
    recvTimeout = 3000;
    _portMode = portMode;
    _is_blocked = blockMode;
}

HttpClient::~HttpClient()
{
    stop();
}

uint8_t HttpClient::connected()
{
    if ((_sock < 0) || (_sock == 0xFF)) {
        _is_connected = false;
        return 0;
    } else {
        if (_is_connected) {
            return 1;
        } else {
            stop();
            return 0;
        }
    }
}

int HttpClient::available()
{
    if (!_is_connected || _sock < 0) {
        return 0;
    }

    while (true) {
        if (clientdrv.availData(_sock) > 0) {
            return 1;
        } else {
            int err = clientdrv.getLastErrno(_sock);
            if (err == EAGAIN) {
                continue;
            }
            if (err != 0) {
                _is_connected = false;
            }
            return 0;
        }
    }
}

int HttpClient::read()
{
    int ret;
    int err;
    uint8_t b[1];

    if (!available()) {
        return -1;
    }

    ret = clientdrv.getData(_sock, b);
    if (ret > 0) {
        return b[0];
    } else {
        err = clientdrv.getLastErrno(_sock);
        if (err != EAGAIN) {
            _is_connected = false;
        }
    }
    return ret;
}

int HttpClient::read(uint8_t *buf, size_t size)
{
    uint16_t _size = size;
    int ret;
    int err;

    ret = clientdrv.getDataBuf(_sock, buf, _size);
    if (ret <= 0) {
        err = clientdrv.getLastErrno(_sock);
        if (err != EAGAIN) {
            _is_connected = false;
        }
    }
    return ret;
}

int HttpClient::recv(uint8_t *buf, size_t size)
{
    uint16_t _size = size;
    int ret;
    int err;

    ret = clientdrv.recvData(_sock, buf, _size);
    if (ret <= 0) {
        err = clientdrv.getLastErrno(_sock);
        if (err != EAGAIN) {
            _is_connected = false;
        }
    }
    return ret;
}

void HttpClient::stop()
{
    if (_sock < 0) {
        return;
    }
    clientdrv.stopSocket(_sock);
    _is_connected = false;
    _sock = -1;
}

size_t HttpClient::write(uint8_t b)
{
    return write(&b, 1);
}

// set WiFi client to blocking/non-blocking mode
void HttpClient::setBlockingMode()
{
    _is_blocked = BLOCKING_MODE;
}

void HttpClient::setNonBlockingMode()
{
    _is_blocked = NON_BLOCKING_MODE;
}

size_t HttpClient::write(const uint8_t *buf, size_t size)
{
    if (_sock < 0) {
        setWriteError();
        return 0;
    }
    if (size == 0) {
        setWriteError();
        return 0;
    }

    if (!clientdrv.sendData(_sock, buf, size)) {
        setWriteError();
        _is_connected = false;
        return 0;
    }
    return size;
}

HttpClient::operator bool()
{
    return _sock >= 0;
}

int HttpClient::connect(const char *host, uint16_t port)
{
    IPAddress remote_addr;
    IPv6Address remote_addr_v6;

    if (getIPv6Status() == 0) {
        if (WiFi.hostByName(host, remote_addr)) {
            return connect(remote_addr, port);
        }
    } else {
        // printf("\n\r[INFO]HttpClient.cpp: connect hostByNameV6() \n\r");
        if (WiFi.hostByNamev6(host, remote_addr_v6)) {
            // printf("[INFO]HttpClient.cpp: connect ipv6: %s\n\r", host);
            _sock = clientdrv.startClientV6(host, port, TCP_MODE);
        } else {
        }
        // whether sock is connected
        if (_sock < 0) {
            _is_connected = false;
            return 0;
        } else {
            _is_connected = true;
            clientdrv.setSockRecvTimeout(_sock, recvTimeout);
        }
        return 1;
    }
    return 0;
}

int HttpClient::connect(IPAddress ip, uint16_t port)
{
    _is_connected = false;
    _sock = clientdrv.startClient(ip, port, _portMode, _is_blocked);
    // whether sock is connected
    if (_sock < 0) {
        _is_connected = false;
        return 0;
    } else {
        _is_connected = true;
        clientdrv.setSockRecvTimeout(_sock, recvTimeout);
    }
    return 1;
}

int HttpClient::connectv6(IPv6Address ipv6, uint16_t port)
{
    _is_connected = false;
    _sock = clientdrv.startClientv6(ipv6, port);
    // printf("[INFO]wifiClient.cpp: connectv6 sock value: %x\n\r", _sock);
    if (_sock < 0) {
        _is_connected = false;
        // printf("[INFO]wifiClient.cpp: connectv6 not connected\n\r");
        return 0;
    } else {
        _is_connected = true;
        // printf("[INFO]wifiClient.cpp: connectv6 connected\n\r");
        clientdrv.setSockRecvTimeout(_sock, recvTimeout);
    }
    return 1;
}

int HttpClient::peek()
{
    uint8_t b;
    if (!available()) {
        return -1;
    }
    clientdrv.getData(_sock, &b, 1);

    return b;
}

void HttpClient::flush()
{
    while (available()) {
        read();
    }
}

// extend API from RTK

int HttpClient::setRecvTimeout(int timeout)
{
    if (connected()) {
        recvTimeout = timeout;
        clientdrv.setSockRecvTimeout(_sock, recvTimeout);
    }
    return 0;
}

int HttpClient::read(char *buf, size_t size)
{
    read(((uint8_t *)buf), size);

    return 0;
}

int HttpClient::enableIPv6()
{
    return clientdrv.enableIPv6();
}

int HttpClient::getIPv6Status()
{
    return clientdrv.getIPv6Status();
}
