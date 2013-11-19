#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/Tab.h"

namespace PPTX2OfficeDrawing
{
	WORD Converter::GetTextAlignFromStr(const std::string& str)
	{
		if(str == "l") return 0;
		if(str == "ctr") return 1;
		if(str == "r") return 2;
		if(str == "just") return 3;
		if(str == "dist") return 4;
		if(str == "thaiDist") return 5;
		if(str == "justLow") return 6;
		return 0;
	}

	BYTE Converter::GetTextStrike(const std::string& str)
	{
		if(str == "noStrike") return 0;
		if(str == "sngStrike") return 1;
		if(str == "dblStrike") return 2;
		return 0;
	}

	void Converter::TextStyleConvert(const nullable_property<PPTX::Logic::TextListStyle>& pptxStyle, NSPresentationEditor::CTextStyle& pStyle)
	{
		if(pptxStyle.is_init())
			TextStyleConvert(pptxStyle.get(), pStyle);
	}

	void Converter::TextStyleConvert(const PPTX::Logic::TextListStyle& pptxStyle, NSPresentationEditor::CTextStyle& pStyle)
	{
		for(int i = 0; i < 10; i++)
		{
			if(pptxStyle.levels[i].is_init())
			{
				CTextStyleLevel StyleLevel;
				StyleLevel.cLevel = i;
				TextStyleLevelConvert(pptxStyle.levels[i].get(), StyleLevel);
				pStyle.m_arrLevels.Add(StyleLevel);
			}
		}
	}

	void Converter::TextStyleLevelConvert(const PPTX::Logic::TextParagraphPr& pptxStyle, NSPresentationEditor::CTextStyleLevel& pStyleLevel)
	{
		ParagraphPropertiesConvert(pptxStyle, pStyleLevel.oPFRun);

		if(pptxStyle.defRPr.is_init())
			RunPropertiesConvert(*pptxStyle.defRPr, pStyleLevel.oCFRun);
	}

	void Converter::ParagraphPropertiesConvert(const nullable_property<PPTX::Logic::TextParagraphPr>& pptxStyle, NSPresentationEditor::STextPFRun& pStyleLevel)
	{
		if(pptxStyle.is_init())
			ParagraphPropertiesConvert(*pptxStyle, pStyleLevel);
	}

	void Converter::ParagraphPropertiesConvert(const PPTX::Logic::TextParagraphPr& pptxStyle, NSPresentationEditor::STextPFRun& pStyleLevel)
	{
		pStyleLevel.lineSpacing_ = pptxStyle.lnSpc.is_init();
		if(pStyleLevel.lineSpacing_)
		{
			int space = pptxStyle.lnSpc->GetVal();
			if(space < 0)
				pStyleLevel.lineSpacing = space * 127;
			else
				pStyleLevel.lineSpacing = space;
		}

		pStyleLevel.spaceBefore_ = pptxStyle.spcBef.is_init();
		if(pStyleLevel.spaceBefore_)
		{
			int space = pptxStyle.spcBef->GetVal();
			if(space < 0)
				pStyleLevel.spaceBefore = space * 127;
			else
				pStyleLevel.spaceBefore = space;
		}

		pStyleLevel.spaceAfter_ = pptxStyle.spcAft.is_init();
		if(pStyleLevel.spaceAfter_)
		{
			int space = pptxStyle.spcAft->GetVal();
			if(space < 0)
				pStyleLevel.spaceAfter = space * 127;
			else
				pStyleLevel.spaceAfter = space;
		}

		pStyleLevel.defaultTabSize_	= pptxStyle.defTabSz.is_init();
		pStyleLevel.defaultTabSize	= pptxStyle.defTabSz.get_value_or(376300);

		pStyleLevel.leftMargin_	= pptxStyle.marL.is_init();
		pStyleLevel.leftMargin	= pptxStyle.marL.get_value_or(0);

		pStyleLevel.indent_	= pptxStyle.indent.is_init();
		pStyleLevel.indent	= pptxStyle.indent.get_value_or(0);

		pStyleLevel.align_			= pptxStyle.algn.is_init();
		pStyleLevel.textAlignment	= GetTextAlignFromStr(pptxStyle.algn.get_value_or("l"));

		pStyleLevel.tabStops_	= (pptxStyle.tabLst->size() > 0);
		for(std::list<PPTX::Logic::Tab>::const_iterator i = pptxStyle.tabLst->begin(); i != pptxStyle.tabLst->end(); i++)
			pStyleLevel.tabsStops.Add(i->pos.get_value_or(pStyleLevel.defaultTabSize));

		pStyleLevel.textDirection_	= pptxStyle.rtl.is_init();
		pStyleLevel.textDirectional	= (pptxStyle.rtl.get_value_or(false))?1:0;
/*
		WORD lIndentLevel;

		bool hasBullet_;
		bool bulletHasFont_;
		bool bulletHasColor_;
		bool bulletHasSize_;
		bool bulletFont_;
		bool bulletColor_;
		bool bulletSize_;
		bool bulletChar_;

		bool fontAlign_;
		bool charWrap_;
		bool wordWrap_;
		bool overflow_;
		bool bulletBlip_;

		bool bulletScheme_;
		bool bulletHasScheme_;
		
		WORD bulletFlag;
		WCHAR bulletChar;
		WORD bulletFontRef;
		WORD bulletSize;

		SColorAtom bulletColor;

		WORD fontAlign;
		WORD wrapFlags;
*/
/*
			BulletColor buColor;
			BulletSize buSize;
			BulletTypeface buTypeface;
			Bullet ParagraphBullet;

			// Attribs
			nullable_property<bool> eaLnBrk;						// (East Asian Line Break)
			nullable_property<std::string, Limit::FontAlign> fontAlgn;// (Font Alignment)
			nullable_property<bool> hangingPunct;					// (Hanging Punctuation)
			nullable_property<bool> latinLnBrk;						// (Latin Line Break)
			nullable_property<int, setter::between<int, 0, 51206400> > marR;// (Right Margin)
*/
	}

	void Converter::RunPropertiesConvert(const nullable_property<PPTX::Logic::RunProperties>& pRunProps, NSPresentationEditor::STextCFRun& pCFRun)
	{
		if(pRunProps.is_init())
			RunPropertiesConvert(*pRunProps, pCFRun);
	}

	void Converter::RunPropertiesConvert(const PPTX::Logic::RunProperties& pRunProps, NSPresentationEditor::STextCFRun& pCFRun)
	{
		pCFRun.hasBold = pRunProps.b.get_value_or(false);
		if(pCFRun.hasBold)
			pCFRun.fontStyle |= 0x01;

		pCFRun.hasItalic = pRunProps.i.get_value_or(false);
		if(pCFRun.hasItalic)
			pCFRun.fontStyle |= 0x02;

		pCFRun.hasUnderline = (pRunProps.u.get_value_or("none") != "none");
		if(pCFRun.hasUnderline)
			pCFRun.fontStyle |= 0x04;

		pCFRun.hasSize = pRunProps.sz.is_init();
		pCFRun.fontSize = pRunProps.sz.get_value_or(1000)/100;

		pCFRun.StrikeOut = GetTextStrike(pRunProps.strike.get_value_or("noStrike"));

		int baseline = pRunProps.baseline.get_value_or(0);
		pCFRun.hasBaselineOffset = (baseline != 0);
		if(pCFRun.hasBaselineOffset)
			pCFRun.BaselineOffset = baseline / 1000.0;//(baseline / 100000.0) * CFRun.fontSize * (25.4 / 72);

//		CFRun.strFontName = CStringW(TextProps.GetRunFont(level, lpSpTextRun).c_str());
		//выше может быть указатель на текстовую схему !!!
/*
		CFRun.hasColor = true;
		if(isHyperlink)
			CFRun.oColor.FromValue(TextProps.GetHyperlinkABGR());
		else
			CFRun.oColor.FromValue(TextProps.GetRunABGR(level, lpSpTextRun));
*/
	 return;
/*
		bool hasShadow;
		bool hasFehint;
		bool hasKimi;

		bool hasEmboss;
		BYTE hasStyle;

		bool hasTypeface;
		bool hasColor;
		bool hasPosition;
		bool hasPp10ext;
		bool hasOldEATypeface;
		bool hasAnsiTypeface;
		bool hasSymbolTypeface;

		bool hasNewEATypeface;
		bool hasCsTypeface;
		bool hasPp11ext;

		WORD fontStyle;
		WORD fontRef;
		WORD fontEAFontRef;
		WORD ansiFontRef;
		WORD symbolFontRef;
		SColorAtom oColor;
		WORD position;

		CStringW strFontName;
*/
/*
			nullable_property<Ln> ln;
			UniFill Fill;
			EffectProperties EffectList;
			//highlight (Highlight Color)  §21.1.2.3.4 
			//uLn (Underline Stroke)  §21.1.2.3.14 
			//uLnTx (Underline Follows Text)  §21.1.2.3.15 
			//uFill (Underline Fill)  §21.1.2.3.12 
			//uFillTx (Underline Fill Properties Follow Text)  §21.1.2.3.13 
			nullable_property<TextFont> latin;
			nullable_property<TextFont> ea;
			nullable_property<TextFont> cs;
			nullable_property<TextFont> sym;
			nullable_property<Hyperlink> hlinkClick;
			nullable_property<Hyperlink> hlinkMouseOver;
			//rtl (Right to Left Run)  §21.1.2.2.8 

			// Attributes
			nullable_property<std::string> altLang;
			nullable_property<std::string> bmk;
			nullable_property<std::string, Limit::TextCaps> cap;
			nullable_property<bool> dirty;
			nullable_property<bool> err;
			nullable_property<int, setter::between<int, 0, 400000> > kern;
			nullable_property<bool> kumimoji;
			nullable_property<std::string> lang;
			nullable_property<bool> noProof;
			nullable_property<bool> normalizeH;
			nullable_property<bool> smtClean;
			nullable_property<int, setter::only_positive<int> > smtId;
			nullable_property<int, setter::between<int, -4000, 4000> > spc;
*/
	}
}
