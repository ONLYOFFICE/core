#pragma once

#include "..\..\Common\DocxFormat\Source\Base\SmartPtr.h"

template <class T, class I> class Aggregat
{
public:
	Aggregat()
	{
	}

	Aggregat(T* _item) : m_item(_item)
	{
	}

	T& operator* ()
	{
		*return m_item;  
	}

	T* get()
	{
		return m_item.operator->();
	}
	
	const T* get() const
	{
		return m_item.operator->();
	}

	T* operator ->()
	{
		return m_item.operator->();
	}

	const T* operator ->() const 
	{
		return m_item.operator->();
	}

	template <class A> const bool is() const
	{ 
		return (typeid(*this->m_item) == typeid(A));
	}

	template <class A> A& as()
	{ 
		return static_cast<A&>(*this->m_item);
	}

	template <class A> const A& as() const
	{ 
		return static_cast<const A&>(*this->m_item);
	}

	template <> const bool is<I>() const
	{ 
		return true;
	}

	template <> I& as<I>()
	{ 
		return static_cast<I&>(*this);
	}

	template <> const I& as<I>() const
	{ 
		return static_cast<const I&>(*this);
	}

	inline bool IsInit() const
	{ 
		return m_item.IsInit();
	}

	inline T* Get()
	{
		return m_item.operator->();
	}

	inline const T* Get() const
	{
		return m_item.operator->();
	}

protected:

	NSCommon::smart_ptr<T> m_item;
};