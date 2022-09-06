#pragma once

#include <memory>
#include <stdexcept>
#include <vector>
#include <exception>

template <class T>
using SVectorBase = std::vector<std::shared_ptr<T>>;

template <class T>
using SVectorBasePtr = std::shared_ptr<SVectorBase<T>>;

template <class T>
class SVector : public SVectorBasePtr<T>
{
public:
    SVector(bool bInit = true, size_t res = 0)
    {
        if (bInit || res)
            init(res);
    }

    void init(size_t res = 0)
    {
            SVectorBasePtr<T>::reset(new SVectorBase<T>(res));
    }

    inline bool isInit() const
    {
        return SVectorBasePtr<T>::get() != nullptr;
    }

    void canUse() const
    {
        if (!isInit())
            throw std::bad_cast();
    }

    size_t size() const
    {
        canUse();
        return SVectorBasePtr<T>::get()->size();
    }

    bool empty() const
    {
        canUse();
        return SVectorBasePtr<T>::get()->empty();
    }

    std::shared_ptr<T>& operator[](size_t index)
    {
        canUse();
        return SVectorBasePtr<T>::get()->operator[](index);
    }

    const std::shared_ptr<T>& operator[](size_t index) const
    {
        canUse();
        return SVectorBasePtr<T>::get()->operator[](index);
    }

    void push_back(const std::shared_ptr<T> &el)
    {
        if (!isInit())
            init();

        SVectorBasePtr<T>::get()->push_back(el);
    }

    void clear()
    {
        canUse();
        SVectorBasePtr<T>::get()->clear();
    }
};
