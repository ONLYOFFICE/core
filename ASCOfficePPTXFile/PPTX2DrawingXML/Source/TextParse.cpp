#include "stdafx.h"
#include "../Converter.h"

#include "Logic/Paragraph.h"
#include "Logic/RunElem.h"
#include "Logic/Runs/Run.h"
#include "Logic/Runs/Fld.h"
#include "Logic/Runs/Br.h"
#include "Logic/RunProperties.h"
#include "nullable_property.h"

namespace PPTX2DrawingXML
{
	BYTE Converter::GetTextStrike(const std::string& str)
	{
		if(str == "noStrike") return 0;
		if(str == "sngStrike") return 1;
		if(str == "dblStrike") return 2;
		return 0;
	}

	void Converter::TextParse(const PPTX::Logic::TxBody& pShapeText, CTextAttributesEx& pText, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction)
	{
		pText.m_oAttributes.m_nTextAlignVertical = GetTextAnchorFromStr(TextProps.GetAnchor());
//		pText.m_bVertical = (pShapeText.bodyPr->vert.get_value_or("horz") != "horz");
//		pText.m_oAttributes.m_dTextRotate = pShapeText.bodyPr->rot.get_value_or(0) / 60000.0;
		pText.m_sText = _T("");
		for(std::list<PPTX::Logic::Paragraph>::const_iterator SpTextPar = pShapeText.Paragrs->begin(); SpTextPar != pShapeText.Paragrs->end(); SpTextPar++)
		{
			STextPFRun paragraph;

			paragraph.lCount = SpTextPar->GetWText().length();

			int level = 0;

			if(SpTextPar->pPr.is_init())
				level = /*(*/SpTextPar->pPr->lvl.get_value_or(0);//>4) ?4:(SpTextPar->pPr->lvl.is_init() ?SpTextPar->pPr->lvl.get():0);

			paragraph.lIndentLevel = level;

			paragraph.hasBullet_ = TextProps.HasParagraphBullet(level, SpTextPar->pPr);
			if(paragraph.lCount == 1)
				paragraph.hasBullet_ = false;
			if(paragraph.hasBullet_)
			{
/*
	//bool hasBullet_;
	bool bulletHasFont_;
	//bool bulletHasColor_;
	//bool bulletHasSize_;
	bool bulletFont_;
	//bool bulletColor_;
	//bool bulletSize_;
	//bool bulletChar_;

	bool tabStops_;
	bool bulletBlip_;

	bool bulletScheme_;
	bool bulletHasScheme_;
	
	//WORD bulletFlag;
	//WCHAR bulletChar;
	WORD bulletFontRef;
	//WORD bulletSize;

	//SColorAtom bulletColor;

	CAtlArray<DWORD> tabsStops;
*/

				paragraph.bulletHasColor_ = true;
				paragraph.bulletColor_ = true;
				paragraph.bulletColor.FromValue(0x0);

				paragraph.bulletHasSize_ = true;
				paragraph.bulletSize_ = true;
				paragraph.bulletSize = 100;
				
				paragraph.bulletChar_ = true;
				paragraph.bulletChar = L'*';

				paragraph.bulletHasFont_ = true;

				paragraph.bulletFlag = 0x0F;
			}

			paragraph.align_ = true;
			paragraph.textAlignment = GetTextAlignFromStr ( TextProps.GetParagraphAlgn ( level, SpTextPar->pPr) );
			//if(type == "ctrTitle")
			//	paragraph.textAlignment = GetTextAlignFromStr("ctr");

			paragraph.leftMargin_ = true;
			paragraph.leftMargin = TextProps.GetParagraphLeftMargin(level, SpTextPar->pPr);

			paragraph.indent_ = true;
			paragraph.indent = TextProps.GetParagraphIndent(level, SpTextPar->pPr);

			paragraph.defaultTabSize_ = true;
			paragraph.defaultTabSize = TextProps.GetParagraphDefTabSz(level, SpTextPar->pPr);

			paragraph.fontAlign_ = true;
			paragraph.fontAlign = GetFontAlignFromStr(TextProps.GetParagraphFontAlgn(level, SpTextPar->pPr));

			if(TextProps.GetParagraphLatinLnBrk(level, SpTextPar->pPr))
			{
				paragraph.charWrap_ = true;
				paragraph.wordWrap_ = false;
				paragraph.wrapFlags &= 0xFD;
				paragraph.wrapFlags |= 0x01;

				paragraph.overflow_ = true;
			}
			else
			{
				paragraph.charWrap_ = false;
				paragraph.wordWrap_ = true;
				paragraph.wrapFlags &= 0xFE;
				paragraph.wrapFlags |= 0x02;

				paragraph.overflow_ = true;
			}

			paragraph.textDirection_ = true;
			paragraph.textDirectional = (TextProps.GetParagraphRtl(level, SpTextPar->pPr))?1:0;

			paragraph.lineSpacing_ = true;
			short int space = TextProps.GetParagraphLnSpc(level, SpTextPar->pPr);
			if(space < 0)
				paragraph.lineSpacing = space * 127 * (1 - LnSpcReduction);// - 13200;
			else
				paragraph.lineSpacing = space * (1 - LnSpcReduction);

			paragraph.spaceBefore_ = true;
			space = TextProps.GetParagraphSpcBef(level, SpTextPar->pPr);
			if(space < 0)
				paragraph.spaceBefore = space * 127;// - 13200;
			else
				paragraph.spaceBefore = space;

			paragraph.spaceAfter_ = true;
			space = TextProps.GetParagraphSpcAft(level, SpTextPar->pPr);
			if(space < 0)
				paragraph.spaceAfter = space * 127;// - 13200;
			else
				paragraph.spaceAfter = space;

			pText.m_arPFs.Add(paragraph);

			for(std::list<PPTX::Logic::RunElem>::const_iterator SpTextRun = SpTextPar->RunElems->begin(); SpTextRun != SpTextPar->RunElems->end(); SpTextRun++)
			{
				STextCFRun CFRun;
				//CFRun.lCount = GetCStringFromUTF8(CString(SpTextRun->GetText().c_str())).GetLength();//SpTextRun->GetText().length();
				CStringW RunText = CStringW(SpTextRun->GetWText().c_str());
				CFRun.lCount = SpTextRun->GetWText().length();

				if(SpTextRun->is<PPTX::Logic::Run>())
				{
					const nullable_property<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Run>().rPr;
					
					CFRun.hasBold = TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					if(CFRun.hasBold)
						CFRun.fontStyle |= 0x01;
					CFRun.hasItalic = TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					if(CFRun.hasItalic)
						CFRun.fontStyle |= 0x02;
					CFRun.hasUnderline = TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != "none";

					bool isHyperlink = false;
					if(lpSpTextRun.is_init())
						if(lpSpTextRun->hlinkClick.is_init())
						{
							CFRun.hasUnderline = true;
							isHyperlink = true;
						}
					if(CFRun.hasUnderline)
						CFRun.fontStyle |= 0x04;

					CFRun.hasSize = true;
					CFRun.fontSize = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;

					CFRun.strFontName = CStringW(TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr).c_str());
					CFRun.fontPanose = CStringW(TextProps.GetRunPanose(level, lpSpTextRun, SpTextPar->pPr).c_str());
					CFRun.fontCharset.RemoveAll();
					CFRun.fontCharset.Add(TextProps.GetRunCharset(level, lpSpTextRun, SpTextPar->pPr));
					BYTE fontPitchFamily = TextProps.GetRunPitchFamily(level, lpSpTextRun, SpTextPar->pPr);
					switch(fontPitchFamily % 0x10)
					{
					case 0: {CFRun.fontFixed = -1; break;}
					case 1: {CFRun.fontFixed = 1; break;}
					case 2: {CFRun.fontFixed = 0; break;}
					default: {CFRun.fontFixed = -1; break;}
					}
					switch(fontPitchFamily / 0x10)
					{
					case 0: {CFRun.fontPitchFamily = _T("unknown"); break;}
					case 1: {CFRun.fontPitchFamily = _T("roman"); break;}
					case 2: {CFRun.fontPitchFamily = _T("swiss"); break;}
					case 3: {CFRun.fontPitchFamily = _T("modern"); break;}
					case 4: {CFRun.fontPitchFamily = _T("script"); break;}
					case 5: {CFRun.fontPitchFamily = _T("decorative"); break;}
					default: {CFRun.fontPitchFamily = _T("unknown"); break;}
					}

					CFRun.hasColor = true;
					if(isHyperlink)
						CFRun.oColor.FromValue(TextProps.GetHyperlinkABGR());
					else
						CFRun.oColor.FromValue(TextProps.GetRunABGR(level, lpSpTextRun, SpTextPar->pPr));

					CFRun.StrikeOut = GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));

					int baseline = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr);
					CFRun.hasBaselineOffset = (baseline != 0);
					if(CFRun.hasBaselineOffset)
						CFRun.BaselineOffset = baseline / 1000.0;//(baseline / 100000.0) * CFRun.fontSize * (25.4 / 72);

					std::string str = TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);
					if(str == "all") RunText = RunText.MakeUpper();
					//else if(str == "small") RunText = RunText.MakeLower();
//					Text.m_sText += RunText;
				}
				else if(SpTextRun->is<PPTX::Logic::Fld>())
				{
					const nullable_property<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Fld>().rPr;
					
					CFRun.hasBold = TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					if(CFRun.hasBold)
						CFRun.fontStyle |= 0x01;
					CFRun.hasItalic = TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					if(CFRun.hasItalic)
						CFRun.fontStyle |= 0x02;
					CFRun.hasUnderline = TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != "none";
					if(CFRun.hasUnderline)
						CFRun.fontStyle |= 0x04;

					CFRun.hasSize = true;
					CFRun.fontSize = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;

					CFRun.strFontName = CStringW(TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr).c_str());

					CFRun.hasColor = true;
					CFRun.oColor.FromValue(TextProps.GetRunABGR(level, lpSpTextRun, SpTextPar->pPr));

					CFRun.StrikeOut = GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));

//					Text.m_sText += RunText;
				}
				else if(SpTextRun->is<PPTX::Logic::Br>())
				{
					const nullable_property<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Br>().rPr;
					
					CFRun.hasSize = true;
					CFRun.fontSize = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;
				}
				if(CFRun.lCount > 0)
				{
					pText.m_sText += RunText;
					pText.m_arCFs.Add(CFRun);
				}
			}

			STextCFRun CFRun;
			CFRun.lCount = 1;
			CFRun.hasSize = true;
/*
			if(SpTextPar->endParaRPr.is_init())
			{
				if(SpTextPar->endParaRPr->sz.is_init())
					CFRun.fontSize = (WORD(SpTextPar->endParaRPr->sz.get() * FontScale + 50))/100;
				else if(pText.m_arCFs.GetCount() > 0)
					CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount()-1).fontSize;
				else
					CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
			}
			else
			{
				if(pText.m_arCFs.GetCount() > 0)
					CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount()-1).fontSize;
				else
					CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
			}
*/
			if(pText.m_arCFs.GetCount() > 0)
				CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount() - 1).fontSize;
			else if(SpTextPar->endParaRPr.is_init())
			{
				if(SpTextPar->endParaRPr->sz.is_init())
					CFRun.fontSize = (WORD(SpTextPar->endParaRPr->sz.get() * FontScale + 50))/100;
				else
					CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
			}
			else
				CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;

			pText.m_arCFs.Add(CFRun);
			pText.m_sText += L"\n";
		}
	}
} // namespace PPTX2DrawingXML