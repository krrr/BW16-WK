#pragma once

#include "Client.h"
#include <ArduinoJson.h>
#include "server_drv.h"

class HttpClient: public Client {
public:
    HttpClient();
    HttpClient(uint8_t sock);
    ~HttpClient();

    // Client / Stream implementation
    virtual int connect(const char *host, uint16_t port) override { (void)host; (void)port; return 0; }
    virtual int connect(IPAddress ip, uint16_t port) override { (void)ip; (void)port; return 0; }
    virtual size_t write(uint8_t b) override;
    virtual size_t write(const uint8_t *buf, size_t size) override;
    virtual int available() override;
    virtual int read() override;
    virtual int read(uint8_t *buf, size_t size) override;
    virtual int peek() override;
    virtual void flush() override;
    virtual void stop() override;
    virtual uint8_t connected() override;
    virtual operator bool() override;

    int8_t getSock() const { return _sock; }

    // HTTP Parsing & Responses
    bool parseRequest();
    const String& method() const { return _method; }
    const String& path() const { return _path; }
    const String& queryString() const { return _queryString; }
    const String& body();
    String queryParam(const String& name) const;

    void sendJson(JsonDocument& doc);
    void sendJsonFail(const String& message);
    void sendJsonSuccess(const String& message);
    void sendHtml(const uint8_t* content, size_t length, bool gzip = false);
    void sendNotFound();

    static String urlDecode(const String& s);

    friend class HttpServer;
    using Print::write;

private:
    int8_t _sock;
    ServerDrv clientdrv;
    bool _is_connected;
    int recvTimeout;

    String _method;
    String _path;
    String _queryString;
    String _body;
    int _contentLength;
};