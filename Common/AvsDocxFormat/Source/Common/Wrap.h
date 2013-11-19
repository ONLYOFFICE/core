#pragma once
#ifndef COMMON_WRAP_INCLUDE_H_
#define COMMON_WRAP_INCLUDE_H_


namespace Common
{
	class Wrap
	{
	public:
		Wrap();

		enum Type
		{
			square,
			tight,
			topAndBottom,
			none
		};

	public:
		const Type type() const;

	public:
		static const Wrap Square();
		static const Wrap Tight();
		static const Wrap TopAndBottom();
		static const Wrap None();

	public:
		const bool isSquare() const;
		const bool isTight() const;	
		const bool isTopAndBottom() const;
		const bool isNone() const;

	public:
		void setSquare();
		void setTight();
		void setTopAndBottom();
		void setNone();

	protected:
		Type m_type;

	protected:
		Wrap(const Type type);
		void fromBase(const Wrap& wrap);
	};
} // namespace Common

#endif // COMMON_WRAP_INCLUDE_H_