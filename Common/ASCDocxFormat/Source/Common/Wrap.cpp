
// auto inserted precompiled begin
#include "precompiled_common.h"
// auto inserted precompiled end

#include "Wrap.h"


namespace Common
{

	Wrap::Wrap()
		: m_type(square)
	{
	}

	
	const Wrap::Type Wrap::type() const
	{
		return m_type;
	}


	const Wrap Wrap::Square()
	{
		return Wrap(square);
	}


	const Wrap Wrap::Tight()
	{
		return Wrap(tight);
	}


	const Wrap Wrap::TopAndBottom()
	{
		return Wrap(topAndBottom);
	}


	const Wrap Wrap::None()
	{
		return Wrap(none);
	}


	const bool Wrap::isSquare() const
	{
		return m_type == square;
	}


	const bool Wrap::isTight() const
	{
		return m_type == tight;
	}


	const bool Wrap::isTopAndBottom() const
	{
		return m_type == topAndBottom;
	}


	const bool Wrap::isNone() const
	{
		return m_type == none;
	}


	void Wrap::setSquare()
	{
		m_type = square;
	}


	void Wrap::setTight()
	{
		m_type = tight;
	}


	void Wrap::setTopAndBottom()
	{
		m_type = topAndBottom;
	}


	void Wrap::setNone()
	{
		m_type = none;
	}


	Wrap::Wrap(const Wrap::Type type)
		: m_type(type)
	{
	}


	void Wrap::fromBase(const Wrap& wrap)
	{
		m_type = wrap.m_type;
	}

} // namespace Common