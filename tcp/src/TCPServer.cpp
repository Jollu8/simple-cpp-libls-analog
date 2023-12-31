//
/// Created by Jollu Emil
//

#include "TCPServer.h"


char  TCPServer::msg[MAX_PACKET_SIZE];
int TCPServer::m_num_client;
int TCPServer::m_last_closed;
bool TCPServer::m_is_online;

std::vector<DescriptSocket *> TCPServer::m_Message;
std::vector<DescriptSocket *> TCPServer::m_new_sockfd;


void *TCPServer::Task(void *argv) {
    int n;
    auto *desc = (DescriptSocket *) argv;
    pthread_detach(pthread_self());

    std::cerr << "Open client [ id:" << desc->id << " ip:" << desc->ip << " socket:" << desc->socket << " send:"
              << desc->enable_message_runtime << " ]" << std::endl;
    while (true) {
        n = recv(desc->socket, msg, MAX_PACKET_SIZE, 0);
        if (n != -1) {
            if (n == 0) {
                m_is_online = false;
                std::cerr << "Close client [ id:" << desc->id << " ip:" << desc->ip
                          << " socket:" << desc->socket << " ]" << std::endl;
                m_last_closed = desc->id;
                close(desc->socket);
                int id = desc->id;
                auto new_end = std::remove_if(m_new_sockfd.begin(), m_new_sockfd.end(), [&id](DescriptSocket *device) {
                    return device->id == id;
                });
                m_new_sockfd.erase(new_end, m_new_sockfd.end());
                if (m_num_client > 0) --m_num_client;
                break;
            }
            msg[n] = 0;
            desc->message = std::string(msg);
            std::lock_guard<std::mutex> guard(m_mt);
            m_Message.emplace_back(desc);

        }
        usleep(600);

    }
    delete desc;

    std::cerr << "Exit thread: " << std::this_thread::get_id() << std::endl;
    pthread_exit(nullptr);

}


int TCPServer::Setup(int port, std::vector<int> opts) {
    int opt = 1;
    m_is_online = false;

    m_last_closed = -1;
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&m_server_address, 0, sizeof(m_server_address));

    for (auto i = 0u; i < opts.size(); ++i)
        if ((setsockopt(m_sockfd, SOL_SOCKET, opts.size(), (char *) &opt, sizeof(opt))) < 0) {
            std::cerr << "Error setsockopt" << std::endl;
            return -1;
        }

    m_server_address.sin_family = AF_INET;
    m_server_address.sin_addr.s_addr = htons(INADDR_ANY);
    m_server_address.sin_port = htons(port);

    if (::bind(m_sockfd, (sockaddr *) &m_server_address, sizeof(m_server_address)) < 0) {
        std::cerr << "Error bind" << std::endl;
        return -1;
    }

    if (listen(m_sockfd, 5) < 0) {
        std::cerr << "Error << listen" << std::endl;
        return -1;

    }

    m_num_client = 0;
    m_is_online = true;
    return 0;


}

void TCPServer::Accepted() {
    socklen_t soSize = sizeof(m_client_address);
    DescriptSocket *so = new DescriptSocket;
    so->socket = accept(m_sockfd, (sockaddr *) &m_client_address, &soSize);
    so->id = m_num_client;
    so->ip = inet_ntoa(m_client_address.sin_addr);

    m_new_sockfd.emplace_back(so);
    std::cerr << "Accept client[ id:" << m_new_sockfd[m_num_client]->id
              << " ip:" << m_new_sockfd[m_num_client]->ip
              << " handle:" << m_new_sockfd[m_num_client]->socket << " ]" << std::endl;

    pthread_create(&m_server_thread[m_num_client], nullptr, &Task, (void *) m_new_sockfd[m_num_client]);
    m_is_online = true;
    ++m_num_client;
}


std::vector<DescriptSocket*> TCPServer::GetMessage() {
    std::lock_guard<std::mutex> guard(m_mt);
    return m_Message;
}

void TCPServer::Send(const std::string& msg_, int id) {
    send(m_new_sockfd[id]->socket, msg_.c_str(), msg_.size(), 0);
}

int TCPServer::GetLastClosedSockets() {
    return m_last_closed;
}

void TCPServer::Clean(int id) {
    m_Message[id] = nullptr;
    memset(msg, 0, MAX_PACKET_SIZE);
}

std::string TCPServer::GetIPAddr(int id) {
    return m_new_sockfd[id]->ip;
}

bool TCPServer::IsOnline() {
    return m_is_online;
}

void TCPServer::Detach(int id) {
    close(m_new_sockfd[id]->socket);
    m_new_sockfd[id]->ip = "";
    m_new_sockfd[id]->id = -1;
    m_new_sockfd[id]->message = "";

}


void TCPServer::Closed() const {
    close(m_sockfd);
}


