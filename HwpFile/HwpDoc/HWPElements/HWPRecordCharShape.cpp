#include "HWPRecordCharShape.h"
#include "../HWPElements/HWPRecordFaceName.h"

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

void CHWPRecordCharShape::ReadContainerData(CXMLNode& oNode, short arValues[], int nDefaultValue)
{
	arValues[(int)ELang::HANGUL]   = oNode.GetAttributeInt(L"hangul",   nDefaultValue);
	arValues[(int)ELang::LATIN]    = oNode.GetAttributeInt(L"latin",    nDefaultValue);
	arValues[(int)ELang::HANJA]    = oNode.GetAttributeInt(L"hanja",    nDefaultValue);
	arValues[(int)ELang::JAPANESE] = oNode.GetAttributeInt(L"japanese", nDefaultValue);
	arValues[(int)ELang::OTHER]    = oNode.GetAttributeInt(L"other",    nDefaultValue);
	arValues[(int)ELang::SYMBOL]   = oNode.GetAttributeInt(L"symbol",   nDefaultValue);
	arValues[(int)ELang::USER]     = oNode.GetAttributeInt(L"user",     nDefaultValue);
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
		m_arCharOffset[nIndex] = (HWP_BYTE)(oBuffer[0] & 0x00FF);
		oBuffer.Skip(1);
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

CHWPRecordCharShape::CHWPRecordCharShape(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_HWP_CHAR_SHAPE, 0, 0), m_pParent(&oDocInfo),
      m_bItalic(false), m_bBold(false), m_bEmboss(false), m_bEngrave(false),
      m_bSuperScript(false), m_bSubScript(false)
{
	m_eUnderline = EUnderline::NONE;
	m_eUnderLineShape = ELineStyle1::SOLID;
	m_eOutline = EOutline::NONE;
	m_eShadow = EShadow::NONE;
	m_eStrikeOutShape = ELineStyle2::NONE;

	m_nHeight = oNode.GetAttributeInt(L"height", 1000);
	m_nTextColor = oNode.GetAttributeColor(L"textColor", 0x000000);
	m_nShadeColor = oNode.GetAttributeColor(L"shadeColor", 0xFFFFFFFF);
	m_bUseFontSpace = oNode.GetAttributeBool(L"useFontSpace");
	m_bUseKerning = oNode.GetAttributeBool(L"useKerning");

	m_eSymMark = GetAccent(oNode.GetAttribute(L"symMark"));

	m_shBorderFillIDRef = oNode.GetAttributeInt(L"borderFillIDRef");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hh:fontRef" == oChild.GetName())
		{
			if (nullptr == m_pParent)
				continue;

			const CHWPRecordFaceName* pFaceName = nullptr;

			#define UPDATE_FACENAME(node_name, elang_type) \
			pFaceName = dynamic_cast<const CHWPRecordFaceName*>(m_pParent->GetFaceName(oChild.GetAttributeInt(node_name))); \
			if (nullptr != pFaceName) \
				m_arFontNames[(int)elang_type] = pFaceName->GetFaceName()

			UPDATE_FACENAME(L"hangul",   ELang::HANGUL);
			UPDATE_FACENAME(L"latin",    ELang::LATIN);
			UPDATE_FACENAME(L"hanja",    ELang::HANJA);
			UPDATE_FACENAME(L"japanese", ELang::JAPANESE);
			UPDATE_FACENAME(L"other",    ELang::OTHER);
			UPDATE_FACENAME(L"symbol",   ELang::SYMBOL);
			UPDATE_FACENAME(L"user",     ELang::USER);
		}
		else if (L"hh:ratio" == oChild.GetName())
			ReadContainerData(oChild, m_arRatios, 100);
		else if (L"hh:spacing" == oChild.GetName())
			ReadContainerData(oChild, m_arSpacings);
		else if (L"hh:relSz" == oChild.GetName())
			ReadContainerData(oChild, m_arRelSizes, 100);
		else if (L"hh:offset" == oChild.GetName())
			ReadContainerData(oChild, m_arCharOffset);
		else if (L"hh:underline" == oChild.GetName())
		{
			m_eUnderline = GetUnderline(oChild.GetAttribute(L"type"));
			m_eUnderLineShape = GetLineStyle1(oChild.GetAttribute(L"shape"));
			m_nUnderlineColor = oChild.GetAttributeColor(L"color");
		}
		else if (L"hh:strikeout" == oChild.GetName())
		{
			m_eStrikeOutShape = GetLineStyle2(oChild.GetAttribute(L"shape"));
			m_nStrikeOutColor = oChild.GetAttributeColor(L"color");

			if (L"3D" == oChild.GetAttribute(L"shape"))
				m_eStrikeOutShape = ELineStyle2::NONE;
		}
		else if (L"hh:outline" == oChild.GetName())
		{
			m_eOutline = GetOutline(oChild.GetAttribute(L"type"));
		}
		else if (L"hh:shadow" == oChild.GetName())
		{
			HWP_STRING sType = oChild.GetAttribute(L"type");

			if (L"DROP" == sType)
				m_eShadow = EShadow::DISCRETE;
			else if (L"CONTINUOUS" == sType)
				m_eShadow = EShadow::CONTINUOUS;
			else
				m_eShadow = EShadow::NONE;

			m_nShadowColor = oChild.GetAttributeColor(L"color");
			m_chShadowOffsetX = (HWP_BYTE)oChild.GetAttributeInt(L"offsetX");
			m_chShadowOffsetY = (HWP_BYTE)oChild.GetAttributeInt(L"offsetY");
		}
		else if (L"hh:italic" == oChild.GetName())
			m_bItalic = true;
		else if (L"hh:bold" == oChild.GetName())
			m_bBold = true;
		else if (L"hh:emboss" == oChild.GetName())
			m_bEmboss = true;
		else if (L"hh:engrave" == oChild.GetName())
			m_bEmboss = true;
		else if (L"hh:supscript" == oChild.GetName())
			m_bSuperScript = true;
		else if (L"hh:subscript" == oChild.GetName())
			m_bSubScript = true;
	}
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
	return ELineStyle2::NONE != m_eStrikeOutShape;
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
