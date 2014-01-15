
// auto inserted precompiled begin
#include "precompiled_common.h"
// auto inserted precompiled end

#include "NumFormat.h"


namespace Common
{

	NumFormat::NumFormat()
		: m_type(decimal)
	{
	}

	
	const NumFormat::Type NumFormat::type() const
	{
		return m_type;
	}


	const NumFormat NumFormat::UpperLetter()
	{
		return NumFormat(upperLetter);
	}


	const NumFormat NumFormat::LowerLetter()
	{
		return NumFormat(lowerLetter);
	}


	const NumFormat NumFormat::UpperRoman()
	{
		return NumFormat(upperRoman);
	}


	const NumFormat NumFormat::LowerRoman()
	{
		return NumFormat(lowerRoman);
	}


	const NumFormat NumFormat::Decimal()
	{
		return NumFormat(decimal);
	}


	const NumFormat NumFormat::Symbol()
	{
		return NumFormat(symbol);
	}


	const NumFormat NumFormat::Bullet()
	{
		return NumFormat(bullet);
	}


	const NumFormat NumFormat::Chicago()
	{
	    return NumFormat(chicago);
	}


	const bool NumFormat::isUpperLetter() const
	{
		return m_type == upperLetter;
	}


	const bool NumFormat::isLowerLetter() const
	{
		return m_type == lowerLetter;
	}


	const bool NumFormat::isUpperRoman() const
	{
		return m_type == upperRoman;
	}


	const bool NumFormat::isLowerRoman() const
	{
		return m_type == lowerRoman;
	}


	const bool NumFormat::isDecimal() const
	{
		return m_type == decimal;
	}


	const bool NumFormat::isSymbol() const
	{
		return m_type == symbol;
	}


	const bool NumFormat::isBullet() const
	{
		return m_type == bullet;
	}

	
	const bool NumFormat::isChicago() const
	{
		return m_type == chicago;
	}


	void NumFormat::setUpperLetter()
	{
		m_type = upperLetter;
	}


	void NumFormat::setLowerLetter()
	{
		m_type = lowerLetter;
	}


	void NumFormat::setUpperRoman()
	{
		m_type = upperRoman;
	}


	void NumFormat::setLowerRoman()
	{
		m_type = lowerRoman;
	}


	void NumFormat::setDecimal()
	{
		m_type = decimal;
	}


	void NumFormat::setSymbol()
	{
		m_type = symbol;
	}


	void NumFormat::setBullet()
	{
		m_type = bullet;
	}


	void NumFormat::setChicago()
	{
		m_type = chicago;
	}


	NumFormat::NumFormat(const NumFormat::Type type)
		: m_type(type)
	{
	}


	void NumFormat::fromBase(const NumFormat& numFormat)
	{
		m_type = numFormat.m_type;
	}

} // namespace Common