#ifndef LITTLEHTTPD_SOCKET_H
#define LITTLEHTTPD_SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

class Socket {
public:
    Socket();
    Socket(int fd);
    ~Socket();

    int fd_;

    bool Create_();
    void Destroy_();
    bool Bind_(unsigned short port);
    bool Listen_(unsigned int size);
    Socket *Accept_();
    ssize_t Write_(const void *buffer, size_t len);
    ssize_t Read_(void *buffer, size_t len);

private:
};


#endif //LITTLEHTTPD_SOCKET_H
