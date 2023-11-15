//
// Created by Jollu Emil
//
#include "TCPClient.h"

#include <iostream>
#include <signal.h>


TCPClient tcp;

void sig_exit(int s) {
    tcp.Exit();
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./client ip port massage" << std::endl;
        return 0;
    }
    signal(SIGINT, sig_exit);

    tcp.Setup(argv[1], std::atoi(argv[2]));
    while (true) {
        tcp.Send(argv[3]);
        std::string rec = tcp.Receive();
        if (rec != "") std::cout << rec << std::endl;
        sleep(1);
    }
    return 0;
}
