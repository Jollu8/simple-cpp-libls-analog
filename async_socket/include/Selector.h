//
// Created by Jollu Emil
//

#pragma once

#include "Common.h"
#include "Event.h"


namespace coro {
    using SelectorFunc = std::function<void()>;

    class SelectorRecord {
    private:
        Ptr<Event> m_event;
        SelectorFunc m_func;
        EventWaitToken m_token;

    public:
        SelectorRecord(Ptr<Event> event, SelectorFunc func, EventWaitToken token);

        Ptr<Event> event() const { return m_event; }

        SelectorFunc func() const { return m_func; }

        EventWaitToken token() const { return m_token; }

    };

    class Selector {
        ~Selector();

        Selector &on(Ptr<Event> event, SelectorFunc func);

    private:
        std::vector<SelectorRecord> m_record;

    };
}