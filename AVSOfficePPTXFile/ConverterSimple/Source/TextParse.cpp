#include "stdafx.h"
#include "../Converter.h"

#include "../../PPTXFormat/Logic/Paragraph.h"
#include "../../PPTXFormat/Logic/RunElem.h"
#include "../../PPTXFormat/Logic/Runs/Run.h"
#include "../../PPTXFormat/Logic/Runs/Fld.h"
#include "../../PPTXFormat/Logic/Runs/Br.h"
#include "../../PPTXFormat/Logic/RunProperties.h"

namespace PPTX2EditorSimple
{
	BYTE Converter::GetTextStrike(const CString& str)
	{
		if (str == _T("noStrike"))	return 0;
		if (str == _T("sngStrike")) return 1;
		if (str == _T("dblStrike")) return 2;
		return 0;
	}
	void Converter::TextParse(const PPTX::Logic::TxBody& pShapeText, NSPresentationEditor::CTextAttributesEx& pText, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction)
	{
		pText.m_oAttributes.m_nTextAlignVertical = Converter::GetTextAnchorFromStr(TextProps.GetAnchor());
//		pText.m_bVertical = (pShapeText.bodyPr->vert.get_value_or("horz") != "horz");
//		pText.m_oAttributes.m_dTextRotate = pShapeText.bodyPr->rot.get_value_or(0) / 60000.0;
		pText.m_sText = _T("");

		size_t nCount = pShapeText.Paragrs.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Logic::Paragraph* SpTextPar = &pShapeText.Paragrs[i];
			
			STextPFRun paragraph;
			paragraph.lCount = SpTextPar->GetText().GetLength();

			int level = 0;

			if(SpTextPar->pPr.is_init())
				level = /*(*/SpTextPar->pPr->lvl.get_value_or(0);//>4) ?4:(SpTextPar->pPr->lvl.is_init() ?SpTextPar->pPr->lvl.get():0);

			paragraph.Level = level;

			paragraph.hasBullet.SetType(TextProps.HasParagraphBullet(level, SpTextPar->pPr));
			if(paragraph.lCount == 1)
				paragraph.hasBullet.SetType(false);
			if (0 != paragraph.hasBullet.GetType())
			{
				paragraph.bulletColor.SetType(true);
				paragraph.bulletColor.GetValue() = 0x00;

				paragraph.bulletSize.SetType(true);
				paragraph.bulletSize = 100;

				paragraph.bulletChar.SetType(true);
				paragraph.bulletChar.GetValue() = L'*';
			}

			paragraph.textAlignment.SetType(true);
			paragraph.textAlignment.GetValue() = Converter::GetTextAlignFromStr ( TextProps.GetParagraphAlgn ( level, SpTextPar->pPr) );
			//if(type == "ctrTitle")
			//	paragraph.textAlignment = GetTextAlignFromStr("ctr");

			paragraph.leftMargin.SetType(true);
			paragraph.leftMargin.GetValue() = TextProps.GetParagraphLeftMargin(level, SpTextPar->pPr);

			paragraph.indent.SetType(true);
			paragraph.indent.GetValue() = TextProps.GetParagraphIndent(level, SpTextPar->pPr);

			paragraph.defaultTabSize.SetType(true);
			paragraph.defaultTabSize.GetValue() = TextProps.GetParagraphDefTabSz(level, SpTextPar->pPr);

			paragraph.fontAlign.SetType(true);
			paragraph.fontAlign.GetValue() = Converter::GetFontAlignFromStr(TextProps.GetParagraphFontAlgn(level, SpTextPar->pPr));

			if(TextProps.GetParagraphLatinLnBrk(level, SpTextPar->pPr))
			{
				paragraph.wrapFlags.SetType(true);
				paragraph.wrapFlags.GetValue() &= 0xFD;
				paragraph.wrapFlags.GetValue() |= 0x01;
			}
			else
			{
				paragraph.wrapFlags.SetType(true);
				paragraph.wrapFlags.GetValue() &= 0xFE;
				paragraph.wrapFlags.GetValue() |= 0x02;
			}

			paragraph.textDirection.SetType(true);
			paragraph.textDirection.GetValue() = (TextProps.GetParagraphRtl(level, SpTextPar->pPr))?1:0;

			paragraph.lineSpacing.SetType(true);
			short int space = TextProps.GetParagraphLnSpc(level, SpTextPar->pPr);
			if(space < 0)
				paragraph.lineSpacing.GetValue() = (LONG)(space * 127 * (1 - LnSpcReduction));// - 13200;
			else
				paragraph.lineSpacing.GetValue() = (LONG)(space * (1 - LnSpcReduction));

			paragraph.spaceBefore.SetType(true);
			space = TextProps.GetParagraphSpcBef(level, SpTextPar->pPr);
			if(space < 0)
				paragraph.spaceBefore.GetValue() = space * 127;// - 13200;
			else
				paragraph.spaceBefore.GetValue() = space;

			paragraph.spaceAfter.SetType(true);
			space = TextProps.GetParagraphSpcAft(level, SpTextPar->pPr);
			if(space < 0)
				paragraph.spaceAfter.GetValue() = space * 127;// - 13200;
			else
				paragraph.spaceAfter.GetValue() = space;

			pText.m_arPFs.Add(paragraph);

			size_t nCountCF = SpTextPar->RunElems.GetCount();
			for (size_t j = 0; j < nCountCF; ++j)
			{
				const PPTX::Logic::RunElem* SpTextRun = &SpTextPar->RunElems[j];

				STextCFRun CFRun;

				CFRun.Level = level;
				
				CString RunText = SpTextRun->GetText();
				CFRun.lCount = RunText.GetLength();

				if (SpTextRun->is<PPTX::Logic::Run>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Run>().rPr;
					
					CFRun.FontStyle.SetType(true);
					
					bool bBold = TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					if (bBold)
						CFRun.FontStyle.GetValue() |= 0x01;
					bool bItalic = TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					if (bItalic)
						CFRun.FontStyle.GetValue() |= 0x02;
					bool bUnderline = (TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none"));

					bool isHyperlink = false;
					if(lpSpTextRun.is_init())
						if(lpSpTextRun->hlinkClick.is_init())
						{
							bUnderline = true;
							isHyperlink = true;
						}
					if (bUnderline)
						CFRun.FontStyle.GetValue() |= 0x04;
					
					CFRun.Size.SetType(true);
					CFRun.Size.GetValue() = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;

					CFRun.strFontName = CStringW(TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr));
					CFRun.strPanose = CStringW(TextProps.GetRunPanose(level, lpSpTextRun, SpTextPar->pPr));
					CFRun.arFontCharsets.RemoveAll();
					CFRun.arFontCharsets.Add(TextProps.GetRunCharset(level, lpSpTextRun, SpTextPar->pPr));
					BYTE fontPitchFamily = TextProps.GetRunPitchFamily(level, lpSpTextRun, SpTextPar->pPr);
					switch(fontPitchFamily % 0x10)
					{
					case 0: {CFRun.lFontFixed = -1; break;}
					case 1: {CFRun.lFontFixed = 1; break;}
					case 2: {CFRun.lFontFixed = 0; break;}
					default: {CFRun.lFontFixed = -1; break;}
					}
					switch(fontPitchFamily / 0x10)
					{
					case 0: {CFRun.strPitchFamily = _T("unknown"); break;}
					case 1: {CFRun.strPitchFamily = _T("roman"); break;}
					case 2: {CFRun.strPitchFamily = _T("swiss"); break;}
					case 3: {CFRun.strPitchFamily = _T("modern"); break;}
					case 4: {CFRun.strPitchFamily = _T("script"); break;}
					case 5: {CFRun.strPitchFamily = _T("decorative"); break;}
					default: {CFRun.strPitchFamily = _T("unknown"); break;}
					}

					CFRun.Color.SetType(true);
					if(isHyperlink)
						CFRun.Color.GetValue() = TextProps.GetHyperlinkBGRA();
					else
						CFRun.Color.GetValue() = TextProps.GetRunBGRA(level, lpSpTextRun, SpTextPar->pPr);

					BYTE StrikeOut = Converter::GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));

					CFRun.FontStyle.GetValue() |= (StrikeOut << 8);

					int baseline = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr);
					CFRun.BaseLineOffset.SetType((bool)(baseline != 0));
					if (0 != CFRun.BaseLineOffset.GetType())
						CFRun.BaseLineOffset = (double)baseline / 1000.0;//(baseline / 100000.0) * CFRun.fontSize * (25.4 / 72);

					CString str = TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);
					if (str == _T("all"))
						RunText = RunText.MakeUpper();
					//else if(str == "small") RunText = RunText.MakeLower();
//					Text.m_sText += RunText;
				}
				else if(SpTextRun->is<PPTX::Logic::Fld>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Fld>().rPr;
					
					CFRun.FontStyle.SetType(true);
					
					bool bBold = TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					if(bBold)
						CFRun.FontStyle.GetValue() |= 0x01;
					bool bItalic = TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					if(bItalic)
						CFRun.FontStyle.GetValue() |= 0x02;
					bool bUnderline = (TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none"));
					if(bUnderline)
						CFRun.FontStyle.GetValue() |= 0x04;

					CFRun.Size.SetType(true);
					CFRun.Size.GetValue() = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;

					CFRun.strFontName = TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr);

					CFRun.Color.SetType(true);
					CFRun.Color.GetValue() = (TextProps.GetRunABGR(level, lpSpTextRun, SpTextPar->pPr));

					CFRun.FontStyle.GetValue() |= (Converter::GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr)) << 8);

//					Text.m_sText += RunText;
				}
				else if(SpTextRun->is<PPTX::Logic::Br>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Br>().rPr;
					
					CFRun.Size.SetType(true);
					CFRun.Size.GetValue() = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;
				}
				if(CFRun.lCount > 0)
				{
					pText.m_sText += RunText;
					pText.m_arCFs.Add(CFRun);
				}
			}

			STextCFRun CFRun;
			CFRun.lCount = 1;
			CFRun.Size.SetType(true);
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
				CFRun.Size.GetValue() = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount() - 1).Size.GetValue();
			else if(SpTextPar->endParaRPr.is_init())
			{
				if(SpTextPar->endParaRPr->sz.is_init())
					CFRun.Size.GetValue() = (WORD(SpTextPar->endParaRPr->sz.get() * FontScale + 50))/100;
				else
					CFRun.Size.GetValue() = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
			}
			else
				CFRun.Size.GetValue() = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;

			pText.m_arCFs.Add(CFRun);
			pText.m_sText += _T("\n");
		}
	}

//	void Converter::TextParse(const PPTX::Logic::TxBody& pShapeText, NSPresentationEditor::CTextAttributesEx& pText, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction)
//	{
//		pText.m_oAttributes.m_nTextAlignVertical = GetTextAnchorFromStr(TextProps.GetAnchor());
////		pText.m_bVertical = (pShapeText.bodyPr->vert.get_value_or("horz") != "horz");
////		pText.m_oAttributes.m_dTextRotate = pShapeText.bodyPr->rot.get_value_or(0) / 60000.0;
//		pText.m_sText = _T("");
//
//		size_t nCount = pShapeText.Paragrs.GetCount();
//		for (size_t i = 0; i < nCount; ++i)
//		{
//			const PPTX::Logic::Paragraph* SpTextPar = &pShapeText.Paragrs[i];
//			
//			STextPFRun paragraph;
//			paragraph.lCount = SpTextPar->GetText().GetLength();
//
//			int level = 0;
//
//			if(SpTextPar->pPr.is_init())
//				level = /*(*/SpTextPar->pPr->lvl.get_value_or(0);//>4) ?4:(SpTextPar->pPr->lvl.is_init() ?SpTextPar->pPr->lvl.get():0);
//
//			paragraph.lIndentLevel = level;
//
//			paragraph.hasBullet_ = TextProps.HasParagraphBullet(level, SpTextPar->pPr);
//			if(paragraph.lCount == 1)
//				paragraph.hasBullet_ = false;
//			if(paragraph.hasBullet_)
//			{
//				paragraph.bulletHasColor_ = true;
//				paragraph.bulletColor_ = true;
//				paragraph.bulletColor.FromValue(0x0);
//
//				paragraph.bulletHasSize_ = true;
//				paragraph.bulletSize_ = true;
//				paragraph.bulletSize = 100;
//				
//				paragraph.bulletChar_ = true;
//				paragraph.bulletChar = L'*';
//
//				paragraph.bulletHasFont_ = true;
//
//				paragraph.bulletFlag = 0x0F;
//			}
//
//			paragraph.align_ = true;
//			paragraph.textAlignment = GetTextAlignFromStr ( TextProps.GetParagraphAlgn ( level, SpTextPar->pPr) );
//			//if(type == "ctrTitle")
//			//	paragraph.textAlignment = GetTextAlignFromStr("ctr");
//
//			paragraph.leftMargin_ = true;
//			paragraph.leftMargin = TextProps.GetParagraphLeftMargin(level, SpTextPar->pPr);
//
//			paragraph.indent_ = true;
//			paragraph.indent = TextProps.GetParagraphIndent(level, SpTextPar->pPr);
//
//			paragraph.defaultTabSize_ = true;
//			paragraph.defaultTabSize = TextProps.GetParagraphDefTabSz(level, SpTextPar->pPr);
//
//			paragraph.fontAlign_ = true;
//			paragraph.fontAlign = GetFontAlignFromStr(TextProps.GetParagraphFontAlgn(level, SpTextPar->pPr));
//
//			if(TextProps.GetParagraphLatinLnBrk(level, SpTextPar->pPr))
//			{
//				paragraph.charWrap_ = true;
//				paragraph.wordWrap_ = false;
//				paragraph.wrapFlags &= 0xFD;
//				paragraph.wrapFlags |= 0x01;
//
//				paragraph.overflow_ = true;
//			}
//			else
//			{
//				paragraph.charWrap_ = false;
//				paragraph.wordWrap_ = true;
//				paragraph.wrapFlags &= 0xFE;
//				paragraph.wrapFlags |= 0x02;
//
//				paragraph.overflow_ = true;
//			}
//
//			paragraph.textDirection_ = true;
//			paragraph.textDirectional = (TextProps.GetParagraphRtl(level, SpTextPar->pPr))?1:0;
//
//			paragraph.lineSpacing_ = true;
//			short int space = TextProps.GetParagraphLnSpc(level, SpTextPar->pPr);
//			if(space < 0)
//				paragraph.lineSpacing = (LONG)(space * 127 * (1 - LnSpcReduction));// - 13200;
//			else
//				paragraph.lineSpacing = (LONG)(space * (1 - LnSpcReduction));
//
//			paragraph.spaceBefore_ = true;
//			space = TextProps.GetParagraphSpcBef(level, SpTextPar->pPr);
//			if(space < 0)
//				paragraph.spaceBefore = space * 127;// - 13200;
//			else
//				paragraph.spaceBefore = space;
//
//			paragraph.spaceAfter_ = true;
//			space = TextProps.GetParagraphSpcAft(level, SpTextPar->pPr);
//			if(space < 0)
//				paragraph.spaceAfter = space * 127;// - 13200;
//			else
//				paragraph.spaceAfter = space;
//
//			pText.m_arPFs.Add(paragraph);
//
//			size_t nCountCF = SpTextPar->RunElems.GetCount();
//			for (size_t j = 0; j < nCountCF; ++j)
//			{
//				const PPTX::Logic::RunElem* SpTextRun = &SpTextPar->RunElems[j];
//				
//				STextCFRun CFRun;
//				CString RunText = SpTextRun->GetText();
//				CFRun.lCount = RunText.GetLength();
//
//				if (SpTextRun->is<PPTX::Logic::Run>())
//				{
//					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Run>().rPr;
//					
//					CFRun.hasBold = TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
//					if(CFRun.hasBold)
//						CFRun.fontStyle |= 0x01;
//					CFRun.hasItalic = TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
//					if(CFRun.hasItalic)
//						CFRun.fontStyle |= 0x02;
//					CFRun.hasUnderline = (TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none"));
//
//					bool isHyperlink = false;
//					if(lpSpTextRun.is_init())
//						if(lpSpTextRun->hlinkClick.is_init())
//						{
//							CFRun.hasUnderline = true;
//							isHyperlink = true;
//						}
//					if(CFRun.hasUnderline)
//						CFRun.fontStyle |= 0x04;
//
//					CFRun.hasSize = true;
//					CFRun.fontSize = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;
//
//					CFRun.strFontName = CStringW(TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr));
//					CFRun.fontPanose = CStringW(TextProps.GetRunPanose(level, lpSpTextRun, SpTextPar->pPr));
//					CFRun.fontCharset.RemoveAll();
//					CFRun.fontCharset.Add(TextProps.GetRunCharset(level, lpSpTextRun, SpTextPar->pPr));
//					BYTE fontPitchFamily = TextProps.GetRunPitchFamily(level, lpSpTextRun, SpTextPar->pPr);
//					switch(fontPitchFamily % 0x10)
//					{
//					case 0: {CFRun.fontFixed = -1; break;}
//					case 1: {CFRun.fontFixed = 1; break;}
//					case 2: {CFRun.fontFixed = 0; break;}
//					default: {CFRun.fontFixed = -1; break;}
//					}
//					switch(fontPitchFamily / 0x10)
//					{
//					case 0: {CFRun.fontPitchFamily = _T("unknown"); break;}
//					case 1: {CFRun.fontPitchFamily = _T("roman"); break;}
//					case 2: {CFRun.fontPitchFamily = _T("swiss"); break;}
//					case 3: {CFRun.fontPitchFamily = _T("modern"); break;}
//					case 4: {CFRun.fontPitchFamily = _T("script"); break;}
//					case 5: {CFRun.fontPitchFamily = _T("decorative"); break;}
//					default: {CFRun.fontPitchFamily = _T("unknown"); break;}
//					}
//
//					CFRun.hasColor = true;
//					if(isHyperlink)
//						CFRun.oColor.FromValue(TextProps.GetHyperlinkABGR());
//					else
//						CFRun.oColor.FromValue(TextProps.GetRunABGR(level, lpSpTextRun, SpTextPar->pPr));
//
//					CFRun.StrikeOut = GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));
//
//					int baseline = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr);
//					CFRun.hasBaselineOffset = (baseline != 0);
//					if(CFRun.hasBaselineOffset)
//						CFRun.BaselineOffset = baseline / 1000.0;//(baseline / 100000.0) * CFRun.fontSize * (25.4 / 72);
//
//					CString str = TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);
//					if (str == _T("all"))
//						RunText = RunText.MakeUpper();
//					//else if(str == "small") RunText = RunText.MakeLower();
////					Text.m_sText += RunText;
//				}
//				else if(SpTextRun->is<PPTX::Logic::Fld>())
//				{
//					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Fld>().rPr;
//					
//					CFRun.hasBold = TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
//					if(CFRun.hasBold)
//						CFRun.fontStyle |= 0x01;
//					CFRun.hasItalic = TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
//					if(CFRun.hasItalic)
//						CFRun.fontStyle |= 0x02;
//					CFRun.hasUnderline = (TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none"));
//					if(CFRun.hasUnderline)
//						CFRun.fontStyle |= 0x04;
//
//					CFRun.hasSize = true;
//					CFRun.fontSize = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;
//
//					CFRun.strFontName = TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr);
//
//					CFRun.hasColor = true;
//					CFRun.oColor.FromValue(TextProps.GetRunABGR(level, lpSpTextRun, SpTextPar->pPr));
//
//					CFRun.StrikeOut = GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));
//
////					Text.m_sText += RunText;
//				}
//				else if(SpTextRun->is<PPTX::Logic::Br>())
//				{
//					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Br>().rPr;
//					
//					CFRun.hasSize = true;
//					CFRun.fontSize = (WORD(TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50))/100;
//				}
//				if(CFRun.lCount > 0)
//				{
//					pText.m_sText += RunText;
//					pText.m_arCFs.Add(CFRun);
//				}
//			}
//
//			STextCFRun CFRun;
//			CFRun.lCount = 1;
//			CFRun.hasSize = true;
///*
//			if(SpTextPar->endParaRPr.is_init())
//			{
//				if(SpTextPar->endParaRPr->sz.is_init())
//					CFRun.fontSize = (WORD(SpTextPar->endParaRPr->sz.get() * FontScale + 50))/100;
//				else if(pText.m_arCFs.GetCount() > 0)
//					CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount()-1).fontSize;
//				else
//					CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
//			}
//			else
//			{
//				if(pText.m_arCFs.GetCount() > 0)
//					CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount()-1).fontSize;
//				else
//					CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
//			}
//*/
//			if(pText.m_arCFs.GetCount() > 0)
//				CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount() - 1).fontSize;
//			else if(SpTextPar->endParaRPr.is_init())
//			{
//				if(SpTextPar->endParaRPr->sz.is_init())
//					CFRun.fontSize = (WORD(SpTextPar->endParaRPr->sz.get() * FontScale + 50))/100;
//				else
//					CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
//			}
//			else
//				CFRun.fontSize = (WORD(TextProps.GetRunSize(level) * FontScale + 50))/100;
//
//			pText.m_arCFs.Add(CFRun);
//			pText.m_sText += _T("\n");
//		}
//	}
} // namespace PPTX2DrawingXML