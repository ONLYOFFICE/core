#pragma once
#ifndef UTILITY_NULLABLE_PROPERTY_INCLUDE_H_
#define UTILITY_NULLABLE_PROPERTY_INCLUDE_H_

#include <string>

#include "Setter.h"
#include "Getter.h"
#include "nullable.h"
#include "ToString.h"

#include "CallTraits.h"

template<typename Type, class Setter = setter::simple<Type>, class Getter = getter::simple<Type> >
class nullable_property
{
private:
	typedef typename NSCallTraits<Type>::param_type Parameter;

public:
	nullable_property() {}
	nullable_property(const Setter& setter, const Getter& getter = Getter()) : _value(setter, getter) {}
	nullable_property(Parameter value, const Setter& setter = Setter(), const Getter& getter = Getter())
		: _value (value, setter, getter) {}
	template<typename U, class S, class G> 
	nullable_property(nullable_property<U, S, G> const& rhs, const Setter& setter = Setter(), const Getter& getter = Getter())
		: _value(rhs, setter, getter) {}
	nullable_property(const nullable__<Type>& value) : _value(value) {}

	template<typename U> const nullable_property& operator =(const U& value)
	{
		return ::nullable_property_setter(*this, value);
	}

	const nullable_property& operator =(const nullable_property& value)
	{
		return ::nullable_property_setter(*this, value);
	}

	template<typename U> const nullable_property& nullable_property_setter(const U& value)
	{
		_value = static_cast<Type>(value);
		return *this;
	}

	const nullable_property& nullable_property_setter(const nullable_property& value)
	{
		_value = value;
		return *this;
	}

	template<typename U, class S, class G> const nullable_property& nullable_property_setter(const nullable_property<U, S, G>& value)
	{
		_value = value;
		return *this;
	}

	const nullable_property& nullable_property_setter(const nullable__<Type>& value)
	{
		_value = value;
		return *this;
	}

	operator const Type() const {return get();}
	operator const nullable__<Type>() const {return _value.get_nullable();}
	const Type get() const {return _value.get();}
	const Type operator*() const {return get();}
	const Type get_value_or(Parameter value) const {return _value.is_init() ? get() : value;}
	const Type get_value_or_default() const {return get_value_or(Type());}

	Type const* const operator->() const {return _value.get_ptr();}
	Type*             operator->()       {return _value.get_ptr();}

	Type& operator*() {return *_value;}

	const bool is_init() const {return _value.is_init();}
	void reset() {_value.reset();}
	void init() {_value.init();}

	const std::string ToString() const {return ::ToString(get());}

	const bool operator ==(nullable_property<Type, Setter, Getter> const& rhs) const {return get() == rhs.get()}
	const bool operator !=(nullable_property<Type, Setter, Getter> const& rhs) const {return get() != rhs.get();}
	const bool operator < (nullable_property<Type, Setter, Getter> const& rhs) const {return get() <  rhs.get();}
	const bool operator > (nullable_property<Type, Setter, Getter> const& rhs) const {return get() >  rhs.get();}
	const bool operator <=(nullable_property<Type, Setter, Getter> const& rhs) const {return get() <= rhs.get();}
	const bool operator >=(nullable_property<Type, Setter, Getter> const& rhs) const {return get() >= rhs.get();}

	const bool operator ==(Parameter rhs) const {return get() == rhs;}
	const bool operator !=(Parameter rhs) const {return get() != rhs;}
	const bool operator < (Parameter rhs) const {return get() <  rhs;}
	const bool operator > (Parameter rhs) const {return get() >  rhs;}
	const bool operator <=(Parameter rhs) const {return get() <= rhs;}
	const bool operator >=(Parameter rhs) const {return get() >= rhs;}

	template<typename T> const bool operator ==(const T rhs) const {return get() == rhs;}
	template<typename T> const bool operator !=(const T rhs) const {return get() != rhs;}
	template<typename T> const bool operator < (const T rhs) const {return get() < rhs;}
	template<typename T> const bool operator > (const T rhs) const {return get() > rhs;}
	template<typename T> const bool operator <=(const T rhs) const {return get() <= rhs;}
	template<typename T> const bool operator >=(const T rhs) const {return get() >= rhs;}

private:

	template<typename Type, class Setter, class Getter, bool simple>
	class InternalNullableProperty
	{
	private:
		typedef typename NSCallTraits<Type>::param_type Parameter;

	public:
		InternalNullableProperty() {}
		InternalNullableProperty(Parameter value, const Setter&, const Getter&) : _value(value) { Setter()(*_value, value); }
		InternalNullableProperty(const nullable__<Type>& value)
		{
			if (value.is_init())
			{
				_value = value;
				Setter()(*_value, value.get());
			}
		}

		inline void operator =(Parameter value) 
		{
			_value = value;
			Setter()(*_value, value);
		}

		template<typename U, class S, class G>
		void operator =(const nullable_property<U, S, G>& rhs)
		{
			if (rhs.is_init())
			{
				_value = rhs.get();
				Setter()(*_value, rhs.get());
			}
			else
			{
				_value = nullable__<Type>();
			}
		}

		inline void operator =(const nullable__<Type>& value) 
		{
			if (value.is_init())
			{
				_value = value;
				Setter()(*_value, value.get());
			}
			else
			{
				_value = nullable__<Type>();
			}
		}

		inline const Type get() const {return Getter()(*_value);}
		inline const nullable__<Type> get_nullable() const {return is_init() ? nullable__<Type>(get()) : nullable__<Type>();}
		inline Type const* const get_ptr() const {return _value.get_ptr();}
		inline Type* get_ptr()	{return _value.get_ptr();}

		inline Type& operator*() {return *_value;}

		inline const bool is_init() const {return _value.is_init();}
		inline void reset() {_value.reset();}
		inline void init() {_value.init();}

	private:
		nullable__<Type> _value;
	};

	template<typename Type, class Setter, class Getter>
	class InternalNullableProperty<Type, Setter, Getter, false>
	{
	private:
		typedef typename NSCallTraits<Type>::param_type Parameter;

	public:
		InternalNullableProperty() {}
		InternalNullableProperty(const Setter& setter, const Getter& getter)
			:
		_setter(setter),
			_getter(getter)
		{
		}
		InternalNullableProperty(Parameter value, const Setter& setter, const Getter& getter)
			:
		_value(value),
			_setter(setter),
			_getter(getter)
		{
			_setter(*_value, value);
		}
		template<typename U, class S, class G> 
		InternalNullableProperty(nullable_property<U, S, G> const& rhs, const Setter& setter, const Getter& getter)
			:
		_setter(setter),
			_getter(getter)
		{
			if (rhs.is_init())
			{
				_value = rhs.get();
				_setter(*_value, rhs.get());
			}
		}

		InternalNullableProperty(const nullable__<Type>& value)
		{
			if (value.is_init())
			{
				_value = value;
				_setter(*_value, value.get());
			}
		}

		inline void operator =(Parameter value) 
		{
			_value = value;
			_setter(*_value, value); 
		}

		template<typename U, class S, class G>
		void operator =(const nullable_property<U, S, G>& rhs)
		{
			if (rhs.is_init())
			{
				_value = rhs.get();
				_setter(*_value, rhs.get());
			}
			else
			{
				_value = nullable__<Type>();
			}
		}

		inline void operator =(const nullable__<Type>& value) 
		{
			if (value.is_init())
			{
				_value = value;
				_setter(*_value, value.get());
			}
			else
			{
				_value = nullable__<Type>();
			}
		}

		inline const Type get() const {return _getter(*_value);}
		inline const nullable__<Type> get_nullable() const {return is_init() ? nullable__<Type>(get()) : nullable__<Type>();}
		inline Type const* const get_ptr() const {return _value.get_ptr();}
		inline Type*							get_ptr()				{return _value.get_ptr();}

		inline Type& operator*() {return *_value;}

		inline const bool is_init() const {return _value.is_init();}
		inline void reset() {_value.reset();}
		inline void init() {_value.init();}

	private:
		nullable__<Type>	_value;
		Setter				_setter;
		Getter				_getter;
	};

private:

	InternalNullableProperty<Type, Setter, Getter, true> _value;
};

template<class T, class S, class G> const bool operator ==(const T x, nullable_property<T, S, G> const& y) {return y == x;} 
template<class T, class S, class G> const bool operator !=(const T x, nullable_property<T, S, G> const& y) {return y != x;}
template<class T, class S, class G> const bool operator < (const T x, nullable_property<T, S, G> const& y) {return y >= x;}
template<class T, class S, class G> const bool operator > (const T x, nullable_property<T, S, G> const& y) {return y <= x;}
template<class T, class S, class G> const bool operator <=(const T x, nullable_property<T, S, G> const& y) {return y >  x;}
template<class T, class S, class G> const bool operator >=(const T x, nullable_property<T, S, G> const& y) {return y <  x;}

template<typename V, class T, class S, class G> const bool operator ==(const V x, nullable_property<T, S, G> const& y) {return y == x;}
template<typename V, class T, class S, class G> const bool operator !=(const V x, nullable_property<T, S, G> const& y) {return y != x;}
template<typename V, class T, class S, class G> const bool operator < (const V x, nullable_property<T, S, G> const& y) {return y < x;}
template<typename V, class T, class S, class G> const bool operator > (const V x, nullable_property<T, S, G> const& y) {return y > x;}
template<typename V, class T, class S, class G> const bool operator <=(const V x, nullable_property<T, S, G> const& y) {return y <= x;}
template<typename V, class T, class S, class G> const bool operator >=(const V x, nullable_property<T, S, G> const& y) {return y >= x;}

template<typename T, class S, class G, typename U>
const nullable_property<T, S, G>& nullable_property_setter(nullable_property<T, S, G>& lhs, const U& rhs)
{
	return lhs.nullable_property_setter(rhs);
}

template<typename T, typename U, class S, class G>
const nullable__<T>& nullable_setter(nullable__<T>& lhs, const nullable_property<U, S, G>& rhs)
{
	if (rhs.is_init())
		return lhs.nullable_setter(rhs);
	else
		return lhs;
}

template<typename T, class S, class G> 
const std::string ToString(const nullable_property<T, S, G>& value)
{
	return value.ToString();
}

template<typename T, class S, class G>
const nullable_property<T, S, G> merge(const nullable_property<T, S, G>& prev, const nullable_property<T, S, G>& current)
{
	if (!current.is_init())
		return prev;
	return current;
}

#endif // UTILITY_NULLABLE_PROPERTY_INCLUDE_H_