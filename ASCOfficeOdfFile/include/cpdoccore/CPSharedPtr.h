#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cpdoccore {

template <class T>
	struct shared_ptr 
	{
		typedef ::boost::shared_ptr<T> Type;
	};

#define _CP_PTR(T) shared_ptr< T >::Type

}

