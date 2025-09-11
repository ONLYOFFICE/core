#include "HWPRecordCharShape.h"
#include "../HWPElements/HWPRecordFaceName.h"

#include "../Common/NodeNames.h"

namespace HWP
{
EAccent GetAccent(int nValue)
{
	SWITCH(EAccent, nValue)
	{
		DEFAULT(EAccent::NONE);
		CASE(EAccent::DOT);
		CASE(EAccent::RING);
		CASE(EAccent::CARON);
		CASE(EAccent::TILDE);
		CASE(EAccent::ARAEA);
		CASE(EAccent::TWOARAEA);
	}
}

EAccent GetAccent(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(DOT, sValue, EAccent);
	ELSE_IF_STRING_IN_ENUM(RING, sValue, EAccent);
	ELSE_IF_STRING_IN_ENUM(CARON, sValue, EAccent);
	ELSE_IF_STRING_IN_ENUM(TILDE, sValue, EAccent);
	ELSE_IF_STRING_IN_ENUM(ARAEA, sValue, EAccent);
	ELSE_IF_STRING_IN_ENUM(TWOARAEA, sValue, EAccent);
	ELSE_STRING_IN_ENUM(NONE, EAccent);
}

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
	SWITCH(EUnderline, nValue)
	{
		DEFAULT(EUnderline::NONE);
		CASE(EUnderline::BOTTOM);
		CASE(EUnderline::CENTER);
		CASE(EUnderline::TOP);
	}
}

EUnderline GetUnderline(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(BOTTOM, sValue, EUnderline);
	ELSE_IF_STRING_IN_ENUM(CENTER, sValue, EUnderline);
	ELSE_IF_STRING_IN_ENUM(TOP, sValue, EUnderline);
	ELSE_STRING_IN_ENUM(NONE, EUnderline);
}

EOutline GetOutline(int nValue)
{
	SWITCH(EOutline, nValue)
	{
		DEFAULT(EOutline::NONE);
		CASE(EOutline::SOLID);
		CASE(EOutline::DOTTED);
		CASE(EOutline::BOLD);
		CASE(EOutline::DASHED);
		CASE(EOutline::DASH_DOT);
		CASE(EOutline::DASH_2DOT);
	}
}

EOutline GetOutline(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(SOLID, sValue, EOutline);
	ELSE_IF_STRING_IN_ENUM(DOTTED, sValue, EOutline);
	ELSE_IF_STRING_IN_ENUM(BOLD, sValue, EOutline);
	ELSE_IF_STRING_IN_ENUM(DASHED, sValue, EOutline);
	ELSE_IF_STRING_IN_ENUM(DASH_DOT, sValue, EOutline);
	ELSE_IF_STRING_IN_ENUM(DASH_2DOT, sValue, EOutline);
	ELSE_STRING_IN_ENUM(NONE, EOutline);
}

EShadow GetShadow(int nValue)
{
	SWITCH(EShadow, nValue)
	{
		DEFAULT(EShadow::NONE);
		CASE(EShadow::DISCRETE);
		CASE(EShadow::CONTINUOUS);
	}
}

EShadow GetShadow(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(DISCRETE, sValue, EShadow);
	ELSE_IF_STRING_IN_ENUM(CONTINUOUS, sValue, EShadow);
	ELSE_STRING_IN_ENUM(NONE, EShadow);
}

void CHWPRecordCharShape::ReadContainerData(CXMLReader& oReader, short arValues[], int nDefaultValue)
{
	for (unsigned int unIndex = 0; unIndex < (int)ELang::MAX; ++unIndex)
		arValues[unIndex] = nDefaultValue;

	START_READ_ATTRIBUTES(oReader)
	{
		TO_LOWER(sAttributeName);

		if ("hangul" == sAttributeName)
			arValues[(int)ELang::HANGUL] = oReader.GetInt();
		else if ("latin" == sAttributeName)
			arValues[(int)ELang::LATIN] = oReader.GetInt();
		else if ("hanja" == sAttributeName)
			arValues[(int)ELang::HANJA] = oReader.GetInt();
		else if ("japanese" == sAttributeName)
			arValues[(int)ELang::JAPANESE] = oReader.GetInt();
		else if ("other" == sAttributeName)
			arValues[(int)ELang::OTHER] = oReader.GetInt();
		else if ("symbol" == sAttributeName)
			arValues[(int)ELang::SYMBOL] = oReader.GetInt();
		else if ("user" == sAttributeName)
			arValues[(int)ELang::USER] = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}

CHWPRecordCharShape::CHWPRecordCharShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		short shFontID;
		oBuffer.ReadShort(shFontID);

		const CHWPRecordFaceName* pFaceName = dynamic_cast<const CHWPRecordFaceName*>(m_pParent->GetFaceName(shFontID));

		if (nullptr != pFaceName)
			m_arFontNames[nIndex] = pFaceName->GetFaceName();
	}

	#define READ_ELEMENT(_type) (_type)(oBuffer[0] & 0x00FF); oBuffer.Skip(1)

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arRatios[nIndex] = READ_ELEMENT(short);
	}

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arSpacings[nIndex] = READ_ELEMENT(HWP_BYTE);
	}

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arRelSizes[nIndex] = READ_ELEMENT(short);
	}

	for (int nIndex = 0; nIndex < MAX_ELEMENTS; ++nIndex)
	{
		m_arCharOffset[nIndex] = READ_ELEMENT(HWP_BYTE);
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
	oBuffer.ReadColor(m_nShadowColor);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadShort(m_shBorderFillIDRef);

	if (nVersion > 5030 && nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadColor(m_nStrikeOutColor);

	oBuffer.RemoveLastSavedPos();
}

CHWPRecordCharShape::CHWPRecordCharShape(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_HWP_CHAR_SHAPE, 0, 0), m_pParent(&oDocInfo),
      m_nHeight(1000), m_bItalic(false), m_bBold(false), m_eUnderline(EUnderline::NONE),
      m_eUnderLineShape(ELineStyle1::SOLID), m_eOutline(EOutline::NONE), m_eShadow(EShadow::NONE), m_bEmboss(false), m_bEngrave(false),
      m_bSuperScript(false), m_bSubScript(false), m_eStrikeOutShape(ELineStyle2::NONE), m_nShadeColor(0xFFFFFFFF)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Height, eType) == sAttributeName)
			m_nHeight = oReader.GetInt();
		else if (GetAttributeName(EAttribute::TextColor, eType)== sAttributeName)
			m_nTextColor = oReader.GetColor();
		else if (GetAttributeName(EAttribute::ShadeColor, eType) == sAttributeName)
			m_nShadeColor = oReader.GetColor(0xFFFFFFFF);
		else if (GetAttributeName(EAttribute::UseFontSpace, eType) == sAttributeName)
			m_bUseFontSpace = oReader.GetBool();
		else if (GetAttributeName(EAttribute::Height, eType) == sAttributeName)
			m_bUseKerning = oReader.GetBool();
		else if (GetAttributeName(EAttribute::SymMask, eType) == sAttributeName)
			m_eSymMark = GetAccent(oReader.GetText());
		else if (GetAttributeName(EAttribute::BorderFillId, eType) == sAttributeName)
			m_shBorderFillIDRef = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::FontId, eType) == sNodeName)
		{
			if (nullptr == m_pParent)
				continue;

			const CHWPRecordFaceName* pFaceName = nullptr;

			#define UPDATE_FACENAME(elang_type)\
			{\
				pFaceName = dynamic_cast<const CHWPRecordFaceName*>(m_pParent->GetFaceName(oReader.GetInt()));\
				if (nullptr != pFaceName)\
					m_arFontNames[(int)elang_type] = pFaceName->GetFaceName();\
			}

			START_READ_ATTRIBUTES(oReader)
			{
				TO_LOWER(sAttributeName);

				if ("hangul" == sAttributeName)
					UPDATE_FACENAME(ELang::HANGUL)
				else if ("latin" == sAttributeName)
					UPDATE_FACENAME(ELang::LATIN)
				else if ("hanja" == sAttributeName)
					UPDATE_FACENAME(ELang::HANJA)
				else if ("japanese" == sAttributeName)
					UPDATE_FACENAME(ELang::JAPANESE)
				else if ("other" == sAttributeName)
					UPDATE_FACENAME(ELang::OTHER)
				else if ("symbol" == sAttributeName)
					UPDATE_FACENAME(ELang::SYMBOL)
				else if ("user" == sAttributeName)
					UPDATE_FACENAME(ELang::USER)
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::Ratio, eType) == sNodeName)
			ReadContainerData(oReader, m_arRatios, 100);
		else if (GetNodeName(ENode::CharSpacing, eType) == sNodeName)
			ReadContainerData(oReader, m_arSpacings);
		else if (GetNodeName(ENode::RelSize, eType) == sNodeName)
			ReadContainerData(oReader, m_arRelSizes, 100);
		else if (GetNodeName(ENode::CharOffset, eType) == sNodeName)
			ReadContainerData(oReader, m_arCharOffset);
		else if (GetNodeName(ENode::Underline, eType) == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
					m_eUnderline = GetUnderline(oReader.GetText());
				else if (GetAttributeName(EAttribute::Shape, eType) == sAttributeName)
					m_eUnderLineShape = GetLineStyle1(oReader.GetText());
				else if (GetAttributeName(EAttribute::Color, eType) == sAttributeName)
					m_nUnderlineColor = oReader.GetColor();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::Outline, eType) == sNodeName)
			m_eOutline = GetOutline(oReader.GetAttribute(GetAttributeName(EAttribute::Type, eType)));
		else if (GetNodeName(ENode::Shadow, eType) == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};

					if (GetValueName(EValue::Drop, eType))
						m_eShadow = EShadow::DISCRETE;
					else if (GetValueName(EValue::Continuous, eType))
						m_eShadow = EShadow::CONTINUOUS;
					else
						m_eShadow = EShadow::NONE;
				}
				else if (GetAttributeName(EAttribute::Color, eType) == sAttributeName)
					m_nShadowColor = oReader.GetColor();
				else if (GetAttributeName(EAttribute::OffsetX, eType) == sAttributeName)
					m_chShadowOffsetX = (HWP_BYTE)oReader.GetInt();
				else if (GetAttributeName(EAttribute::OffsetY, eType) == sAttributeName)
					m_chShadowOffsetY = (HWP_BYTE)oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::Italic, eType) == sNodeName)
			m_bItalic = true;
		else if (GetNodeName(ENode::Bold, eType) == sNodeName)
			m_bBold = true;
		else if (GetNodeName(ENode::Emboss, eType) == sNodeName)
			m_bEmboss = true;
		else if (GetNodeName(ENode::Engrave, eType) == sNodeName)
			m_bEngrave = true;
		else if (GetNodeName(ENode::SuperScript, eType) == sNodeName)
			m_bSuperScript = true;
		else if (GetNodeName(ENode::SubScript, eType) == sNodeName)
			m_bSubScript = true;
		else if (EHanType::HWPX == eType && "hh:strikeout" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Shape, eType) == sAttributeName)
					m_eStrikeOutShape = GetLineStyle2(oReader.GetText());
				else if (GetAttributeName(EAttribute::Color, eType) == sAttributeName)
					m_nStrikeOutColor = oReader.GetColor();
			}
			END_READ_ATTRIBUTES(oReader)
		}
	}
	END_WHILE
}

bool CHWPRecordCharShape::Bold() const
{
	return m_bBold;
}

bool CHWPRecordCharShape::Italic() const
{
	return m_bItalic;
}

bool CHWPRecordCharShape::Underline() const
{
	return EUnderline::NONE != m_eUnderline;
}

bool CHWPRecordCharShape::StrikeOut() const
{
	return 0x01 == m_chStrikeOut;
}

bool CHWPRecordCharShape::SuperScript() const
{
	return m_bSuperScript;
}

bool CHWPRecordCharShape::SubScript() const
{
	return m_bSubScript;
}

int CHWPRecordCharShape::GetHeight() const
{
	return m_nHeight;
}

EUnderline CHWPRecordCharShape::GetUnderlineType() const
{
	return m_eUnderline;
}

ELineStyle1 CHWPRecordCharShape::GetUnderlineStyle() const
{
	return m_eUnderLineShape;
}

int CHWPRecordCharShape::GetUnderlineColor() const
{
	return m_nUnderlineColor;
}

ELineStyle2 CHWPRecordCharShape::GetStrikeOutType() const
{
	return m_eStrikeOutShape;
}

int CHWPRecordCharShape::GetStrikeOutColor() const
{
	return m_nStrikeOutColor;
}

short CHWPRecordCharShape::GetRelSize(ELang eLang) const
{
	if (ELang::MAX == eLang)
		return 0;

	return m_arRelSizes[(int)eLang];
}

HWP_STRING CHWPRecordCharShape::GetFontName(ELang eLang) const
{
	if (ELang::MAX == eLang)
		return HWP_STRING();

	return m_arFontNames[(int)eLang];
}

short CHWPRecordCharShape::GetRatio(ELang eLang) const
{
	if (ELang::MAX == eLang)
		return 0;

	return m_arRatios[(int)eLang];
}

short CHWPRecordCharShape::GetSpacing(ELang eLang) const
{
	if (ELang::MAX == eLang)
		return 0;

	return m_arSpacings[(int)eLang];
}

int CHWPRecordCharShape::GetTextColor() const
{
	return m_nTextColor;
}

int CHWPRecordCharShape::GetShadeColor() const
{
	return m_nShadeColor;
}

short CHWPRecordCharShape::GetBorderFillID() const
{
	return m_shBorderFillIDRef;
}
}
