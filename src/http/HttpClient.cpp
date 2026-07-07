/* 本文件修改自Arduino的WiFiClient，并去除了TCP相关的逻辑。 */
#include "WiFi.h"

extern "C" {
#include "wl_definitions.h"
#include "wl_types.h"
#include "string.h"
#include "errno.h"
}

#include "HttpClient.h"
#include "HttpServer.h"
#include "server_drv.h"

HttpClient::HttpClient():
    _sock(MAX_SOCK_NUM), _is_connected(false), recvTimeout(3000), _contentLength(0)
{
}

HttpClient::HttpClient(uint8_t sock):
    _sock(sock), recvTimeout(3000), _contentLength(0)
{
    _is_connected = ((sock >= 0) && (sock != 0xFF));
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
    }
    if (_is_connected) {
        return 1;
    }
    stop();
    return 0;
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

size_t HttpClient::write(const uint8_t *buf, size_t size)
{
    if (_sock < 0 || size == 0) {
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

static char hexNibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

String HttpClient::urlDecode(const String& s) {
    String out;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '%' && i + 2 < (int)s.length()) {
            out += (char)(hexNibble(s[i+1]) << 4 | hexNibble(s[i+2]));
            i += 2;
        } else if (s[i] == '+') {
            out += ' ';
        } else {
            out += s[i];
        }
    }
    return out;
}

bool HttpClient::parseRequest() {
    unsigned long timeout = millis() + 2000;
    while (!available() && millis() < timeout) {
        delay(1);
    }
    if (!available()) {
        return false;
    }

    String reqLine = readStringUntil('\r');
    read(); // consume '\n'

    // Parse method and path
    int firstSpace = reqLine.indexOf(' ');
    if (firstSpace < 1) return false;
    _method = reqLine.substring(0, firstSpace);

    int secondSpace = reqLine.indexOf(' ', firstSpace + 1);
    if (secondSpace < 0) return false;
    String fullPath = reqLine.substring(firstSpace + 1, secondSpace);

    int qm = fullPath.indexOf('?');
    if (qm >= 0) {
        _path = fullPath.substring(0, qm);
        _queryString = fullPath.substring(qm + 1);
    } else {
        _path = fullPath;
        _queryString = "";
    }

    // Read headers
    _contentLength = 0;
    while (available()) {
        String line = readStringUntil('\n');
        String trimmed = line;
        trimmed.trim();
        if (trimmed.length() == 0) break;

        String lowerLine = trimmed;
        lowerLine.toLowerCase();
        if (lowerLine.startsWith("content-length:")) {
            int colon = lowerLine.indexOf(':');
            if (colon >= 0) {
                _contentLength = lowerLine.substring(colon + 1).toInt();
            }
        }
    }

    _body = "";
    return true;
}

const String& HttpClient::body() {
    if (_body.length() == 0 && _contentLength > 0 && connected()) {
        _body.reserve(_contentLength);
        unsigned long bodyTimeout = millis() + 1000;
        while (_body.length() < (unsigned int)_contentLength && millis() < bodyTimeout) {
            if (available()) {
                _body += (char)read();
            } else {
                delay(10);
            }
        }
    }
    return _body;
}

String HttpClient::queryParam(const String& name) const {
    String target = name + "=";
    int p = -1;
    int searchStart = 0;
    while ((p = _queryString.indexOf(target, searchStart)) >= 0) {
        if (p == 0 || _queryString[p - 1] == '&') {
            int vstart = p + target.length();
            int vend = _queryString.indexOf('&', vstart);
            if (vend < 0) vend = _queryString.length();
            return urlDecode(_queryString.substring(vstart, vend));
        }
        searchStart = p + 1;
    }
    return "";
}

void HttpClient::sendJson(JsonDocument& doc) {
    String json;
    serializeJson(doc, json);
    println("HTTP/1.1 200 OK");
    println("Content-Type: application/json; charset=utf-8");
    print("Content-Length: ");
    println(json.length());
    println("Connection: close");
    println();
    print(json);
}

void HttpClient::sendJsonFail(const String& message) {
    JsonDocument doc;
    doc["success"] = false;
    doc["message"] = message;
    sendJson(doc);
}

void HttpClient::sendHtml(const uint8_t* content, size_t length, bool gzip) {
    println("HTTP/1.1 200 OK");
    println("Content-Type: text/html; charset=utf-8");
    if (gzip) {
        println("Content-Encoding: gzip");
    }
    print("Content-Length: ");
    println(length);
    println("Connection: close");
    println();
    write(content, length);
}

void HttpClient::sendNotFound() {
    println("HTTP/1.1 404 Not Found");
    println("Connection: close");
    println();
}
