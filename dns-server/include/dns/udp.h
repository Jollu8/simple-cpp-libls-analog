#pragma once

#include <boost/asio.hpp>
#include <iostream>

#ifndef NDEBUG

#include <fmt/format.h>

#endif

#include "dns/responder.h"

namespace dnslib {
    template<std::derived_from<Lookup> L>
    class UdpNameServer {
    public:
        UdpNameServer(boost::asio::io_service &io, int port, Responder<L> responder) : m_socket(
                io, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v6(), port)),
            m_responder(std::move(responder)) {
            std::error_code const ignore_err;
            m_socket.set_option(boost::asio::ip::v6_only(false), (boost::system::error_code &) ignore_err);
        }

        boost::asio::awaitable<void> start() {
            try {
                while (true) {
                    co_await handle_recv();
                }
            } catch (const std::exception &e) {
                std::cerr << "Exception caught: " << e.what() << "\n";
            }
        }

        int port() { return m_socket.local_endpoint().port(); }

    private:
        boost::asio::ip::udp::socket m_socket;
        Responder<L> m_responder;
        boost::asio::ip::udp::endpoint m_remote_endpoint;
        std::array<uint8_t, 512> m_data{};

        boost::asio::awaitable<void> handle_recv() {
            auto bytes_recv = co_await m_socket.async_receive_from(boost::asio::buffer(m_data), m_remote_endpoint,
                                                                   boost::asio::use_awaitable);
            std::error_code err;
            DataBuffer buffer(m_data);
            buffer.limit(bytes_recv);
            Header header;
            err = read_header(buffer, header);
            if (err) {
                header.rcode = ResponseCode::RC_FORMAT_ERROR;
                Message response = Message{
                    .header = header
                };
                co_await m_socket.async_send_to(boost::asio::buffer(response.bytes()), m_remote_endpoint,
                                                boost::asio::use_awaitable);
                co_return;
            }

            std::vector<Question> question_rrs;
            for (int i = 0; i < header.qdcount; ++i) {
                Question question;
                err = read_question(buffer, question);
                if (err) {
                    header.rcode = ResponseCode::RC_FORMAT_ERROR;
                    Message response = Message{.header = header};
                    co_await m_socket.async_send_to(boost::asio::buffer(response.bytes()), m_remote_endpoint,
                                                    boost::asio::use_awaitable);
                    co_return;
                }
                question_rrs.emplace_back(std::move(question));
            }
            Message request{.header = header, .questions = std::move(question_rrs)};
#ifndef NDBUG
            std::cout << "Received request\n";
            std::cout << "- Header:\n";
            std::cout << fmt::format("   QR={} OP={} AA={} TC={} RD={}\n",
                                     static_cast<uint8_t>(request.header.qr),
                                     static_cast<uint8_t>(request.header.opcode),
                                     static_cast<uint8_t>(request.header.aa),
                                     static_cast<uint8_t>(request.header.tc),
                                     static_cast<uint8_t>(request.header.rd));
            if (request.questions.empty()) {
                std::cout << "- No question\n";
            } else {
                auto question = request.questions[0];
                std::cout << "- Question:\n";
                std::cout << fmt::format("    {}\n",
                                         labels_to_string(question.labels));
                std::cout << fmt::format("    CLS={} TYPE={}\n",
                                         static_cast<uint8_t>(question.qclass),
                                         static_cast<uint8_t>(question.qtype));
            }
#endif

            Message response = co_await m_responder.respond(std::move(request));
            auto response_bytes = response.bytes();
            if (response_bytes.size() > 512) {
                response.header.tc = 1;
                response_bytes.resize(512);
            }
            co_await m_socket.async_send_to(boost::asio::buffer(response_bytes), m_remote_endpoint,
                                            boost::asio::use_awaitable);
        }
    };
} // namespace dnslib
