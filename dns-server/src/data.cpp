#include "dns/data.h"

#include <algorithm>
#include <sstream>
#include <utility>

namespace dnslib {
    const int PointerJumpLimit = 100;

    std::vector<uint8_t> Header::bytes() const {
        uint16_t const meta = qr << 15 | static_cast<uint16_t>(opcode) << 11 |
                              aa << 10 | tc << 9 | rd << 8 | ra << 7 | z << 4 |
                              static_cast<uint16_t>(rcode);
        std::vector<uint8_t> bytes(12);
        auto *data = reinterpret_cast<uint16_t *>(bytes.data());
        data[0] = htons(id);
        data[1] = htons(meta);
        data[2] = htons(qdcount);
        data[3] = htons(ancount);
        data[4] = htons(nscount);
        data[5] = htons(arcount);
        return bytes;
    }

    std::vector<uint8_t>Rr::bytes() const {
        std::vector<uint8_t> bytes;
        size_t required_size = 0;
        for(const auto & label : labels)
            required_size += 1 + label.size();
        required_size += 10;
        required_size += rdata.size();



    }
}
