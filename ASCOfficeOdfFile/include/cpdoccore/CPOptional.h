#pragma once

#include <boost/optional/optional_io.hpp>
#include <boost/optional.hpp>

namespace cpdoccore
{
    /// внешнее объявление для Optional, используем boost::optional

    template <class T>
    struct optional
    {
        typedef T Base;
        typedef boost::optional<T> Type;
    };


    #define _CP_OPT(V) optional<V>::Type

}
