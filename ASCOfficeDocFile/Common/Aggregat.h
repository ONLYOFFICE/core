#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

template <class T, class I> class Aggregat: private boost::noncopyable
{
protected:
	boost::shared_ptr<T> m_item;

public:
	Aggregat()
	{
	}

	Aggregat( T* _item ):
	m_item(_item)
	{
	}

	T& operator * () const
	{
		*return this->m_item;  
	}

	T* get() const
	{
		return this->m_item.get();
	}

	T* operator -> () const
	{
		return this->m_item.operator -> ();
	}

	template <class A> const bool is() const
	{ 
		return ( typeid(*this->m_item) == typeid(A) );
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
};