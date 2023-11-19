//
// Created by Jollu Emil
//

#pragma once

#include "Common.h"
#include "Time.h"

extern "C" {
void __cdecl coroSwapContext(coro::Coroutine *form, coro::Coroutine *to);
void __cdecl coroStart() throw();
}


namespace coro {
    Ptr<Coroutine> current();

    Ptr<Coroutine> main();

    void yield();

    void sleep(Time const &time);

    void fault(int signo, siginfo_t *info, void *context);


    class Stack {
    public:
        Stack(std::uint32_t size);

        ~Stack();

        std::uint8_t *end() { return m_data + m_size; }

        std::uint8_t *begin() { return m_data; }

    private:
        std::uint8_t *m_data;
        std::uint64_t m_size;

        friend class Coroutine;

    };

    class ExitException {
    };

    class Coroutine : public std::enable_shared_from_this<Coroutine> {
    public:
        enum Status {
            NEW,
            RUNNABLE,
            RUNNING,
            BLOCKED,
            WAITING,
            DELETED,
            EXITED,
        };

        ~Coroutine();

        template<class F>
        Coroutine(F func) : m_stack(CORO_STACK_SIZE) { init(func); }

        Status status() const { return m_status; }

        void join();

    private:
        Coroutine();

        void init(std::function<void()> const &func);

        void exit();

        void start() throw();

        void swap();

        void yield();

        void block();

        void unblock();

        void wait();

        void notify();

        bool isMain() { return !m_stack.begin(); }

        std::uint8_t m_stack_pointer;
        std::function<void()> m_func;
        Status m_status;
        Stack m_stack;
        Ptr<Event> m_event;


        friend Ptr<Coroutine> coro::current();

        friend Ptr<Coroutine> coro::main();

        friend void coro::yield();

        friend void coro::sleep(const coro::Time &time);

        friend void::coroStart() throw();

        friend void coro::fault(int signo, siginfo_t *info, void *context);

        friend class coro::Hub;

        friend class coro::Socket;

        friend class coro::Event;

        friend class coro::Selector;
    };
}