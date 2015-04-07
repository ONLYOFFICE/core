#include "Align.h"


namespace Common
{

	Align::Align()
		: m_type(right)
	{
	}

	
	const Align::Type Align::type() const
	{
		return m_type;
	}


	const Align Align::Right()
	{
		return Align(right);
	}


	const Align Align::Left()
	{
		return Align(left);
	}


	const Align Align::Center()
	{
		return Align(center);
	}


	const Align Align::Both()
	{
		return Align(both);
	}


	const bool Align::isRight() const
	{
		return m_type == right;
	}


	const bool Align::isLeft() const
	{
		return m_type == left;
	}


	const bool Align::isCenter() const
	{
		return m_type == center;
	}


	const bool Align::isBoth() const
	{
		return m_type == both;
	}


	void Align::setRight()
	{
		m_type = right;
	}


	void Align::setLeft()
	{
		m_type = left;
	}


	void Align::setCenter()
	{
		m_type = center;
	}


	void Align::setBoth()
	{
		m_type = both;
	}


	Align::Align(const Align::Type type)
		: m_type(type)
	{
	}


	void Align::fromBase(const Align& align)
	{
		m_type = align.m_type;
	}

} // namespace Common