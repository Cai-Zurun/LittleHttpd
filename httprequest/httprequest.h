#ifndef LITTLEHTTPD_HTTPREQUEST_H
#define LITTLEHTTPD_HTTPREQUEST_H
#include "../socket/socket.h"
#include "string"
#include "iostream"
#include "string.h"
#include "stdio.h"

using namespace std;


class HttpRequest {
public:
    enum Error {
        Correct,
        Unimplemented,
        NotFound,
    };
    HttpRequest();
    HttpRequest(Socket *client);

    char method_[255];
    bool cgi_;
    char path_[255];
    Error error_;
    Socket *client_;
    char *queryString_;

    void Parse_();
    int GetLine_(char *buf, int size);

private:
};

#endif //LITTLEHTTPD_HTTPREQUEST_H
