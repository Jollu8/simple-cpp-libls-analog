//
//// Created by Jollu Emil
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

class TCPClient {
public: // Constructor

    TCPClient();

public: // Methods;

    bool Setup(const std::string &address, int port);

    bool Send(const std::string &data);

    std::string Receive(int size = 4096);

    std::string Read();

    void Exit();

private: // Fields
    int m_sock;
    std::string m_address;
    int m_port;
    sockaddr_in m_server;

};
