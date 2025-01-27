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
	m_eHeadingType = ::HWP::GetHeadingType((nTypeBits >> 23) & 0x03);
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

CHWPRecordParaShape::CHWPRecordParaShape(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_PARA_SHAPE, 0, 0), m_pParent(&oDocInfo),
      m_eAlign(EHorizontalAlign::JUSTIFY), m_bWidowOrphan(false), m_bKeepWithNext(false),
      m_bPageBreakBefore(false), m_eVertAlign(EVerticalAlign::BASELINE), m_eHeadingType(EHeadingType::NONE),
      m_bConnect(false), m_bIgnoreMargin(false), m_bParaTailShape(false)
{
	m_shTabDef = oNode.GetAttributeInt(L"tabPrIDRef");
	m_chCondense = (HWP_BYTE)oNode.GetAttributeInt(L"condense");

	m_bFontLineHeight = oNode.GetAttributeBool(L"fontLineHeight");
	m_bSnapToGrid = oNode.GetAttributeBool(L"snapToGrid");

	for (CXMLNode& oChild : oNode.GetChilds())
		RecursiveParaShape(oChild);
}

void CHWPRecordParaShape::RecursiveParaShape(CXMLNode& oNode)
{
	if (L"hh:align" == oNode.GetName())
	{
		m_eAlign = ::HWP::GetHorizontalAlign(oNode.GetAttributeInt(L"horizontal"));
		m_eVertAlign = ::HWP::GetVerticalAlign(oNode.GetAttributeInt(L"vertical"));
	}
	else if (L"hh:heading" == oNode.GetName())
	{
		m_eHeadingType = ::HWP::GetHeadingType(oNode.GetAttributeInt(L"type"));
		m_shHeadingIdRef = oNode.GetAttributeInt(L"idRef");
		m_chHeadingLevel = (HWP_BYTE)oNode.GetAttributeInt(L"level");
	}
	else if (L"hh:breakSetting" == oNode.GetName())
	{
		HWP_STRING sType = oNode.GetAttribute(L"breakLatinWord");

		if (L"KEEP_WORD" == sType)
			m_chBreakLatinWord = 0;
		else if (L"BREAK_WORD" == sType)
			m_chBreakLatinWord = 1;

		sType = oNode.GetAttribute(L"breakNonLatinWord");

		if (L"KEEP_WORD" == sType)
			m_chBreakNonLatinWord = 0;
		else if (L"BREAK_WORD" == sType)
			m_chBreakNonLatinWord = 1;

		m_bWidowOrphan = oNode.GetAttributeBool(L"widowOrphan");
		m_bKeepWithNext = oNode.GetAttributeBool(L"keepWithNext");
		m_bPageBreakBefore = oNode.GetAttributeBool(L"pageBreakBefore");

		sType = oNode.GetAttribute(L"lineWrap");

		if (L"BREAK" == sType)
			m_chLineWrap = 0;
		else if (L"SQUEEZE" == sType)
			m_chLineWrap = 1;
	}
	else if (L"hh:lineSpacing" == oNode.GetName())
	{
		HWP_STRING sType = oNode.GetAttribute(L"type");

		if (L"PERCENT" == sType)
			m_nLineSpacingType = 0;
		else if (L"FIXED" == sType)
			m_nLineSpacingType = 1;
		else if (L"BETWEENLINES" == sType)
			m_nLineSpacingType = 2;
		else if (L"AT_LEAST" == sType)
			m_nLineSpacingType = 4;

		m_nLineSpacing = oNode.GetAttributeInt(L"value");
	}
	else if (L"hh:border" == oNode.GetName())
	{
		m_shBorderFill = oNode.GetAttributeInt(L"borderFillIDRef");
		m_shOffsetLeft = oNode.GetAttributeInt(L"offsetLeft");
		m_shOffsetRight = oNode.GetAttributeInt(L"offsetRight");
		m_shOffsetTop = oNode.GetAttributeInt(L"offsetTop");
		m_shOffsetBottom = oNode.GetAttributeInt(L"offsetBottom");

		m_bConnect = oNode.GetAttributeBool(L"connect");
		m_bIgnoreMargin = oNode.GetAttributeBool(L"ignoreMargin");
	}
	else if (L"hh:autoSpacing" == oNode.GetName())
	{
		m_bAutoSpaceEAsianEng = oNode.GetAttributeBool(L"eAsianEng");
		m_bAutoSpaceEAsianNum = oNode.GetAttributeBool(L"eAsianNum");
	}
	else if (L"hc:intent" == oNode.GetName())
		m_nIndent = oNode.GetAttributeInt(L"value");
	else if (L"hc:left" == oNode.GetName())
		m_nMarginLeft = oNode.GetAttributeInt(L"value");
	else if (L"hc:right" == oNode.GetName())
		m_nMarginRight = oNode.GetAttributeInt(L"value");
	else if (L"hc:prev" == oNode.GetName())
		m_nMarginPrev = oNode.GetAttributeInt(L"value");
	else if (L"hc:next" == oNode.GetName())
		m_nMarginNext = oNode.GetAttributeInt(L"value");
	else if (L"hp:switch" == oNode.GetName() ||
	         L"hp:case" == oNode.GetName() ||
	         L"hp:default" == oNode.GetName() ||
	         L"hh:margin" == oNode.GetName())
	{
		for (CXMLNode& oChild : oNode.GetChilds())
			RecursiveParaShape(oChild);
	}
}

EHorizontalAlign CHWPRecordParaShape::GetHorizantalAlign() const
{
	return m_eAlign;
}

EVerticalAlign CHWPRecordParaShape::GetVerticalAlign() const
{
	return m_eVertAlign;
}

EHeadingType CHWPRecordParaShape::GetHeadingType() const
{
	return m_eHeadingType;
}

HWP_BYTE CHWPRecordParaShape::GetHeadingLevel() const
{
	return m_chHeadingLevel;
}

short CHWPRecordParaShape::GetHeadingIdRef() const
{
	return m_shHeadingIdRef;
}

bool CHWPRecordParaShape::GetPageBreakBefore() const
{
	return m_bPageBreakBefore;
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

int CHWPRecordParaShape::GetIndent() const
{
	return m_nIndent;
}

bool CHWPRecordParaShape::KeepWithNext() const
{
	return m_bKeepWithNext;
}
}
