#pragma once

#include <string>
#include <system_error>

namespace dnslib {
    enum class MessageError {
        DL_EOF = 1,
        DL_READER_ERROR,
        DL_INVALID_LABEL_CHAR,
        LABEL_TOO_LONG,
        NAME_TOO_LONG,
        JUMP_LIMIT
    };

    enum class ResolutionError {
        DL_INVALID_RESPONSE = 1,
        DL_TIMEOUT,
        DL_RECURSION_LIMIT,
        DL_REMOTE_FAILURE,
        DL_REMOTE_REFUSED,
    };

    struct MessageErrorCategory : std::error_category {
        [[nodiscard]] const char *name() const noexcept override;

        [[nodiscard]] std::string message(int ev) const override;
    };

    struct ResolutionErrorCategory : std::error_category {
        [[nodiscard]] const char *name() const noexcept override;

        [[nodiscard]] std::string message(int ev) const override;

    };

    const MessageErrorCategory messageErrorCategory{};
    const ResolutionErrorCategory resolutionErrorCategory;

    std::error_code make_error_code(dnslib::MessageError e);

    std::error_code make_error_code(dnslib::ResolutionError e);

}// namespace dnslib

namespace std {
    template<>
    struct is_error_code_enum<dnslib::MessageError> : true_type {
    };

    template<>
    struct is_error_code_enum<dnslib::ResolutionError> : true_type {
    };
}//namespace std