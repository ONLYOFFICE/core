#include "ContText.h"
#include "../../resources/ColorTable.h"
#include "../../resources/SingletonTemplate.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CContText::CContText(CFontManager* pManager):
		CBaseItem(ElemType::etContText), m_pManager(pManager)
	{
	}

	CContText::CContText(const CContText& rCont):
		CBaseItem(ElemType::etContText)
	{
		*this = rCont;
	}

	CContText::~CContText()
	{
		Clear();
	}

	void CContText::Clear()
	{
		m_pFontStyle = nullptr;
	}

	CContText& CContText::operator= (const CContText& rCont)
	{
		if (this == &rCont)
		{
			return *this;
		}

		CBaseItem::operator=(rCont);

		m_pFontStyle = rCont.m_pFontStyle;

		m_bIsStrikeoutPresent = rCont.m_bIsStrikeoutPresent;
		m_bIsDoubleStrikeout = rCont.m_bIsDoubleStrikeout;

		m_bIsHighlightPresent = rCont.m_bIsHighlightPresent;
		m_lHighlightColor = rCont.m_lHighlightColor;

		m_bIsUnderlinePresent = rCont.m_bIsUnderlinePresent;
		m_eUnderlineType = rCont.m_eUnderlineType;
		m_lUnderlineColor = rCont.m_lUnderlineColor;

		m_bIsShadowPresent = rCont.m_bIsShadowPresent;
		m_bIsOutlinePresent = rCont.m_bIsOutlinePresent;
		m_bIsEmbossPresent = rCont.m_bIsEmbossPresent;
		m_bIsEngravePresent = rCont.m_bIsEngravePresent;

		m_oText =rCont.m_oText;

		m_dSpaceWidthMM = rCont.m_dSpaceWidthMM;
		m_bSpaceIsNotNeeded = rCont.m_bSpaceIsNotNeeded;

		m_eVertAlignType = rCont.m_eVertAlignType;

		m_pManager = rCont.m_pManager;

		m_pShape = rCont.m_pShape;
		m_pCont = rCont.m_pCont;

		m_iNumDuplicates = rCont.m_iNumDuplicates;

		return *this;
	}

	void CContText::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		if (m_bIsNotNecessaryToUse)
		{
			return;
		}

		oWriter.WriteString(L"<w:r>");
		oWriter.WriteString(L"<w:rPr>");

		oWriter.WriteString(L"<w:rStyle w:val=\"");
		oWriter.WriteString(m_pFontStyle->wsFontStyleId);
		oWriter.WriteString(L"\"/>");

		LONG lCalculatedSpacing = 0;

		if (!m_pFontStyle->wsFontName.empty() && !m_oText.empty())
		{
			if (m_eVertAlignType != eVertAlignType::vatSubscript &&
					m_eVertAlignType != eVertAlignType::vatSuperscript)
			{
				// нужно перемерять...
				NSStructures::CFont oFont;
				oFont.Name = m_pFontStyle->wsFontName;
				oFont.Bold = m_pFontStyle->bBold;
				oFont.Italic = m_pFontStyle->bItalic;
				oFont.Size = m_pFontStyle->dFontSize;
				m_pManager->LoadFontByName(oFont);

				double dBoxX;
				double dBoxY;
				double dBoxWidth;
				double dBoxHeight;
				m_pManager->MeasureString(m_oText.ToStdWString(), 0, 0, dBoxX, dBoxY, dBoxWidth, dBoxHeight, CFontManager::mtPosition);

				double dSpacing = (m_dWidth - dBoxWidth) / (m_oText.length());
				dSpacing *= c_dMMToDx;

				//mm to points * 20
				lCalculatedSpacing = static_cast<LONG>(dSpacing);
			}
		}

		//note принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
		lCalculatedSpacing -= 6;

		if (lCalculatedSpacing != 0)
		{
			oWriter.WriteString(L"<w:spacing w:val=\"");
			oWriter.AddInt(lCalculatedSpacing);
			oWriter.WriteString(L"\"/>");
		}

		if (m_bIsEmbossPresent)
		{
			oWriter.WriteString(L"<w:emboss/>");
		}
		else if (m_bIsEngravePresent)
		{
			oWriter.WriteString(L"<w:imprint/>");
		}
		else
		{
			if (m_bIsOutlinePresent)
			{
				oWriter.WriteString(L"<w:outline/>");
			}
			if (m_bIsShadowPresent)
			{
				oWriter.WriteString(L"<w:shadow/>");
			}
		}

		if (m_bIsStrikeoutPresent)
		{
			if (m_bIsDoubleStrikeout)
			{
				oWriter.WriteString(L"<w:dstrike/>");
			}
			else
			{
				oWriter.WriteString(L"<w:strike/>");
			}
		}

		if (m_bIsUnderlinePresent)
		{
			oWriter.WriteString(L"<w:u w:val=");
			oWriter.WriteString(SingletonInstance<LinesTable>().ConvertLineToString(m_eUnderlineType));

			if (m_lUnderlineColor != m_pFontStyle->oBrush.Color1)
			{
				oWriter.WriteString(L" w:color=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
				oWriter.WriteString(L"\"");
			}
			oWriter.WriteString(L"/>");
		}

		if (m_bIsHighlightPresent)
		{
			//note В <w:style это не работает
			ColorTable& colorTable = SingletonInstance<ColorTable>();
			if (colorTable.IsStandardColor(m_lHighlightColor))
			{
				oWriter.WriteString(L"<w:highlight w:val=\"");
				oWriter.WriteString(colorTable.ConverColorToString(ConvertColorBGRToRGB(m_lHighlightColor)));
			}
			else
			{
				oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lHighlightColor));
			}
			oWriter.WriteString(L"\"/>");
		}

		if (m_eVertAlignType == eVertAlignType::vatSubscript)
		{
			oWriter.WriteString(L"<w:vertAlign w:val=\"subscript\"/>");
		}
		else if (m_eVertAlignType == eVertAlignType::vatSuperscript)
		{
			oWriter.WriteString(L"<w:vertAlign w:val=\"superscript\"/>");
		}

		oWriter.WriteString(L"</w:rPr>");

		oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
		oWriter.WriteEncodeXmlString(m_oText.ToStdWString());
		oWriter.WriteString(L"</w:t>");

		oWriter.WriteString(L"</w:r>");
	}

	void CContText::AddWideSpaceToXml(double dSpacingMM,
									  NSStringUtils::CStringBuilder& oWriter,
									  bool bIsNeedSaveFormat)
	{
		oWriter.WriteString(L"<w:r><w:rPr>");

		oWriter.WriteString(L"<w:rStyle w:val=\"");
		oWriter.WriteString(m_pFontStyle->wsFontStyleId);
		oWriter.WriteString(L"\"/>");

		double dSpaceMMSize = m_dSpaceWidthMM;
		LONG lCalculatedSpacing = static_cast<LONG>((dSpacingMM - dSpaceMMSize) * c_dMMToDx);

		//note принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
		lCalculatedSpacing -= 1;
		if (lCalculatedSpacing != 0)
		{
			oWriter.WriteString(L"<w:spacing w:val=\"");
			oWriter.AddInt(lCalculatedSpacing);
			oWriter.WriteString(L"\"/>");
		}

		if (m_bIsEmbossPresent && bIsNeedSaveFormat)
		{
			oWriter.WriteString(L"<w:emboss/>");
		}
		else if (m_bIsEngravePresent && bIsNeedSaveFormat)
		{
			oWriter.WriteString(L"<w:imprint/>");
		}
		else
		{
			if (m_bIsOutlinePresent && bIsNeedSaveFormat)
			{
				oWriter.WriteString(L"<w:outline/>");
			}
			if (m_bIsShadowPresent && bIsNeedSaveFormat)
			{
				oWriter.WriteString(L"<w:shadow/>");
			}
		}

		if (m_bIsStrikeoutPresent && bIsNeedSaveFormat)
		{
			if (m_bIsDoubleStrikeout)
			{
				oWriter.WriteString(L"<w:dstrike/>");
			}
			else
			{
				oWriter.WriteString(L"<w:strike/>");
			}
		}

		if (m_bIsUnderlinePresent && bIsNeedSaveFormat)
		{
			oWriter.WriteString(L"<w:u w:val=");
			oWriter.WriteString(SingletonInstance<LinesTable>().ConvertLineToString(m_eUnderlineType));

			if (m_lUnderlineColor != m_pFontStyle->oBrush.Color1)
			{
				oWriter.WriteString(L" w:color=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
				oWriter.WriteString(L"\"");
			}
			oWriter.WriteString(L"/>");
		}

		if (m_bIsHighlightPresent && bIsNeedSaveFormat)
		{
			//note В <w:style это не работает
			ColorTable& colorTable = SingletonInstance<ColorTable>();
			if (colorTable.IsStandardColor(m_lHighlightColor))
			{
				oWriter.WriteString(L"<w:highlight w:val=\"");
				oWriter.WriteString(colorTable.ConverColorToString(ConvertColorBGRToRGB(m_lHighlightColor)));
			}
			else
			{
				oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lHighlightColor));
			}
			oWriter.WriteString(L"\"/>");
		}

		oWriter.WriteString(L"</w:rPr>");

		oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
		oWriter.WriteString(L" ");
		oWriter.WriteString(L"</w:t>");

		oWriter.WriteString(L"</w:r>");
	}

	bool CContText::IsEqual(const CContText *pCont)
	{
		bool bIf1 = m_pFontStyle->wsFontStyleId == pCont->m_pFontStyle->wsFontStyleId;
		bool bIf2 = m_bIsStrikeoutPresent == pCont->m_bIsStrikeoutPresent;
		bool bIf3 = m_bIsDoubleStrikeout == pCont->m_bIsDoubleStrikeout;
		bool bIf4 = m_bIsHighlightPresent == pCont->m_bIsHighlightPresent;
		bool bIf5 = m_lHighlightColor == pCont->m_lHighlightColor;
		bool bIf6 = m_bIsUnderlinePresent == pCont->m_bIsUnderlinePresent;
		bool bIf7 = m_eUnderlineType == pCont->m_eUnderlineType;
		bool bIf8 = m_lUnderlineColor == pCont->m_lUnderlineColor;
		bool bIf9 = m_bIsShadowPresent == pCont->m_bIsShadowPresent;
		bool bIf10 = m_bIsOutlinePresent == pCont->m_bIsOutlinePresent;
		bool bIf11 = m_bIsEmbossPresent == pCont->m_bIsEmbossPresent;
		bool bIf12 = m_bIsEngravePresent == pCont->m_bIsEngravePresent;
		bool bIf13 = m_eVertAlignType == pCont->m_eVertAlignType;
		bool bIf14 = m_eVertAlignType == eVertAlignType::vatUnknown && pCont->m_eVertAlignType == eVertAlignType::vatBase;
		bool bIf15 = m_eVertAlignType == eVertAlignType::vatBase && pCont->m_eVertAlignType == eVertAlignType::vatUnknown;

		return (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 && bIf7 &&
				bIf8 && bIf9 && bIf10 && bIf11 && bIf12 && (bIf13 || bIf14 || bIf15));
	}

	UINT CContText::GetNumberOfFeatures()
	{
		UINT ret = 0;
		if (m_pFontStyle->bBold)
		{
			ret++;
		}
		if (m_pFontStyle->bItalic)
		{
			ret++;
		}
		if (m_bIsStrikeoutPresent)
		{
			ret++;
		}
		if (m_bIsDoubleStrikeout)
		{
			ret++;
		}
		if (m_bIsHighlightPresent)
		{
			ret++;
		}
		if (m_bIsUnderlinePresent)
		{
			ret++;
		}
		if (m_eVertAlignType != eVertAlignType::vatUnknown)
		{
			ret++;
		}

		return ret;
	}

	bool CContText::IsDuplicate(CContText* pCont, eVerticalCrossingType eVType)
	{
		if (eVType == eVerticalCrossingType::vctDublicate &&
				m_oText == pCont->m_oText)
		{
			pCont->m_bIsNotNecessaryToUse = true;
			m_iNumDuplicates++;
			return true;
		}
		return false;
	}

	bool CContText::IsThereAreFontEffects(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType)
	{
		//Условие пересечения по вертикали
		bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext; //текущий cont выше
		bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext; //текущий cont ниже

		//Условие пересечения по горизонтали
		bool bIf3 = eHType == eHorizontalCrossingType::hctCurrentLeftOfNext; //текущий cont левее
		bool bIf4 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext; //текущий cont правее

		//Размеры шрифта и текст должны бать одинаковыми
		bool bIf5 = m_pFontStyle->dFontSize == m_pFontStyle->dFontSize;
		bool bIf6 = m_oText == pCont->m_oText;

		//Цвет тени должен быть серым
		bool bIf7 = m_pFontStyle->oBrush.Color1 == c_iGreyColor;
		bool bIf8 = pCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor;
		bool bIf9 = m_pFontStyle->oBrush.Color1 == c_iBlackColor;
		bool bIf10 = pCont->m_pFontStyle->oBrush.Color1 == c_iBlackColor;
		bool bIf11 = m_pFontStyle->oBrush.Color1 == c_iGreyColor2;
		bool bIf12 = pCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor2;

		//note Каждый символ с Emboss или Engrave разбиваются на 3 символа с разными цветами
		//note Логика подобрана для конкретного примера - возможно нужно будет ее обобщить.
		//todo существует проблема неправильного определением FontEffects с физически пересекаемыми строчками - файл generaltest.pdf p.14
		if (bIf5 && bIf6)
		{
			if (m_bIsEmbossPresent && bIf12)
			{
				if (bIf1 && bIf3)
				{
					m_bIsEmbossPresent = true;
					pCont->m_bIsNotNecessaryToUse = true;
					return true;
				}
			}

			if (m_bIsEngravePresent && bIf10)
			{
				if (bIf1 && bIf3)
				{
					m_bIsEngravePresent = true;
					pCont->m_bIsNotNecessaryToUse = true;
					return true;
				}
			}

			//Shadow
			if (bIf1 && bIf3 && bIf8)
			{
				m_bIsShadowPresent = true;
				pCont->m_bIsNotNecessaryToUse = true;
				return true;
			}
			else if (bIf2 && bIf4 && bIf7)
			{
				pCont->m_bIsShadowPresent = true;
				m_bIsNotNecessaryToUse = true;
				return true;
			}

			//Emboss
			//Первый проход
			//c_iBlackColor -> c_iBlackColor -> c_iGreyColor2
			else if (bIf1 && bIf3 && bIf9)
			{
				pCont->m_bIsEmbossPresent = true;
				m_bIsNotNecessaryToUse = true;
				return true;
			}
			//Engrave
			else if (bIf1 && bIf3 && bIf11)
			{
				pCont->m_bIsEngravePresent = true;
				m_bIsNotNecessaryToUse = true;
				return true;
			}
		}
		return false;
	}

	bool CContText::IsVertAlignTypeBetweenConts(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType)
	{
		//Условие пересечения по вертикали
		bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext ||
				eVType == eVerticalCrossingType::vctCurrentInsideNext;
		bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext;
		//Условие пересечения по горизонтали
		bool bIf3 = (eHType == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
					 eHType == eHorizontalCrossingType::hctCurrentLeftOfNext) &&
				fabs(m_dRight - pCont->m_dLeft) < c_dTHE_STRING_X_PRECISION_MM * 3;
		bool bIf4 = (eHType == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext ||
					 eHType == eHorizontalCrossingType::hctCurrentRightOfNext) &&
				fabs(m_dLeft - pCont->m_dRight) < c_dTHE_STRING_X_PRECISION_MM * 3;
		//Размеры шрифта должны бать разными
		bool bIf5 = m_pFontStyle->dFontSize * 0.7 > pCont->m_pFontStyle->dFontSize;
		bool bIf6 = m_pFontStyle->dFontSize <  pCont->m_pFontStyle->dFontSize * 0.7;

		if (bIf3 || bIf4)
		{
			if (bIf1 && bIf5)
			{
				pCont->m_eVertAlignType = eVertAlignType::vatSubscript;
				pCont->m_pCont = this;
				m_eVertAlignType = eVertAlignType::vatBase;
				m_pCont = pCont;
				return true;
			}
			else if (bIf2 && bIf5)
			{
				pCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
				pCont->m_pCont = this;
				m_eVertAlignType = eVertAlignType::vatBase;
				m_pCont = pCont;
				return true;
			}
			else if (bIf1 && bIf6)
			{
				m_eVertAlignType = eVertAlignType::vatSuperscript;
				m_pCont = pCont;
				pCont->m_eVertAlignType = eVertAlignType::vatBase;
				pCont->m_pCont = this;
				return true;
			}
			else if (bIf2 && bIf6)
			{
				m_eVertAlignType = eVertAlignType::vatSubscript;
				m_pCont = pCont;
				pCont->m_eVertAlignType = eVertAlignType::vatBase;
				pCont->m_pCont = this;
				return true;
			}
		}
		return false;
	}

	double CContText::CalculateWideSpace()
	{
		//note подобранное условие - не везде хорошо работает
		return m_dSpaceWidthMM * 2.5;
	}

	double CContText::CalculateThinSpace()
	{
		//note подобранное условие - не везде хорошо работает
		return m_dSpaceWidthMM * 0.25;
	}
}
