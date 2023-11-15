//
/// Created by Jollu Emil
//


#include <iostream>
#include <csignal>
#include <ctime>
#include "TCPServer.h"

TCPServer tcp;
pthread_t msg1[MAX_CLIENT];
int num_message = 0;
int time_send = 2700;

void close_app(int s) {
    tcp.Closed();
    exit(0);
}

void *send_client(void *m) {
    auto *desc = (DescriptSocket *) m;

    while (true) {
        if (!tcp.IsOnline() && tcp.GetLastClosedSockets() == desc->id) {
            std::cerr << "Connection choice: stop Send_client( id:" << desc->id
                      << " ip:" << desc->ip
                      << " )" << std::endl;
            break;
        }
        auto t = std::time(nullptr);
        auto *now = std::localtime(&t);
        int hour = now->tm_hour;
        int min = now->tm_min;
        int sec = now->tm_sec;

        std::string date =
                std::to_string(now->tm_year + 1900) + "-" +
                std::to_string(now->tm_mon + 1) + "-" +
                std::to_string(now->tm_mday) + " " +
                std::to_string(hour) + ":" +
                std::to_string(min) + ":" +
                std::to_string(sec) + "\n";
        std::cout << date << std::endl;

        tcp.Send(date, desc->id);
        sleep(time_send);


    }
    pthread_exit(nullptr);


}


void *receive(void *m) {
    pthread_detach(pthread_self());
    std::vector<DescriptSocket *> desc;

    while (true) {
        desc = tcp.GetMessage();
        for (unsigned i = 0; i < desc.size(); ++i) {
            if (desc[i]) {
                if (!desc[i]->enable_message_runtime) {
                    desc[i]->enable_message_runtime = true;
                    if (pthread_create(&msg1[num_message], nullptr, send_client, (void *) desc[i]) == 0) {
                        std::cerr << "ACTIVE MESSAGE THREAD" << std::endl;
                    }

                    ++num_message;

                }

                std::cout
                        << "id:" << desc[i]->id << std::endl
                        << "ip:" << desc[i]->ip << std::endl
                        << "message:" << desc[i]->socket << std::endl
                        << "socket:" << desc[i]->enable_message_runtime << std::endl;
                tcp.Clean(i);
            }
        }
        usleep(1000);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./server port (opt)time-send" << std::endl;
        return 0;
    }
    if (argc == 3) {
        time_send = std::atoi(argv[2]);
        std::signal(SIGINT, close_app);

        pthread_t msg;
        std::vector<int> opts = {SO_REUSEPORT, SO_REUSEADDR};

        if (tcp.Setup(std::atoi(argv[1]), opts) == 0) {
            if (pthread_create(&msg, nullptr, receive, (void *) 0) == 0) {
                while (true) {
                    tcp.Accepted();
                    std::cerr << "Accepted" << std::endl;
                }
            }
        } else std::cerr << "Error socket" << std::endl;
    }
}