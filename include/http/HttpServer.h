#pragma once

#include "Server.h"
#include "server_drv.h"

class HttpClient;

class HttpServer: public Server {
public:
    HttpServer();
    HttpServer(uint16_t port);
    ~HttpServer();

    virtual void begin() override;
    HttpClient available();
    virtual size_t write(uint8_t b) override { (void)b; return 0; }

private:
    void close();

    ServerDrv serverdrv;
    uint16_t _port;
    int _sock_ser;
};
