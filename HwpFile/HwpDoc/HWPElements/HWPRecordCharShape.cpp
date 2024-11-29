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
	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		short shFontID;
		oBuffer.ReadShort(shFontID);
		//fontName[i] 	= ((HwpRecord_FaceName)parent.faceNameList.get(fontID)).faceName;
	}

	#define READ_SHORT (short)((*oBuffer.GetCurPtr()) & 0x00FF); oBuffer.Skip(1)

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
		m_arCharOffset[nIndex] = (BYTE)((*oBuffer.GetCurPtr()) & 0x00FF);
	}
}
}
