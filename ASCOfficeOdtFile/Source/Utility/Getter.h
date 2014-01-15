#pragma once
#ifndef UTILITY_GETTER_INCLUDE_H_
#define UTILITY_GETTER_INCLUDE_H_

#include <stdexcept>
#include <boost/call_traits.hpp>


namespace getter
{
	template<typename Type>
	class base_getter
	{
	protected:
		typedef typename boost::call_traits<Type>::param_type Parameter;
	public:
		const Type operator()(Parameter _value) const
		{
			must_release();
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


	template<typename Type>
	class write_only : private base_getter<Type>
	{
	public:
		const Type operator()(Parameter _value) const
		{
			it_is_write_only();
		}
	};

} // namespace getter

#endif // UTILITY_GETTER_INCLUDE_H_