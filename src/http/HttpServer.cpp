/* 本文件修改自Arduino的WiFiServer，并去除了TCP相关的逻辑。 */
#include "http/HttpServer.h"
#include "WiFi.h"
#include "http/HttpClient.h"
#include "server_drv.h"

HttpServer::HttpServer() : _port(80), _sock_ser(-1) {}

HttpServer::HttpServer(uint16_t port) : _port(port), _sock_ser(-1) {}

HttpServer::~HttpServer() {
    close();
}

void HttpServer::begin() {
    _sock_ser = serverdrv.startServer(_port, TCP_MODE, NON_BLOCKING_MODE);
    if (_sock_ser < 0) {
        printf("\n[ERROR] HTTP Server start failed \n\r");
    } else {
        printf("\n[INFO] HTTP Server started successfully on port %d \n\r", _port);
    }
}

HttpClient HttpServer::available() {
    int client_fd = serverdrv.getAvailable(_sock_ser);
    return HttpClient(client_fd);
}

void HttpServer::close() {
    if (_sock_ser >= 0) {
        serverdrv.stopSocket(_sock_ser);
        _sock_ser = -1;
    }
}
