#include "socket.h"

Socket::Socket() {
    fd_ = -1;
}

Socket::Socket(int fd) {
    fd_ = fd;
}

Socket::~Socket() {
    Destroy_();
}

bool Socket::Create_() {
    void *res;
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        return false;
    }
    // 设置地址可重复使用，方便测试
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, res, sizeof(res));
    return true;
}

void Socket::Destroy_() {
    if (fd_ >= 0) {
        close(fd_);
    }
    fd_ = -1;
}

bool Socket::Bind_(unsigned short port) {
    struct sockaddr_in sockAddr;
    sockAddr.sin_family      = AF_INET;
    sockAddr.sin_port        = htons(port);
    sockAddr.sin_addr.s_addr = INADDR_ANY;  //INADDR_ANY用于多网卡机器，表示任意网卡的IP都能访问httpd,即0.0.0.0

    int res = bind(fd_, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
    if (res == -1) {
        return false;
    }
    return true;
}

bool Socket::Listen_(unsigned int size) {
    int res = listen(fd_, size);
    if (res == -1) {
        return false;
    }
    return true;
}

Socket * Socket::Accept_() {
    struct sockaddr_in sockAddr;

    socklen_t sockLen = sizeof(sockAddr);

    int fd = accept(fd_, (struct sockaddr *)&sockAddr, &sockLen);
    if (fd == -1) {
        return NULL;
    }
    return (new Socket(fd));
}

ssize_t Socket::Write_(const void *buffer, size_t len) {
    return write(fd_, buffer, len);
}

ssize_t Socket::Read_(void *buffer, size_t len) {
    return read(fd_, buffer, len);
}