#include "stdafx.h"
#include "./../Converter.h"
#include "OdtFormat/Content/List.h"
#include "OdtFormat/Content/Span.h"
#include "OdtFormat/Content/ParagraphItem.h"
#include "Utility.h"

namespace Odp2DrawingXML
{
	void Converter::TextVectorConvert(const std::vector<Odt::Content::Text>& pTextBox, CTextAttributesEx& pText, const Odt::Logic::Properties& ParentProps, bool master)const
	{
		pText.m_sText = L"";
		pText.m_oAttributes.m_nTextAlignVertical = 
			ParentProps.Graphic.is_init() ? GetTextAnchorFromStr(ParentProps.Graphic->TextareaVerticalAlign.get_value_or("top")) : GetTextAnchorFromStr("top");
		Odt::Logic::ListStyle listStyle;
		for(size_t i = 0; i < pTextBox.size(); i++)
		{
			TextConvert(pTextBox.at(i), pText, ParentProps, listStyle, 0, master);
		}
	}

	void Converter::TextConvert(const Odt::Content::Text& pTextItem, CTextAttributesEx& pText, const Odt::Logic::Properties& ParentProps, const Odt::Logic::ListStyle listStyle , const int level, bool master)const
	{
		if(pTextItem.is<Odt::Content::Paragraph>())
		{
			Odt::Logic::Properties properties;
			if(pTextItem.as<Odt::Content::Paragraph>().Style.is_init())
			{
				std::string style = pTextItem.as<Odt::Content::Paragraph>().Style.get();
				properties = m_Folder->GetPropertiesFromStyle(style, master);
			}
			properties = Odt::Logic::Properties::merge(ParentProps, properties);
			ParagraphConvert(pTextItem.as<Odt::Content::Paragraph>(), pText, properties, listStyle, level, master);
		}
		else if(pTextItem.is<Odt::Content::List>())
		{
			for(size_t items = 0; items < pTextItem.as<Odt::Content::List>().Items->size(); items++)
			{
				for(size_t i = 0; i < pTextItem.as<Odt::Content::List>().Items->at(items).Texts->size(); i++)
				{
					Odt::Logic::ListStyle lStyle = listStyle;
					if(pTextItem.as<Odt::Content::List>().Style.is_init())
					{
						std::string style = pTextItem.as<Odt::Content::List>().Style.get();
						lStyle = m_Folder->GetListStyle(style, master);
						//properties = m_Folder->GetPropertiesFromListStyle(style, level + 1);
						//ParentProps.ListLevel = properties.ListLevel;
					}
//					properties = Odt::Logic::Properties::merge(ParentProps, properties);
					TextConvert(pTextItem.as<Odt::Content::List>().Items->at(items).Texts->at(i), pText, ParentProps, lStyle, level + 1, master);
				}
			}
		}
	}

	void Converter::ParagraphConvert(const Odt::Content::Paragraph& pParagraph, CTextAttributesEx& pText, const Odt::Logic::Properties& ParentProps, const Odt::Logic::ListStyle listStyle, const int level, bool master)const
	{
		STextPFRun pfRun;
		pfRun.lCount = 0;

		pfRun.lIndentLevel = level;

		pfRun.hasBullet_ = (level != 0);
		pfRun.leftMargin = 0;
		LONG lMinLabelOffset = 0;
		if(pfRun.hasBullet_)
		{
			pfRun.bulletHasColor_ = true;
			pfRun.bulletColor_ = true;
			pfRun.bulletColor.FromValue(0x0);

			pfRun.bulletHasSize_ = true;
			pfRun.bulletSize_ = true;
			pfRun.bulletSize = 100;
			
			pfRun.bulletChar_ = true;
			pfRun.bulletChar = L'*';

			pfRun.bulletHasFont_ = true;

			pfRun.bulletFlag = 0x0F;

			try
			{
				if(listStyle.listLevelStyleProperties(level).ListLevel.is_init())
				{
					pfRun.leftMargin += listStyle.listLevelStyleProperties(level).ListLevel->SpaceBefore.is_init()?
						listStyle.listLevelStyleProperties(level).ListLevel->SpaceBefore->value(UniversalUnit::Mm):0;

					if (listStyle.listLevelStyleProperties(level).ListLevel->MinLabelWidth.is_init())
						lMinLabelOffset = listStyle.listLevelStyleProperties(level).ListLevel->MinLabelWidth->value(UniversalUnit::Mm);
				}
			}
			catch(...)
			{
			}
		}

		if(ParentProps.Paragraph.is_init())
		{
			pfRun.align_ = true;
			pfRun.textAlignment = GetTextAlignFromStr(ParentProps.Paragraph->Align.is_init()?ParentProps.Paragraph->Align->ToString():"left");

			pfRun.leftMargin_ = true;
			pfRun.leftMargin += ParentProps.Paragraph->MarginLeft.is_init()? (ParentProps.Paragraph->MarginLeft->value(UniversalUnit::Mm)) : 0;
			//pfRun.leftMargin *= 10;// TODO : UniversalUnit !!!

			pfRun.indent_ = true;
			pfRun.indent = ParentProps.Paragraph->TextIndent.is_init()? (ParentProps.Paragraph->TextIndent->value(UniversalUnit::Mm)) : 0;
			//pfRun.indent *= 10;// TODO : UniversalUnit !!!

			if (0 == pfRun.leftMargin && 0 == pfRun.indent)
			{
				pfRun.indent		= -lMinLabelOffset;
				pfRun.leftMargin	= lMinLabelOffset;
			}

//			pfRun.defaultTabSize_ = true;
//			pfRun.defaultTabSize = ParentProps.Paragraph->TabStopDistance.is_init()? (ParentProps.Paragraph->TabStopDistance->value() * 10) : 0;
			//pfRun.defaultTabSize *= 10;// TODO : UniversalUnit !!!
/*
		paragraph.fontAlign_ = true;
		paragraph.fontAlign = GetFontAlignFromStr(TextProps.GetParagraphFontAlgn(level, *SpTextPar));

		if(TextProps.GetParagraphLatinLnBrk(level, *SpTextPar))
		{
			paragraph.charWrap_ = true;
			paragraph.wordWrap_ = false;
			paragraph.wrapFlags &= 0xFD;
			paragraph.wrapFlags |= 0x01;

			paragraph.overflow_ = true;
		}
		else
		{
*/
			pfRun.charWrap_ = false;
			pfRun.wordWrap_ = true;
			pfRun.wrapFlags &= 0xFE;
			pfRun.wrapFlags |= 0x02;

			pfRun.overflow_ = true;
/*
		}

		paragraph.textDirection_ = true;
		paragraph.textDirectional = (TextProps.GetParagraphRtl(level, *SpTextPar))?1:0;
*/
			pfRun.lineSpacing_ = ParentProps.Paragraph->LineHeight.is_init();
			if(pfRun.lineSpacing_)
			{
				if(ParentProps.Paragraph->LineHeight->Type == UniversalUnit::Percent)
					pfRun.lineSpacing = ParentProps.Paragraph->LineHeight->value(UniversalUnit::Percent);
				else
					//pfRun.lineSpacing = -ParentProps.Paragraph->LineHeight->value(UniversalUnit::Mm);
					pfRun.lineSpacing = 100;
			}

			pfRun.spaceBefore_ = true;
			pfRun.spaceBefore  = ParentProps.Paragraph->MarginTop.is_init() ? (-ParentProps.Paragraph->MarginTop->value(UniversalUnit::Mm)) : 0;

			pfRun.spaceAfter_ = true;
			pfRun.spaceAfter  = ParentProps.Paragraph->MarginBottom.is_init() ? (-ParentProps.Paragraph->MarginBottom->value(UniversalUnit::Mm)) : 0;
		}

		for(size_t runs = 0; runs < pParagraph.Items->size(); runs++)
		{
			Odt::Logic::Properties properties;
			if(pParagraph.Items->at(runs).is<Odt::Content::Span>())
			{
				if(pParagraph.Items->at(runs).as<Odt::Content::Span>().Style.is_init())
				{
					std::string style = pParagraph.Items->at(runs).as<Odt::Content::Span>().Style.get();
					properties = m_Folder->GetPropertiesFromStyle(style, master);
				}
			}
			properties = Odt::Logic::Properties::merge(ParentProps, properties);

			STextCFRun cfRun;
			std::wstring text = Encoding::utf82unicode(pParagraph.Items->at(runs).toTxt());
			if (0 == text.length())
				text = L"\n";
			cfRun.lCount = text.length();
			pfRun.lCount += cfRun.lCount;

			if(properties.Text.is_init())
			{
				if(properties.Text->FontSize.is_init())
					cfRun.fontSize = properties.Text->FontSize.get();
				else
					cfRun.fontSize = 12;
				cfRun.hasSize = true;

				cfRun.hasBold = properties.Text->Bold.get_value_or(false);
				if(cfRun.hasBold)
					cfRun.fontStyle |= 0x01;
				cfRun.hasItalic = properties.Text->Italic.get_value_or(false);
				if(cfRun.hasItalic)
					cfRun.fontStyle |= 0x02;
				//cfRun.hasUnderline = properties.Text->Under.get_value_or(false);
				if(cfRun.hasUnderline)
					cfRun.fontStyle |= 0x04;

				cfRun.strFontName = CStringW(properties.Text->FontName.get_value_or(properties.Text->FontFamily.get_value_or("Times New Roman")).c_str());
				int pos = cfRun.strFontName.Find(L"'");
				while(pos >= 0)
				{
					cfRun.strFontName.Delete(pos);
					pos = cfRun.strFontName.Find(L"'");
				}

				cfRun.hasColor = true;
				if(properties.Text->FontColor.is_init())
				{
					cfRun.oColor.R = (BYTE)properties.Text->FontColor->Red.get();
					cfRun.oColor.G = (BYTE)properties.Text->FontColor->Green.get();
					cfRun.oColor.B = (BYTE)properties.Text->FontColor->Blue.get();
				}
				else
					cfRun.oColor.FromValue(0, 0, 0);
			}
			pText.m_sText += CStringW(text.c_str());
			pText.m_arCFs.Add(cfRun);
		}

		//pText.m_sText += L"\n";

		//STextCFRun cfEnd;
		//cfEnd.lCount = 1;
		//pText.m_arCFs.Add(cfEnd);

		//pfRun.lCount++;
		if((pfRun.lCount == 1) && (level == 1))
		{
			pfRun.hasBullet_		= false;
			pfRun.bulletHasColor_	= false;
			pfRun.bulletColor_		= false;
			pfRun.bulletHasSize_	= false;
			pfRun.bulletSize_		= false;
			pfRun.bulletChar_		= false;
			pfRun.bulletHasFont_	= false;
			pfRun.bulletFlag = 0;
		}
		pText.m_arPFs.Add(pfRun);
	}
}