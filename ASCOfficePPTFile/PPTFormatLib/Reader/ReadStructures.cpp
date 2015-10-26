#pragma once

//#include "../../../ASCPresentationEditor/OfficeDrawing/TextAttributesEx.h"
#include "ReadStructures.h"

#include <zlib.h>


using namespace NSPresentationEditor;


namespace NSZLib
{
	bool Decompress(const BYTE* pSrcBuffer, const ULONG& lSrcBufferLen,
								BYTE* pDstBuffer, ULONG& lDstBufferLen)
	{
		try
		{
			if (Z_OK == uncompress(pDstBuffer, &lDstBufferLen, pSrcBuffer, lSrcBufferLen))
			{
                return true;
			}
		}
		catch(...)
		{
		}
        return false;
	}
}

namespace NSStreamReader
{
	void Read(POLE::Stream* pStream, NSPresentationEditor::CTextSIRun& oRun, bool bIsIndentation)
	{
		if (bIsIndentation)
		{
			oRun.lCount = StreamUtils::ReadDWORD(pStream);
		}
		
		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);

		oRun.bSpell					= (0x01 == (0x01 & flag1));
		oRun.bLang					= (0x02 == (0x02 & flag1));
		oRun.bAltLang				= (0x04 == (0x04 & flag1));
		// unused
		// unused		
		oRun.bPp10ext				= (0x20 == (0x20 & flag1));
		oRun.bBidi					= (0x40 == (0x40 & flag1));
		// unused
		// reserved
		oRun.bSmartTag				= (0x02 == (0x02 & flag2));

		if (oRun.bSpell)
		{
			oRun.Spell = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bLang)
		{
			oRun.Lang = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bAltLang)
		{
			oRun.AltLang = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bBidi)
		{
			oRun.Bidi = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bPp10ext)
		{
			DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
			BYTE flag1 = (BYTE)(dwFlags);
			BYTE flag2 = (BYTE)(dwFlags >> 8);
			BYTE flag3 = (BYTE)(dwFlags >> 16);
			BYTE flag4 = (BYTE)(dwFlags >> 24);

			oRun.pp10runid = (0x0F & flag1);
			oRun.bGramma = (0x80 == (0x80 & flag4));
		}

		/*if (bSmartTag)
		{
			DWORD tabStopsCount = StreamUtils::ReadDWORD(pStream);
			arSmartTags.clear();

			for (int i = 0; i < (int)tabStopsCount; ++i)
			{
				arSmartTags.Add(StreamUtils::ReadDWORD(pStream));
			}
		}*/
	}

	void Read(POLE::Stream* pStream, NSPresentationEditor::CTextRuler& oRun)
	{
		double dScaleX				= 625 * 2.54 ;
		//1/576 inch = 72/576 pt = 360000 *72 * 2.54 /(72*576) emu

		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		bool bDefaultTabSize_				= (0x01 == (0x01 & flag1));
		bool bCLevels_						= (0x02 == (0x02 & flag1));
		bool bTabStops_						= (0x04 == (0x04 & flag1));
		
		bool bLeftMargin1_					= (0x08 == (0x08 & flag1));
		bool bLeftMargin2_					= (0x10 == (0x10 & flag1));
		bool bLeftMargin3_					= (0x20 == (0x20 & flag1));
		bool bLeftMargin4_					= (0x40 == (0x40 & flag1));
		bool bLeftMargin5_					= (0x80 == (0x80 & flag1));

		bool bIndent1_						= (0x01 == (0x01 & flag2));
		bool bIndent2_						= (0x02 == (0x02 & flag2));
		bool bIndent3_						= (0x04 == (0x04 & flag2));
		bool bIndent4_						= (0x08 == (0x08 & flag2));
		bool bIndent5_						= (0x10 == (0x10 & flag2));

		if (bCLevels_)
			oRun.CLevels = StreamUtils::ReadSHORT(pStream);
		if (bDefaultTabSize_)
			oRun.DefaultTabSize = StreamUtils::ReadSHORT(pStream) * dScaleX;
		
		if (bTabStops_)
        {
            WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
			oRun.tabsStops.clear();

            for (int i = 0; i < (int)tabStopsCount; ++i)
            {
				oRun.tabsStops.push_back(StreamUtils::ReadDWORD(pStream) * dScaleX);
            }
        }

		if (bLeftMargin1_)
			oRun.LeftMargin1 = StreamUtils::ReadSHORT(pStream) * dScaleX;
		if (bIndent1_)
			oRun.Indent1 = StreamUtils::ReadSHORT(pStream) * dScaleX;

		if (bLeftMargin2_)
			oRun.LeftMargin2 = StreamUtils::ReadSHORT(pStream) * dScaleX;
		if (bIndent2_)
			oRun.Indent2 = StreamUtils::ReadSHORT(pStream) * dScaleX;

		if (bLeftMargin3_)
			oRun.LeftMargin3 = StreamUtils::ReadSHORT(pStream) * dScaleX;
		if (bIndent3_)
			oRun.Indent3 = StreamUtils::ReadSHORT(pStream) * dScaleX;

		if (bLeftMargin4_)
			oRun.LeftMargin4 = StreamUtils::ReadSHORT(pStream) * dScaleX;
		if (bIndent4_)
			oRun.Indent4 = StreamUtils::ReadSHORT(pStream) * dScaleX;

		if (bLeftMargin5_)
			oRun.LeftMargin5 = StreamUtils::ReadSHORT(pStream) * dScaleX;
		if (bIndent5_)
			oRun.Indent5 = StreamUtils::ReadSHORT(pStream) * dScaleX;
	}
}


void CTextPFRun_ppt::LoadFromStream(POLE::Stream* pStream, bool bIsIndentation)
{
	double dScaleX				= 625 * 2.54 ;
	//1/576 inch = 72/576 pt = 360000 *72 * 2.54 /(72*576) emu

	if (bIsIndentation)
	{
		m_lCount	= StreamUtils::ReadLONG(pStream);
		m_lLevel	= (LONG)StreamUtils::ReadWORD(pStream);

		if (m_lLevel > 0x0004)
			m_lLevel = 0x0004;
	}

	DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
	BYTE flag1 = (BYTE)(dwFlags);
	BYTE flag2 = (BYTE)(dwFlags >> 8);
	BYTE flag3 = (BYTE)(dwFlags >> 16);
	BYTE flag4 = (BYTE)(dwFlags >> 24);

	bool hasBullet_				= (0x01 == (0x01 & flag1));
	bool bulletHasFont_			= (0x02 == (0x02 & flag1));
	bool bulletHasColor_		= (0x04 == (0x04 & flag1));
	bool bulletHasSize_			= (0x08 == (0x08 & flag1));

	bool bulletFontRef_			= (0x10 == (0x10 & flag1));
	bool bulletColor_			= (0x20 == (0x20 & flag1));
	bool bulletSize_			= (0x40 == (0x40 & flag1));
	bool bulletChar_			= (0x80 == (0x80 & flag1));

	bool leftMargin_			= (0x01 == (0x01 & flag2));
	// reserved
	bool indent_				= (0x04 == (0x04 & flag2));
	bool textAlignment_			= (0x08 == (0x08 & flag2));
	bool lineSpacing_			= (0x10 == (0x10 & flag2));
	bool spaceBefore_			= (0x20 == (0x20 & flag2));
	bool spaceAfter_			= (0x40 == (0x40 & flag2));
	bool defaultTabSize_		= (0x80 == (0x80 & flag2));

	bool fontAlign_				= (0x01 == (0x01 & flag3));
	bool charWrap_				= (0x02 == (0x02 & flag3));
	bool wordWrap_				= (0x04 == (0x04 & flag3));
	bool overflow_				= (0x08 == (0x08 & flag3));		
	bool tabStops_				= (0x10 == (0x10 & flag3));
	bool textDirection_			= (0x20 == (0x20 & flag3));
	//reserved
	bool bulletBlip_			= (0x80 == (0x80 & flag3));

	bool bulletScheme_			= (0x01 == (0x01 & flag4));
	bool bulletHasScheme_		= (0x02 == (0x02 & flag4));

	if (hasBullet_ || bulletHasFont_ || bulletHasColor_ || bulletHasSize_)
	{
		WORD bulletFlag		= StreamUtils::ReadWORD(pStream);
		if (bulletFlag & 0x0F)
            m_oRun.hasBullet	= (bool)(0x01 == (bulletFlag & 0x01));
		else
            m_oRun.hasBullet	= false;
	}

	if (bulletChar_)
	{
		if (sizeof(wchar_t) == 2)
		{
			m_oRun.bulletChar		= (WCHAR)StreamUtils::ReadWORD(pStream);
		}
		else
		{
			unsigned short	utf16	= (unsigned short)StreamUtils::ReadWORD(pStream);
			std::wstring	utf32	= NSFile::CUtf8Converter::GetWStringFromUTF16(&utf16, 1);
			if (!utf32.empty())
				m_oRun.bulletChar = utf32.c_str()[0];
		}
	}
	if (bulletFontRef_)
		m_oRun.bulletFontRef	= StreamUtils::ReadWORD(pStream);
	if (bulletSize_)
		m_oRun.bulletSize		= StreamUtils::ReadWORD(pStream);
	if (bulletColor_)
	{
		SColorAtom oColorAtom;
		NSStreamReader::Read(pStream, oColorAtom);

		NSPresentationEditor::CColor oColor;
		
		oColor.R = oColorAtom.R;
		oColor.G = oColorAtom.G;
		oColor.B = oColorAtom.B;
		oColor.A = 255;
		oColor.m_lSchemeIndex = -1;

		if (oColorAtom.Index < 10)
		{
			oColor.m_lSchemeIndex = oColorAtom.Index;
			NSPresentationEditor::CorrectColorPPT(oColor.m_lSchemeIndex);
		}

		m_oRun.bulletColor = oColor;
	}

	if (textAlignment_)
		m_oRun.textAlignment		= StreamUtils::ReadWORD(pStream);

	if (lineSpacing_)
		m_oRun.lineSpacing			= - StreamUtils::ReadSHORT(pStream);

	if (spaceBefore_)
		m_oRun.spaceBefore			= - (LONG)StreamUtils::ReadSHORT(pStream);

	if (spaceAfter_)
		m_oRun.spaceAfter			= - (LONG)StreamUtils::ReadSHORT(pStream);

	if (leftMargin_)
		m_oRun.leftMargin			= (LONG)StreamUtils::ReadSHORT(pStream) * dScaleX;

	if (indent_)
		m_oRun.indent				= (LONG)StreamUtils::ReadSHORT(pStream) * dScaleX;

	if (defaultTabSize_)
		m_oRun.defaultTabSize		= (LONG)StreamUtils::ReadWORD(pStream)  * dScaleX;

	if (tabStops_)
	{
		WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
		m_oRun.tabStops.clear();

		if (tabStopsCount > 10)
			tabStopsCount = 10;

		for (int i = 0; i < (int)tabStopsCount; ++i)
		{
			m_oRun.tabStops.push_back(StreamUtils::ReadDWORD(pStream) * dScaleX );
		}

		if (0 < m_oRun.tabStops.size())
			m_oRun.defaultTabSize = m_oRun.tabStops[0];
	}

	if (fontAlign_)
		m_oRun.fontAlign	= StreamUtils::ReadWORD(pStream);
	
	if (charWrap_ || wordWrap_ || overflow_)
	{
		m_oRun.wrapFlags	= StreamUtils::ReadWORD(pStream);
	}

	if (textDirection_)
		m_oRun.textDirection = StreamUtils::ReadWORD(pStream);
}


void CTextCFRun_ppt::LoadFromStream(POLE::Stream* pStream, bool bIsIndentation)
{
	if (bIsIndentation)
	{
		m_lCount	= StreamUtils::ReadLONG(pStream);
	}

	DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
	BYTE flag1 = (BYTE)(dwFlags);
	BYTE flag2 = (BYTE)(dwFlags >> 8);
	BYTE flag3 = (BYTE)(dwFlags >> 16);
	BYTE flag4 = (BYTE)(dwFlags >> 24);

	bool hasBold				= (0x01 == (0x01 & flag1));
	bool hasItalic				= (0x02 == (0x02 & flag1));
	bool hasUnderline			= (0x04 == (0x04 & flag1));
	// unused
	bool hasShadow				= (0x10 == (0x10 & flag1));
	bool hasFehint				= (0x20 == (0x20 & flag1));
	// unused
	bool hasKimi				= (0x80 == (0x80 & flag1));

	// unused
	bool hasEmboss				= (0x02 == (0x02 & flag2));
	// unused
	BYTE hasStyle				= ((0x3C & flag2) >> 2);
	// unused

	bool typeface_				= (0x01 == (0x01 & flag3));
	bool size_					= (0x02 == (0x02 & flag3));
	bool color_					= (0x04 == (0x04 & flag3));
	bool BaseLineOffset_		= (0x08 == (0x08 & flag3));
	bool EAFontRef_				= (0x20 == (0x20 & flag3));
	bool AnsiFontRef_			= (0x40 == (0x40 & flag3));
	bool SymbolFontRef_			= (0x80 == (0x80 & flag3));

	bool hasNewEATypeface		= (0x01 == (0x01 & flag4));
	bool hasCsTypeface			= (0x02 == (0x02 & flag4));
	bool hasPp11ext				= (0x04 == (0x04 & flag4));

	bool bIsFontStylePresent = (hasBold || hasItalic || hasUnderline || hasShadow || 
		hasFehint || hasKimi || hasEmboss || hasStyle != 0);
	
	if (bIsFontStylePresent)
	{
		WORD fontStyle = StreamUtils::ReadWORD(pStream);

		if (0x01 == (0x01 & fontStyle))
            m_oRun.FontBold = true;
		if (0x02 == (0x02 & fontStyle))
            m_oRun.FontItalic = true;
		if (0x04 == (0x04 & fontStyle))
            m_oRun.FontUnderline = true;
		if (0x10 == (0x10 & fontStyle))
            m_oRun.FontShadow = true;
	}

	if (typeface_)
		m_oRun.Typeface = StreamUtils::ReadWORD(pStream);

	if (EAFontRef_)
		m_oRun.EAFontRef = StreamUtils::ReadWORD(pStream);

	if (AnsiFontRef_)
		m_oRun.AnsiFontRef = StreamUtils::ReadWORD(pStream);

	if (SymbolFontRef_)
		m_oRun.SymbolFontRef = StreamUtils::ReadWORD(pStream);
	
	if (size_)
		m_oRun.Size = StreamUtils::ReadWORD(pStream);

	if (color_)
	{
		SColorAtom oColorAtom;
		NSStreamReader::Read(pStream, oColorAtom);

		NSPresentationEditor::CColor oColor;
		
		oColor.R = oColorAtom.R;
		oColor.G = oColorAtom.G;
		oColor.B = oColorAtom.B;
		oColor.A = 255;
		oColor.m_lSchemeIndex = -1;

		if (oColorAtom.Index < 10/* && oColorAtom.bSchemeIndex*/)
		{
			oColor.m_lSchemeIndex = oColorAtom.Index;
			NSPresentationEditor::CorrectColorPPT(oColor.m_lSchemeIndex);
		}
		else
		{
		}

		m_oRun.Color = oColor;
	}

	if (BaseLineOffset_)
		m_oRun.BaseLineOffset = (double)StreamUtils::ReadSHORT(pStream);

	// или два последних наоборот????
}


namespace NSPresentationEditor
{
	void ConvertPPTTextToEditorStructure(std::vector<CTextPFRun_ppt>& oArrayPF, std::vector<CTextCFRun_ppt>& oArrayCF, 
		std::wstring& strText, NSPresentationEditor::CTextAttributesEx& oAttributes)
	{
		int nCountPFs = (int)oArrayPF.size();
		int nCountCFs = (int)oArrayCF.size();

		oAttributes.m_arParagraphs.clear();
		int nCurrentPF = 0;
		int nCurrentCF = 0;
		int nOffsetCF  = 0;

		int nIndexLast = strText.length();
		int nIndexText = 0;
		
		for (int nIndexPF = 0; nIndexPF < nCountPFs; ++nIndexPF)
		{
			CParagraph elm;
			oAttributes.m_arParagraphs.push_back(elm);
			NSPresentationEditor::CParagraph* pPar = &oAttributes.m_arParagraphs[nIndexPF];

			pPar->m_oPFRun		= oArrayPF[nIndexPF].m_oRun;
			pPar->m_lTextType	= oAttributes.m_lTextType;
			pPar->m_lTextLevel	= oArrayPF[nIndexPF].m_lLevel;

			int nCountInPF = oArrayPF[nIndexPF].m_lCount;

			while (true)
			{
				if (nCurrentCF >= nCountCFs)
					break;

				int nCountAdd = oArrayCF[nCurrentCF].m_lCount - nOffsetCF;
				
				int nCountAddTrue = nCountAdd;
				if (nIndexText +  nCountAdd > nIndexLast)
				{
					nCountAddTrue = nIndexLast - nIndexText;
					if (nCountAddTrue < 0) nCountAddTrue = 0;
				}
				
				if (nCountAdd > nCountInPF)
				{
					nOffsetCF += nCountInPF;

					NSPresentationEditor::CSpan oSpan;
					oSpan.m_oRun	= oArrayCF[nCurrentCF].m_oRun;

					nCountAddTrue = nCountInPF;
					if (nIndexText +  nCountInPF > nIndexLast)
					{
						nCountAddTrue = nIndexLast - nIndexText;
						if (nCountAddTrue < 0) nCountAddTrue = 0;
					}
					if (nCountAddTrue > 0)
					{
						oSpan.m_strText = strText.substr(nIndexText, nCountAddTrue);
						pPar->m_arSpans.push_back(oSpan);
					}
					
					nIndexText += nCountInPF;
					break;
				}
				else if (nCountAdd == nCountInPF)
				{					
					nOffsetCF = 0;

					NSPresentationEditor::CSpan oSpan;
					oSpan.m_oRun	= oArrayCF[nCurrentCF].m_oRun;

					if (nCountAddTrue > 0)
					{
						oSpan.m_strText = strText.substr(nIndexText, nCountAddTrue);
						pPar->m_arSpans.push_back(oSpan);
					}
					nIndexText += nCountAdd;
					++nCurrentCF;
					break;
				}
				else
				{
					nOffsetCF = 0;

					NSPresentationEditor::CSpan oSpan;
					oSpan.m_oRun	= oArrayCF[nCurrentCF].m_oRun;

					if (nCountAddTrue > 0)
					{
						oSpan.m_strText = strText.substr(nIndexText, nCountAddTrue);
						pPar->m_arSpans.push_back(oSpan);
					}

					nIndexText += nCountAdd;
					nCountInPF -= nCountAdd;
					++nCurrentCF;
				}
			}
		}
	}
}
