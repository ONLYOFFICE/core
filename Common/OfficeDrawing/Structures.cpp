#pragma once
#include "stdafx.h"
#include "ElementsContainer.h"

using namespace NSAttributes;

// PFRun -------------------------------------------------------------------------
void STextPFRun::ReadFromStream(IStream* pStream)
{
	if (bIsExt)
	{
		lCount = StreamUtils::ReadDWORD(pStream);
		lIndentLevel = StreamUtils::ReadWORD(pStream);
	}

	DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
	BYTE flag1 = (BYTE)(dwFlags);
	BYTE flag2 = (BYTE)(dwFlags >> 8);
	BYTE flag3 = (BYTE)(dwFlags >> 16);
	BYTE flag4 = (BYTE)(dwFlags >> 24);

	hasBullet_				= (0x01 == (0x01 & flag1));
	bulletHasFont_			= (0x02 == (0x02 & flag1));
	bulletHasColor_			= (0x04 == (0x04 & flag1));
	bulletHasSize_			= (0x08 == (0x08 & flag1));
	bulletFont_				= (0x10 == (0x10 & flag1));
	bulletColor_			= (0x20 == (0x20 & flag1));
	bulletSize_				= (0x40 == (0x40 & flag1));
	bulletChar_				= (0x80 == (0x80 & flag1));

	leftMargin_				= (0x01 == (0x01 & flag2));
	// reserved
	indent_					= (0x04 == (0x04 & flag2));
	align_					= (0x08 == (0x08 & flag2));
	lineSpacing_			= (0x10 == (0x10 & flag2));
	spaceBefore_			= (0x20 == (0x20 & flag2));
	spaceAfter_				= (0x40 == (0x40 & flag2));
	defaultTabSize_			= (0x80 == (0x80 & flag2));

	fontAlign_				= (0x01 == (0x01 & flag3));
	charWrap_				= (0x02 == (0x02 & flag3));
	wordWrap_				= (0x04 == (0x04 & flag3));
	overflow_				= (0x08 == (0x08 & flag3));
	tabStops_				= (0x10 == (0x10 & flag3));
	textDirection_			= (0x20 == (0x20 & flag3));
	// reserved
	bulletBlip_				= (0x80 == (0x80 & flag3));

	bulletScheme_			= (0x01 == (0x01 & flag4));
	bulletHasScheme_		= (0x02 == (0x02 & flag4));

	if (hasBullet_ || bulletHasFont_ || bulletHasColor_ || bulletHasSize_)
		bulletFlag = StreamUtils::ReadWORD(pStream);

    if (bulletChar_)
        bulletChar = (WCHAR)StreamUtils::ReadWORD(pStream);

    if (bulletFont_)
        bulletFontRef = StreamUtils::ReadWORD(pStream);

	if (bulletSize_)
        bulletSize = StreamUtils::ReadWORD(pStream);

	if (bulletColor_)
	{
		bulletColor.FromStream(pStream);
	}

    if (align_)
	{
		textAlignment = StreamUtils::ReadWORD(pStream);
	}

    if (lineSpacing_)
	{
		lineSpacing = StreamUtils::ReadSHORT(pStream);
	}

    if (spaceBefore_)
	{
		spaceBefore = StreamUtils::ReadSHORT(pStream);
	}

    if (spaceAfter_)
	{
		spaceAfter = StreamUtils::ReadSHORT(pStream);
	}

    if (leftMargin_)
        leftMargin = StreamUtils::ReadSHORT(pStream);

    if (indent_)
        indent = StreamUtils::ReadSHORT(pStream);

    if (defaultTabSize_)
        defaultTabSize = StreamUtils::ReadWORD(pStream);

	if (tabStops_)
    {
        WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
		tabsStops.RemoveAll();

		// 
		if (tabStopsCount > 10)
			tabStopsCount = 10;

        for (int i = 0; i < (int)tabStopsCount; ++i)
        {
			tabsStops.Add(StreamUtils::ReadDWORD(pStream));
        }

		/*if (tabStopsCount > 0)
		{
			defaultTabSize_	= true;
			defaultTabSize	= tabsStops[0]; 
		}*/
    }

    if (fontAlign_)
        fontAlign = StreamUtils::ReadWORD(pStream);
	
	if (charWrap_ || wordWrap_ || overflow_)
        wrapFlags = StreamUtils::ReadWORD(pStream);

    if (textDirection_)
        textDirectional = StreamUtils::ReadWORD(pStream);
}
void STextPFRun::ApplyProperties(CElementsContainer* pSlide, CTextAttributesEx* pTextAttributes)
{
	if (NULL == pTextAttributes)
		return;

	if (align_)
	{
		switch (textAlignment)
		{
		case 0:
		case 1:
		case 2:
			{
				pTextAttributes->m_oAttributes.m_nTextAlignHorizontal = textAlignment;
				break;
			}
		default: break;
		};
	}
}
// -------------------------------------------------------------------------------
// CFRun -------------------------------------------------------------------------
void STextCFRun::ReadFromStream(IStream* pStream)
{
	if (bIsExt)
	{
		lCount = StreamUtils::ReadDWORD(pStream);
	}

	DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
	BYTE flag1 = (BYTE)(dwFlags);
	BYTE flag2 = (BYTE)(dwFlags >> 8);
	BYTE flag3 = (BYTE)(dwFlags >> 16);
	BYTE flag4 = (BYTE)(dwFlags >> 24);

	hasBold					= (0x01 == (0x01 & flag1));
	hasItalic				= (0x02 == (0x02 & flag1));
	hasUnderline			= (0x04 == (0x04 & flag1));
	// unused
	hasShadow				= (0x10 == (0x10 & flag1));
	hasFehint				= (0x20 == (0x20 & flag1));
	// unused
	hasKimi					= (0x80 == (0x80 & flag1));

	// unused
	hasEmboss				= (0x02 == (0x02 & flag2));
	// unused
	hasStyle				= ((0x3C & flag2) >> 2);
	// unused

	hasTypeface				= (0x01 == (0x01 & flag3));
	hasSize					= (0x02 == (0x02 & flag3));
	hasColor				= (0x04 == (0x04 & flag3));
	hasPosition				= (0x08 == (0x08 & flag3));
	hasPp10ext				= (0x10 == (0x10 & flag3));
	hasOldEATypeface		= (0x20 == (0x20 & flag3));
	hasAnsiTypeface			= (0x40 == (0x40 & flag3));
	hasSymbolTypeface		= (0x80 == (0x80 & flag3));

	hasNewEATypeface		= (0x01 == (0x01 & flag4));
	hasCsTypeface			= (0x02 == (0x02 & flag4));
	hasPp11ext				= (0x04 == (0x04 & flag4));

	bool bIsFontStylePresent = (hasBold || hasItalic || hasUnderline || hasShadow || 
		hasFehint || hasKimi || hasEmboss || hasStyle != 0);
	
    if (bIsFontStylePresent)
	{
		fontStyle = StreamUtils::ReadWORD(pStream);
	}

    if (hasTypeface)
	{
		fontRef = StreamUtils::ReadWORD(pStream);
	}

	if (hasOldEATypeface)
		fontEAFontRef = StreamUtils::ReadWORD(pStream);

	if (hasAnsiTypeface)
		ansiFontRef = StreamUtils::ReadWORD(pStream);

	if (hasSymbolTypeface)
		symbolFontRef = StreamUtils::ReadWORD(pStream);
	
	if (hasSize)
	{
		fontSize = StreamUtils::ReadWORD(pStream);
	}

	if (hasColor)
	{
		oColor.FromStream(pStream);
	}

	if (hasPosition)
		position = StreamUtils::ReadWORD(pStream);

	// или два последних наоборот????
}
void STextCFRun::ApplyProperties(CElementsContainer* pSlide, CTextAttributesEx* pTextAttributes)
{
	if (NULL == pTextAttributes)
		return;

	bool bIsFontStylePresent = (hasBold || hasItalic || hasUnderline || hasShadow || 
		hasFehint || hasKimi || hasEmboss || hasStyle != 0);
	
    if (bIsFontStylePresent)
	{
		if (hasBold)
		{
			pTextAttributes->m_oAttributes.m_oFont.m_bBold = (0x01 == (0x01 & fontStyle));
		}
		if (hasItalic)
		{
			pTextAttributes->m_oAttributes.m_oFont.m_bItalic = (0x02 == (0x02 & fontStyle));
		}
		if (hasUnderline)
		{
			pTextAttributes->m_oAttributes.m_oFont.m_bUnderline = (0x04 == (0x04 & fontStyle));
		}
		if (hasShadow)
		{
			pTextAttributes->m_oAttributes.m_oTextShadow.m_bVisible = (0x10 == (0x10 & fontStyle));
		}
	}

    if (hasTypeface)
	{
		if (NULL != pSlide && NULL != pSlide->m_pFonts)
		{
			if (fontRef < pSlide->m_pFonts->GetSize())
			{
				pTextAttributes->m_oAttributes.m_oFont.m_strFontName = (*(pSlide->m_pFonts))[fontRef].m_strFontName;
				
				// font name
				strFontName = (*(pSlide->m_pFonts))[fontRef].m_strFontName;

				// charset
				fontCharset.RemoveAll();
				fontCharset.Add((*(pSlide->m_pFonts))[fontRef].m_lCharset);

				// pitchfamily
				fontPitchFamily = (*(pSlide->m_pFonts))[fontRef].m_strPitchFamily;

				// font panose
				fontPanose		= _T("");
				// font fixed
				fontFixed		= -1;
			}
		}
	}

	if (hasSize)
	{
		pTextAttributes->m_oAttributes.m_oFont.m_nSize = (int)fontSize;
	}

	if (hasColor)
	{
		if (0 <= oColor.Index && oColor.Index <= 7 && NULL != pSlide)
		{
			// цвет из палитры...
			if (oColor.Index < pSlide->m_arColorScheme.GetSize())
			{
				oColor = pSlide->m_arColorScheme[oColor.Index];
			}
		}
		
		oColor.ToColor(&(pTextAttributes->m_oAttributes.m_oTextBrush.m_oColor1));
	}
}
// -------------------------------------------------------------------------------
// TextRuler ---------------------------------------------------------------------
void STextRuler::ApplyProperties(CElementsContainer* pSlide, CTextAttributesEx* pTextAttributes)
{
	pTextAttributes->m_oRuler = *this;
}
void STextRuler::CorrectRuler(CTextAttributesEx* pTextAttributes)
{
	if (NULL == pTextAttributes)
		return;

	size_t lCount = pTextAttributes->m_arPFs.GetCount();
	for (size_t i = 0; i < lCount; ++i)
	{
		WORD lIndentLevel = pTextAttributes->m_arPFs[i].lIndentLevel;
		switch (lIndentLevel)
		{
		case 0:
		{
			if (bLeftMargin1) 
				pTextAttributes->m_arPFs[i].leftMargin	= LeftMargin1;
			if (bIndent1) 
			{
				pTextAttributes->m_arPFs[i].indent		= Indent1;
			}

			pTextAttributes->m_arPFs[i].indent			-= pTextAttributes->m_arPFs[i].leftMargin;
			break;
		}
		case 1:
		{
			if (bLeftMargin2) 
				pTextAttributes->m_arPFs[i].leftMargin	= LeftMargin2;
			if (bIndent2) 
			{
				pTextAttributes->m_arPFs[i].indent		= Indent2;
			}
			
			pTextAttributes->m_arPFs[i].indent		-= pTextAttributes->m_arPFs[i].leftMargin;

			break;
		}
		case 2:
		{
			if (bLeftMargin3) 
				pTextAttributes->m_arPFs[i].leftMargin	= LeftMargin3;
			if (bIndent3) 
			{
				pTextAttributes->m_arPFs[i].indent		= Indent3;
			}

			pTextAttributes->m_arPFs[i].indent		-= pTextAttributes->m_arPFs[i].leftMargin;
			break;
		}
		case 3:
		{
			if (bLeftMargin4) 
				pTextAttributes->m_arPFs[i].leftMargin	= LeftMargin4;
			if (bIndent4) 
			{
				pTextAttributes->m_arPFs[i].indent		= Indent4;
				
			}
			pTextAttributes->m_arPFs[i].indent		-= pTextAttributes->m_arPFs[i].leftMargin;

			break;
		}
		case 4:
		{
			if (bLeftMargin5) 
				pTextAttributes->m_arPFs[i].leftMargin	= LeftMargin5;
			if (bIndent5) 
			{
				pTextAttributes->m_arPFs[i].indent		= Indent5;
				
			}
			
			pTextAttributes->m_arPFs[i].indent		-= pTextAttributes->m_arPFs[i].leftMargin;

			break;
		}
		default:
			break;
		};
	}
}
// -------------------------------------------------------------------------------
// SIRuler -----------------------------------------------------------------------
void STextSIRun::ReadFromStream(IStream* pStream)
{
	if (bIsExt)
	{
		lCount = StreamUtils::ReadDWORD(pStream);
	}
	
	DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
	BYTE flag1 = (BYTE)(dwFlags);
	BYTE flag2 = (BYTE)(dwFlags >> 8);

	bSpell					= (0x01 == (0x01 & flag1));
	bLang					= (0x02 == (0x02 & flag1));
	bAltLang				= (0x04 == (0x04 & flag1));
	// unused
	// unused		
	bPp10ext				= (0x20 == (0x20 & flag1));
	bBidi					= (0x40 == (0x40 & flag1));
	// unused
	// reserved
	bSmartTag				= (0x02 == (0x02 & flag2));

	if (bSpell)
	{
		Spell = StreamUtils::ReadWORD(pStream);
	}
	if (bLang)
	{
		Lang = StreamUtils::ReadWORD(pStream);
	}
	if (bAltLang)
	{
		AltLang = StreamUtils::ReadWORD(pStream);
	}
	if (bBidi)
	{
		Bidi = StreamUtils::ReadWORD(pStream);
	}
	if (bPp10ext)
	{
		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		pp10runid = (0x0F & flag1);
		bGramma = (0x80 == (0x80 & flag4));
	}

	/*if (bSmartTag)
    {
        DWORD tabStopsCount = StreamUtils::ReadDWORD(pStream);
		arSmartTags.RemoveAll();

        for (int i = 0; i < (int)tabStopsCount; ++i)
        {
			arSmartTags.Add(StreamUtils::ReadDWORD(pStream));
        }
    }*/
}
void STextSIRun::ApplyProperties(CElementsContainer* pSlide, CTextAttributesEx* pTextAttributes)
{
	if (NULL == pTextAttributes)
		return;
}
// -------------------------------------------------------------------------------
// TextMasterStyleLevel ----------------------------------------------------------
void STextMasterStyleLevel::ReadFromStream(IStream* pStream)
{
	if (bLevelPresent)
	{
		cLevel = StreamUtils::ReadWORD(pStream);
	}
	oPFRun.ReadFromStream(pStream);
	oCFRun.ReadFromStream(pStream);
}
void STextMasterStyleLevel::ApplyProperties(CElementsContainer* pSlide, CTextAttributesEx* pTextAttributes)
{
	oPFRun.ApplyProperties(pSlide, pTextAttributes);
	oCFRun.ApplyProperties(pSlide, pTextAttributes);
}
// -------------------------------------------------------------------------------
// TextAttributesEx --------------------------------------------------------------
CTextAttributesEx::CTextAttributesEx() :
			m_oAttributes(),
			m_arPFs(),
			m_arCFs(),
			m_arSIs(),
			m_oRuler()
{
	m_sText = _T("");

	m_lMasterTextType = NSOfficePPT::NoPresent;
	m_lTextType = NSOfficePPT::Other;
			
	m_oBounds.left = 0;
	m_oBounds.top = 0;
	m_oBounds.right = 50;
	m_oBounds.bottom = 50;

	m_bVertical = FALSE;
	m_bRightToLeft = FALSE;

	m_lWrapMode	= 0;
}

CTextAttributesEx& CTextAttributesEx::operator =(const CTextAttributesEx& oSrc)
{
	m_sText = oSrc.m_sText;
	m_oBounds = oSrc.m_oBounds;

	m_lMasterTextType = oSrc.m_lMasterTextType;
	m_lTextType = oSrc.m_lTextType;

	m_oAttributes	= oSrc.m_oAttributes;
	m_bVertical		= oSrc.m_bVertical;
	m_bRightToLeft	= oSrc.m_bRightToLeft;

	m_arPFs.Copy(oSrc.m_arPFs);
	m_arCFs.Copy(oSrc.m_arCFs);
	m_arSIs.Copy(oSrc.m_arSIs);
	m_oRuler = oSrc.m_oRuler;

	m_lWrapMode = oSrc.m_lWrapMode;

	return *this;
}

CTextAttributesEx::CTextAttributesEx(const CTextAttributesEx& oSrc)
{
	*this = oSrc;
}

CString CTextAttributesEx::ToString(CGeomShapeInfo& oInfo, CMetricInfo& pMetricInfo, double dStartTime, double dEndTime)
{
	m_oBounds.left		= (LONG)oInfo.m_dLeft;
	m_oBounds.top		= (LONG)oInfo.m_dTop;
	m_oBounds.right		= (LONG)(oInfo.m_dLeft + oInfo.m_dWidth);
	m_oBounds.bottom	= (LONG)(oInfo.m_dTop + oInfo.m_dHeight);

	double dRight		= oInfo.m_dLeft + oInfo.m_dWidth;
	double dLeft		= oInfo.m_dLeft;
	if (2 == m_lWrapMode)
	{
		LONG lAlign = 0;
		if (m_arPFs.GetCount() > 0)
			lAlign = m_arPFs[0].textAlignment;
		else
			lAlign = m_oAttributes.m_nTextAlignHorizontal;
		
		switch (lAlign)
		{
		case 1:
			{
				// center
				dLeft	-= pMetricInfo.m_lMillimetresHor;
				dRight	+= pMetricInfo.m_lMillimetresHor;
				break;
			}
		default:
			{
				dRight	= pMetricInfo.m_lMillimetresHor;
				break;
			}
		};
	}

	CString strText = _T("");
    // add Text Param
	strText.Format(_T("<ImagePaint-DrawTextEx left='%lf' top='%lf' right='%lf' bottom='%lf' angle='%lf' flags='%d' "), 
		dLeft, oInfo.m_dTop, dRight, oInfo.m_dTop + oInfo.m_dHeight, oInfo.m_dRotate, oInfo.GetFlags());

	// вот здесь нужно разрулить пререход на новую строку	
	m_sText.Replace((WCHAR)(11), '\n');

	RecalcParagraphs();
	
	// теперь разберемся с хмл
	m_sText.Replace(L"&",	L"&amp;");
	m_sText.Replace(L"'",	L"&apos;");
	m_sText.Replace(L"<",	L"&lt;");
	m_sText.Replace(L">",	L"&gt;");
	m_sText.Replace(L"\"",	L"&quot;");
	//m_sText.Replace(L"&",	L"&amp;");
	
	strText += (_T("text='") + StreamUtils::ConvertCStringWToCString(m_sText) + _T("' "));

	strText += (_T("vertical='") + NSAttributes::BoolToString(m_bVertical == TRUE) + _T("' "));
	strText += (_T("righttoleft='") + NSAttributes::BoolToString(m_bRightToLeft == TRUE) + _T("' "));

	strText += (_T("widthmm='") + CDirectory::ToString(pMetricInfo.m_lMillimetresHor) + _T("' "));
	strText += (_T("heightmm='") + CDirectory::ToString(pMetricInfo.m_lMillimetresVer) + _T("' "));
	strText += (_T("widthl='") + CDirectory::ToString(pMetricInfo.m_lUnitsHor) + _T("' "));
	strText += (_T("heightl='") + CDirectory::ToString(pMetricInfo.m_lUnitsVer) + _T("' "));

	strText += _T(">");

    strText += m_oAttributes.ToString();

#ifdef PPT_DEF
	// теперь пробежимся и проставим все маргины и инденты
	m_oRuler.CorrectRuler(this);
#endif
	for (size_t nIndex = 0; nIndex < m_arPFs.GetCount(); ++nIndex)
	{
		strText += m_arPFs[nIndex].ToString();
	}
	for (size_t nIndex = 0; nIndex < m_arCFs.GetCount(); ++nIndex)
	{
		strText += m_arCFs[nIndex].ToString2();
	}
	//for (size_t nIndex = 0; nIndex < m_arSIs.GetCount(); ++nIndex)
	//{
	//	strText += m_arSIs[nIndex].ToString();
	//}
	//strText += m_oRuler.ToString(); 
	
	CString strTimeLine = _T("");
	strTimeLine.Format(_T("<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0'/>"),
			dStartTime, dEndTime);

	return strText + strTimeLine + _T("</ImagePaint-DrawTextEx>");
}
void CTextAttributesEx::ApplyProperties(CElementsContainer* pContainer)
{
	if (NULL == pContainer)
		return;

	size_t nCount = 0;

	nCount = m_arPFs.GetCount();
	for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
	{
		if (m_arPFs[nIndex].bulletColor_)
		{
			int nColorIndex = m_arPFs[nIndex].bulletColor.Index;
			if (0 <= nColorIndex && nColorIndex <= 7)
			{
				// цвет из палитры...
				if (nColorIndex < pContainer->m_arColorScheme.GetSize())
				{
					m_arPFs[nIndex].bulletColor = pContainer->m_arColorScheme[nColorIndex];
				}
			}
		}
	}

	nCount = m_arCFs.GetCount();
	for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
	{
		if (m_arCFs[nIndex].hasColor)
		{
			int nColorIndex = m_arCFs[nIndex].oColor.Index;
			if (0 <= nColorIndex && nColorIndex <= 7)
			{
				// цвет из палитры...
				if (nColorIndex < pContainer->m_arColorScheme.GetSize())
				{
					m_arCFs[nIndex].oColor = pContainer->m_arColorScheme[nColorIndex];
				}
			}
		}

		if (m_arCFs[nIndex].hasTypeface)
		{
			if (NULL != pContainer->m_pFonts)
			{
				if (m_arCFs[nIndex].fontRef < pContainer->m_pFonts->GetSize())
				{
					CFont_* pFont = &(*(pContainer->m_pFonts))[m_arCFs[nIndex].fontRef];
					
					// font name
					m_arCFs[nIndex].strFontName = pFont->m_strFontName;

					// charset
					m_arCFs[nIndex].fontCharset.RemoveAll();
					m_arCFs[nIndex].fontCharset.Add(pFont->m_lCharset);

					// pitchfamily
					m_arCFs[nIndex].fontPitchFamily = pFont->m_strPitchFamily;

					// font panose
					m_arCFs[nIndex].fontPanose		= _T("");
					// font fixed
					m_arCFs[nIndex].fontFixed		= -1;
				}
				/*if (m_arCFs[nIndex].fontEAFontRef < pContainer->m_pFonts->GetSize())
				{
					m_arCFs[nIndex].strFontName = (*(pContainer->m_pFonts))[m_arCFs[nIndex].fontEAFontRef].m_strFontName;
				}*/
			}
		}
	}
}

LONG CTextAttributesEx::GetIndentLevelCF(size_t nIndex)
{
	if ((0 > nIndex) || (nIndex >= m_arCFs.GetCount()))
		return 0;

	DWORD lStartCharacter = 0;
	for (size_t i = 0; i < nIndex; ++i)
	{
		lStartCharacter += m_arCFs[i].lCount;
	}

	size_t lCountPFs = m_arPFs.GetCount();
	DWORD lStartParagraph = 0;
	for (size_t i = 0; i < lCountPFs; ++i)
	{
		if ((lStartParagraph + m_arPFs[i].lCount) > lStartCharacter)
			return (LONG)m_arPFs[i].lIndentLevel;

		lStartParagraph += m_arPFs[i].lCount;
	}

	return 0;
}

#ifdef PPT_DEF
void CTextAttributesEx::RecalcParagraphs()
{
	// здесь разбиваем параграф, если в нем есть символ \n
	LONG nCurIndex = 0;
	CAtlArray<STextPFRun> oArray;

	//m_sText.Replace(TCHAR(10), TCHAR(13));

	CAtlArray<TCHAR> oArrayDelimeters;
	oArrayDelimeters.Add(TCHAR(13));
	size_t lCountDelimeters = oArrayDelimeters.GetCount();

	
	for (size_t i = 0; i < m_arPFs.GetCount(); ++i)
	{
		CString strText = m_sText.Mid(nCurIndex, m_arPFs[i].lCount);
		
		LONG lTextLen	= strText.GetLength();
		LONG lTextBegin	= 0;

		for (LONG lIndex = 0; lIndex < lTextLen; ++lIndex)
		{
			if (lIndex == (lTextLen - 1))
			{
				// значит здесь делиметр
				// но его может не быть! тогда просто формируем еще один параграф
				
				// проверяем, встретился ли делиметр
				size_t lIndexDel = 0;
				for (; lIndexDel < lCountDelimeters; ++lIndexDel)
				{
					if (strText.GetAt(lIndex) == oArrayDelimeters[lIndexDel])
					{
						oArray.Add(m_arPFs[i]);

						DWORD dwLen = (DWORD)(lIndex - lTextBegin + 1);
						
						oArray[oArray.GetCount() - 1].lCount = dwLen;
						if (1 == dwLen)
							oArray[oArray.GetCount() - 1].bulletFlag = 0;

						lTextBegin = lIndex + 1;
						break;
					}
				}
				if (lIndexDel == lCountDelimeters)
				{
					if ('\n' != strText.GetAt(lIndex))
					{					
						oArray.Add(m_arPFs[i]);
						oArray[oArray.GetCount() - 1].lCount = (DWORD)(lIndex - lTextBegin + 1);
					}
					else
					{
						oArray.Add(m_arPFs[i]);
						oArray[oArray.GetCount() - 1].lCount = (DWORD)(lIndex - lTextBegin);

						oArray.Add(m_arPFs[i]);
						oArray[oArray.GetCount() - 1].lCount = 1;
						oArray[oArray.GetCount() - 1].bulletFlag = 0;

						m_sText.SetAt(lIndex, ' ');
					}
					break;
				}
			}
			
			// проверяем, встретился ли делиметр
			for (size_t lIndexDel = 0; lIndexDel < lCountDelimeters; ++lIndexDel)
			{
				if (strText.GetAt(lIndex) == oArrayDelimeters[lIndexDel])
				{
					oArray.Add(m_arPFs[i]);

					DWORD dwLen = (DWORD)(lIndex - lTextBegin + 1);
					
					oArray[oArray.GetCount() - 1].lCount = dwLen;
					if (1 == dwLen)
					{
						oArray[oArray.GetCount() - 1].bulletFlag = 0;
					}
					else if((0 == dwLen) && (lIndex == (lTextLen - 1)) && (i == (m_arPFs.GetCount() - 1)))
					{
						oArray[oArray.GetCount() - 1].lCount = 1;
						oArray[oArray.GetCount() - 1].bulletFlag = 0;
						m_sText += _T(" ");
					}

					lTextBegin = lIndex + 1;
					break;
				}
			}
		}

		nCurIndex += m_arPFs[i].lCount;
	}

	m_sText.Replace(TCHAR(10), TCHAR(13));

	m_arPFs.RemoveAll();
	m_arPFs.Copy(oArray);
}
#else
void CTextAttributesEx::RecalcParagraphs()
{
	// здесь разбиваем параграф, если в нем есть символ \n
	LONG nCurIndex = 0;
	CAtlArray<STextPFRun> oArray;

	CAtlArray<TCHAR> oArrayDelimeters;
	oArrayDelimeters.Add(TCHAR(13));
	size_t lCountDelimeters = oArrayDelimeters.GetCount();
	
	for (size_t i = 0; i < m_arPFs.GetCount(); ++i)
	{
		CString strText = m_sText.Mid(nCurIndex, m_arPFs[i].lCount);
		
		LONG lTextLen	= strText.GetLength();
		LONG lTextBegin	= 0;

		for (LONG lIndex = 0; lIndex < lTextLen; ++lIndex)
		{
			if (lIndex == (lTextLen - 1))
			{
				// значит здесь делиметр
				// но его может не быть! тогда просто формируем еще один параграф
				
				// проверяем, встретился ли делиметр
				size_t lIndexDel = 0;
				for (; lIndexDel < lCountDelimeters; ++lIndexDel)
				{
					if (strText.GetAt(lIndex) == oArrayDelimeters[lIndexDel])
					{
						oArray.Add(m_arPFs[i]);

						DWORD dwLen = (DWORD)(lIndex - lTextBegin + 1);
						
						oArray[oArray.GetCount() - 1].lCount = dwLen;
						if (1 == dwLen)
							oArray[oArray.GetCount() - 1].bulletFlag = 0;

						lTextBegin = lIndex + 1;
						break;
					}
				}
				if (lIndexDel == lCountDelimeters)
				{
					oArray.Add(m_arPFs[i]);
					oArray[oArray.GetCount() - 1].lCount = (DWORD)(lIndex - lTextBegin + 1);
					break;
				}
			}
			
			// проверяем, встретился ли делиметр
			for (size_t lIndexDel = 0; lIndexDel < lCountDelimeters; ++lIndexDel)
			{
				if (strText.GetAt(lIndex) == oArrayDelimeters[lIndexDel])
				{
					oArray.Add(m_arPFs[i]);

					DWORD dwLen = (DWORD)(lIndex - lTextBegin + 1);
					
					oArray[oArray.GetCount() - 1].lCount = dwLen;
					if (1 == dwLen)
						oArray[oArray.GetCount() - 1].bulletFlag = 0;

					lTextBegin = lIndex + 1;
					break;
				}
			}
		}

		nCurIndex += m_arPFs[i].lCount;
	}

	m_arPFs.RemoveAll();
	m_arPFs.Copy(oArray);
}
#endif
// -------------------------------------------------------------------------------
