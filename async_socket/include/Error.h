//
// Created by Jollu Emil
//

#pragma once

#include "Common.h"

namespace coro {
    class SystemError {
    public:
        SystemError(std::int32_t error);

        SystemError(std::string const &msg) : m_error(0), m_msg(msg) {}

        SystemError();

    private:
        void operator=(SystemError const &) {}

        std::int32_t m_error;
        std::string const m_msg;

    };
}
