#include "HWPRecordCharShape.h"

namespace HWP
{

ELang GetLang(int nValue)
{
	switch (static_cast<ELang>(nValue))
	{
		case ELang::HANGUL:
		case ELang::LATIN:
		case ELang::HANJA:
		case ELang::JAPANESE:
		case ELang::OTHER:
		case ELang::SYMBOL:
		case ELang::USER:
		case ELang::MAX:
			return static_cast<ELang>(nValue);
		default:
			return ELang::HANGUL;
	}
}

EUnderline GetUnderline(int nValue)
{
	switch(static_cast<EUnderline>(nValue))
	{
		case EUnderline::NONE:
		case EUnderline::BOTTOM:
		case EUnderline::CENTER:
		case EUnderline::TOP:
			return static_cast<EUnderline>(nValue);
		default:
			return EUnderline::NONE;
	}
}

EOutline GetOutline(int nValue)
{
	switch(static_cast<EOutline>(nValue))
	{
		case EOutline::NONE:
		case EOutline::SOLID:
		case EOutline::DOTTED:
		case EOutline::BOLD:
		case EOutline::DASHED:
		case EOutline::DASH_DOT:
		case EOutline::DASH_2DOT:
			return static_cast<EOutline>(nValue);
		default:
			return EOutline::NONE;
	}
}

EShadow GetShadow(int nValue)
{
	switch(static_cast<EShadow>(nValue))
	{
		case EShadow::NONE:
		case EShadow::DISCRETE:
		case EShadow::CONTINUOUS:
			return static_cast<EShadow>(nValue);
		default:
			return EShadow::NONE;
	}
}

CHWPRecordCharShape::CHWPRecordCharShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		short shFontID;
		oBuffer.ReadShort(shFontID);
		//fontName[i] 	= ((HwpRecord_FaceName)parent.faceNameList.get(fontID)).faceName;
	}

	#define READ_SHORT (short)(oBuffer[0] & 0x00FF); oBuffer.Skip(1)

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arRatios[nIndex] = READ_SHORT;
	}

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arSpacings[nIndex] = READ_SHORT;
	}

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arRelSizes[nIndex] = READ_SHORT;
	}

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arCharOffset[nIndex] = (HWP_BYTE)((*oBuffer.GetCurPtr()) & 0x00FF);
	}

	oBuffer.ReadInt(m_nHeight);

	int nAttrBits;
	oBuffer.ReadInt(nAttrBits);

	// Attributes
	m_bItalic = CHECK_FLAG(nAttrBits, 0x01);
	m_bBold = CHECK_FLAG(nAttrBits, 0x02);
	m_eUnderline = GetUnderline((nAttrBits >> 2) & 0x03);
	m_eUnderLineShape = GetLineStyle1((nAttrBits >> 4) & 0x0F);
	m_eOutline = GetOutline((nAttrBits >> 8) & 0x07);
	m_eShadow = GetShadow((nAttrBits >> 11) & 0x03);
	m_bEmboss = CHECK_FLAG(nAttrBits, 0x2000);
	m_bEngrave = CHECK_FLAG(nAttrBits, 0x4000);
	m_bSuperScript = CHECK_FLAG(nAttrBits, 0x8000);
	m_bSubScript = CHECK_FLAG(nAttrBits, 0xF000);
	m_chStrikeOut = (HWP_BYTE)((nAttrBits >> 18) & 0x07);
	m_eSymMark = GetAccent((nAttrBits >> 21) & 0x0F);
	m_bUseFontSpace = CHECK_FLAG(nAttrBits, 0x2000000);
	m_eStrikeOutShape = GetLineStyle2((nAttrBits >> 26) & 0x0F);
	m_bUseKerning =  CHECK_FLAG(nAttrBits, 0x40000000);

	oBuffer.ReadByte(m_chShadowOffsetX);
	oBuffer.ReadByte(m_chShadowOffsetY);
	oBuffer.ReadColor(m_nTextColor);
	oBuffer.ReadColor(m_nUnderlineColor);
	oBuffer.ReadColor(m_nShadeColor);
	oBuffer.ReadColor(m_nShadeColor);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadShort(m_shBorderFillIDRef);

	if (nVersion > 5030 && nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadColor(m_nStrikeOutColor);

	oBuffer.RemoveLastSavedPos();
}
}
