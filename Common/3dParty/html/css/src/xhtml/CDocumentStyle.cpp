#include "CDocumentStyle.h"

#include <iostream>
#include <wchar.h>
#include <math.h>
#include <algorithm>
#include <list>

#define DEFAULT_LINEHEIGHT 240
#define LINEHEIGHTSCALE 10 // Значение LineHeight в OOXML должно быть в 10 раз больше чем указано в стиле

namespace NSCSS
{
	CStyleUsed::CStyleUsed(const CCompiledStyle &oStyle, bool bIsPStyle)
		: m_oStyle(oStyle), m_bIsPStyle(bIsPStyle)
	{}

	bool CStyleUsed::operator==(const CStyleUsed &oUsedStyle) const
	{
		return (m_bIsPStyle == oUsedStyle.m_bIsPStyle) && (m_oStyle == oUsedStyle.m_oStyle);
	}

	std::wstring CStyleUsed::getId()
	{
		return m_sId;
	}

	void CStyleUsed::setId(const std::wstring &sId)
	{
		m_sId = sId;
	}

	CDocumentStyle::CDocumentStyle() : m_arStandardStyles({L"a", L"li", L"h1", L"h2", L"h3", L"h4", L"h5", L"h6", L"h1-c",
	    L"h2-c", L"h3-c", L"h4-c", L"h5-c", L"h6-c", L"p-c", L"p", L"div-c", L"div", L"a-c"}) {}

	CDocumentStyle::~CDocumentStyle()
	{
		m_arStandardStyles.    clear();
		m_arStyleUsed.         clear();
		m_arStandardStylesUsed.clear();
	}

	std::wstring CDocumentStyle::GetStyle() const
	{
		return m_sStyle;
	}

	std::wstring CDocumentStyle::GetIdAndClear()
	{
		std::wstring sId = m_sId;
		Clear();
		return sId;
	}

	void CDocumentStyle::Clear()
	{
		m_sId.clear();
		m_sStyle.clear();
	}

	void CDocumentStyle::SetStyle(const std::wstring& sStyle)
	{
		if (sStyle.empty())
			return;
		m_sStyle = sStyle;
	}

	void CDocumentStyle::SetId   (const std::wstring& sId)
	{
		if (sId.empty())
			return;
		m_sId = sId;
	}

	void CDocumentStyle::CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement)
	{
		oElement.AddBasicProperties(CSSProperties::BasicProperties::B_BasedOn, L"normal");

		if (arStandartedStyles.empty())
			return;

		std::vector<std::wstring> arStyles;
		for (const std::wstring& sStyleName : arStandartedStyles)
		{
			if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.end())
				arStyles.push_back(sStyleName);
		}

		if (arStyles.empty())
			return;

		std::wstring sId;
		for (std::vector<std::wstring>::const_reverse_iterator iStyleName = arStyles.rbegin(); iStyleName != arStyles.rend(); ++iStyleName)
		{
			sId += *iStyleName + L'+';
			CXmlElement oTempElement;
			CreateStandardStyle(*iStyleName, oTempElement);
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), *iStyleName) == m_arStandardStylesUsed.end())
			{
				m_sStyle = oTempElement.GetStyle() + m_sStyle;
				m_arStandardStylesUsed.push_back(*iStyleName);
			}
			oElement += oTempElement;
		}
		sId.pop_back();

		oElement.AddBasicProperties(BProperties::B_Name, sId);
		oElement.AddBasicProperties(BProperties::B_StyleId, sId);
	}

	void CDocumentStyle::CreateStandardStyle(const std::wstring& sNameStyle, CXmlElement& oElement)
	{
		if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) != m_arStandardStyles.end())
			oElement.CreateDefaultElement(sNameStyle);
	}

	void CDocumentStyle::ConvertStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle)
	{
		if (oStyle.GetId().empty())
			return;

		std::wstring sName = oStyle.GetId();
		const size_t posPoint = sName.find(L'.');

		if (posPoint != std::wstring::npos)
			sName = sName.substr(0, posPoint);
		else
		{
			const size_t posLattice = sName.find(L'#');
			if (posLattice != std::wstring::npos)
				sName = sName.substr(0, posLattice);
			else
			{
				const size_t posDash = sName.find(L'-');
				if (posDash != std::wstring::npos)
					sName = sName.substr(0, posDash);
			}
		}

		if (!bIsPStyle)
			sName += L"-c";

		CXmlElement oParentStyle;
		std::vector<std::wstring> arParentsName = oStyle.GetParentsName();

		if (!bIsPStyle)
			for (std::wstring& sParentName : arParentsName)
				sParentName += L"-c";

		if (!arParentsName.empty())
		{
			CombineStandardStyles(arParentsName, oParentStyle);

			if (!oParentStyle.Empty())
			{
				oParentStyle.AddBasicProperties(BProperties::B_StyleId, L"(" + oParentStyle.GetStyleId() + L")");
				if (!bIsPStyle)
				{
					oParentStyle.AddBasicProperties(BProperties::B_StyleId, oParentStyle.GetStyleId() + L"-c");
					oParentStyle.AddBasicProperties(BProperties::B_Type, L"character");
				}
			}
		}

		CXmlElement oStandardXmlElement;
		if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.end())
			CreateStandardStyle(sName, oStandardXmlElement);

		if (oStandardXmlElement.Empty() && !oParentStyle.Empty())
		{
			const std::wstring sParentsStyleID = oParentStyle.GetStyleId();
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sParentsStyleID) == m_arStandardStylesUsed.end())
			{
				if (bIsPStyle)
				{
					oParentStyle.AddBasicProperties(BProperties::B_QFormat, L"true");
					m_sStyle += oParentStyle.GetPStyle();
				}
				else
					m_sStyle += oParentStyle.GetRStyle();

				m_arStandardStylesUsed.push_back(sParentsStyleID);
			}
			if (oStyle.Empty())
			{
				m_sId = sParentsStyleID;
				return;
			}

			oElement.AddBasicProperties(BProperties::B_BasedOn, sParentsStyleID);
		}
		else if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
		{
			std::wstring sStandPlusParent = oStandardXmlElement.GetStyleId() + L"+" + oParentStyle.GetStyleId();
			if(std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandPlusParent) != m_arStandardStylesUsed.end())
			{
				if (oStyle.Empty())
				{
					m_sId = sStandPlusParent;
					return;
				}
				oElement.AddBasicProperties(BProperties::B_BasedOn, sStandPlusParent);
			}
			else
			{
				CXmlElement oTempElement = oParentStyle;
				oTempElement += oStandardXmlElement;

				oTempElement.AddBasicProperties(BProperties::B_StyleId, sStandPlusParent);

				if (bIsPStyle)
				{
					oTempElement.AddBasicProperties(BProperties::B_QFormat, L"true");
					m_sStyle += oTempElement.GetPStyle();
				}
				else
					m_sStyle += oTempElement.GetRStyle();

				m_arStandardStylesUsed.push_back(sStandPlusParent);

				if (oStyle.Empty())
				{
					m_sId = sStandPlusParent;
					return;
				}
				oElement.AddBasicProperties(BProperties::B_BasedOn, oTempElement.GetStyleId());
			}
		}
		else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
		{
			std::wstring sStandartStyleID = oStandardXmlElement.GetStyleId();
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandartStyleID) == m_arStandardStylesUsed.end())
			{
				if (bIsPStyle)
				{
					oStandardXmlElement.AddBasicProperties(BProperties::B_QFormat, L"true");
					m_sStyle += oStandardXmlElement.GetPStyle();
				}
				else
					m_sStyle += oStandardXmlElement.GetRStyle();

				m_arStandardStylesUsed.push_back(sStandartStyleID);
			}
			if (oStyle.Empty())
			{
				m_sId = sStandartStyleID;
				return;
			}
			oElement.AddBasicProperties(BProperties::B_BasedOn, sStandartStyleID);
		}

		if (oStyle.Empty() && oElement.Empty())
		{
			m_sId = L"normal";
			return;
		}

		m_sId = oStyle.GetId();
		if(!bIsPStyle)
			m_sId += L"-c";
		else
			oElement.AddBasicProperties(BProperties::B_QFormat, L"true");

		oElement.AddBasicProperties(BProperties::B_StyleId, m_sId);
		oElement.AddBasicProperties(BProperties::B_Name, m_sId);
		oElement.AddBasicProperties(BProperties::B_Type, bIsPStyle ? L"paragraph" : L"character");
		oElement.AddBasicProperties(BProperties::B_CustomStyle, L"1");
	}

	void CDocumentStyle::SetPStyle   (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
	{
		ConvertStyle(oStyle, oXmlElement, true);
		if (oStyle.Empty() && oXmlElement.Empty())
			return;
		
		oXmlElement.AddPropertiesInP(PProperties::P_Jc, oStyle.m_oText.GetAlign().ToWString());

		std::wstring sInfValue;
		sInfValue.reserve(64);

//		if (!oStyle.m_oPadding.GetLeft().Empty() && !oStyle.m_oPadding.GetLeft().Zero())
//			sInfValue += L"w:left=\""   + DOUBLE_TO_INTW(oStyle.m_oPadding.GetLeft().ToDouble(NSCSS::Twips)) + L"\" ";

//		if (!oStyle.m_oPadding.GetRight().Empty() && !oStyle.m_oPadding.GetRight().Zero())
//			sInfValue += L"w:right=\""  + DOUBLE_TO_INTW(oStyle.m_oPadding.GetRight().ToDouble(NSCSS::Twips)) + L"\" ";

		const int nIndent = oStyle.m_oText.GetIndent().ToInt(NSCSS::Twips);
		
		if (0 != nIndent)
			sInfValue += L"w:firstLine=\"" + std::to_wstring(nIndent) + L"\" ";

		oXmlElement.AddPropertiesInP(PProperties::P_Ind, sInfValue);

		std::wstring sSpacingValue;
		sSpacingValue.reserve(128);

//		if (!oStyle.m_oPadding.GetBottom().Empty() && !oStyle.m_oPadding.GetBottom().Zero())
//			sSpacingValue += L"w:after=\""   + DOUBLE_TO_INTW(oStyle.m_oPadding.GetBottom().ToDouble(NSCSS::Twips)) + L"\" ";

//		if (!oStyle.m_oPadding.GetTop().Empty() && !oStyle.m_oPadding.GetTop().Zero())
//			sSpacingValue += L"w:before=\""  + DOUBLE_TO_INTW(oStyle.m_oPadding.GetTop().ToDouble(NSCSS::Twips)) + L"\" ";

//		else/* if (!oStyle.m_pBorder.Empty() || !oStyle.m_oMargin.GetPermission())*/
//			sSpacingValue += L"w:after=\"0\" w:before=\"0\"";

		if (!oStyle.m_oFont.GetLineHeight().Empty() && !oStyle.m_oFont.GetLineHeight().Zero())
			sSpacingValue += L" w:line=\"" + std::to_wstring(oStyle.m_oFont.GetLineHeight().ToInt(NSCSS::Twips, DEFAULT_LINEHEIGHT)) + L"\" w:lineRule=\"auto\"";

		if (!sSpacingValue.empty())
		{
			oXmlElement.AddPropertiesInP(PProperties::P_Spacing, sSpacingValue);
			oXmlElement.AddPropertiesInP(PProperties::P_ContextualSpacing, L"true");
		}

		if (!oStyle.m_oBackground.Empty())
		{
			const std::wstring wsColor = oStyle.m_oBackground.GetColor().ToWString();
			if (wsColor != L"ffffff")
				oXmlElement.AddPropertiesInP(PProperties::P_Shd, wsColor);
		}

		if (!oStyle.m_oBorder.Empty() && !oStyle.HaveThisParent(L"table"))
		{
			if (oStyle.m_oBorder.EqualSides())
			{
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_TopBorder);
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_LeftBorder);
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_BottomBorder);
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_RightBorder);
			}
			else
			{
				if (!oStyle.m_oBorder.GetTopBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_TopBorder);

				if (!oStyle.m_oBorder.GetRightBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_RightBorder);

				if (!oStyle.m_oBorder.GetBottomBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_BottomBorder);

				if (!oStyle.m_oBorder.GetLeftBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_LeftBorder);
			}
		}
	}

	void CDocumentStyle::SetAllBorderStyle(const CCompiledStyle &oStyle, CXmlElement &oXmlElement)
	{
		const std::wstring wsBorder = CalculateBorderStyle(oStyle.m_oBorder.GetLeftBorder());

		oXmlElement.AddPropertiesInP(PProperties::P_TopBorder,    wsBorder);
		oXmlElement.AddPropertiesInP(PProperties::P_RightBorder,  wsBorder);
		oXmlElement.AddPropertiesInP(PProperties::P_BottomBorder, wsBorder);
		oXmlElement.AddPropertiesInP(PProperties::P_LeftBorder,   wsBorder);
	}

	void CDocumentStyle::SetBorderStyle(const CCompiledStyle &oStyle, CXmlElement &oXmlElement, const PProperties &enBorderProperty)
	{
		const NSCSS::NSProperties::CBorderSide* pBorder = NULL;

		switch(enBorderProperty)
		{
			case PProperties::P_BottomBorder:
			{
				pBorder = &oStyle.m_oBorder.GetBottomBorder();
				break;
			}
			case PProperties::P_LeftBorder:
			{
				pBorder = &oStyle.m_oBorder.GetLeftBorder();
				break;
			}
			case PProperties::P_RightBorder:
			{
				pBorder = &oStyle.m_oBorder.GetRightBorder();
				break;
			}
			case PProperties::P_TopBorder:
			{
				pBorder = &oStyle.m_oBorder.GetTopBorder();
				break;
			}
			default:
				return;
		}

		oXmlElement.AddPropertiesInP(enBorderProperty, CalculateBorderStyle(*pBorder));
	}

	std::wstring CDocumentStyle::CalculateBorderStyle(const NSProperties::CBorderSide &oBorder)
	{
		if (oBorder.Empty())
			return L"";
		
		std::wstring wsColor = oBorder.GetColor().ToWString();
		std::wstring wsStyle = oBorder.GetStyle().ToWString();
		double dWidth = oBorder.GetWidth().ToDouble(Point) * 8; // Так как значение указано в восьмых долях точки

		if (wsColor.empty())
			wsColor = L"auto";

		if (wsStyle.empty())
			wsStyle = L"single";

		if (1 > dWidth)
			dWidth = 1;

		return  L"w:val=\"" + wsStyle + L"\" w:sz=\"" + std::to_wstring(static_cast<int>(dWidth)) + + L"\" w:space=\"0\" w:color=\"" + wsColor + L"\"";
	}

	void CDocumentStyle::SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
	{
		ConvertStyle(oStyle, oXmlElement, false);
		if (oStyle.Empty() && oXmlElement.Empty())
			return;

		if (!oStyle.m_oFont.GetSize().Empty())
			oXmlElement.AddPropertiesInR(RProperties::R_Sz, std::to_wstring(oStyle.m_oFont.GetSize().ToInt(NSCSS::Point) * 2)); // Значения шрифта увеличивает на 2

		if (oStyle.m_oText.GetDecoration().m_oLine.Underline())
			oXmlElement.AddPropertiesInR(RProperties::R_U, (!oStyle.m_oText.GetDecoration().m_oStyle.Empty()) ? oStyle.m_oText.GetDecoration().m_oStyle.ToWString() : L"single");
		
		oXmlElement.AddPropertiesInR(RProperties::R_Highlight, oStyle.m_oBackground.GetColor().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_Color, oStyle.m_oText.GetColor().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_RFonts, oStyle.m_oFont.GetFamily().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_I, oStyle.m_oFont.GetStyle().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_B, oStyle.m_oFont.GetWeight().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_SmallCaps, oStyle.m_oFont.GetVariant().ToWString());
	}

	void CDocumentStyle::WriteRStyle (const NSCSS::CCompiledStyle& oStyle)
	{
		if(oStyle.GetId().empty())
		{
			m_sId = L"normal";
			return;
		}

		CStyleUsed structStyle(oStyle, false);

		std::list<CStyleUsed>::iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

		if (oItem != m_arStyleUsed.end())
		{
			m_sId = (*oItem).getId();
			return;
		}
		CXmlElement oXmlElement;
		SetRStyle(oStyle, oXmlElement);

		if (!oStyle.Empty() || !oXmlElement.Empty())
		{
			structStyle.setId(oXmlElement.GetStyleId());
			m_arStyleUsed.push_back(structStyle);
			m_sStyle += oXmlElement.GetRStyle();
		}
	}

	void CDocumentStyle::WriteLitePStyle(const CCompiledStyle &oStyle)
	{
		if (oStyle.Empty())
			return;

		CXmlElement oXmlElement;
		SetPStyle(oStyle, oXmlElement);

		if (!oXmlElement.Empty())
			m_sStyle += oXmlElement.GetPStyle(true);
	}

	void CDocumentStyle::WriteLiteRStyle(const CCompiledStyle &oStyle)
	{
		if (oStyle.Empty())
			return;

		CXmlElement oXmlElement;
		SetRStyle(oStyle, oXmlElement);

		if (!oXmlElement.Empty())
			m_sStyle += oXmlElement.GetRStyle(true);
	}

	void CDocumentStyle::WritePStyle (const NSCSS::CCompiledStyle& oStyle)
	{
		if(oStyle.GetId().empty())
		{
			m_sId = L"normal";
			return;
		}

		CStyleUsed structStyle(oStyle, true);
		std::list<CStyleUsed>::iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

		if (oItem != m_arStyleUsed.end())
		{
			m_sId = (*oItem).getId();
			return;
		}

		CXmlElement oXmlElement;
		SetPStyle(oStyle, oXmlElement);

		if (!oStyle.Empty() || !oXmlElement.Empty())
		{
			structStyle.setId(oXmlElement.GetStyleId());
			m_arStyleUsed.push_back(structStyle);
			m_sStyle += oXmlElement.GetPStyle();
		}
	}
}
