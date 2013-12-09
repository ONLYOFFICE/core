#pragma once
#ifndef UTILITY_PROPERTY_INCLUDE_H_
#define UTILITY_PROPERTY_INCLUDE_H_

#include "Setter.h"
#include "Getter.h"
#include <string>
#include "ToString.h"

#include "CallTraits.h"

template<typename Type, class Setter = setter::simple<Type>, class Getter = getter::simple<Type> >
class property
{
private:
	typedef typename NSCallTraits<Type>::param_type Parameter;

public:
	property() : _value() {}
	property(const Setter& setter, const Getter& getter = Getter()) : _value(setter, getter) {}
	property(Parameter value, const Setter& setter = Setter(), const Getter& getter = Getter()) : _value(value, setter, getter) {}
	template<typename U, class S, class G>
	property(property<U, S, G> const& rhs, const Setter& setter = Setter(), const Getter& getter = Getter()) : _value(rhs, setter, getter) {}

	template<typename U> const property& operator =(const U& value)
	{
		return ::property_setter(*this, value);
	}
	const property& operator =(const property& value)
	{
		return ::property_setter(*this, value);
	}

	template<typename U> const property& property_setter(const U& value)
	{
		_value = static_cast<Type>(value);
		return *this;
	}
	const property& property_setter(const property& value)
	{
		_value = value;
		return *this;
	}
	template<typename U, class S, class G> const property& operator =(const property<U, S, G>& value)
	{
		_value = value;
		return *this;
	}

	operator const Type() const {return get();}

	const Type get() const {return _value.get();}
	Type& get() {return _value.get();}

	const Type operator*() const	{return get();}
	Type& operator*() {return get();}

	Type const* const operator->() const {return _value.get_ptr();}
	Type*             operator->()       {return _value.get_ptr();}

	const std::string ToString() const	{return ::ToString(get());}

	const bool operator	==(property<Type, Setter, Getter> const& rhs) const {return get() == rhs.get();}
	const bool operator	!=(property<Type, Setter, Getter> const& rhs) const {return get() != rhs.get();}
	const bool operator	< (property<Type, Setter, Getter> const& rhs) const {return get() <  rhs.get();}
	const bool operator	> (property<Type, Setter, Getter> const& rhs) const {return get() >  rhs.get();}
	const bool operator	<=(property<Type, Setter, Getter> const& rhs) const {return get() <= rhs.get();}
	const bool operator	>=(property<Type, Setter, Getter> const& rhs) const {return get() >= rhs.get();}

	const bool operator	==(Parameter rhs) const {return get() == rhs;}
	const bool operator	!=(Parameter rhs) const {return get() != rhs;}
	const bool operator	< (Parameter rhs) const {return get() <  rhs;}
	const bool operator	> (Parameter rhs) const {return get() >  rhs;}
	const bool operator	<=(Parameter rhs) const {return get() <= rhs;}
	const bool operator	>=(Parameter rhs) const {return get() >= rhs;}

	template<typename T> const bool operator ==(const T rhs) const {return get() == rhs;}
	template<typename T> const bool operator !=(const T rhs) const {return get() != rhs;}
	template<typename T> const bool operator < (const T rhs) const {return get() < rhs;}
	template<typename T> const bool operator > (const T rhs) const {return get() > rhs;}
	template<typename T> const bool operator <=(const T rhs) const {return get() <= rhs;}
	template<typename T> const bool operator >=(const T rhs) const {return get() >= rhs;}

	template<typename T> const property& operator +=(const T value) {_value += value; return *this;}
	template<typename T> const property& operator -=(const T value) {_value -= value; return *this;}
	template<typename T> const property& operator *=(const T value) {_value *= value; return *this;}
	template<typename T> const property& operator /=(const T value) {_value /= value; return *this;}

	template<typename T> const Type operator +(const T rhs) const {return _value + rhs;}
	template<typename T> const Type operator -(const T rhs) const {return _value - rhs;}
	template<typename T> const Type operator *(const T rhs) const {return _value * rhs;}
	template<typename T> const Type operator /(const T rhs) const {return _value / rhs;}

	template<typename T, class S, class G> const Type operator +(const property<T, S, G>& rhs) const {return _value + rhs.get();}
	template<typename T, class S, class G> const Type operator -(const property<T, S, G>& rhs) const {return _value - rhs.get();}
	template<typename T, class S, class G> const Type operator *(const property<T, S, G>& rhs) const {return _value * rhs.get();}
	template<typename T, class S, class G> const Type operator /(const property<T, S, G>& rhs) const {return _value / rhs.get();}

private:

	template<typename Type, class Setter, class Getter, bool simple>
	class InternalProperty
	{
	private:
		typedef typename NSCallTraits<Type>::param_type Parameter;

	public:
		InternalProperty() {Setter()(_value, Type());}
		InternalProperty(Parameter value, const Setter&, const Getter&) : _value(value)
		{
			Setter()(_value, value);
		}

		template<typename U, class S, class G>
		InternalProperty(property<U, S, G> const& rhs, const Setter&, const Getter&) : _value(rhs.get()) 
		{
			Setter()(_value, rhs.get());
		}

		void operator =(Parameter value) {Setter()(_value, value);}

		const Type get() const {return Getter()(_value);}
		Type& get() {return _value;}

		Type const* const get_ptr() const {return &_value;}
		Type*			  get_ptr()		  {return &_value;}

		template<typename T> void operator +=(const T value) {Setter()(_value, _value + value);}
		template<typename T> void operator -=(const T value) {Setter()(_value, _value - value);}
		template<typename T> void operator *=(const T value) {Setter()(_value, _value * value);}
		template<typename T> void operator /=(const T value) {Setter()(_value, _value / value);}

		template<typename T> const Type operator +(const T value) const {return _value + value;}
		template<typename T> const Type operator -(const T value) const {return _value - value;}
		template<typename T> const Type operator *(const T value) const {return _value * value;}
		template<typename T> const Type operator /(const T value) const {return _value / value;}

	private:
		Type _value;
	};

	template<typename Type, class Setter, class Getter>
	class InternalProperty<Type, Setter, Getter, false>
	{
	private:
		typedef typename NSCallTraits<Type>::param_type Parameter;

	public:
		InternalProperty() {_setter(_value, Type());}
		
		InternalProperty(const Setter& setter, const Getter& getter) : _setter(setter), _getter(getter)
		{
			_setter(_value, Type());
		}
		
		InternalProperty(Parameter value, const Setter& setter, const Getter& getter) : _value(value), _setter(setter), _getter(getter)
		{
			_setter(_value, value);
		}

		template<typename U, class S, class G>
		InternalProperty(property<U, S, G> const& rhs, const Setter& setter, const Getter& getter) : _value(rhs.get()), _setter(setter), _getter(getter)
		{
			_setter(_value, rhs.get());
		}

		inline void operator =(Parameter value) {_setter(_value, value);}
		inline void operator =(const InternalProperty& rhs)
		{
			if(this != &rhs)
			{
				_setter(_value, rhs.get());
			}
			return *this;
		}
		
		template<typename U, class S, class G> void operator =(const property<U, S, G> & rhs)
		{
			_setter(_value, rhs.get());
		}

		const Type get() const {return _getter(_value);}
		Type& get() {return _value;}

		Type const* const get_ptr() const {return &_value;}
		Type*			  get_ptr()		  {return &_value;}

		template<typename T> void operator +=(const T value) {_setter(_value, _value + value);}
		template<typename T> void operator -=(const T value) {_setter(_value, _value - value);}
		template<typename T> void operator *=(const T value) {_setter(_value, _value * value);}
		template<typename T> void operator /=(const T value) {_setter(_value, _value / value);}

		template<typename T> const Type operator +(const T value) const {return _value + value;}
		template<typename T> const Type operator -(const T value) const {return _value - value;}
		template<typename T> const Type operator *(const T value) const {return _value * value;}
		template<typename T> const Type operator /(const T value) const {return _value / value;}

	private:
		Type	_value;
		Setter	_setter;
		Getter	_getter;
	};

private:
	InternalProperty<Type, Setter, Getter, true>	_value;
};

template<class T, class S, class G> const bool operator ==(const T x, property<T, S, G> const& y) {return y == x;} 
template<class T, class S, class G> const bool operator !=(const T x, property<T, S, G> const& y) {return y != x;}
template<class T, class S, class G> const bool operator < (const T x, property<T, S, G> const& y) {return y >= x;}
template<class T, class S, class G> const bool operator > (const T x, property<T, S, G> const& y) {return y <= x;}
template<class T, class S, class G> const bool operator <=(const T x, property<T, S, G> const& y) {return y >  x;}
template<class T, class S, class G> const bool operator >=(const T x, property<T, S, G> const& y) {return y <  x;}

template<typename V, class T, class S, class G> const bool operator ==(const V x, property<T, S, G> const& y) {return y == x;}
template<typename V, class T, class S, class G> const bool operator !=(const V x, property<T, S, G> const& y) {return y != x;}
template<typename V, class T, class S, class G> const bool operator < (const V x, property<T, S, G> const& y) {return y < x;}
template<typename V, class T, class S, class G> const bool operator > (const V x, property<T, S, G> const& y) {return y > x;}
template<typename V, class T, class S, class G> const bool operator <=(const V x, property<T, S, G> const& y) {return y <= x;}
template<typename V, class T, class S, class G> const bool operator >=(const V x, property<T, S, G> const& y) {return y >= x;}


template<typename T, class S, class G, typename U>
const property<T, S, G>& property_setter(property<T, S, G>& lhs, const U& rhs)
{
	return lhs.property_setter(rhs);
}


template<typename T, class S, class G> 
const std::string ToString(const property<T, S, G>& value)
{
	return value.ToString();
}

#endif // UTILITY_PROPERTY_INCLUDE_H_