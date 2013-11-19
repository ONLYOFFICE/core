#pragma once
#ifndef COMMON_INDEX_INCLUDE_H_
#define COMMON_INDEX_INCLUDE_H_


namespace Common
{
	class Index
	{
	public:
		Index();

	enum Type
	{
		super,
		normal,
		sub
	};

	public:
		const Type type() const;

	public:
		static const Index Super();
		static const Index Normal();
		static const Index Sub();

	public:
		const bool isSuper() const;
		const bool isNormal() const;
		const bool isSub() const;

	public:
		void setSuper();
		void setNormal();
		void setSub();

	protected:
		Type m_type;

	protected:
		Index(const Type type);
		void fromBase(const Index& index);
	};
} // namespace Common

#endif // COMMON_INDEX_INCLUDE_H_