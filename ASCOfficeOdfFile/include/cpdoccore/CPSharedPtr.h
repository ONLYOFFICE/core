#ifndef _CP_SHARED_PTR_H_
#define _CP_SHARED_PTR_H_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cpdoccore {

/// ¬нешнее объ€вление дл€ shared_ptr, используем boost::shared_ptr

template <class T>
struct shared_ptr 
{
    typedef ::boost::shared_ptr<T> Type;
};

#define _CP_PTR(T) shared_ptr< T >::Type

}

#endif
