//
// Created by Jollu Emil .
//  Socket.hpp
//  This file is part of VallauriSoft

#pragma once

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <cstdlib>

#include <arpa/inet.h>

const int MAX_BUFFER = 1024;

namespace Socket {
    using Socket = int;
    using Ip = std::string;
    using Port = unsigned int;
    using Data = std::string;

    typedef struct {
        Ip ip;
        Port port;

    } Address;

    typedef struct {
        Address address;
        Data data;
    } Datagram;

    sockaddr_in *to_sockaddr(Address *a) {
        sockaddr_in *ret;
        ret = (sockaddr_in *) malloc(sizeof(sockaddr_in));
        ret->sin_family = AF_INET;
        inet_aton(a->ip.c_str(), &(ret->sin_addr));
        ret->sin_port = htons(a->port);
        return ret;
    }

    Address *from_sockaddr(sockaddr_in *address) {
        Address *ret;
        ret = (Address *) malloc(sizeof(Address));
        ret->ip = inet_ntoa(address->sin_addr);
        ret->port = ntohs(address->sin_port);

        return ret;
    }

    class Exception {
    private:
        std::string m_message;
    public:
        Exception(const std::string &error) {
            this->m_message = error;

        }

        virtual const char *what() const {
            return this->m_message.c_str();
        }
    };


}