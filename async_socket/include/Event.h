//
// Created by Jollu Emil
//


#pragma once

#include "Common.h"

namespace coro {
    class EventRecord {
    public:
        EventRecord(Ptr<Coroutine> coro) ;
        EventRecord();
        Ptr<Coroutine> coroutine () const {return m_coroutine;}

    private:
        Ptr<Coroutine> m_coroutine;
    };

    using EventWaitToken = std::size_t;

    class Event {
    private:

        std::size_t waiters() const {return m_waiter.size();}
        EventWaitToken waitToken(Ptr<Coroutine> waiter);
        bool waitTokenValid(Ptr<Coroutine> waiter, EventWaitToken token);
        void waitTokenDel(EventWaitToken token);


        std::vector<EventRecord> m_waiter;
        friend class Selector;


    };
}




