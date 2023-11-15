//
/// Created by Jollu Emil
//

#include "TCPClient.h"
#include <memory>

TCPClient::TCPClient() {
    m_sock = -1;
    m_port = 0;
    m_address = "";
}

bool TCPClient::Setup(const std::string &address, int port) {
    if(m_sock == -1) {
        m_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(m_sock == -1) std::cout << "Could not create socket" << std::endl;

    }
    if((signed) inet_addr(address.c_str())  == -1) {
        hostent *he;
        in_addr ** addr_list;
        if((he = gethostbyname(address.c_str())) == nullptr) {
            herror("gethostbyname");
            std::cout << "Failed to resolve hostname\n";
            return false;
        }
        addr_list = (in_addr**) he->h_addr_list;
        for(auto i = 0; addr_list[i] != nullptr; ++i) {
            m_server.sin_addr = *addr_list[i];
            break;
        }
    }
    else
        m_server.sin_addr.s_addr = inet_addr(address.c_str());

    m_server.sin_family = AF_INET;
    m_server.sin_port = htons(port);
    if(connect(m_sock, (sockaddr*)&m_server, sizeof (m_server)) < 0) {
        perror("connect failed. Error");
        return false;
    }
    return true;

}

bool TCPClient::Send(const std::string & data) {
    if(m_sock != -1) {
        if(send(m_sock, data.c_str(), strlen(data.c_str()), 0) < 0) {
            std::cout << "Send failed : " << data << std::endl;
            return false;
        }
    }
    else return false;
    return true;
}


std::string TCPClient::Receive( int size_) {
    char *buffer = new char [size_];
    memset(&buffer[0], 0, sizeof (buffer));
    std::string reply;
    if(recv(m_sock, buffer, size_, 0) < 0) {
        std::cout << "receive failed!" << std::endl;
        delete[] buffer;
        return nullptr;
    }

    buffer[size_-1] = '\0';
    reply = buffer;
    delete[] buffer;
    return reply;

}

std::string TCPClient::Read() {
    char *buffer = new char [1]{};
    std::string reply;
    while(buffer[0] != '\n') {
        if(recv(m_sock, buffer, sizeof (buffer), 0) < 0) {
            std::cout << "receive failed!" << std::endl;
            return nullptr;
        }
        reply += buffer[0];
    }
    return reply;
}

void TCPClient::Exit() {
    close(m_sock);
}
