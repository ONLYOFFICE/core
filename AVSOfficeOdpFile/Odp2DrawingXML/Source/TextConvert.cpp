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
		pText.m_lTextType = 0;
		pText.m_arParagraphs.RemoveAll();
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
		pText.m_lTextType = 0;
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
		CParagraph oParagraph;
		oParagraph.m_lTextType	= 0;
		oParagraph.m_lTextLevel = level;

		CTextPFRun* pRunPF = &oParagraph.m_oPFRun;
			
		pRunPF->hasBullet = (BOOL)((level != 0) ? TRUE : FALSE);
		pRunPF->leftMargin = (LONG)0;

		LONG lMinLabelOffset = 0;
		if (pRunPF->hasBullet.get())
		{
			pRunPF->bulletColor = CColor::CreateColor(0x00);
			pRunPF->bulletSize	= (WORD)100;
			pRunPF->bulletChar	= (WCHAR)'*';

			try
			{
				if (listStyle.listLevelStyleProperties(level).ListLevel.is_init())
				{
					pRunPF->leftMargin.get() += (LONG)(listStyle.listLevelStyleProperties(level).ListLevel->SpaceBefore.is_init()?
						listStyle.listLevelStyleProperties(level).ListLevel->SpaceBefore->value(UniversalUnit::Mm):0);

					if (listStyle.listLevelStyleProperties(level).ListLevel->MinLabelWidth.is_init())
						lMinLabelOffset = (LONG)listStyle.listLevelStyleProperties(level).ListLevel->MinLabelWidth->value(UniversalUnit::Mm);
				}
			}
			catch(...)
			{
			}
		}

		if (ParentProps.Paragraph.is_init())
		{
			pRunPF->textAlignment = GetTextAlignFromStr(ParentProps.Paragraph->Align.is_init()?ParentProps.Paragraph->Align->ToString():"left");

			pRunPF->leftMargin.get() += (LONG)(ParentProps.Paragraph->MarginLeft.is_init()? (ParentProps.Paragraph->MarginLeft->value(UniversalUnit::Mm)) : 0);
			//pfRun.leftMargin *= 10;// TODO : UniversalUnit !!!

			pRunPF->indent = (LONG)(ParentProps.Paragraph->TextIndent.is_init()? (ParentProps.Paragraph->TextIndent->value(UniversalUnit::Mm)) : 0);
			//pfRun.indent *= 10;// TODO : UniversalUnit !!!

			if (0 == pRunPF->leftMargin.get() && 0 == pRunPF->indent.get())
			{
				pRunPF->indent.get()		= -lMinLabelOffset;
				pRunPF->leftMargin.get()	= lMinLabelOffset;
			}

			//pfRun.defaultTabSize_ = true;
			//pfRun.defaultTabSize = ParentProps.Paragraph->TabStopDistance.is_init()? (ParentProps.Paragraph->TabStopDistance->value() * 10) : 0;
			//pfRun.defaultTabSize *= 10;// TODO : UniversalUnit !!!

			//paragraph.fontAlign_ = true;
			//paragraph.fontAlign = GetFontAlignFromStr(TextProps.GetParagraphFontAlgn(level, *SpTextPar));
			//
			//if(TextProps.GetParagraphLatinLnBrk(level, *SpTextPar))
			//{
			//	paragraph.charWrap_ = true;
			//	paragraph.wordWrap_ = false;
			//	paragraph.wrapFlags &= 0xFD;
			//	paragraph.wrapFlags |= 0x01;
			//
			//	paragraph.overflow_ = true;
			//}
			//else
			//{
				pRunPF->wrapFlags = (WORD)0;
				pRunPF->wrapFlags.get() &= 0xFE;
				pRunPF->wrapFlags.get() |= 0x02;
			//}

			//paragraph.textDirection_ = true;
			//paragraph.textDirectional = (TextProps.GetParagraphRtl(level, *SpTextPar))?1:0;

			if (ParentProps.Paragraph->LineHeight.is_init())
			{
				if (ParentProps.Paragraph->LineHeight->Type == UniversalUnit::Percent)
					pRunPF->lineSpacing = (LONG)ParentProps.Paragraph->LineHeight->value(UniversalUnit::Percent);
				else
					//pfRun.lineSpacing = -ParentProps.Paragraph->LineHeight->value(UniversalUnit::Mm);
					pRunPF->lineSpacing = 100;
			}

			pRunPF->spaceBefore = (LONG)(ParentProps.Paragraph->MarginTop.is_init() ? (-ParentProps.Paragraph->MarginTop->value(UniversalUnit::Mm)) : 0);
			pRunPF->spaceAfter  = (LONG)(ParentProps.Paragraph->MarginBottom.is_init() ? (-ParentProps.Paragraph->MarginBottom->value(UniversalUnit::Mm)) : 0);
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

			CSpan oSpan;
			std::wstring text = Encoding::utf82unicode(pParagraph.Items->at(runs).toTxt());
			if (0 == text.length())
				text = L"\n";
			
			oSpan.m_strText = (CString)text.c_str();
			CTextCFRun* pRunCF = &oSpan.m_oRun;

			if (properties.Text.is_init())
			{
				if (properties.Text->FontSize.is_init())
					pRunCF->Size = (WORD)properties.Text->FontSize.get();
				else
					pRunCF->Size = (WORD)12;

				pRunCF->FontBold		= (BOOL)properties.Text->Bold.get_value_or(false);
				pRunCF->FontItalic		= (BOOL)properties.Text->Italic.get_value_or(false);
				pRunCF->FontUnderline	= (BOOL)((properties.Text->UnderlineType.get_value_or("none") == "none") ? FALSE : TRUE);
				
				CString strFontName = CString(properties.Text->FontName.get_value_or(properties.Text->FontFamily.get_value_or("Times New Roman")).c_str());
				int pos = strFontName.Find(L"'");
				while (pos >= 0)
				{
					strFontName.Delete(pos);
					pos = strFontName.Find(L"'");
				}
				pRunCF->FontProperties = new NSPresentationEditor::CFontProperties();
				pRunCF->FontProperties->strFontName = strFontName;

				pRunCF->Color = CColor::CreateColor(0x00);
				if (properties.Text->FontColor.is_init())
				{
					pRunCF->Color->R = (BYTE)properties.Text->FontColor->Red.get();
					pRunCF->Color->G = (BYTE)properties.Text->FontColor->Green.get();
					pRunCF->Color->B = (BYTE)properties.Text->FontColor->Blue.get();
				}
			}
			oParagraph.m_arSpans.Add(oSpan);
		}

		//pText.m_sText += L"\n";
		//STextCFRun cfEnd;
		//cfEnd.lCount = 1;
		//pText.m_arCFs.Add(cfEnd);
		//pfRun.lCount++;
		if (((oParagraph.m_arSpans.GetCount() == 0) || 
			((oParagraph.m_arSpans.GetCount() == 1) && oParagraph.m_arSpans[0].m_strText == _T(""))) && 
			(level == 1))
		{
			pRunPF->hasBullet = (BOOL)FALSE;
		}
		pText.m_arParagraphs.Add(oParagraph);
	}
}