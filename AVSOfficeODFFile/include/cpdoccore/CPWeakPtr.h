#ifndef _CP_WEAK_PTR_H_
#define _CP_WEAK_PTR_H_

#include <boost/weak_ptr.hpp>

namespace cpdoccore {

template <class T>
struct weak_ptr 
{
    typedef ::boost::weak_ptr<T> Type;
};

}

#endif
