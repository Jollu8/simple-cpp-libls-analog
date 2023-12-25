#pragma once

#include <boost/asio.hpp>

#include "dns/data.h"
#include "dns/lookup.h"
#include "dns/resolver.h"

namespace dnslib {
    template<std::derived_from<Lookup> L>
    class Responder {
    public:
    private:
        L m_lookup;

        boost::asio::awaitable<void> add_additional_records_from_mx(
                std::span<std::string const> labels,
                Message &response,
                bool recursive
        ) {
            auto found_records = co_await m_lookup.find_records(labels, RrType::RT_A, RrClass::RC_IN, recursive);
            for (auto &record: found_records.records) {
                response.additional.push_back(record);
            }
            co_return;
        }

        void check_authorities(const Question &question, Message &response) {
            auto authorities = m_lookup.find_authorities(question.labels, question.qclass);
            for (auto &authority: authorities) {
                auto ns_record = Rr::ns_record(authority.domain, authority.name, authority.ttl);
                response.authorities.push_back(ns_record);
                for (auto &ip: authority.ips) {
                    auto a_record = Rr::a_record(authority.name, ip, 0);
                    response.additional.push_back(a_record);
                }
            }
            if (!authorities.empty()) {
                response.header.aa = 0;
            }
        }
    };
} // namespace dnslib