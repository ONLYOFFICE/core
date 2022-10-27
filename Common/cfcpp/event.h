#pragma once

#include <functional>
#include <vector>


template <class T>
class Event
{
public:
    Event& operator+=(T& act)
    {
        events.push_back(act);
        return *this;
    }

    const Event& operator()()const
    {
        for (auto& event : events)
            event();
        return *this;
    }

    void clear()
    {
        events.clear();
    }

    inline std::size_t size() const {return events.size();}

protected:
    std::vector<T> events;
};
