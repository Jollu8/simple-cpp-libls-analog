//
// Created by Jollu Emil
//

#pragma once
#ifdef __APPLE__

#include <sys/mman.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#endif

#ifdef __linux__
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

// openssl
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>
#include <cerrno>
#include <cstdio>
#include <memory>
#include <functional>
#include <unordered_set>
#include <cassert>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <mutex>


#ifndef CORO_STACK_SIZE
const std::int32_t CORO_STACK_SIZE = 1048576;
#endif


namespace coro {
    class Coroutine;
    class Event;
    class Hub;
    class Selector;
    class Socket;

    template <typename T>
    using Ptr = std::shared_ptr<T>;
    template<typename T>
    using WeakPtr = std::weak_ptr<T>;
}



