#include <iostream>
#include "server/server.h"
#include "socket/socket.h"

using namespace std;

int main() {
    unsigned short port = 8888;
    bool res;

    Server *server = new Server(port);
    res = server->Start_();
    if (!res) {
        cout << "ERROR: Server could NOT be started!" << endl;
        return 1;
    }

    while (true) {
        Socket *client = server->Accept_();

        server->Handle_(client);
    }
}
