#ifndef LITTLEHTTPD_SERVER_H
#define LITTLEHTTPD_SERVER_H
#include "../socket/socket.h"
#include "iostream"
#include <arpa/inet.h>
#include "../httprequest/httprequest.h"
#include "../httpresponse/httpresponse.h"

using namespace std;

class Server {
public:
    Server(unsigned short port = 80);
    ~Server();
    bool Start_();
    void Stop_();
    Socket *Accept_();
    bool Handle_(Socket *client);

private:
    Socket socket_;
    unsigned short port_;
};


#endif //LITTLEHTTPD_SERVER_H
