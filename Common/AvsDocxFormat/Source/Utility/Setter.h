#pragma once
#ifndef UTILITY_SETTER_INCLUDE_H_
#define UTILITY_SETTER_INCLUDE_H_

#include <stdexcept>
#include <boost/call_traits.hpp>
#include <algorithm>
#include <set>


namespace setter
{

	template<typename Type>
	class base_setter
	{
	protected:
		typedef typename boost::call_traits<Type>::param_type Parameter;
	public:
		void operator()(Type& _value, Parameter value)
		{
			must_release();
		}
	};


	class none
	{
	public:
		template<typename T, typename E>
		void operator()(T, E)
		{
		}
	};


	template<typename Type>
	class simple : private base_setter<Type>
	{
	public:
		void operator()(Type& _value, Parameter value)
	    {
			_value = value;
		}
	};


	class read_only
	{
	public:
		template<typename T, typename E>
		void operator()(T, E)
		{
			it_is_read_only();
		}
	};


	template<typename Type>
	class only_positive : private base_setter<Type>
	{
	public:
		void operator()(Type& _value, Parameter value)
		{
			_value = value < static_cast<Type>(0) ? static_cast<Type>(0) : value;
		}
	};


	template<typename Type, int min_value = 0, int max_value = min_value>
	class between : private base_setter<Type>
	{
	public:
		void operator()(Type& _value, Parameter value)
		{
			if (value < min_value)
				_value = min_value;
			else if (max_value < value)
				_value = max_value;
			else
				_value = value;
		}
	};


	template<typename Type, int min_value = 0, int max_value = min_value>
	class between_throw : private base_setter<Type>
	{
	public:
		void operator()(Type& _value, Parameter value)
		{
			if (value < min_value)
				throw std::range_error("between error");
			else if (max_value < value)
				throw std::range_error("between error");
			else
				_value = value;
		}
	};


	template<typename Type>
	class interval : private base_setter<Type>
	{
	public:
		interval(Parameter min, Parameter max)
			:	_min(min),
				_max(max)
		{
			if (_max < _min)
				std::swap(_min, _max);
		}

		void operator()(Type& _value, Parameter value)
		{
			if (value < _min)
				_value = _min;	
			else if (_max < value)
				_value = _max;
			else
				_value = value;
		}

	private:
		Type _min;
		Type _max;
	};


	template<typename Type>
	class interval_throw : private base_setter<Type>
	{
	public:
		interval_throw(Parameter min, Parameter max)
			:	_min(min),
				_max(max)
		{
			if (_max < _min)
				std::swap(_min, _max);
		}

		void operator()(Type& _value, Parameter value)
		{
			if (value < _min)
				throw std::range_error("interval error");
			else if (_max < value)
				throw std::range_error("interval error");
			else
				_value = value;
		}

	private:
		Type _min;
		Type _max;
	};


	template<typename Type>
	class from : private base_setter<Type>
	{
	protected:
		void add(Parameter value)
		{
			_list.insert(value);
		}

	private:
		virtual const Type no_find() const = 0;

	public:
		void operator()(Type& _value, Parameter value)
		{
			if (_list.find(value) != _list.end())
				_value = value;
			else
				_value = no_find();
		}

	private:
		std::set<Type>	_list;
	};

} // namespace setter

#endif // UTILITY_SETTER_INCLUDE_H_