//
// Created by Jollu Emil
//

#pragma once

#include "Common.h"

namespace coro {
    class SocketAddr {
    public:
        SocketAddr(std::string const &host, short port) : m_host(host), m_port(port) {}

        struct sockaddr_in sockaddr() const;

        struct in_addr inAddr() const;

        std::string const &host() const { return m_host; }

        short port() const { return m_port; }

    private:
        std::string m_host;
        short m_port;
    };

    class SocketCloseException {
    public:
        SocketCloseException() {}
    };

    using SocketHandle = int;

    class Socket {
    public:
        Socket(int type = SOCK_STREAM, int protocol = IPPROTO_TCP);

        virtual ~Socket() { close(); }

        void bind(SocketAddr const &addr);

        void listen(int backlog);

        void shutdown(int now);

        void close();

        virtual void connect(SocketAddr const &addr);

        virtual Ptr<Socket> accept();

        virtual ssize_t write(char const *buf, std::size_t len, int flags = 0);

        virtual ssize_t read(char *buf, std::size_t len, int flags = 0);

        void writeAll(char const *buf, std::size_t len, int flags = 0);

        void readAll(char *buf, std::size_t len, int flags = 0);

        int setsockopt(int level, int option, int value);

    protected:
        Socket(SocketHandle sd, char const *bodus);

        SocketHandle acceptRaw();

    private:
        SocketHandle m_sd;
    };
}
