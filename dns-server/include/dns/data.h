#pragma once

#include <arpa/inet.h>
#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <sstream>
#include <string>

#include "dns/buffer.h"
#include "dns/error.h"

namespace dnslib {
    enum class RrType : uint16_t {
        RT_A = 1,
        RT_NS = 2,
        RT_MD = 3,
        RT_MF = 4,
        RT_CNAME = 5,
        RT_SOA = 6,
        RT_MB = 7,
        RT_MG = 8,
        RT_MR = 9,
        RT_NULL = 10,
        RT_WKS = 11,
        RT_PTR = 12,
        RT_H_INFO = 13,
        RT_M_INFO = 14,
        RT_MX = 15,
        RT_TXT = 16,
        RT_AAAA = 28,
        RT_AXFR = 252, // QTYPE
        RT_MAIL_B = 253, // QTYPE
        RT_MAIL_A = 254,
        RT_ALL = 255,
    };

    enum class RrClass : uint16_t {
        RC_IN = 1,
        RC_CS = 2,
        RC_CH = 3,
        RC_HS = 4,
    };

    enum class ResponseCode : uint8_t {
        RC_OK = 0,
        RC_FORMAT_ERROR = 1,
        RC_SERVER_FAILURE = 2,
        RC_NAME_ERROR = 3,
        RC_NOT_IMPLEMENTED = 4,
        RC_REFUSED = 5,
    };

    using Labels = std::vector<std::string>;
    using IPV4Type = uint32_t;
    using IPV6Type = std::array<uint8_t, 16>;
    using IPType = std::variant<IPV4Type, IPV6Type>;

    std::string labels_to_string(std::span<std::string const> labels);

    struct Rr {
        std::vector<std::string> labels;
        RrType rrType;
        RrClass rrClass;
        uint32_t ttl;
        std::vector<uint8_t> rdata;

        std::vector<uint8_t> bytes() const;

        bool operator==(const Rr &) const = default;

        static Rr a_record(Labels labels, uint32_t ip, uint32_t ttl);

        static Rr aaaa_record(Labels labels, std::array<uint8_t, 16> ip, uint32_t ttl);

        static Rr
        mx_record(Labels labels, uint16_t prefernce, Labels exchange, uint32_t ttl, RrClass rrClass = RrClass::RC_IN);

        static class Rr
        ns_record(Labels label, Labels authority_labels, uint32_t ttl, RrClass rrClass = RrClass::RC_IN);

        static Rr cname_record(Labels labels, Labels cname, uint32_t ttl, RrClass rrClass = RrClass::RC_IN);

        static Rr
        hinfo_record(Labels labels, std::string cpu, std::string os, uint32_t ttl, RrClass rrClass = RrClass::RC_IN);
    };

    std::error_code read_labels(DataBuffer &buffer, std::vector<std::string> &labels);

    std::error_code read_rr(DataBuffer &buffer, Rr &rr);

    enum class Opcode : uint8_t {
        OP_QUERY = 0,
        OP_I_QUERY = 1,
        OP_STATUS = 2,
    };

    struct Header {
        uint16_t qr: 1 = 0;
        Opcode opcode : 4;
        uint32_t aa : 1 = 0;
        uint16_t tc: 1 = 0;
        uint16_t rd: 1;
        uint16_t ra: 1 = 0;
        uint16_t z: 3 = 0;
        ResponseCode rcode: 4 = ResponseCode::RC_OK;
        uint16_t qdcount = 0;
        uint16_t ancount = 0;
        uint16_t nscount = 0;
        uint16_t arcount = 0;

        std::vector<uint8_t> bytes() const;

        bool operator==(const Header &) const = default;
    };

    std::error_code read_header(DataBuffer &stream, Header &header);

    struct Question {
        std::vector<std::string> labels;
        RrType qtype;
        RrClass qclass;

        auto operator<=>(const Question &) const = default;

        std::vector<uint8_t> bytes() const;
    };

    std::error_code read_question(DataBuffer &buffer, Question &question);

    struct Message {
        Header header;
        std::vector<Question> questions{};
        std::vector<Rr> answers{};
        std::vector<Rr> authorities{};
        std::vector<Rr> additional{};

        bool operator==(const Message &) const = default;

        std::vector<uint8_t> bytes() const;
    };

    std::error_code read_message(DataBuffer &buffer, Message &message);

}

