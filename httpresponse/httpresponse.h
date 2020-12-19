#ifndef LITTLEHTTPD_HTTPRESPONSE_H
#define LITTLEHTTPD_HTTPRESPONSE_H
#include "../httprequest/httprequest.h"
#include "../socket/socket.h"
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

#define SERVER_STRING "Server: littleHttpd/0.1.0\r\n"

class HttpResponse {
public:
    HttpResponse(HttpRequest *httpRequest);
    void Response_();


private:
    HttpRequest *httpRequest_;
    Socket *client_;

    void Unimplemented_();
    void NotFound_();
    void ServeFile_(char *filename);
    void Headers_();
    void Cat_(FILE *resource);
    void ExecuteCgi_();
    void BadRequest_();
    void CannotExecute_();
};


#endif //LITTLEHTTPD_HTTPRESPONSE_H
