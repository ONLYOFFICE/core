#pragma once
#ifndef COMMON_ALIGN_INCLUDE_H_
#define COMMON_ALIGN_INCLUDE_H_


namespace Common
{
	class Align
	{
	public:
		Align();

		enum Type
		{
			right,
			left,
			center,
			both
		};

	public:
		const Type type() const;

	public:
		static const Align Right();
		static const Align Left();
		static const Align Center();
		static const Align Both();

	public:
		const bool isRight() const;
		const bool isLeft() const;	
		const bool isCenter() const;
		const bool isBoth() const;

	public:
		void setRight();
		void setLeft();
		void setCenter();
		void setBoth();

	protected:
		Type m_type;

	protected:
		Align(const Type type);
		void fromBase(const Align& align);
	};
} // namespace Common

#endif // COMMON_ALIGN_INCLUDE_H_