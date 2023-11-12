//
// Created by Jollu Emil on 11/12/23.
//
#pragma once

#include "../socket/Socket.h"

class UDP {
private:
    Socket::Socket m_socket_id;
    bool m_binded;
public:
    UDP() {
        this->m_socket_id = socket(AF_INET, SOCK_DGRAM, 0);
        if (this->m_socket_id == -1) throw Socket::Exception("[Constructor] Cannot create socket");
        this->m_binded = false;
    }

    ~UDP() = default;


public:
    void Close() const {
        shutdown(this->m_socket_id, SHUT_RDWR);
    }

    void Bind(Socket::Port port) {
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        if (this->m_binded) {
            this->Close();
            this->m_socket_id = socket(AF_INET, SOCK_DGRAM, 0);
        }

        if (::bind(this->m_socket_id, (sockaddr *) &address, sizeof(sockaddr_in)) == -1) {
            std::stringstream error;
            error << "[listen_on_port] with [port=" << "]" << port << "] Cannot bind socket";
            throw Socket::Exception(error.str());
        }
        this->m_binded = true;
    }

    void Send(const Socket::Ip& ip, Socket::Port port, const Socket::Data& data) const {
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_aton(ip.c_str(), &address.sin_addr);

        if (sendto(this->m_socket_id, (void *) data.c_str(), data.size() + 1, 0, (sockaddr *) &address,
                   sizeof(sockaddr_in)) == -1) {
            std::stringstream error;
            error << "[send] with [ip=" << ip << "] [prot=" << port << "] [data=" << data << "] Cannot send";
            throw Socket::Exception(error.str());
        }


    }

    [[nodiscard]] Socket::Datagram Receive() const {
        int size = sizeof(sockaddr_in);
        char *buffer = (char *) malloc(sizeof(char) * MAX_BUFFER);
        sockaddr_in address{};
        Socket::Datagram ret;
        if (recvfrom(this->m_socket_id, (void *) buffer, MAX_BUFFER, 0, (sockaddr *) &address, (socklen_t *) &size) ==
            1)
            throw Socket::Exception("[receive] Cannot receive");

        ret.data = buffer;
        ret.address.ip = inet_ntoa(address.sin_addr);
        ret.address.port = ntohs(address.sin_port);

        free(buffer);
        return ret;


    }
};

