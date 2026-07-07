#pragma once

#include "Server.h"
#include "server_drv.h"

class HttpClient;

class HttpServer: public Server {
public:
    HttpServer();
    HttpServer(uint16_t);
    HttpServer(tBlockingMode);
    HttpServer(uint16_t, tBlockingMode);
    ~HttpServer();

    virtual void begin();
    HttpClient available(uint8_t *status = NULL);
    virtual int available(int server_fd);
    virtual uint8_t connected();
    virtual int recv(uint8_t *buf, size_t size);
    virtual size_t write(uint8_t b);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close();
    virtual void setBlockingMode();
    virtual void setNonBlockingMode();
    // extend API from RTK
    virtual int setTimeout(int timeout);
    // IPv6 related
    int enableIPv6();
    int getIPv6Status();
    using Print::write;

private:
    ServerDrv serverdrv;
    uint16_t _port;
    int _sock_ser;
    bool _is_connected;
    uint8_t data[DATA_LENTH];
    int recvTimeout;
    tBlockingMode _is_blocked = NON_BLOCKING_MODE;
};

