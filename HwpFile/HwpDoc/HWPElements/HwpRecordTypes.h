#ifndef HWPRECORDTYPES_H
#define HWPRECORDTYPES_H

#include "../Common/Common.h"
#include "../Common/NodeNames.h"

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
	SWITCH(ELineStyle1, nValue)
	{
		DEFAULT(ELineStyle1::SOLID);
		CASE(ELineStyle1::DASH);
		CASE(ELineStyle1::DOT);
		CASE(ELineStyle1::DASH_DOT);
		CASE(ELineStyle1::DASH_DOT_DOT);
		CASE(ELineStyle1::LONG_DASH);
		CASE(ELineStyle1::CIRCLE);
		CASE(ELineStyle1::DOUBLE_SLIM);
		CASE(ELineStyle1::SLIM_THICK);
		CASE(ELineStyle1::THICK_SLIM);
		CASE(ELineStyle1::SLIM_THICK_SLIM);
		CASE(ELineStyle1::WAVE);
		CASE(ELineStyle1::DOUBLE_WAVE);
		CASE(ELineStyle1::THICK_3D);
		CASE(ELineStyle1::THICK_3D_REVERS_LI);
		CASE(ELineStyle1::SOLID_3D);
		CASE(ELineStyle1::SOLID_3D_REVERS_LI);
	}
}

inline ELineStyle1 GetLineStyle1(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::Solid, eType) == sValue)
		return ELineStyle1::SOLID;
	if (GetValueName(EValue::Dash, eType) == sValue)
		return ELineStyle1::DASH;
	if (GetValueName(EValue::DashDot, eType) == sValue)
		return ELineStyle1::DASH_DOT;
	if (GetValueName(EValue::DashDotDot, eType) == sValue)
		return ELineStyle1::DASH_DOT_DOT;
	if (GetValueName(EValue::LongDash, eType) == sValue)
		return ELineStyle1::LONG_DASH;
	if (GetValueName(EValue::Circle, eType) == sValue)
		return ELineStyle1::CIRCLE;
	if (GetValueName(EValue::DoubleSlim, eType) == sValue)
		return ELineStyle1::DOUBLE_SLIM;
	if (GetValueName(EValue::SlimThick, eType) == sValue)
		return ELineStyle1::SLIM_THICK;
	if (GetValueName(EValue::ThickSlim, eType) == sValue)
		return ELineStyle1::THICK_SLIM;
	if (GetValueName(EValue::SlimThickSlim, eType) == sValue)
		return ELineStyle1::SLIM_THICK_SLIM;

	// Остальные значения встречаются только в hwpx
	if (EHanType::HWPX != eType)
		return ELineStyle1::SOLID;
	
	if ("WAVE" == sValue)
		return ELineStyle1::WAVE;
	if ("DOUBLE_WAVE" == sValue)
		return ELineStyle1::DOUBLE_WAVE;
	if ("THICK_3D" == sValue)
		return ELineStyle1::THICK_3D;
	if ("THICK_3D_REVERS_LI" == sValue)
		return ELineStyle1::THICK_3D_REVERS_LI;
	if ("SOLID_3D" == sValue)
		return ELineStyle1::SOLID_3D;
	if ("SOLID_3D_REVERS_LI" == sValue)
		return ELineStyle1::SOLID_3D_REVERS_LI;

	return ELineStyle1::SOLID;
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
	SWITCH(ELineStyle2, nValue)
	{
		DEFAULT(ELineStyle2::NONE);
		CASE(ELineStyle2::SOLID);
		CASE(ELineStyle2::DASH);
		CASE(ELineStyle2::DOT);
		CASE(ELineStyle2::DASH_DOT);
		CASE(ELineStyle2::DASH_DOT_DOT);
		CASE(ELineStyle2::LONG_DASH);
		CASE(ELineStyle2::CIRCLE);
		CASE(ELineStyle2::DOUBLE_SLIM);
		CASE(ELineStyle2::SLIM_THICK);
		CASE(ELineStyle2::THICK_SLIM);
		CASE(ELineStyle2::SLIM_THICK_SLIM);
	}
}

inline ELineStyle2 GetLineStyle2(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::None, eType) == sValue)
		return ELineStyle2::NONE;
	if (GetValueName(EValue::Solid, eType) == sValue)
		return ELineStyle2::SOLID;
	if (GetValueName(EValue::Dash, eType) == sValue)
		return ELineStyle2::DASH;
	if (GetValueName(EValue::Dot, eType) == sValue)
		return ELineStyle2::DOT;
	if (GetValueName(EValue::DashDot, eType) == sValue)
		return ELineStyle2::DASH_DOT;
	if (GetValueName(EValue::DashDotDot, eType) == sValue)
		return ELineStyle2::DASH_DOT_DOT;
	if (GetValueName(EValue::LongDash, eType) == sValue)
		return ELineStyle2::LONG_DASH;
	if (GetValueName(EValue::Circle, eType) == sValue)
		return ELineStyle2::CIRCLE;
	if (GetValueName(EValue::DoubleSlim, eType) == sValue)
		return ELineStyle2::DOUBLE_SLIM;
	if (GetValueName(EValue::SlimThick, eType) == sValue)
		return ELineStyle2::SLIM_THICK;
	if (GetValueName(EValue::ThickSlim, eType) == sValue)
		return ELineStyle2::THICK_SLIM;
	if (GetValueName(EValue::SlimThickSlim, eType) == sValue)
		return ELineStyle2::SLIM_THICK_SLIM;

	return ELineStyle2::NONE;
}

enum class ELineStyle3
{
	NONE,
	SOLID,
	DOT,
	THICK,
	DASH,
	DASH_DOT,
	DASH_DOT_DOT
};

inline ELineStyle3 GetLineStyle3(int nValue)
{
	SWITCH(ELineStyle3, nValue)
	{
		DEFAULT(ELineStyle3::NONE);
		CASE(ELineStyle3::SOLID);
		CASE(ELineStyle3::DOT);
		CASE(ELineStyle3::THICK);
		CASE(ELineStyle3::DASH);
		CASE(ELineStyle3::DASH_DOT);
		CASE(ELineStyle3::DASH_DOT_DOT);
	}
}

inline ELineStyle3 GetLineStyle3(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::None, eType) == sValue)
		return ELineStyle3::NONE;
	if (GetValueName(EValue::Solid, eType) == sValue)
		return ELineStyle3::SOLID;
	if (GetValueName(EValue::Dot, eType) == sValue)
		return ELineStyle3::DOT;
	if (GetValueName(EValue::Thick, eType) == sValue)
		return ELineStyle3::THICK;
	if (GetValueName(EValue::Dash, eType) == sValue)
		return ELineStyle3::DASH;
	if (GetValueName(EValue::DashDot, eType) == sValue)
		return ELineStyle3::DASH_DOT;
	if (GetValueName(EValue::DashDotDot, eType) == sValue)
		return ELineStyle3::DASH_DOT_DOT;

	return ELineStyle3::NONE;
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
	HANGUL_SYLLABLE,
	CIRCLED_HANGUL_SYLLABLE,
	HANGUL_JAMO,
	CIRCLED_HANGUL_JAMO,
	HANGUL_PHONETIC,
	IDEOGRAPH,
	CIRCLED_IDEOGRAPH
};

inline ENumberShape1 GetNumberShape1(int nValue)
{
	SWITCH(ENumberShape1, nValue)
	{
		DEFAULT(ENumberShape1::DIGIT);
		CASE(ENumberShape1::CIRCLE_DIGIT);
		CASE(ENumberShape1::ROMAN_CAPITAL);
		CASE(ENumberShape1::ROMAN_SMALL);
		CASE(ENumberShape1::LATIN_CAPITAL);
		CASE(ENumberShape1::LATIN_SMALL);
		CASE(ENumberShape1::CIRCLED_LATIN_CAPITAL);
		CASE(ENumberShape1::CIRCLED_LATIN_SMALL);
		CASE(ENumberShape1::HANGUL_SYLLABLE);
		CASE(ENumberShape1::CIRCLED_HANGUL_SYLLABLE);
		CASE(ENumberShape1::HANGUL_JAMO);
		CASE(ENumberShape1::CIRCLED_HANGUL_JAMO);
		CASE(ENumberShape1::HANGUL_PHONETIC);
		CASE(ENumberShape1::IDEOGRAPH);
		CASE(ENumberShape1::CIRCLED_IDEOGRAPH);
	}
}

inline ENumberShape1 GetNumberShape1(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::Digit, eType) == sValue)
		return ENumberShape1::DIGIT;
	if (GetValueName(EValue::CircledDigit, eType) == sValue)
		return ENumberShape1::CIRCLE_DIGIT;
	if (GetValueName(EValue::RomanCapital, eType) == sValue)
		return ENumberShape1::ROMAN_CAPITAL;
	if (GetValueName(EValue::RomanSmall, eType) == sValue)
		return ENumberShape1::ROMAN_SMALL;
	if (GetValueName(EValue::LatinCapital, eType) == sValue)
		return ENumberShape1::LATIN_SMALL;
	if (GetValueName(EValue::CircledLatinCapital, eType) == sValue)
		return ENumberShape1::CIRCLED_LATIN_CAPITAL;
	if (GetValueName(EValue::CircledLatinSmall, eType) == sValue)
		return ENumberShape1::CIRCLED_LATIN_SMALL;
	if (GetValueName(EValue::HangulSyllable, eType) == sValue)
		return ENumberShape1::HANGUL_SYLLABLE;
	if (GetValueName(EValue::CircledHangulSyllable, eType) == sValue)
		return ENumberShape1::CIRCLED_HANGUL_SYLLABLE;
	if (GetValueName(EValue::HangulJamo, eType) == sValue)
		return ENumberShape1::HANGUL_JAMO;
	if (GetValueName(EValue::CircledHangulJamo, eType) == sValue)
		return ENumberShape1::CIRCLED_HANGUL_JAMO;
	if (GetValueName(EValue::HangulPhonetic, eType) == sValue)
		return ENumberShape1::HANGUL_PHONETIC;
	if (GetValueName(EValue::Ideograph, eType) == sValue)
		return ENumberShape1::IDEOGRAPH;
	if (GetValueName(EValue::CircledIdeograph, eType) == sValue)
		return ENumberShape1::CIRCLED_IDEOGRAPH;

	return ENumberShape1::DIGIT;
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
	HANGUL_SYLLABLE,
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
	SWITCH(ENumberShape2, nValue)
	{
		DEFAULT(ENumberShape2::DIGIT);
		CASE(ENumberShape2::CIRCLE_DIGIT);
		CASE(ENumberShape2::ROMAN_CAPITAL);
		CASE(ENumberShape2::ROMAN_SMALL);
		CASE(ENumberShape2::LATIN_CAPITAL);
		CASE(ENumberShape2::LATIN_SMALL);
		CASE(ENumberShape2::CIRCLED_LATIN_CAPITAL);
		CASE(ENumberShape2::CIRCLED_LATIN_SMALL);
		CASE(ENumberShape2::HANGUL_SYLLABLE);
		CASE(ENumberShape2::CIRCLED_HANGUL_SYLLABLE);
		CASE(ENumberShape2::HANGUL_JAMO);
		CASE(ENumberShape2::CIRCLED_HANGUL_JAMO);
		CASE(ENumberShape2::HANGUL_PHONETIC);
		CASE(ENumberShape2::IDEOGRAPH);
		CASE(ENumberShape2::CIRCLED_IDEOGRAPH);
		CASE(ENumberShape2::DECAGON_CIRCLE);
		CASE(ENumberShape2::DECAGON_CRICLE_HANGJA);
		CASE(ENumberShape2::SYMBOL);
		CASE(ENumberShape2::USER_HWP_CHAR);
	}
}

inline ENumberShape2 GetNumberShape2(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::Digit, eType) == sValue)
		return ENumberShape2::DIGIT;
	if (GetValueName(EValue::CircledDigit, eType) == sValue)
		return ENumberShape2::CIRCLE_DIGIT;
	if (GetValueName(EValue::RomanCapital, eType) == sValue)
		return ENumberShape2::ROMAN_CAPITAL;
	if (GetValueName(EValue::RomanSmall, eType) == sValue)
		return ENumberShape2::ROMAN_SMALL;
	if (GetValueName(EValue::LatinCapital, eType) == sValue)
		return ENumberShape2::LATIN_SMALL;
	if (GetValueName(EValue::CircledLatinCapital, eType) == sValue)
		return ENumberShape2::CIRCLED_LATIN_CAPITAL;
	if (GetValueName(EValue::CircledLatinSmall, eType) == sValue)
		return ENumberShape2::CIRCLED_LATIN_SMALL;
	if (GetValueName(EValue::HangulSyllable, eType) == sValue)
		return ENumberShape2::HANGUL_SYLLABLE;
	if (GetValueName(EValue::CircledHangulSyllable, eType) == sValue)
		return ENumberShape2::CIRCLED_HANGUL_SYLLABLE;
	if (GetValueName(EValue::HangulJamo, eType) == sValue)
		return ENumberShape2::HANGUL_JAMO;
	if (GetValueName(EValue::CircledHangulJamo, eType) == sValue)
		return ENumberShape2::CIRCLED_HANGUL_JAMO;
	if (GetValueName(EValue::HangulPhonetic, eType) == sValue)
		return ENumberShape2::HANGUL_PHONETIC;
	if (GetValueName(EValue::Ideograph, eType) == sValue)
		return ENumberShape2::IDEOGRAPH;
	if (GetValueName(EValue::CircledIdeograph, eType) == sValue)
		return ENumberShape2::CIRCLED_IDEOGRAPH;
	if (GetValueName(EValue::DecagonCircle, eType) == sValue)
		return ENumberShape2::DECAGON_CIRCLE;
	if (GetValueName(EValue::DecagonCircleHanja, eType) == sValue)
		return ENumberShape2::DECAGON_CRICLE_HANGJA;
	if (GetValueName(EValue::Symbol, eType) == sValue)
		return ENumberShape2::SYMBOL;
	if (GetValueName(EValue::UserChar, eType) == sValue)
		return ENumberShape2::USER_HWP_CHAR;

	return ENumberShape2::DIGIT;
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

inline ELineArrowStyle GetLineArrowStyle(const std::string& sValue, EHanType eType, bool bHeadFill = false)
{
	if (sValue.empty() || GetValueName(EValue::Normal, eType) == sValue)
		return ELineArrowStyle::NORMAL;

	if (GetValueName(EValue::Arrow, eType) == sValue)
		return ELineArrowStyle::ARROW;
	else if (GetValueName(EValue::Spear, eType) == sValue)
		return ELineArrowStyle::SPEAR;
	else if (GetValueName(EValue::ConcaveArrow, eType) == sValue)
		return ELineArrowStyle::CONCAVE_ARROW;
	if (GetValueName(EValue::Diamond, eType) == sValue)
		return ELineArrowStyle::DIAMOND;
	if (GetValueName(EValue::Circle, eType) == sValue)
		return ELineArrowStyle::CIRCLE;
	if (GetValueName(EValue::Box, eType) == sValue)
		return ELineArrowStyle::BOX;
	else if (GetValueName(EValue::EmptyDiamond, eType) == sValue)
	{
		if (EHanType::HWPX == eType)
			return bHeadFill ? ELineArrowStyle::DIAMOND : ELineArrowStyle::EMPTY_DIAMOND;

		return ELineArrowStyle::EMPTY_DIAMOND;
	}
	else if (GetValueName(EValue::EmptyCircle, eType) == sValue)
	{
		if (EHanType::HWPX == eType)
			return bHeadFill ? ELineArrowStyle::CIRCLE : ELineArrowStyle::EMPTY_CIRCLE;

		return ELineArrowStyle::EMPTY_CIRCLE;
	}
	else if (GetValueName(EValue::EmptyBox, eType) == sValue)
	{
		if (EHanType::HWPX == eType)
			return bHeadFill ? ELineArrowStyle::BOX : ELineArrowStyle::EMPTY_BOX;

		return ELineArrowStyle::EMPTY_BOX;
	}

	return ELineArrowStyle::NORMAL;
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
	SWITCH(ELineArrowSize, nValue)
	{
		DEFAULT(ELineArrowSize::SMALL_SMALL);
		CASE(ELineArrowSize::SMALL_MEDIUM);
		CASE(ELineArrowSize::SMALL_LARGE);
		CASE(ELineArrowSize::MEDIUM_SMALL);
		CASE(ELineArrowSize::MEDIUM_MEDIUM);
		CASE(ELineArrowSize::MEDIUM_LARGE);
		CASE(ELineArrowSize::LARGE_SMALL);
		CASE(ELineArrowSize::LARGE_MEDIUM);
		CASE(ELineArrowSize::LARGE_LARGE);
	}
}

inline ELineArrowSize GetLineArrowSize(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() && GetValueName(EValue::SmallSmall, eType) == sValue)
		return ELineArrowSize::SMALL_SMALL;
	if (GetValueName(EValue::SmallMedium, eType) == sValue)
		return ELineArrowSize::SMALL_MEDIUM;
	if (GetValueName(EValue::SmallLarge, eType) == sValue)
		return ELineArrowSize::SMALL_LARGE;
	if (GetValueName(EValue::MediumSmall, eType) == sValue)
		return ELineArrowSize::MEDIUM_SMALL;
	if (GetValueName(EValue::MediumMedium, eType) == sValue)
		return ELineArrowSize::MEDIUM_MEDIUM;
	if (GetValueName(EValue::MediumLarge, eType) == sValue)
		return ELineArrowSize::MEDIUM_LARGE;
	if (GetValueName(EValue::LargeSmall, eType) == sValue)
		return ELineArrowSize::LARGE_SMALL;
	if (GetValueName(EValue::LargeMedium, eType) == sValue)
		return ELineArrowSize::LARGE_MEDIUM;
	if (GetValueName(EValue::LargeLarge, eType) == sValue)
		return ELineArrowSize::LARGE_LARGE;

	return ELineArrowSize::SMALL_SMALL;
}

}

#endif // HWPRECORDTYPES_H
