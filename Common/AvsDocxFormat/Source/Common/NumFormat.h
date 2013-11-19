#pragma once
#ifndef COMMON_NUM_FORMAT_INCLUDE_H_
#define COMMON_NUM_FORMAT_INCLUDE_H_


namespace Common
{
	class NumFormat
	{
	public:
		NumFormat();

		//TODO: Add other types
		enum Type
		{
			upperLetter,
			lowerLetter,
			upperRoman,
			lowerRoman,
			decimal,
			symbol,
			bullet,
			chicago
		};

	public:
		const Type type() const;

	public:
		static const NumFormat UpperLetter();
		static const NumFormat LowerLetter();
		static const NumFormat UpperRoman();
		static const NumFormat LowerRoman();
		static const NumFormat Decimal();
		static const NumFormat Symbol();
		static const NumFormat Bullet();
		static const NumFormat Chicago();

	public:
		const bool isUpperLetter() const;
		const bool isLowerLetter() const;	
		const bool isUpperRoman() const;
		const bool isLowerRoman() const;
		const bool isDecimal() const;
		const bool isSymbol() const;
		const bool isBullet() const;
		const bool isChicago() const;

	public:
		void setUpperLetter();
		void setLowerLetter();
		void setUpperRoman();
		void setLowerRoman();
		void setDecimal();
		void setSymbol();
		void setBullet();
		void setChicago();

	protected:
		Type m_type;

	protected:
		NumFormat(const Type type);
		void fromBase(const NumFormat& numFormat);
	};
} // namespace Common

#endif // COMMON_NUM_FORMAT_INCLUDE_H_