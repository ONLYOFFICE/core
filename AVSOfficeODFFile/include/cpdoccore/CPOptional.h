#ifndef _CP_OPTIONAL_H_
#define _CP_OPTIONAL_H_

#include <boost/optional.hpp>

namespace cpdoccore {

/// внешнее объ€вление дл€ Optional, используем boost::optional

template <class T>
struct optional
{
    typedef T Base;
    typedef ::boost::optional<T> Type;
};

#define _CP_OPT(T) optional< T >::Type

}

#endif
