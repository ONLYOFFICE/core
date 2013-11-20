#pragma once
#ifndef IITEMABLE_INCLUDE_H_
#define IITEMABLE_INCLUDE_H_

#include <boost/shared_ptr.hpp>


template<class Item>
class IItemable
{
public:
	IItemable() {};
	IItemable(Item* item) : m_item(item) {};
	~IItemable() {}


public:
	template<class T> const bool is() const
	{
		if (m_item == 0)
			return false;
		return typeid(*m_item) == typeid(T);
	}

	template<class T> T& as()
	{
		return static_cast<T&>(*m_item);
	}

	template<class T> const T& as() const
	{
		return static_cast<const T&>(*m_item);
	}


public:
	template<class T> void create()
	{
		m_item.reset(new T());
	}

	template<class T> void create(const T& value)
	{
		m_item.reset(new T(value));
	}

protected: 
	boost::shared_ptr<Item> m_item;
};

#endif // IITEMABLE_INCLUDE_H_