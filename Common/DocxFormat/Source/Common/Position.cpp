#include "Position.h"


namespace Common
{

	Position::Position()
		: m_type(foreground)
	{
	}

	
	const Position::Type Position::type() const
	{
		return m_type;
	}


	const Position Position::Background()
	{
		return Position(background);
	}


	const Position Position::Foreground()
	{
		return Position(foreground);
	}


	const bool Position::isBackground() const
	{
		return m_type == background;
	}


	const bool Position::isForeground() const
	{
		return m_type == foreground;
	}


	void Position::setBackground()
	{
		m_type = background;
	}


	void Position::setForeground()
	{
		m_type = foreground;
	}


	Position::Position(const Position::Type type)
		: m_type(type)
	{
	}


	void Position::fromBase(const Position& position)
	{
		m_type = position.m_type;
	}

} // namespace Common