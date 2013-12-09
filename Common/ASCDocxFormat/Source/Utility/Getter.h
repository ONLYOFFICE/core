#pragma once
#ifndef UTILITY_GETTER_INCLUDE_H_
#define UTILITY_GETTER_INCLUDE_H_

#include <stdexcept>

#include "CallTraits.h"

namespace getter
{
	template<typename Type>
	class base_getter
	{
	protected:
		typedef typename NSCallTraits<Type>::param_type Parameter;
	public:
		const Type operator()(Parameter _value) const
		{
			//must_release();
		}
	};

	template<typename Type>
	class simple : private base_getter<Type>
	{
	public:
		const Type operator ()(Parameter _value) const
		{
			return _value;
		}
	};
}

#endif // UTILITY_GETTER_INCLUDE_H_