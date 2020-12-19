#include "httprequest.h"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(Socket *client) {
    client_ = client;
    error_ = Error::Correct;
    cgi_ = false;
}

void HttpRequest::Parse_() {
    char buf[1024];
    char url[255];

    GetLine_(buf, sizeof(buf));
    size_t i = 0, j = 0;

    while ((buf[j] != ' ') && (i < sizeof(method_) - 1)) {
        method_[i] = buf[j];
        i++; j++;
    }
    method_[i] = '\0';

    if (strcasecmp(method_, "GET") && strcasecmp(method_, "POST")) {
        error_ = Error::Unimplemented;
        return;
    }

    if (strcasecmp(method_, "POST") == 0) {
        cgi_ = true;
    }
    // 跳过中间的空格
    while ((buf[j] == ' ') && (j < sizeof(buf))) {
        j++;
    }

    i = 0;
    while ((buf[j] != ' ') && (i < sizeof(url)) && (j < sizeof(buf))) {
        url[i] = buf[j];
        i++; j++;
    }
    url[i] = '\0';

    if (strcasecmp(method_, "GET") == 0) {
        queryString_ = url;
        while ((*queryString_ != '?') && (*queryString_ != '\0')) {
            queryString_++;
        }
        if (*queryString_ == '?') {
            cgi_ = true;
            *queryString_ = '\0';
            queryString_++;
        }
    }

    strcpy(path_, "/Users/zrun/CLionProjects/littleHttpd2/htdocs");
    strcat(path_, url);
    if (path_[strlen(path_) - 1] == '/'){
        strcat(path_, "index.html");
    }
}

int HttpRequest::GetLine_(char *buf, int size) {
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n')) {
        n = read(client_->fd_, &c, 1);
        if (n > 0) {
            if (c == '\r') {
                n = read(client_->fd_, &c, 1);
                if ((n > 0) && (c == '\n'))
                    break;
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';
    return i;
}
