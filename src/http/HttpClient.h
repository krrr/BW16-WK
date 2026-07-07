#pragma once

#include "Client.h"
#include "IPAddress.h"
#include "IPv6Address.h"
#include "Print.h"
#include "server_drv.h"
#include <ArduinoJson.h>


class HttpClient: public Client {
public:
    HttpClient();
    HttpClient(uint8_t sock);
    HttpClient(tPortMode portMode);
    HttpClient(tBlockingMode blockMode);
    HttpClient(uint8_t sock, tPortMode portMode);
    HttpClient(uint8_t sock, tPortMode portMode, tBlockingMode blockMode);
    ~HttpClient();

    uint8_t status();
    virtual uint8_t connected();
    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int recv(uint8_t *buf, size_t size);
    virtual void stop();
    virtual void setBlockingMode();
    virtual void setNonBlockingMode();
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual operator bool();
    virtual int connect(const char *host, uint16_t port);
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connectv6(IPv6Address ipv6, uint16_t port);
    virtual int peek();
    virtual void flush();
    // extend API from RTK
    int setRecvTimeout(int timeout);
    int read(char *buf, size_t size);
    // IPv6 related
    int enableIPv6();
    int getIPv6Status();

    // HTTP Parsing & Responses
    bool parseRequest();
    const String& method() const { return _method; }
    const String& path() const { return _path; }
    const String& queryString() const { return _queryString; }
    const String& body() const { return _body; }
    String queryParam(const String& name) const;

    void sendJson(JsonDocument& doc);
    void sendJsonFail(const String& message);
    void sendHtml(const uint8_t* content, size_t length, bool gzip = false);
    void sendNotFound();

    static String urlDecode(const String& s);

    friend class HttpServer;
    using Print::write;

private:
    int8_t _sock;
    ServerDrv clientdrv;
    bool _is_connected;
    uint8_t data[DATA_LENTH];
    int recvTimeout;
    tPortMode _portMode = TCP_MODE;
    tBlockingMode _is_blocked = NON_BLOCKING_MODE;

    String _method;
    String _path;
    String _queryString;
    String _body;
};

#ifdef __cplusplus
extern "C" {
#endif
extern int update_ota_local(char *ip, int port);
#ifdef __cplusplus
}
#endif
