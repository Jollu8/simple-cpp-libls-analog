#pragma once

#include "dns/lookup.h"
#include "dns/resolver.h"

namespace dnslib {
    template<std::derived_from<Resolver> R>
    class RecursiveLookup : Lookup {
    public:
        RecursiveLookup(
                boost::asio::io_context &io,
                R resolver,
                std::chrono::milliseconds timeout = 5s
        ) :
                m_io(io),
                m_resolver(std::move(resolver)),
                m_timeout(timeout) {}

        boost::asio::awaitable<FoundRecords> find_records(
                std::span<std::string const> labels,
                RrType qtype,
                RrClass qclass,
                bool recursive
        ) override;

        std::vector<DomainAuthority> find_authorities(
                std::span<std::string const>,
                RrClass

        ) override {
            return {};
        }
        bool supports_recursion() override {return true;}


    private:
        boost::asio::io_context &m_io;
        R m_resolver;
        std::chrono::milliseconds m_timeout;

    };

    template<std::derived_from<Resolver> R>
    inline boost::asio::awaitable<FoundRecords> RecursiveLookup<R>::find_records(std::span<const std::string> labels,
                                                                                 dnslib::RrType qtype,
                                                                                 dnslib::RrClass qclass,
                                                                                 bool /*recursive*/) {
        Labels label_vec(labels.begin(), labels.end());
        Resolution resolution = co_await m_resolver.resolve(label_vec, qtype, qclass, true, m_timeout);
        std::error_code err;
        if(resolution.rcode == ResponseCode::RC_REFUSED) {
            err = ResolutionError::DL_REMOTE_REFUSED;
        }
        co_return FoundRecords{
            .records = resolution.records,
            .err = err
        };
    }
} // namespace dnslib