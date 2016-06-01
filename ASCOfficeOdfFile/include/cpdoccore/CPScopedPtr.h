#pragma once

#include <boost/scoped_ptr.hpp>

namespace cpdoccore {

template <class T>
	struct scoped_ptr 
	{
		typedef ::boost::scoped_ptr<T> Type;
	};

#define _CP_SCOPED_PTR(T) scoped_ptr< T >::Type

}
