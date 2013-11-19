#include "Index.h"


namespace Common
{

	Index::Index() 
		: m_type(normal)
	{
	}


	const Index::Type Index::type() const
	{
		return m_type;
	}


	const Index Index::Super()	
	{
		return Index(super);
	}


	const Index Index::Normal()
	{
		return Index(normal);
	}


	const Index Index::Sub()
	{
		return Index(sub);
	}


	const bool Index::isSuper() const
	{
		return m_type == super;
	}


	const bool Index::isNormal() const
	{
		return m_type == normal;
	}


	const bool Index::isSub() const
	{
		return m_type == sub;
	}


	void Index::setSuper()
	{
		m_type = super;
	}


	void Index::setNormal()
	{
		m_type = normal;
	}


	void Index::setSub()
	{
		m_type = sub;
	}


	Index::Index(const Index::Type type) 
		: m_type(type)
	{
	}


	void Index::fromBase(const Index& index)
	{
		m_type = index.m_type;
	}

} // namespace Common