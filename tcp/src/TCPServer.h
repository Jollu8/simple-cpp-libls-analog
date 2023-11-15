//
/// Created by Jollu Emil
//

#pragma once

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <thread>
#include <algorithm>
#include <mutex>

const int MAX_PACKET_SIZE = 40960;
const int MAX_CLIENT = 1000;

struct DescriptSocket {
    int socket = -1;
    std::string ip;
    int id = -1;
    std::string message;
    bool enable_message_runtime = false;
};

class TCPServer {
public:
    int Setup(int port, std::vector<int> opts = std::vector<int>());

    std::vector<DescriptSocket *> GetMessage();

    void Accepted();

    void Send(const std::string& msg, int id);

    static void Detach(int id);

    void Clean(int id);

    static bool IsOnline();

    std::string GetIPAddr(int id);

    static int GetLastClosedSockets();

    void Closed() const;


private: // Fields
    int m_sockfd;
    [[maybe_unused]] int m_n;
    [[maybe_unused]] int m_pid;
    sockaddr_in m_server_address;
    sockaddr_in m_client_address;
    pthread_t m_server_thread[MAX_CLIENT];

private: // Statics
    static std::vector<DescriptSocket *> m_new_sockfd;
    static char msg[MAX_PACKET_SIZE];
    static std::vector<DescriptSocket *> m_Message;

    static bool m_is_online;
    static int m_last_closed;
    static int m_num_client;
    static std::mutex m_mt;

    static void *Task(void *argv);

};



