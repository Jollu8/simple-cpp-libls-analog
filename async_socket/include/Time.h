//
// Created by Jollu Emil
//

#pragma once

#include "Common.h"

namespace coro {


    class Time {
    public: // Constructor
        Time() : Time(0) {}

    public: // Statics
        static Time sec(double sec) { return Time((std::int64_t) (sec * 1000'000)); }

        static Time milliSec(std::int64_t ms) { return Time(ms * 1000); }

        static Time microSec(std::int64_t us) { return Time(us); }

        static Time now();

    public: // Operators
        bool operator<(Time const &rhs) const { return m_micro_sec < rhs.m_micro_sec; }

        bool operator>(Time const &rhs) const { return m_micro_sec > rhs.m_micro_sec; }

        bool operator==(Time const &rhs) const { return m_micro_sec == rhs.m_micro_sec; }

        bool operator<=(Time const &rhs) const { return !operator>(rhs); }

        bool operator>=(Time const &rhs) const { return !operator<(rhs); }

        bool operator!=(Time const &rhs) const { return !operator==(rhs); }

        Time operator-(Time const &rhs) const { return Time(m_micro_sec - rhs.m_micro_sec); }

        Time operator+(Time const &rhs) const { return Time(m_micro_sec + rhs.m_micro_sec); }

        Time &operator+=(Time const &rhs) {
            m_micro_sec += rhs.m_micro_sec;
            return *this;
        }

        Time &operator-=(Time const &rhs) {
            m_micro_sec -= rhs.m_micro_sec;
            return *this;
        }

        std::int64_t milliSec() const { return m_micro_sec / 1000; }

        std::int64_t microSec() const { return m_micro_sec; }

        double sec() const { return (double) m_micro_sec / 1000'000.; }


    private:
        explicit Time(std::int64_t micro_sec) : m_micro_sec(micro_sec) {}

        std::int64_t m_micro_sec;
    };

}

