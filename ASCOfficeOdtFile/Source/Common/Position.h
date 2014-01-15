#pragma once
#ifndef COMMON_POSITION_INCLUDE_H_
#define COMMON_POSITION_INCLUDE_H_


namespace Common
{
	class Position
	{
	public:
		Position();

		enum Type
		{
			background,
			foreground
		};

	public:
		const Type type() const;

	public:
		static const Position Background();
		static const Position Foreground();

	public:
		const bool isBackground() const;
		const bool isForeground() const;

	public:
		void setBackground();
		void setForeground();

	protected:
		Type m_type;

	protected:
		Position(const Type type);
		void fromBase(const Position& position);
	};
} // namespace Common

#endif // COMMON_POSITION_INCLUDE_H_