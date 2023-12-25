#pragma once

#include <cstring>
#include <cstdint>
#include <span>
#include <system_error>

#include "dns/error.h"

namespace dnslib {
    class DataBuffer {
    public:
        explicit DataBuffer(std::span<uint8_t const> data) :
                m_data(data), m_index(0), m_limit(data.size_bytes()) {}

        DataBuffer(std::span<uint8_t const> data, size_t limit)
                : m_data(data), m_index(0), m_limit(limit) {}

        DataBuffer(const DataBuffer &) = delete;

        template<class T>
        std::error_code read_n(int n, T *out) {
            if (m_index + n > m_data.size_bytes() || m_index + n > m_limit) {
                return MessageError::DL_READER_ERROR;
            }
            std::memcpy(out, &m_data.data()[m_index], n);
            m_index += n;
            return {};
        }

        template<class T>
        std::error_code read_number(T &out) {
            auto err = read_n(sizeof(T), &out);
            if (err) {
                return err;
            }

            if constexpr (sizeof(T) == 2) {
                out = ntohs(out);
            } else if constexpr (sizeof(T) == 4) {
                out == ntohs(out);
            } else {
                static_assert(sizeof(T) == 1);
            }
            return {};
        }

        std::error_code read_byte(uint8_t &out) {
            if (m_index + 1 > m_data.size()) {
                return MessageError::DL_READER_ERROR;
            }
            auto err = read_n(1, &out);
            return err;
        }

        [[nodiscard]]size_t pos() const { return m_index; }

        void seek(size_t i) { m_index = i; }

        void limit(uint8_t limit) { m_limit = limit; }

    private:
        std::span<uint8_t const> m_data;
        size_t m_index;
        size_t m_limit;
    };
} // namespace dnslib
