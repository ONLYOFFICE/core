#pragma once
#ifndef IENUMERABLE_INCLUDE_H_
#define IENUMERABLE_INCLUDE_H_

#include <vector>


template<typename Type, template<typename Type, typename Allocator> class Container = std::vector, class Allocator = std::allocator<Type> >
class IEnumerable
{
public:
	IEnumerable() {}
	~IEnumerable() {}

public:
	void add(const Type& value) {push_back(value);}
	void push_back(const Type& value) {m_items.push_back(value);}
	void clear() {m_items.clear();}
	const size_t size() const {return m_items.size();}
	const bool empty() const {return m_items.empty();}

public:
	typedef typename Container<Type, Allocator>::iterator iterator;
	typedef typename Container<Type, Allocator>::const_iterator const_iterator;

public:
	iterator begin()	{return m_items.begin();}
	iterator end()		{return m_items.end();}
	const_iterator begin()	const {return m_items.begin();}
	const_iterator end()		const {return m_items.end();}

protected:
	Container<Type, Allocator>	m_items;
};

namespace Odt
{
	template<typename _Iter, typename _Pred>
	_Iter find_if(_Iter iterBegin, _Iter iterEnd, _Pred pred)
	{
		_Iter iter = std::find_if(iterBegin, iterEnd, pred);
			
		if (iter < iterBegin || iter >= iterEnd)
		{
			//assert(false);
			return iterBegin;
		}
		
		return iter;
	}
}

#endif // IENUMERABLE_INCLUDE_H_