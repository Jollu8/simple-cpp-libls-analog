#pragma once

#include <chrono>
#include <future>
#include <shared_mutex>
#include <span>
#include <string>
#include <utility>
#include <variant>
#include <vector>


#include "dns/data.h"

namespace dnslib {
    using boost::asio::ip::udp;
    using namespace std::chrono_literals;
    enum class ServerConnMethod {
        SC_UDP
    };

    struct DnsServer {
        std::variant<IPV4Type, IPV6Type> ip;
        int port = 53;
        ServerConnMethod conn_method;
        bool recursive;

        bool operator==(const DnsServer &) const = default;
    };

    struct Resolution {
        std::vector<Rr> records;
        ResponseCode rcode;
    };

    class Resolver {
    public:
        virtual
        boost::asio::awaitable<Resolution> resolve(
                Labels labels,
                RrType qtype,
                RrClass qclass,
                bool recursive_desired,
                std::chrono::milliseconds timeout
        ) = 0;
    };

    class DefaultResolver : public Resolver {
    public:
        explicit DefaultResolver(boost::asio::io_context &io,
                                 std::vector<DnsServer> server = {}) :
                m_io(io),
                m_slist(std::move(server)),
                m_slist_mutex(std::make_unique<std::shared_mutex>()) {}

    private:
        boost::asio::io_context &m_io;
        std::vector<DnsServer> m_slist;
        std::unique_ptr<std::shared_mutex> m_slist_mutex;

        template<class CompletionToken>
        auto async_query_server(
                const DnsServer &server,
                Message query,
                std::chrono::milliseconds timeout,
                CompletionToken && token
        );

    };
} // namespace dnslib