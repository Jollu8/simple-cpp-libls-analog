//
// Created by Jollu Emil
//

#pragma once

#include "Common.h"
#include "Coroutine.h"

namespace coro {
    Ptr<Hub> hub();

    void run();

    class Timeout {
    public:
        Timeout(Time const &time, Ptr<Coroutine> coro) : m_time(time), m_coroutine(coro) {}

        bool operator<(Timeout const &rhs) const { return m_time > rhs.m_time; }

        bool operator==(Timeout const &rhs) const { return m_time == rhs.m_time; }

        [[nodiscard]] Time const &time() const { return m_time; }

        [[nodiscard]] Ptr<Coroutine> coroutine() const { return m_coroutine; }


    private:
        Time m_time;
        Ptr<Coroutine> m_coroutine;
    };

    class Hub {
    public:
        template<class F>
        Ptr<Coroutine> start(F func) {
            Ptr<Coroutine> coro(new Coroutine(func));
            m_runnable.emplace_back(coro);
            return coro;
        }

        void quiesce();

        void poll();

        void run();

        int handle() const { return m_handle; }

        std::mutex const &mutex() const { return m_mutex; }

    private:
        Hub();

        std::vector<WeakPtr<Coroutine>> m_runnable;
        std::priority_queue<Timeout, std::vector<Timeout>> m_timeout;
        int m_blocked;
        int m_waiting;
        int m_handle;
        std::mutex m_mutex;
        Time m_now;

        void timeoutIs(Timeout const &timeout);

        friend Ptr<Hub> coro::hub();

        friend void coro::sleep(const coro::Time &time);

        friend class Coroutine;

        friend class Event;

    };

    template<typename F>
    Ptr<Coroutine> start(F func) {
        return hub()->start(func);
    }

    template<typename F>
    Ptr<Coroutine> timer(F func, Time const &time) {
        return start([=] {
            std::function<void()> f = func;
            while (true) {
                f();
                coro::sleep(time);
            }
        });
    }

}