#pragma once

#include <memory>
#include <list>


template <class T>
class SList : public std::list<std::shared_ptr<T>>
{
public:
    template<class P>
    SList<P> cast() const
    {
        SList<P> res;
        for (const auto& spEl : *this)
            res.push_back(std::dynamic_pointer_cast<P>(spEl));

        return res;
    }

    std::shared_ptr<T> dequeue()
    {
        if (this->empty())
            return {};

        auto spEl = this->front();
        this->pop_front();

        return spEl;
    }

    inline void enqueue(std::shared_ptr<T> el)
    {
        this->push_back(el);
    }
};
