#include "CXmlElement.h"

#include <algorithm>
#include <vector>
#include <cwctype>
#include <functional>

#include <iostream>
#include "../ConstValues.h"

#define DEFAULTFONTNAME L"Times New Roman"

CXmlElement::CXmlElement()
{}

CXmlElement::~CXmlElement()
{
	m_mBasicValues.clear();
	m_mPStyleValues.clear();
	m_mRStyleValues.clear();
}

CXmlElement::CXmlElement(const std::wstring& sNameDefaultElement)
{
	CreateDefaultElement(sNameDefaultElement);
}

bool CXmlElement::Empty() const
{
	return m_mPStyleValues.empty() && m_mRStyleValues.empty() && GetBasedOn().empty();
}

void CXmlElement::CreateDefaultElement(const std::wstring& sNameDefaultElement)
{
	if (!Empty())
		Clear();

/*	if (sNameDefaultElement == L"p")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"p");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Normal (Web)");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"99");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
		AddBasicProperties(CSSProperties::BasicProperties::B_SemiHidden, L"true");

//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");
	}
	else */if (sNameDefaultElement == L"li")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"li");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"List Paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"34");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_ContextualSpacing, L"true");
		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Ind, L"w:left=\"720\"");
	}
	else if (sNameDefaultElement == L"h1")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h1");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Heading 1");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h1-c");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_OutlineLvl, L"0");
//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");
	}
	else if (sNameDefaultElement == L"h2")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h2");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Heading 2");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h2-c");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_OutlineLvl, L"1");
//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");
	}
	else if (sNameDefaultElement == L"h3")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h3");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Heading 3");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h3-c");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_OutlineLvl, L"2");
//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");
	}
	else if (sNameDefaultElement == L"h4")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h4");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Heading 4");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h4-c");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_OutlineLvl, L"3");
//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");
	}
	else if (sNameDefaultElement == L"h5")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h5");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Heading 5");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h5-c");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_OutlineLvl, L"4");
//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");

	}
	else if (sNameDefaultElement == L"h6")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h6");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Heading 6");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h6-c");

		AddPropertiesInP(CSSProperties::ParagraphProperties::P_OutlineLvl, L"5");
//		AddPropertiesInP(CSSProperties::ParagraphProperties::P_Spacing, L"w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"");
	}
	else if (sNameDefaultElement == L"h1-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h1-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Title 1 Sign");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"9");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h1");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_B, L"bold");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"48");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_Kern, L"36");
	}
	else if (sNameDefaultElement == L"h2-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h2-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Title 2 Sign");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"9");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h2");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_B, L"bold");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"36");
	}
	else if (sNameDefaultElement == L"h3-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h3-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Title 3 Sign");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"9");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h3");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_B, L"bold");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"27");
	}
	else if (sNameDefaultElement == L"h4-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h4-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Title 4 Sign");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"9");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h4");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"24");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_B, L"bold");
	}
	else if (sNameDefaultElement == L"h5-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h5-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Title 5 Sign");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"9");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h5");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"20");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_B, L"bold");
	}
	else if (sNameDefaultElement == L"h6-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"h6-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Title 6 Sign");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"9");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"h6");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"15");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_B, L"bold");
	}
	/*else if (sNameDefaultElement == L"div-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"div-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Div character");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"div");
	}
	else if (sNameDefaultElement == L"div")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"div");
		AddBasicProperties(CSSProperties::BasicProperties::B_CustomStyle, L"1");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Div paragraph");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");
		AddBasicProperties(CSSProperties::BasicProperties::B_Link, L"div-c");
	}*/
	else if (sNameDefaultElement == L"a-c")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"a-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Hyperlink character");
		AddBasicProperties(CSSProperties::BasicProperties::B_UiPriority, L"99");
		AddBasicProperties(CSSProperties::BasicProperties::B_UnhideWhenUsed, L"true");

		AddPropertiesInR(CSSProperties::RunnerProperties::R_Sz, L"24");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_Color, L"0000FF");
		AddPropertiesInR(CSSProperties::RunnerProperties::R_U, L"single");
	}
	else if (sNameDefaultElement == L"a")
	{
		AddBasicProperties(CSSProperties::BasicProperties::B_Type, L"character");
		AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"a-c");
		AddBasicProperties(CSSProperties::BasicProperties::B_StyleId, L"a");
		AddBasicProperties(CSSProperties::BasicProperties::B_Name, L"Hyperlink");
	}
}

void CXmlElement::Clear()
{
	m_mBasicValues.clear();
	m_mPStyleValues.clear();
	m_mRStyleValues.clear();
}

void CXmlElement::AddPropertiesInP(const CSSProperties::ParagraphProperties &enProperties, const std::wstring &sValue)
{
	if (sValue.empty())
		return;

	m_mPStyleValues[enProperties] = sValue;
}

void CXmlElement::AddPropertiesInR(const CSSProperties::RunnerProperties &enProperties, const std::wstring &sValue)
{
	if (sValue.empty())
		return;

	m_mRStyleValues[enProperties] = sValue;
}

void CXmlElement::AddBasicProperties(const CSSProperties::BasicProperties &enProperties, const std::wstring &sValue)
{
	if (sValue.empty())
		return;

	m_mBasicValues[enProperties] = sValue;
}

CXmlElement& CXmlElement::operator+=(const CXmlElement& oElement)
{
	if (oElement.Empty())
		return *this;

	for (const BPropertiesMap::value_type& oBasicElement : oElement.m_mBasicValues)
		m_mBasicValues[oBasicElement.first] = oBasicElement.second;

	for (const PPropertiesMap::value_type& oPElement : oElement.m_mPStyleValues)
		m_mPStyleValues[oPElement.first] = oPElement.second;

	for (const RPropertiesMap::value_type& oRElement : oElement.m_mRStyleValues)
		m_mRStyleValues[oRElement.first] = oRElement.second;

	return *this;
}

CXmlElement& CXmlElement::operator=(const CXmlElement& oElement)
{
	m_mBasicValues = oElement.m_mBasicValues;
	m_mPStyleValues = oElement.m_mPStyleValues;
	m_mRStyleValues = oElement.m_mRStyleValues;

	return *this;
}

bool CXmlElement::operator==(const CXmlElement &oElement) const
{
	return m_mBasicValues == oElement.m_mBasicValues &&
	       m_mPStyleValues == oElement.m_mPStyleValues &&
	       m_mRStyleValues == oElement.m_mRStyleValues;
}

std::wstring CXmlElement::ConvertPStyle(bool bIsLite) const
{
	if (m_mPStyleValues.empty())
		return std::wstring();

	std::wstring sPPr;
	std::wstring sPBdr;

	for (const PPropertiesMap::value_type& oItem : m_mPStyleValues)
	{
		switch (oItem.first)
		{
			case CSSProperties::ParagraphProperties::P_Jc:
			{
				sPPr += L"<w:jc w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_Spacing:
			{
				sPPr += L"<w:spacing " + oItem.second + L"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_ContextualSpacing:
			{
				sPPr += L"<w:contextualSpacing/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_Ind:
			{
				sPPr += L"<w:ind " + oItem.second + L"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_OutlineLvl:
			{
				sPPr += L"<w:outlineLvl w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_Shd:
			{
				sPPr += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + oItem.second + L"\"/>";
				break;
			}

			case CSSProperties::ParagraphProperties::P_TopBorder:
			{
				sPBdr += L"<w:top " + oItem.second + L"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_LeftBorder:
			{
				sPBdr += L"<w:left " + oItem.second + L"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_BottomBorder:
			{
				sPBdr += L"<w:bottom " + oItem.second + L"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_RightBorder:
			{
				sPBdr += L"<w:right " + oItem.second + L"/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_KeepLines:
			{
				sPPr += L"<w:keepNext/>";
				break;
			}
			case CSSProperties::ParagraphProperties::P_KeepNext:
			{
				sPPr += L"<w:keepLines/>";
				break;
			}
			default:
				break;
		}
	}

	if (!sPBdr.empty())
		sPPr += L"<w:pBdr>" + sPBdr + L"</w:pBdr>";

	if (bIsLite)
		return sPPr;

	return L"<w:pPr>" + sPPr + L"</w:pPr>";
}

std::wstring CXmlElement::ConvertRStyle(bool bIsLite) const
{
	if (m_mRStyleValues.empty())
		return std::wstring();

	std::wstring sRStyle;

	for (const RPropertiesMap::value_type& oItem : m_mRStyleValues)
	{
		switch (oItem.first)
		{
			case CSSProperties::RunnerProperties::R_RFonts:
			{
				std::wstring wsFontFamily = (oItem.second.empty() || L"\"inherit\"" == oItem.second) ?  DEFAULTFONTNAME : oItem.second;

				sRStyle += L"<w:rFonts w:ascii=\"" + wsFontFamily + L'\"' +
				           L" w:hAnsi=\"" + wsFontFamily + L'\"' +
				           L" w:cs=\"" + wsFontFamily + L'\"' +
				           L" w:eastAsia=\"" + wsFontFamily + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_Sz:
			{
				sRStyle += L"<w:sz w:val=\""   + oItem.second + L"\"/>" + 
				           L"<w:szCs w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_B:
			{
				if (oItem.second == L"bold")
					sRStyle += L"<w:b/><w:bCs/>";
				else if (oItem.second == L"normal")
					sRStyle += L"<w:b w:val=\"false\"/><w:bCs w:val=\"false\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_I:
			{
				if (oItem.second == L"italic")
					sRStyle += L"<w:i/><w:iCs/>";
				else if (oItem.second == L"normal")
					sRStyle += L"<w:i w:val=\"false\"/><w:iCs w:val=\"false\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_Color:
			{
				sRStyle += L"<w:color w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_U:
			{
				if (oItem.second == L"line-through")
					sRStyle += L"<w:strike/>";
				else
					sRStyle += L"<w:u w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_Highlight:
			{
				if (!oItem.second.empty())
					sRStyle += L"<w:highlight w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_Shd:
			{
				if (!oItem.second.empty())
					sRStyle += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_SmallCaps:
			{
				if (oItem.second == L"smallCaps")
					sRStyle += L"<w:smallCaps/>";
				else if (oItem.second == L"normal")
					sRStyle += L"<w:smallCaps w:val=\"false\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_Kern:
			{
				sRStyle += L"<w:kern w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::RunnerProperties::R_Vanish:
			{
				if (oItem.second == L"true")
					sRStyle += L"<w:vanish/>";
				break;
			}
			default:
				break;
		}
	}

	if (bIsLite)
		return sRStyle;

	return L"<w:rPr>" + sRStyle + L"</w:rPr>";
}

std::wstring CXmlElement::ConvertBasicInfoStyle() const
{
	if (m_mBasicValues.empty())
		return std::wstring();

	std::wstring sBasicInfo;

	for (const BPropertiesMap::value_type& oItem : m_mBasicValues)
	{
		switch (oItem.first)
		{
			case CSSProperties::BasicProperties::B_Name:
			{
				sBasicInfo += L"<w:name w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::BasicProperties::B_BasedOn:
			{
				sBasicInfo += L"<w:basedOn w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::BasicProperties::B_QFormat:
			{
				sBasicInfo += L"<w:qFormat/>";
				break;
			}
			case CSSProperties::BasicProperties::B_Link:
			{
				sBasicInfo += L"<w:link w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::BasicProperties::B_UnhideWhenUsed:
			{
				if (L"true" == oItem.second)
					sBasicInfo += L"<w:unhideWhenUsed/>";
				break;
			}
			case CSSProperties::BasicProperties::B_UiPriority:
			{
				sBasicInfo += L"<w:uiPriority w:val=\"" + oItem.second + L"\"/>";
				break;
			}
			case CSSProperties::BasicProperties::B_SemiHidden:
			{
				if (L"true" == oItem.second)
					sBasicInfo += L"<w:semiHidden/>";
				break;
			}
			default:
				break;
		}
	}

	return sBasicInfo;
}

std::wstring CXmlElement::GetStyleArguments() const
{
	std::wstring wsStyleArguments;
	
	for (const BPropertiesMap::value_type& oItem : m_mBasicValues)
	{
		switch (oItem.first)
		{
			case CSSProperties::BasicProperties::B_CustomStyle:
			{
				wsStyleArguments += L" w:customStyle=\"" + oItem.second + L"\"";
				break;
			}
			case CSSProperties::BasicProperties::B_StyleId:
			{
				wsStyleArguments += L" w:styleId=\"" + oItem.second + L"\"";
				break;
			}
			case CSSProperties::BasicProperties::B_Type:
			{
				wsStyleArguments += L" w:type=\"" + oItem.second + L"\"";
				break;
			}
			case CSSProperties::BasicProperties::B_Default:
			{
				wsStyleArguments += L" w:default=\"" + oItem.second + L"\"";
				break;
			}
			default:
				break;
		}
	}
	
	return wsStyleArguments;
}

std::wstring CXmlElement::GetStyle(bool bGetBasedInfo, bool bGetPInfo, bool bGetRInfo) const
{
	if (Empty())
		return std::wstring();

	std::wstring wsStyleArguments = GetStyleArguments();
	std::wstring wsStyleValue;

	if (bGetBasedInfo)
		wsStyleValue += ConvertBasicInfoStyle();
	
	if (bGetPInfo)
		wsStyleValue += ConvertPStyle();
	
	if (bGetRInfo)
		wsStyleValue += ConvertRStyle();

	if (wsStyleArguments.empty() && wsStyleValue.empty())
		return std::wstring();

	return L"<w:style" + wsStyleArguments + L">" + wsStyleValue + L"</w:style>";
}

std::wstring CXmlElement::GetPStyle(bool bIsLite) const
{
	if (bIsLite)
		return ConvertPStyle(true);

	return GetStyle(true, true, false);
}

std::wstring CXmlElement::GetRStyle(bool bIsLite) const
{
	if (bIsLite)
		return ConvertRStyle(true);

	return GetStyle(true, false, true);
}

std::wstring CXmlElement::GetBasedOn() const
{
	BPropertiesMap::const_iterator posBasedOn = m_mBasicValues.find(CSSProperties::BasicProperties::B_BasedOn);
	return  (posBasedOn != m_mBasicValues.end()) ? posBasedOn->second : std::wstring();
}

std::wstring CXmlElement::GetStyleId() const
{
	BPropertiesMap::const_iterator posStyleId = m_mBasicValues.find(CSSProperties::BasicProperties::B_StyleId);
	return  (posStyleId != m_mBasicValues.end()) ? posStyleId->second : std::wstring();
}

std::wstring CXmlElement::GetName() const
{
	BPropertiesMap::const_iterator posName = m_mBasicValues.find(CSSProperties::BasicProperties::B_Name);
	return  (posName != m_mBasicValues.end()) ? posName->second : std::wstring();
}

