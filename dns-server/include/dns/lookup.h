#pragma once

#include <span>

#include "dns/data.h"
#include "dns/resolver.h"

namespace dnslib {
    struct DomainAuthority {
        Labels domain;
        Labels name;
        RrClass rrClass = RrClass::RC_IN;
        std::vector<uint32_t> ips;
        uint32_t ttl;

        auto operator<=>(const DomainAuthority &) const = default;
    };

    bool is_label_match(std::span<std::string const> labels, const Rr &candidate);

    bool is_authority_mathc(std::span<std::string const> labels, const Rr &candidate);

    bool is_authority_match(std::span<std::string const> labels, const DomainAuthority &authority,
                            RrClass rrClass = RrClass::RC_IN);

    struct FoundRecords {
        std::vector<Rr> records;
        std::error_code err;

    };

    class Lookup {
    public:
        virtual boost::asio::awaitable<FoundRecords> find_records(
                std::span<std::string const> labels, RrType qtype, RrClass qclass, bool recursive
        ) = 0;

        virtual std::vector<DomainAuthority> find_authorities(
                std::span<std::string const> labels, RrClass rrClass
        ) = 0;

        virtual bool supports_recursion() = 0;
    };
} // namespace dnslib