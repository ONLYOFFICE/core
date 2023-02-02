#include "CDocumentStyle.h"

#include <iostream>
#include <wchar.h>
#include <math.h>
#include <algorithm>
#include <list>

namespace NSCSS
{
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

		oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_Name, sId);
		oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_StyleId, sId);
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
				oParentStyle.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_StyleId, L"(" + oParentStyle.GetStyleId() + L")");
				if (!bIsPStyle)
				{
					oParentStyle.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_StyleId, oParentStyle.GetStyleId() + L"-c");
					oParentStyle.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
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
					oParentStyle.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_QFormat, L"true");
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

			oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_BasedOn, sParentsStyleID);
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
				oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_BasedOn, sStandPlusParent);
			}
			else
			{
				CXmlElement oTempElement = oParentStyle;
				oTempElement += oStandardXmlElement;

				oTempElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_StyleId, sStandPlusParent);

				if (bIsPStyle)
				{
					oTempElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_QFormat, L"true");
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
				oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_BasedOn, oTempElement.GetStyleId());
			}
		}
		else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
		{
			std::wstring sStandartStyleID = oStandardXmlElement.GetStyleId();
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandartStyleID) == m_arStandardStylesUsed.end())
			{
				if (bIsPStyle)
				{
					oStandardXmlElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_QFormat, L"true");
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
			oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_BasedOn, sStandartStyleID);
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
			oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_QFormat, L"true");

		oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_StyleId, m_sId);
		oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_Name, m_sId);
		oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_Type, bIsPStyle ? L"paragraph" : L"character");
		oElement.AddBasicProperties(NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
	}

	void CDocumentStyle::SetPStyle   (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
	{
		ConvertStyle(oStyle, oXmlElement, true);
		if (oStyle.Empty() && oXmlElement.Empty())
			return;

		oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_Jc, oStyle.m_oText.GetAlign().ToWString());

		std::wstring sInfValue;
		sInfValue.reserve(64);

		//TODO:: проверить Permission в Margin
		if (!oStyle.m_oMargin.Empty() || !oStyle.m_oPadding.Empty() /*&& oStyle.m_oMargin.GetPermission()*/)
		{
			const std::wstring& sLeftSide  = (oStyle.m_oMargin.GetLeft()  + oStyle.m_oPadding.GetLeft()) .ToWString();
			const std::wstring& sRightSide = (oStyle.m_oMargin.GetRight() + oStyle.m_oPadding.GetRight()).ToWString();

			if (!sLeftSide.empty())
				sInfValue += L"w:left=\""  + sLeftSide + L"\" ";
			if (!sRightSide.empty())
				sInfValue += L"w:right=\"" + sRightSide + L"\" ";
		}

		const std::wstring& sIndent = oStyle.m_oText.GetIndent().ToWString();
		if (!sIndent.empty())
			sInfValue += L"w:firstLine=\"" + sIndent + L"\" ";

		oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_Ind, sInfValue);

		std::wstring sSpacingValue;
		sSpacingValue.reserve(128);

		//TODO:: проверить Permission в Margin
		if (!oStyle.m_oMargin.Empty() || !oStyle.m_oPadding.Empty() /*&& oStyle.m_oMargin.GetPermission()*/)
		{
			sSpacingValue += L"w:afterAutospacing=\"1\" w:after=\""   + (oStyle.m_oMargin.GetBottom() + oStyle.m_oPadding.GetBottom()).ToWString() + L"\" ";
			sSpacingValue += L"w:beforeAutospacing=\"1\" w:before=\"" + (oStyle.m_oMargin.GetTop()    + oStyle.m_oPadding.GetTop())   .ToWString() + L"\" ";
		}
		else/* if (!oStyle.m_pBorder.Empty() || !oStyle.m_oMargin.GetPermission())*/
			    sSpacingValue += L"w:after=\"0\" w:before=\"0\"";

		const std::wstring &sLineHeight = oStyle.m_oFont.GetLineHeight().ToWString();
		if (!sLineHeight.empty())
		{
			    sSpacingValue += L" w:line=\"" + sLineHeight + L"\" w:lineRule=\"auto\"";
		}
		else if (!oStyle.m_oBorder.Empty())
		{
			    sSpacingValue += L" w:line=\"" + std::to_wstring(static_cast<short int>(oStyle.m_oFont.GetSize().ToDouble() * 12 + 0.5f)) + L"\" w:lineRule=\"auto\"";
		}
		else if (!oStyle.m_oBorder.Empty())
			    sSpacingValue += L"w:line=\"240\" w:lineRule=\"auto\" ";

		if (!sSpacingValue.empty())
		{
			oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_Spacing, sSpacingValue);
			oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_ContextualSpacing, L"true");
		}

		if (!oStyle.m_oBackground.Empty())
		{
			const std::wstring wsColor = oStyle.m_oBackground.GetColor().ToWString();
			if (wsColor != L"ffffff")
				oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_Shd, wsColor);
		}

		if (!oStyle.m_oBorder.Empty())
		{
			if (oStyle.m_oBorder.EqualSides())
			{
				const std::wstring sBorderColor = oStyle.m_oBorder.GetLeftBorder().GetColor().ToWString();
				const std::wstring sBorderStyle = oStyle.m_oBorder.GetLeftBorder().GetStyle().ToWString();
				const std::wstring sBorderWidth = oStyle.m_oBorder.GetLeftBorder().GetWidth().ToWString();

				const std::wstring sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"0\" w:sz=\"" +
				        sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

				oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_TopBorder, sBorder);
				oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_LeftBorder, sBorder);
				oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_BottomBorder, sBorder);
				oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_RightBorder, sBorder);
			}
			else
			{
				if (!oStyle.m_oBorder.GetTopBorder().Empty())
				{
					const std::wstring sBorderColor = oStyle.m_oBorder.GetTopBorder().GetColor().ToWString();
					const std::wstring sBorderStyle = oStyle.m_oBorder.GetTopBorder().GetStyle().ToWString();
					const std::wstring sBorderWidth = oStyle.m_oBorder.GetTopBorder().GetWidth().ToWString();

					const std::wstring sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"4\" w:sz=\"" +
					        sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

					oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_TopBorder, sBorder);
				}

				if (!oStyle.m_oBorder.GetRightBorder().Empty())
				{
					const std::wstring sBorderColor = oStyle.m_oBorder.GetRightBorder().GetColor().ToWString();
					const std::wstring sBorderStyle = oStyle.m_oBorder.GetRightBorder().GetStyle().ToWString();
					const std::wstring sBorderWidth = oStyle.m_oBorder.GetRightBorder().GetWidth().ToWString();

					const std::wstring sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"4\" w:sz=\"" +
					        sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

					oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_RightBorder, sBorder);
				}

				if (!oStyle.m_oBorder.GetBottomBorder().Empty())
				{
					const std::wstring sBorderColor = oStyle.m_oBorder.GetBottomBorder().GetColor().ToWString();
					const std::wstring sBorderStyle = oStyle.m_oBorder.GetBottomBorder().GetStyle().ToWString();
					const std::wstring sBorderWidth = oStyle.m_oBorder.GetBottomBorder().GetWidth().ToWString();

					const std::wstring sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"4\" w:sz=\"" +
					        sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

					oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_BottomBorder, sBorder);
				}

				if (!oStyle.m_oBorder.GetLeftBorder().Empty())
				{
					const std::wstring sBorderColor = oStyle.m_oBorder.GetLeftBorder().GetColor().ToWString();
					const std::wstring sBorderStyle = oStyle.m_oBorder.GetLeftBorder().GetStyle().ToWString();
					const std::wstring sBorderWidth = oStyle.m_oBorder.GetLeftBorder().GetWidth().ToWString();

					const std::wstring sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"4\" w:sz=\"" +
					        sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

					oXmlElement.AddPropertiesInP(NSConstValues::NSProperties::ParagraphProperties::P_LeftBorder, sBorder);
				}
			}
		}
	}

	void CDocumentStyle::SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
	{
		ConvertStyle(oStyle, oXmlElement, false);
		if (oStyle.Empty() && oXmlElement.Empty())
			return;

		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_Highlight, oStyle.m_oBackground.GetColor().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_Color, oStyle.m_oText.GetColor().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_U, oStyle.m_oText.GetDecoration().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_Sz, oStyle.m_oFont.GetSize().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_RFonts, oStyle.m_oFont.GetFamily().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_I, oStyle.m_oFont.GetStyle().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_B, oStyle.m_oFont.GetWeight().ToWString());
		oXmlElement.AddPropertiesInR(NSConstValues::NSProperties::RunnerProperties::R_SmallCaps, oStyle.m_oFont.GetVariant().ToWString());
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
			m_sStyle += oXmlElement.GetLitePStyle();
	}

	void CDocumentStyle::WriteLiteRStyle(const CCompiledStyle &oStyle)
	{
		if (oStyle.Empty())
			return;

		CXmlElement oXmlElement;
		SetRStyle(oStyle, oXmlElement);

		if (!oXmlElement.Empty())
			m_sStyle += oXmlElement.GetLiteRStyle();
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
