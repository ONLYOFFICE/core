#pragma once

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/call_traits.hpp>
#include <Exception/WrongAPIUsage.h>

template<typename T>
class nullable
{
private:
	typedef typename boost::call_traits<T>::param_type Parameter;

public:
	nullable() {}
	nullable(Parameter value) : _value(value) {}

	template<typename U>
	const nullable<T>& operator =(const U& value)
	{
		return ::nullable_setter(*this, value);
	}
	template<typename U>
	const nullable<T>& nullable_setter(const U& value)
	{
		_value = static_cast<T>(value);
		return *this;
	}
	const nullable<T>& nullable_setter(const nullable<T>& value)
	{
		_value = value;
		return *this;
	}

	operator Parameter() const {return _value.get();}

	Parameter get() const {return _value.get();}
	Parameter get_value_or(Parameter value) const {return _value.get_value_or(value);}

	T const* operator->() const {return _value.get_ptr();}
	T*       operator->()       {return _value.get_ptr();}

	T const* get_ptr() const {return _value.get_ptr();}
	T*       get_ptr()       {return _value.get_ptr();}

	T const&	operator*() const {return *_value;}
	T&				operator*()				{return *_value;}

	const bool is_init() const {return _value.is_init();}
	void reset() {_value.reset();};

	const bool operator== (nullable<T> const& rhs) const {return _value.get() == rhs._value.get();}
	const bool operator!= (nullable<T> const& rhs) const {return _value.get() != rhs._value.get();}
	const bool operator<  (nullable<T> const& rhs) const {return _value.get() <  rhs._value.get();}
	const bool operator>  (nullable<T> const& rhs) const {return _value.get() >  rhs._value.get();}
	const bool operator<= (nullable<T> const& rhs) const {return _value.get() <= rhs._value.get();}
	const bool operator>= (nullable<T> const& rhs) const {return _value.get() >= rhs._value.get();}

	const bool operator== (Parameter rhs) const {return _value.get() == rhs;}
	const bool operator!= (Parameter rhs) const {return _value.get() != rhs;}
	const bool operator<  (Parameter rhs) const {return _value.get() <  rhs;}
	const bool operator>  (Parameter rhs) const {return _value.get() >  rhs;}
	const bool operator<= (Parameter rhs) const {return _value.get() <= rhs;}
	const bool operator>= (Parameter rhs) const {return _value.get() >= rhs;}

private:
	template<class T, bool big_type>
	class _nullable
	{
	private:
		typedef typename boost::call_traits<T>::param_type Parameter;

	public:
		_nullable() {}
		_nullable(Parameter val) : _value(new T(val)) {}

		void operator =(Parameter val) { _value.reset(new T(val));}
		void operator =(const nullable<T>& other)
		{
			if (other.is_init())
				_value.reset(new T(other));
			else
				_value = boost::shared_ptr<T>();
		}

		Parameter get() const 
		{
			if(is_init())
				return *_value;
			else
				throw EXCEPT::LE::WrongAPIUsage("One of nullable objects was used uninitialized.", __FUNCTION__);
		}
		Parameter get_value_or(Parameter val) const {return is_init() ? get() : val;}

		const bool is_init() const {return _value != 0;}
		void reset() {_value.reset();}

		T const* get_ptr() const {return _value.get();}
		T*       get_ptr()       {return _value.get();}

		T const& operator*() const {return get();}
		T& operator*() {return get();}

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

		void operator =(Parameter val) {_value = val;}
		void operator =(const nullable<T>& other) 
		{ 
			if (other.is_init())
				_value = other; 
			else
				_value = boost::optional<T>();
		}

		Parameter get() const 
		{
			if(is_init())
				return _value.get();
			else
				throw EXCEPT::LE::WrongAPIUsage("One of nullable objects was used uninitialized.", __FUNCTION__);
		}
		Parameter get_value_or(Parameter val) const {return _value.get_value_or(val);}

		const bool is_init() const {return _value.is_initialized();}
		void reset() {_value.reset();}

		T const* get_ptr() const {return _value.get_ptr();}
		T*       get_ptr()       {return _value.get_ptr();}

		T const& operator*() const {return get();}
		T& operator*() {return get();}

	private:
		boost::optional<T> _value;
	};

private:
	static const int size_of_big_object = 128;

	_nullable<T, sizeof(T) / (size_of_big_object + 1) >= 1> _value;
};


template<class T> const bool operator== (typename boost::call_traits<T>::param_type x, nullable<T> const& y) {return y == x;} 
template<class T> const bool operator!= (typename boost::call_traits<T>::param_type x, nullable<T> const& y) {return y != x;}
template<class T> const bool operator<  (typename boost::call_traits<T>::param_type x, nullable<T> const& y) {return y >= x;}
template<class T> const bool operator>  (typename boost::call_traits<T>::param_type x, nullable<T> const& y) {return y <= x;}
template<class T> const bool operator<= (typename boost::call_traits<T>::param_type x, nullable<T> const& y) {return y >  x;}
template<class T> const bool operator>= (typename boost::call_traits<T>::param_type x, nullable<T> const& y) {return y <  x;}


template<typename T, typename U>
const nullable<T>& nullable_setter(nullable<T>& lhs, const U& rhs)
{
	return lhs.nullable_setter(rhs);
}



