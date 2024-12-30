#include "HWPRecordParaShape.h"

namespace HWP
{
EHeadingType GetHeadingType(int nValue)
{
	SWITCH(EHeadingType, nValue)
	{
		CASE(EHeadingType::OUTLINE);
		CASE(EHeadingType::NUMBER);
		CASE(EHeadingType::BULLET);
		DEFAULT(EHeadingType::NONE);
	}
}

EHorizontalAlign GetHorizontalAlign(int nValue)
{
	SWITCH(EHorizontalAlign, nValue)
	{
		CASE(EHorizontalAlign::LEFT);
		CASE(EHorizontalAlign::RIGHT);
		CASE(EHorizontalAlign::CENTER);
		CASE(EHorizontalAlign::DISTRIBUTE);
		CASE(EHorizontalAlign::DISTRIBUTE_SPACE);
		DEFAULT(EHorizontalAlign::JUSTIFY);
	}
}

EVerticalAlign GetVerticalAlign(int nValue)
{
	SWITCH(EVerticalAlign, nValue)
	{
		CASE(EVerticalAlign::TOP);
		CASE(EVerticalAlign::CENTER);
		CASE(EVerticalAlign::BOTTOM);
		DEFAULT(EVerticalAlign::BASELINE);
	}
}

CHWPRecordParaShape::CHWPRecordParaShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	int nTypeBits;
	oBuffer.ReadInt(nTypeBits);

	m_eAlign = GetHorizontalAlign((nTypeBits >> 2) & 0x07);
	m_chBreakLatinWord = (HWP_BYTE)((nTypeBits >> 5) & 0x03);
	m_chBreakNonLatinWord = (HWP_BYTE)((nTypeBits >> 7) & 0x01);
	m_bSnapToGrid = CHECK_FLAG(nTypeBits, 0x80);
	m_chCondense = (HWP_BYTE)((nTypeBits >> 9) & 0x7F);
	m_bWidowOrphan = CHECK_FLAG(nTypeBits, 0x10000);
	m_bKeepWithNext = CHECK_FLAG(nTypeBits, 0x20000);
	m_bPageBreakBefore = CHECK_FLAG(nTypeBits, 0x40000);
	m_eVertAlign = ::HWP::GetVerticalAlign((nTypeBits >> 20) & 0x03);
	m_bFontLineHeight = CHECK_FLAG(nTypeBits, 0x100000);
	m_eHeadingType = GetHeadingType((nTypeBits >> 23) & 0x03);
	m_chHeadingLevel = (HWP_BYTE)((nTypeBits >> 25) & 0x07);
	m_bConnect = CHECK_FLAG(nTypeBits, 0x800000);
	m_bIgnoreMargin = CHECK_FLAG(nTypeBits, 0x1000000);
	m_bParaTailShape = CHECK_FLAG(nTypeBits, 0x2000000);

	oBuffer.ReadInt(m_nMarginLeft);
	oBuffer.ReadInt(m_nMarginRight);
	oBuffer.ReadInt(m_nIndent);
	oBuffer.ReadInt(m_nMarginPrev);
	oBuffer.ReadInt(m_nMarginNext);

	if (nVersion < 5025)
	{
		m_nLineSpacingType = (HWP_BYTE)(nTypeBits & 0x03);
		oBuffer.ReadInt(m_nLineSpacing);
	}
	else
		oBuffer.Skip(4);

	oBuffer.ReadShort(m_shTabDef);
	oBuffer.ReadShort(m_shHeadingIdRef);
	oBuffer.ReadShort(m_shBorderFill);
	oBuffer.ReadShort(m_shOffsetLeft);
	oBuffer.ReadShort(m_shOffsetRight);
	oBuffer.ReadShort(m_shOffsetTop);
	oBuffer.ReadShort(m_shOffsetBottom);

	if (nVersion >= 5017)
	{
		int nAttrBits;
		oBuffer.ReadInt(nAttrBits);

		m_chLineWrap = (HWP_BYTE)(nAttrBits & 0x03);
		m_bAutoSpaceEAsianEng = CHECK_FLAG(nAttrBits, 0x10);
		m_bAutoSpaceEAsianNum = CHECK_FLAG(nAttrBits, 0x20);
	}
	else
		oBuffer.Skip(4);

	if (nVersion >= 5025)
	{
		int nAttrBits;
		oBuffer.ReadInt(nAttrBits);

		m_nLineSpacingType = (HWP_BYTE)(nAttrBits & 0x0F);
		oBuffer.ReadInt(m_nLineSpacing);
	}
	else
		oBuffer.Skip(8);
}

EHorizontalAlign CHWPRecordParaShape::GetHorizantalAlign() const
{
	return m_eAlign;
}

EVerticalAlign CHWPRecordParaShape::GetVerticalAlign() const
{
	return m_eVertAlign;
}

int CHWPRecordParaShape::GetLineSpacingType() const
{
	return m_nLineSpacingType;
}

int CHWPRecordParaShape::GetLineSpacing() const
{
	return m_nLineSpacing;
}

int CHWPRecordParaShape::GetMarginPrev() const
{
	return m_nMarginPrev;
}

int CHWPRecordParaShape::GetMarginNext() const
{
	return m_nMarginNext;
}

bool CHWPRecordParaShape::KeepWithNext() const
{
	return m_bKeepWithNext;
}
}
