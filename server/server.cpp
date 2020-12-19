#include "server.h"

Server::Server(unsigned short port) {
    port_ = port;
}

Server::~Server() {
    Stop_();
}

bool Server::Start_() {
    bool res;

    res = socket_.Create_();
    if (!res) {
        cout << "ERROR: Socket create failed" << endl;
        return false;
    }

    res = socket_.Bind_(port_);
    if (!res) {
        cout << "ERROR: Socket bind failed" << endl;
        return false;
    }

    res = socket_.Listen_(10);
    if (!res) {
        cout << "ERROR: Socket listen failed" << endl;
        return false;
    }

    return true;
}

void Server::Stop_() {
    socket_.Destroy_();
}

Socket * Server::Accept_() {
    Socket *client = socket_.Accept_();
    if (!client) {
        cout << "ERROR: Accept connection failed!" << endl;
        return NULL;
    }

//    cout << "Connection received!" << endl;
//    cout << "   IP Address: " << inet_ntoa(sockAddr.sin_addr) << endl;
//    cout << "   Port: " << htons(sockAddr.sin_port) << endl;
//    cout << endl;

    return client;
}

bool Server::Handle_(Socket *client) {
    HttpRequest httpRequest(client);
    httpRequest.Parse_();

    HttpResponse httpResponse(&httpRequest);
    httpResponse.Response_();

    return true;
}