#pragma once

#include "dns/lookup.h"

namespace dnslib {
    class StaticLookup : public Lookup {
    public:
        StaticLookup() = default;

        boost::asio::awaitable<FoundRecords> find_records(
                std::span<std::string const> labels, RrType qtype, RrClass qclass, bool recursive
        ) override;

        std::vector<DomainAuthority> find_authorities(std::span<std::string const> labels, RrClass rrClass) override;

        void add_record(Rr record) {
            m_records[labels_to_string(record.labels)].push_back(record);
            if (record.labels.at(0) == "*" && record.labels.size() >= 2) {
                m_wildcard_records[labels_to_string(record.labels)].push_back(record);
            }
        }

        void add_authority(const DomainAuthority &authority) {
            m_authorities.emplace_back(authority);
        }

        bool supports_recursion() override { return false; }

    private:
        std::unordered_map<std::string, std::vector<Rr>> m_records;
        std::unordered_map<std::string, std::vector<Rr>> m_wildcard_records;
        std::vector<DomainAuthority> m_authorities;

        void mathc_wildcards(
                std::span<std::string const> labels, RrType qtype,
                RrClass rrClass, std::vector<Rr> &matching_records
        );
    };
} // namespace dnslib