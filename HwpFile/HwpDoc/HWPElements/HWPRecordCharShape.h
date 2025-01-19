#ifndef HWPRECORDCHARSHAPE_H
#define HWPRECORDCHARSHAPE_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"
#include "HwpRecordTypes.h"
#include "../Common/XMLNode.h"

namespace HWP
{

enum class ELang
{
	HANGUL,
	LATIN,
	HANJA,
	JAPANESE,
	OTHER,
	SYMBOL,
	USER,
	MAX
};

enum class EUnderline
{
	NONE,
	BOTTOM,
	CENTER,
	TOP
};

enum class EOutline
{
	NONE,
	SOLID,
	DOTTED,
	BOLD,
	DASHED,
	DASH_DOT,
	DASH_2DOT
};

enum class EShadow
{
	NONE,
	DISCRETE,
	CONTINUOUS
};

enum class EAccent
{
	NONE,
	DOT,
	RING,
	CARON,
	TILDE,
	ARAEA,
	TWOARAEA
};

inline EAccent GetAccent(int nValue)
{
	switch(static_cast<EAccent>(nValue))
	{
		case EAccent::NONE:
		case EAccent::DOT:
		case EAccent::RING:
		case EAccent::CARON:
		case EAccent::TILDE:
		case EAccent::ARAEA:
		case EAccent::TWOARAEA:
			return static_cast<EAccent>(nValue);
		default:
			return EAccent::NONE;
	}
}

#define MAX_ELEMENTS (int)ELang::MAX

class CHWPRecordCharShape : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	HWP_STRING m_arFontNames[MAX_ELEMENTS];
	short m_arRatios[MAX_ELEMENTS];
	short m_arSpacings[MAX_ELEMENTS];
	short m_arRelSizes[MAX_ELEMENTS];
	short m_arCharOffset[MAX_ELEMENTS];
	int m_nHeight;

	bool m_bItalic;
	bool m_bBold;
	EUnderline m_eUnderline;
	ELineStyle1 m_eUnderLineShape;
	int m_nUnderlineColor;
	EOutline m_eOutline;
	EShadow m_eShadow;
	bool m_bEmboss;
	bool m_bEngrave;
	bool m_bSuperScript;
	bool m_bSubScript;
	HWP_BYTE m_chStrikeOut;
	EAccent m_eSymMark;
	bool m_bUseFontSpace;
	ELineStyle2 m_eStrikeOutShape;
	bool m_bUseKerning;

	HWP_BYTE m_chShadowOffsetX;
	HWP_BYTE m_chShadowOffsetY;
	int m_nTextColor;
	int m_nShadeColor;
	int m_nShadowColor;
	short m_shBorderFillIDRef;
	int m_nStrikeOutColor;

	void ReadContainerData(CXMLNode& oNode, short arValues[], int nDefaultValue = 0);
public:
	CHWPRecordCharShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordCharShape(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion);

	bool Bold() const;
	bool Italic() const;
	bool Underline() const;
	bool StrikeOut() const;

	int GetHeight() const;

	EUnderline GetUnderlineType() const;
	ELineStyle1 GetUnderlineStyle() const;
	int GetUnderlineColor() const;

	ELineStyle2 GetStrikeOutType() const;
	int GetStrikeOutColor() const;

	short GetRelSize(ELang eLang) const;
	HWP_STRING GetFontName(ELang eLang) const;
	short GetSpacing(ELang eLang) const;
	int GetTextColor() const;
};
}

#endif // HWPRECORDCHARSHAPE_H
