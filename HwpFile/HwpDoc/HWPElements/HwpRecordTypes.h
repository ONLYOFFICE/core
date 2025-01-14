#ifndef HWPRECORDTYPES_H
#define HWPRECORDTYPES_H

namespace HWP
{
enum class ELineStyle1
{
	SOLID,
	DASH,
	DOT,
	DASH_DOT,
	DASH_DOT_DOT,
	LONG_DASH,
	CIRCLE,
	DOUBLE_SLIM,
	SLIM_THICK,
	THICK_SLIM,
	SLIM_THICK_SLIM,
	WAVE,
	DOUBLE_WAVE,
	THICK_3D,
	THICK_3D_REVERS_LI,
	SOLID_3D,
	SOLID_3D_REVERS_LI
};

inline ELineStyle1 GetLineStyle1(int nValue)
{
	switch(static_cast<ELineStyle1>(nValue))
	{
		case ELineStyle1::SOLID:
		case ELineStyle1::DASH:
		case ELineStyle1::DOT:
		case ELineStyle1::DASH_DOT:
		case ELineStyle1::DASH_DOT_DOT:
		case ELineStyle1::LONG_DASH:
		case ELineStyle1::CIRCLE:
		case ELineStyle1::DOUBLE_SLIM:
		case ELineStyle1::SLIM_THICK:
		case ELineStyle1::THICK_SLIM:
		case ELineStyle1::SLIM_THICK_SLIM:
		case ELineStyle1::WAVE:
		case ELineStyle1::DOUBLE_WAVE:
		case ELineStyle1::THICK_3D:
		case ELineStyle1::THICK_3D_REVERS_LI:
		case ELineStyle1::SOLID_3D:
		case ELineStyle1::SOLID_3D_REVERS_LI:
			return static_cast<ELineStyle1>(nValue);
		default:
			return ELineStyle1::SOLID;
	}
}

enum class ELineStyle2
{
	NONE,
	SOLID,
	DASH,
	DOT,
	DASH_DOT,
	DASH_DOT_DOT,
	LONG_DASH,
	CIRCLE,
	DOUBLE_SLIM,
	SLIM_THICK,
	THICK_SLIM,
	SLIM_THICK_SLIM
};

inline ELineStyle2 GetLineStyle2(int nValue)
{
	switch(static_cast<ELineStyle2>(nValue))
	{
		case ELineStyle2::SOLID:
		case ELineStyle2::DASH:
		case ELineStyle2::DOT:
		case ELineStyle2::DASH_DOT:
		case ELineStyle2::DASH_DOT_DOT:
		case ELineStyle2::LONG_DASH:
		case ELineStyle2::CIRCLE:
		case ELineStyle2::DOUBLE_SLIM:
		case ELineStyle2::SLIM_THICK:
		case ELineStyle2::THICK_SLIM:
		case ELineStyle2::SLIM_THICK_SLIM:
			return static_cast<ELineStyle2>(nValue);
		default:
			return ELineStyle2::NONE;
	}
}

enum class ENumberShape1
{
	DIGIT,
	CIRCLE_DIGIT,
	ROMAN_CAPITAL,
	ROMAN_SMALL,
	LATIN_CAPITAL,
	LATIN_SMALL,
	CIRCLED_LATIN_CAPITAL,
	CIRCLED_LATIN_SMALL,
	HANGLE_SYLLABLE,
	CIRCLED_HANGUL_SYLLABLE,
	HANGUL_JAMO,
	CIRCLED_HANGUL_JAMO,
	HANGUL_PHONETIC,
	IDEOGRAPH,
	CIRCLED_IDEOGRAPH
};

inline ENumberShape1 GetNumberShape1(int nValue)
{
	switch (static_cast<ENumberShape1>(nValue))
	{
		case ENumberShape1::DIGIT:
		case ENumberShape1::CIRCLE_DIGIT:
		case ENumberShape1::ROMAN_CAPITAL:
		case ENumberShape1::ROMAN_SMALL:
		case ENumberShape1::LATIN_CAPITAL:
		case ENumberShape1::LATIN_SMALL:
		case ENumberShape1::CIRCLED_LATIN_CAPITAL:
		case ENumberShape1::CIRCLED_LATIN_SMALL:
		case ENumberShape1::HANGLE_SYLLABLE:
		case ENumberShape1::CIRCLED_HANGUL_SYLLABLE:
		case ENumberShape1::HANGUL_JAMO:
		case ENumberShape1::CIRCLED_HANGUL_JAMO:
		case ENumberShape1::HANGUL_PHONETIC:
		case ENumberShape1::IDEOGRAPH:
		case ENumberShape1::CIRCLED_IDEOGRAPH:
			return static_cast<ENumberShape1>(nValue);
		default:
			return ENumberShape1::DIGIT;
	}
}

enum class ENumberShape2
{
	DIGIT,
	CIRCLE_DIGIT,
	ROMAN_CAPITAL,
	ROMAN_SMALL,
	LATIN_CAPITAL,
	LATIN_SMALL,
	CIRCLED_LATIN_CAPITAL,
	CIRCLED_LATIN_SMALL,
	HANGLE_SYLLABLE,
	CIRCLED_HANGUL_SYLLABLE,
	HANGUL_JAMO,
	CIRCLED_HANGUL_JAMO,
	HANGUL_PHONETIC,
	IDEOGRAPH,
	CIRCLED_IDEOGRAPH,
	DECAGON_CIRCLE,
	DECAGON_CRICLE_HANGJA,
	SYMBOL = 0x80,
	USER_HWP_CHAR = 0x81
};

inline ENumberShape2 GetNumberShape2(int nValue)
{
	switch (static_cast<ENumberShape2>(nValue))
	{
		case ENumberShape2::DIGIT:
		case ENumberShape2::CIRCLE_DIGIT:
		case ENumberShape2::ROMAN_CAPITAL:
		case ENumberShape2::ROMAN_SMALL:
		case ENumberShape2::LATIN_CAPITAL:
		case ENumberShape2::LATIN_SMALL:
		case ENumberShape2::CIRCLED_LATIN_CAPITAL:
		case ENumberShape2::CIRCLED_LATIN_SMALL:
		case ENumberShape2::HANGLE_SYLLABLE:
		case ENumberShape2::CIRCLED_HANGUL_SYLLABLE:
		case ENumberShape2::HANGUL_JAMO:
		case ENumberShape2::CIRCLED_HANGUL_JAMO:
		case ENumberShape2::HANGUL_PHONETIC:
		case ENumberShape2::IDEOGRAPH:
		case ENumberShape2::CIRCLED_IDEOGRAPH:
		case ENumberShape2::DECAGON_CIRCLE:
		case ENumberShape2::DECAGON_CRICLE_HANGJA:
		case ENumberShape2::SYMBOL:
		case ENumberShape2::USER_HWP_CHAR:
			return static_cast<ENumberShape2>(nValue);
		default:
			return ENumberShape2::DIGIT;
	}
}

enum class ELineArrowStyle
{
	NORMAL,
	ARROW,
	SPEAR,
	CONCAVE_ARROW,
	DIAMOND,
	CIRCLE,
	BOX,
	EMPTY_DIAMOND,
	EMPTY_CIRCLE,
	EMPTY_BOX
};

inline ELineArrowStyle GetLineArrowStyle(int nNum, bool bFill)
{
	switch (nNum)
	{
		case 0: return ELineArrowStyle::NORMAL;
		case 1: return ELineArrowStyle::ARROW;
		case 2: return ELineArrowStyle::SPEAR;
		case 3: return ELineArrowStyle::CONCAVE_ARROW;
		case 4:
		case 7: return bFill ? ELineArrowStyle::DIAMOND : ELineArrowStyle::EMPTY_DIAMOND;
		case 5:
		case 8: return bFill ? ELineArrowStyle::CIRCLE : ELineArrowStyle::EMPTY_CIRCLE;
		case 6:
		case 9: return bFill ? ELineArrowStyle::BOX : ELineArrowStyle::EMPTY_BOX;
		default: return ELineArrowStyle::NORMAL;
	}
}

enum class ELineArrowSize
{
	SMALL_SMALL,
	SMALL_MEDIUM,
	SMALL_LARGE,
	MEDIUM_SMALL,
	MEDIUM_MEDIUM,
	MEDIUM_LARGE,
	LARGE_SMALL,
	LARGE_MEDIUM,
	LARGE_LARGE
};

inline ELineArrowSize GetLineArrowSize(int nValue)
{
	switch(static_cast<ELineArrowSize>(nValue))
	{
		case ELineArrowSize::SMALL_SMALL:
		case ELineArrowSize::SMALL_MEDIUM:
		case ELineArrowSize::SMALL_LARGE:
		case ELineArrowSize::MEDIUM_SMALL:
		case ELineArrowSize::MEDIUM_MEDIUM:
		case ELineArrowSize::MEDIUM_LARGE:
		case ELineArrowSize::LARGE_SMALL:
		case ELineArrowSize::LARGE_MEDIUM:
		case ELineArrowSize::LARGE_LARGE:
			return static_cast<ELineArrowSize>(nValue);
		default:
			return ELineArrowSize::SMALL_SMALL;
	}
}

}

#endif // HWPRECORDTYPES_H
