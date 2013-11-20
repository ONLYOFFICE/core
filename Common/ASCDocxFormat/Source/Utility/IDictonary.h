#pragma once
#ifndef IDICTONARY_INCLUDE_H_
#define IDICTONARY_INCLUDE_H_

#include <map>


template<typename Key, typename Value>
class IDictonary
{
public:
	const size_t size() const {return m_container.size();}
	void add(const Key& key, const Value& value) {m_container.insert(std::make_pair(key, value));}
	const bool empty() const {return m_container.empty();}

public:
	Value& operator [](const Key& key) {return m_container[key];}
	const Value& operator [](const Key& key) const {return m_container[key];}

public:
	typedef typename std::map<Key, Value>::iterator iterator;
	typedef typename std::map<Key, Value>::const_iterator const_iterator;

public:
	iterator begin()	{return m_container.begin();}
	iterator end()		{return m_container.end();}
	const_iterator begin()	const {return m_container.begin();}
	const_iterator end()		const {return m_container.end();}
	iterator find(const Key& key) {return m_container.find(key);}
	const_iterator find(const Key& key) const {return m_container.find(key);}

protected:
	std::map<Key, Value>	m_container;
};

#endif // IDICTONARY_INCLUDE_H_