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

EHeadingType GetHeadingType(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(OUTLINE, sValue, EHeadingType);
	ELSE_IF_STRING_IN_ENUM(NUMBER, sValue, EHeadingType);
	ELSE_IF_STRING_IN_ENUM(BULLET, sValue, EHeadingType);
	ELSE_STRING_IN_ENUM(NONE, EHeadingType);
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

EHorizontalAlign GetHorizontalAlign(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(RIGHT, sValue, EHorizontalAlign);
	ELSE_IF_STRING_IN_ENUM(CENTER, sValue, EHorizontalAlign);
	ELSE_IF_STRING_IN_ENUM(DISTRIBUTE, sValue, EHorizontalAlign);
	ELSE_IF_STRING_IN_ENUM(DISTRIBUTE_SPACE, sValue, EHorizontalAlign);
	ELSE_IF_STRING_IN_ENUM(JUSTIFY, sValue, EHorizontalAlign);
	ELSE_STRING_IN_ENUM(LEFT, EHorizontalAlign);
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

EVerticalAlign GetVerticalAlign(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(CENTER, sValue, EVerticalAlign);
	ELSE_IF_STRING_IN_ENUM(BOTTOM, sValue, EVerticalAlign);
	ELSE_IF_STRING_IN_ENUM(BASELINE, sValue, EVerticalAlign);
	ELSE_STRING_IN_ENUM(TOP, EVerticalAlign);
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

CHWPRecordParaShape::CHWPRecordParaShape(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_PARA_SHAPE, 0, 0), m_pParent(&oDocInfo),
      m_eAlign(EHorizontalAlign::JUSTIFY), m_bWidowOrphan(false), m_bKeepWithNext(false),
      m_bPageBreakBefore(false), m_eVertAlign(EVerticalAlign::BASELINE), m_eHeadingType(EHeadingType::NONE),
      m_bConnect(false), m_bIgnoreMargin(false), m_bParaTailShape(false)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("tabPrIDRef" == sAttributeName)
			m_shTabDef = oReader.GetInt();
		else if ("condense" == sAttributeName)
			m_chCondense = (HWP_BYTE)oReader.GetInt();
		else if ("fontLineHeight" == sAttributeName)
			m_bFontLineHeight = oReader.GetBool();
		else if ("snapToGrid" == sAttributeName)
			m_bSnapToGrid = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		RecursiveParaShape(oReader);
}

void CHWPRecordParaShape::RecursiveParaShape(CXMLReader& oReader)
{
	std::string sNodeName;

	WHILE_READ_NEXT_NODE(oReader)
	{
		sNodeName = oReader.GetNameA();

		if ("hh:align" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("horizontal" == sAttributeName)
					m_eAlign = ::HWP::GetHorizontalAlign(oReader.GetText2());
				else if ("vertical" == sAttributeName)
					m_eVertAlign = ::HWP::GetVerticalAlign(oReader.GetText2());
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:heading" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("type" == sAttributeName)
					m_eHeadingType = ::HWP::GetHeadingType(oReader.GetText2());
				else if ("idRef" == sAttributeName)
					m_shHeadingIdRef = oReader.GetInt();
				else if ("level" == sAttributeName)
					m_chHeadingLevel = (HWP_BYTE)oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:breakSetting" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("breakLatinWord" == sAttributeName)
				{
					const std::string sType{oReader.GetText2A()};

					if ("KEEP_WORD" == sType)
						m_chBreakLatinWord = 0;
					else if ("BREAK_WORD" == sType)
						m_chBreakLatinWord = 1;
				}
				else if ("breakNonLatinWord" == sAttributeName)
				{
					const std::string sType{oReader.GetText2A()};

					if ("KEEP_WORD" == sType)
						m_chBreakNonLatinWord = 0;
					else if ("BREAK_WORD" == sType)
						m_chBreakNonLatinWord = 1;
				}
				else if ("widowOrphan" == sAttributeName)
					m_bWidowOrphan = oReader.GetBool();
				else if ("keepWithNext" == sAttributeName)
					m_bKeepWithNext = oReader.GetBool();
				else if ("pageBreakBefore" == sAttributeName)
					m_bPageBreakBefore = oReader.GetBool();
				else if ("lineWrap" == sAttributeName)
				{
					const std::string sType{oReader.GetText2A()};

					if ("BREAK" == sType)
						m_chLineWrap = 0;
					else if ("SQUEEZE" == sType)
						m_chLineWrap = 1;
				}
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:lineSpacing" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("type" == sAttributeName)
				{
					const std::string sType{oReader.GetText2A()};

					if ("PERCENT" == sType)
						m_nLineSpacingType = 0;
					else if ("FIXED" == sType)
						m_nLineSpacingType = 1;
					else if ("BETWEENLINES" == sType)
						m_nLineSpacingType = 2;
					else if ("AT_LEAST" == sType)
						m_nLineSpacingType = 4;
				}
				else if ("value" == sAttributeName)
					m_nLineSpacing = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:border" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("borderFillIDRef" == sAttributeName)
					m_shBorderFill = oReader.GetInt();
				else if ("offsetLeft" == sAttributeName)
					m_shOffsetLeft = oReader.GetInt();
				else if ("offsetRight" == sAttributeName)
					m_shOffsetRight = oReader.GetInt();
				else if ("offsetTop" == sAttributeName)
					m_shOffsetTop = oReader.GetInt();
				else if ("offsetBottom" == sAttributeName)
					m_shOffsetBottom = oReader.GetInt();
				else if ("connect" == sAttributeName)
					m_bConnect = oReader.GetBool();
				else if ("ignoreMargin" == sAttributeName)
					m_bIgnoreMargin = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hh:autoSpacing" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("eAsianEng" == sAttributeName)
					m_bAutoSpaceEAsianEng = oReader.GetBool();
				else if ("eAsianNum" == sAttributeName)
					m_bAutoSpaceEAsianNum = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hc:intent" == sNodeName)
			m_nIndent = oReader.GetAttributeInt("value");
		else if ("hc:left" == sNodeName)
			m_nMarginLeft = oReader.GetAttributeInt("value");
		else if ("hc:right" == sNodeName)
			m_nMarginRight = oReader.GetAttributeInt("value");
		else if ("hc:prev" == sNodeName)
			m_nMarginPrev = oReader.GetAttributeInt("value");
		else if ("hc:next" == sNodeName)
			m_nMarginNext = oReader.GetAttributeInt("value");
		else if ("hh:margin" == sNodeName ||
		         "hp:switch" == sNodeName)
		{
			WHILE_READ_NEXT_NODE(oReader)
				RecursiveParaShape(oReader);
		}
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

int CHWPRecordParaShape::GetLeftIndent() const
{
	return m_nMarginLeft;
}

int CHWPRecordParaShape::GetRightIndent() const
{
	return m_nMarginRight;
}

short CHWPRecordParaShape::GetTabDef() const
{
	return m_shTabDef;
}

bool CHWPRecordParaShape::KeepWithNext() const
{
	return m_bKeepWithNext;
}
}
