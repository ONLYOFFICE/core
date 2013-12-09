#pragma once
#ifndef UTILITY_NULLABLE_INCLUDE_H_
#define UTILITY_NULLABLE_INCLUDE_H_

#include <string>

#include "CallTraits.h"
#include "Exception/not_init_nullable.h"

#include "../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

// TODO: nullable__ change to NSCommon::smart_ptr

template<typename T> class nullable__
{
private:
	typedef typename NSCallTraits<T>::param_type Parameter;

private:

	template<class T, bool big_type> class InternalNullable
	{
	private:
		typedef typename NSCallTraits<T>::param_type Parameter;

	public:
		InternalNullable() {}
		InternalNullable(Parameter value) : _value(new T(value)) {}

		inline void operator =(Parameter value) { _value.reset(new T(value));}
		inline void operator =(const nullable__<T>& value)
		{
			if (value.is_init())
				_value.reset(new T(value));
			else
				_value = NSCommon::smart_ptr<T>();
		}

		inline Parameter get() const {return *_value;}
		inline Parameter get_value_or(Parameter value) const {return is_init() ? get() : value;}


		inline T const* get_ptr() const {return _value.operator->();}	
		inline T*       get_ptr()       {return _value.operator->();}	

		inline T const* operator->() const	{return _value.operator->();}	
		inline T*		operator->()		{return _value.operator->();}		

		inline const T&	operator*() const	{return (_value.operator*());}
		inline T&		operator*()			{return (_value.operator*());}

		inline const bool is_init() const {return _value.is_init();}
		inline void reset() {_value.reset();}

	private:
		NSCommon::smart_ptr<T> _value;
	};

	template<class T> class InternalNullable<T, false>
	{
	private:
		typedef typename NSCallTraits<T>::param_type Parameter;

	public:
		InternalNullable() {}
		InternalNullable(Parameter value) : _value(new T(value)) {}

		inline void operator =(Parameter value) {_value = value;}
		inline void operator =(const nullable__<T>& value) 
		{ 
			if (value.is_init())
				_value.reset(new T(value));
			else
				_value = NSCommon::smart_ptr<T>();
		}		
		inline Parameter get() const {return *_value;}
		inline Parameter get_value_or(Parameter value) const {return is_init() ? get() : value;}

		inline T const* get_ptr() const {return _value.operator->();}	
		inline T*       get_ptr()       {return _value.operator->();}	

		inline T const* operator->() const	{return _value.operator->();}	
		inline T*		operator->()		{return _value.operator->();}		

		inline const T&	operator*() const	{return (_value.operator*());}
		inline T&		operator*()			{return (_value.operator*());}

		inline const bool is_init() const {return _value.is_init();}
		inline void reset() {_value.reset();}

	private:

		NSCommon::smart_ptr<T> _value;
	};

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
	inline const nullable__<T>& nullable_setter(const nullable__<T>& value)
	{
		_value = value;
		return *this;
	}

	inline operator Parameter() const {return _value.get();}

	inline Parameter get() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get();
	}

	inline Parameter get_value_or(Parameter value) const 
	{
		return _value.get_value_or(value);
	}

	inline const T get_value_or_default() const
	{
		return get_value_or(T());
	}

	inline T const* operator->() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	inline T* operator->()       
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	inline T const* get_ptr() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	inline T* get_ptr()
	{
		if (!is_init())
			throw not_init_nullable();
		return _value.get_ptr();
	}

	inline T const& operator*() const 
	{
		if (!is_init())
			throw not_init_nullable();
		return *_value;
	}

	inline T& operator*()
	{
		if (!is_init())
			throw not_init_nullable();
		return *_value;
	}

	inline const bool is_init() const 
	{
		return _value.is_init();
	}

	inline void reset() 
	{
		_value.reset();
	}

	inline void init()
	{
		if (!is_init())
			_value = T();
	}

	inline const std::string ToString() const 
	{
		return ::ToString(get());
	}

	inline const bool operator ==(nullable__<T> const& rhs) const {return _value.get() == rhs._value.get();}
	inline const bool operator !=(nullable__<T> const& rhs) const {return _value.get() != rhs._value.get();}
	inline const bool operator < (nullable__<T> const& rhs) const {return _value.get() <  rhs._value.get();}
	inline const bool operator > (nullable__<T> const& rhs) const {return _value.get() >  rhs._value.get();}
	inline const bool operator <=(nullable__<T> const& rhs) const {return _value.get() <= rhs._value.get();}
	inline const bool operator >=(nullable__<T> const& rhs) const {return _value.get() >= rhs._value.get();}

	inline const bool operator ==(Parameter rhs) const {return _value.get() == rhs;}
	inline const bool operator !=(Parameter rhs) const {return _value.get() != rhs;}
	inline const bool operator < (Parameter rhs) const {return _value.get() <  rhs;}
	inline const bool operator > (Parameter rhs) const {return _value.get() >  rhs;}
	inline const bool operator <=(Parameter rhs) const {return _value.get() <= rhs;}
	inline const bool operator >=(Parameter rhs) const {return _value.get() >= rhs;}

	template<typename T> const bool operator ==(const T rhs) const {return _value.get() == rhs;}
	template<typename T> const bool operator !=(const T rhs) const {return _value.get() != rhs;}
	template<typename T> const bool operator < (const T rhs) const {return _value.get() < rhs;}
	template<typename T> const bool operator > (const T rhs) const {return _value.get() > rhs;}
	template<typename T> const bool operator <=(const T rhs) const {return _value.get() <= rhs;}
	template<typename T> const bool operator >=(const T rhs) const {return _value.get() >= rhs;}

private:

	static const int size_of_big_object = 128;

	InternalNullable<T, sizeof(T) / (size_of_big_object + 1) >= 1> _value;
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

template<typename T, typename U> const nullable__<T>& nullable_setter(nullable__<T>& lhs, const U& rhs)
{
	return lhs.nullable_setter(rhs);
}

template<typename T> const std::string ToString(const nullable__<T>& value)
{
	return value.ToString();
}

#endif // UTILITY_NULLABLE_INCLUDE_H_