/*
  HttpServer.cpp - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "HttpServer.h"
#include "WiFi.h"
#include "HttpClient.h"
#include "server_drv.h"
#include <string.h>


HttpServer::HttpServer(uint16_t port)
{
    _port = port;
}

HttpServer::HttpServer(tBlockingMode blockMode):
    _port(80)
{
    _is_blocked = blockMode;
}


HttpServer::HttpServer(uint16_t port, tBlockingMode blockMode)
{
    _port = port;
    _is_blocked = blockMode;
}

HttpServer::~HttpServer()
{
    close();
}

void HttpServer::begin()
{
    _is_connected = false;
    _sock_ser = serverdrv.startServer(_port, TCP_MODE, _is_blocked);
    if (_sock_ser < 0) {
        _is_connected = false;
        printf("\n[ERROR] Socket connect failed \n\r");
    } else {
        _is_connected = true;
        printf("\n[INFO] Socket connect successfully \n\r");
    }
}

HttpClient HttpServer::available(uint8_t *status)
{
    int client_fd = (int)(status);

    client_fd = serverdrv.getAvailable(_sock_ser);

    return HttpClient(client_fd);
}

int HttpServer::available(int server_fd)
{
    int client_fd;

    client_fd = serverdrv.getAvailable(server_fd);
    _sock_ser = client_fd;

    return client_fd;
}

uint8_t HttpServer::connected()
{
    if ((_sock_ser < 0) || (_sock_ser == 0xFF)) {
        _is_connected = false;
        return 0;
    } else {
        if (_is_connected) {
            return 1;
        } else {
            close();
            return 0;
        }
    }
}

int HttpServer::recv(uint8_t *buf, size_t size)
{
    uint16_t _size = size;
    int ret;
    int err;

    ret = serverdrv.recvData(_sock_ser, buf, _size);
    if (ret <= 0) {
        err = serverdrv.getLastErrno(_sock_ser);
        if (err != 0) {
            _is_connected = false;
        }
    }
    return ret;
}

size_t HttpServer::write(uint8_t b)
{
    return write(&b, 1);
}

size_t HttpServer::write(const uint8_t *buf, size_t size)
{
    if (_sock_ser < 0) {
        setWriteError();
        return 0;
    }
    if (size == 0) {
        setWriteError();
        return 0;
    }

    if (!serverdrv.sendData(_sock_ser, buf, size)) {
        setWriteError();
        return 0;
    }
    return size;
}

void HttpServer::close()
{
    if (_sock_ser < 0) {
        return;
    }
    serverdrv.stopSocket(_sock_ser);
    _is_connected = false;
    _sock_ser = -1;
}

// set WiFi server to blocking/non-blocking mode
void HttpServer::setBlockingMode()
{
    _is_blocked = BLOCKING_MODE;
}

void HttpServer::setNonBlockingMode()
{
    _is_blocked = NON_BLOCKING_MODE;
}


// extend API from RTK
int HttpServer::setTimeout(int timeout)
{
    if (connected()) {
        recvTimeout = timeout;
        serverdrv.setSockRecvTimeout(_sock_ser, recvTimeout);
    }
    return 0;
}

// IPv6 related
int HttpServer::enableIPv6()
{
    return serverdrv.enableIPv6();
}

int HttpServer::getIPv6Status()
{
    return serverdrv.getIPv6Status();
}
