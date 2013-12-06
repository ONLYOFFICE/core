#pragma once
#ifndef UTILITY_NULLABLE_INCLUDE_H_
#define UTILITY_NULLABLE_INCLUDE_H_

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/call_traits.hpp>
#include <string>
#include "ToString.h"
#include "Exception/not_init_nullable.h"

template<typename T>
class nullable__
{
private:
	typedef typename boost::call_traits<T>::param_type Parameter;

public:
	nullable__() {}
	nullable__(Parameter value) : _value(value) {}

	template<typename U>
	const nullable__<T>& operator =(const U& value)
	{
		return ::nullable_setter(*this, value);
	}
	template<typename U>
	const nullable__<T>& nullable_setter(const U& value)
	{
		_value = static_cast<T>(value);
		return *this;
	}
	const nullable__<T>& nullable_setter(const nullable__<T>& value)
	{
		_value = value;
		return *this;
	}

	operator Parameter() const {return _value.get();}

	Parameter get() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get();
	}

	Parameter get_value_or(Parameter value) const 
	{
		return _value.get_value_or(value);
	}

	const T get_value_or_default() const
	{
		return get_value_or(T());
	}

	T const* operator->() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	T* operator->()       
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	T const* get_ptr() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	T* get_ptr()
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	T const& operator*() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return *_value;
	}

	T& operator*()
	{
		if (!is_init())
			throw not_init_nullable();
		return *_value;
	}

	const bool is_init() const 
	{
		return _value.is_init();
	}

	void reset() 
	{
		_value.reset();
	}

	void init()
	{
		if (!is_init())
			_value = T();
	}

	const std::string ToString() const 
	{
		return ::ToString(get());
	}

	const bool operator ==(nullable__<T> const& rhs) const {return _value.get() == rhs._value.get();}
	const bool operator !=(nullable__<T> const& rhs) const {return _value.get() != rhs._value.get();}
	const bool operator < (nullable__<T> const& rhs) const {return _value.get() <  rhs._value.get();}
	const bool operator > (nullable__<T> const& rhs) const {return _value.get() >  rhs._value.get();}
	const bool operator <=(nullable__<T> const& rhs) const {return _value.get() <= rhs._value.get();}
	const bool operator >=(nullable__<T> const& rhs) const {return _value.get() >= rhs._value.get();}

	const bool operator ==(Parameter rhs) const {return _value.get() == rhs;}
	const bool operator !=(Parameter rhs) const {return _value.get() != rhs;}
	const bool operator < (Parameter rhs) const {return _value.get() <  rhs;}
	const bool operator > (Parameter rhs) const {return _value.get() >  rhs;}
	const bool operator <=(Parameter rhs) const {return _value.get() <= rhs;}
	const bool operator >=(Parameter rhs) const {return _value.get() >= rhs;}

	template<typename T> const bool operator ==(const T rhs) const {return _value.get() == rhs;}
	template<typename T> const bool operator !=(const T rhs) const {return _value.get() != rhs;}
	template<typename T> const bool operator < (const T rhs) const {return _value.get() < rhs;}
	template<typename T> const bool operator > (const T rhs) const {return _value.get() > rhs;}
	template<typename T> const bool operator <=(const T rhs) const {return _value.get() <= rhs;}
	template<typename T> const bool operator >=(const T rhs) const {return _value.get() >= rhs;}

private:
	template<class T, bool big_type>
	class _nullable
	{
	private:
		typedef typename boost::call_traits<T>::param_type Parameter;

	public:
		_nullable() {}
		_nullable(Parameter value) : _value(new T(value)) {}

		void operator =(Parameter value) { _value.reset(new T(value));}
		void operator =(const nullable__<T>& value)
		{
			if (value.is_init())
				_value.reset(new T(value));
			else
				_value = boost::shared_ptr<T>();
		}

		Parameter get() const {return *_value;}
		Parameter get_value_or(Parameter value) const {return is_init() ? get() : value;}

		const bool is_init() const {return _value != 0;}
		void reset() {_value.reset();}

		T const* get_ptr() const {return _value.get();}
		T*       get_ptr()       {return _value.get();}

		T const&	operator*() const {return *_value;}
		T&				operator*()				{return *_value;}

	private:
		boost::shared_ptr<T> _value;
	};


	template<class T>
	class _nullable<T, false>
	{
	private:
		typedef typename boost::call_traits<T>::param_type Parameter;

	public:
		_nullable() {}
		_nullable(Parameter value) : _value(value) {}

		void operator =(Parameter value) {_value = value;}
		void operator =(const nullable__<T>& value) 
		{ 
			if (value.is_init())
				_value = value; 
			else
				_value = boost::optional<T>();
		}

		Parameter get() const {return _value.get();}
		Parameter get_value_or(Parameter value) const {return _value.get_value_or(value);}

		const bool is_init() const {return _value.is_initialized();}
		void reset() {_value.reset();}

		T const* get_ptr() const {return _value.get_ptr();}
		T*       get_ptr()       {return _value.get_ptr();}

		T const&	operator*() const {return *_value;}
		T&				operator*()				{return *_value;}

	private:
		boost::optional<T> _value;
	};

private:
	static const int size_of_big_object = 128;

	_nullable<T, sizeof(T) / (size_of_big_object + 1) >= 1> _value;
};


template<class T> const bool operator== (const T x, nullable__<T> const& y) {return y == x;} 
template<class T> const bool operator!= (const T x, nullable__<T> const& y) {return y != x;}
template<class T> const bool operator<  (const T x, nullable__<T> const& y) {return y >= x;}
template<class T> const bool operator>  (const T x, nullable__<T> const& y) {return y <= x;}
template<class T> const bool operator<= (const T x, nullable__<T> const& y) {return y >  x;}
template<class T> const bool operator>= (const T x, nullable__<T> const& y) {return y <  x;}

template<typename V, class T> const bool operator ==(const V x, nullable__<T> const& y) {return y == x;}
template<typename V, class T> const bool operator !=(const V x, nullable__<T> const& y) {return y != x;}
template<typename V, class T> const bool operator < (const V x, nullable__<T> const& y) {return y < x;}
template<typename V, class T> const bool operator > (const V x, nullable__<T> const& y) {return y > x;}
template<typename V, class T> const bool operator <=(const V x, nullable__<T> const& y) {return y <= x;}
template<typename V, class T> const bool operator >=(const V x, nullable__<T> const& y) {return y >= x;}


template<typename T, typename U>
const nullable__<T>& nullable_setter(nullable__<T>& lhs, const U& rhs)
{
	return lhs.nullable_setter(rhs);
}


template<typename T> const std::string ToString(const nullable__<T>& value)
{
	return value.ToString();
}

#endif // UTILITY_NULLABLE_INCLUDE_H_