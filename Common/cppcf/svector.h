#pragma once

#include <memory>
#include <vector>

template <class T>
class SVector : public std::vector<std::shared_ptr<T>>
{
public:
    SVector(size_t res = 0) : std::vector<std::shared_ptr<T>>(res)
    {

    }
    template<class P>
    SVector<P> cast() const
    {
        auto sz = this->size();
        SVector<P> res(sz);
        for (size_t i = 0; i < sz; i++)
            res[i] = std::dynamic_pointer_cast<P> ((*this)[i]);

        return res;
    }
};
