
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Styles.h"
#include <boost/foreach.hpp>
#include <vector>
//#include "Private/TextProperty.h"
//#include "Private/ParagraphProperty.h"
#include "Private/NamedStyle.h"


namespace Docx2Odt
{

	Styles::Styles(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
		: Base(odtFolder, docxFileContainer)
	{
	}

	void Styles::convert(const OOX::Styles& ooxStyles, Odt::Folder& odtFolder)
	{
		convertDefaultStyles(*odtFolder.Styles->Styles->DefaultStyles, ooxStyles);
		convertNamedStyles(*odtFolder.Styles->Styles->NamedStyles, ooxStyles.Named);

		// TODO исправить после реализации стилей
		//convert(*odtFolder.Styles->Styles, docx);
		//convert(*odtFolder.Styles->Styles, docx.Styles);

		// TODO исправить после реализации стелей
		//std::string automaticStyleName = odtFolder.Styles->MasterStyle->pageLayoutName("Standard");
		//convert(*odtFolder.Styles->AutomaticStyles, automaticStyleName, docx.Document->Body->SectPr);		

		// TODO исправить после реализации стилей
//		if (docx.Numbering.is_init())
//			convert(*odtFolder.Styles->getStyles(), docx.Numbering);
	}


	// TODO видимо, надо выделить класс Numbering
	void Styles::convert(const OOX::Numbering& ooxNumbering, Odt::Folder& odtFolder)
	{
		odtFolder.Content->ListStyles->clear();
		BOOST_FOREACH(const OOX::Numbering::AbstractNum& abstractNum, *ooxNumbering.AbstractNums)
		{
			odtFolder.Content->ListStyles->push_back(abstractNum2ListStyle(abstractNum));
		}
	}


	void Styles::convertDefaultStyles(Odt::Logic::DefaultStyles& odtDefaultStyles, const OOX::Styles& ooxStyles)
	{
		Odt::Logic::DefaultStyle& odtDefaultStyle = odtDefaultStyles.find("paragraph");
		//OOX::Theme::File theme;
		//if (m_docxFileContainer.exist<OOX::Theme::File>())
		//	theme = m_docxFileContainer.find<OOX::Theme::File>();

		OOX::Styles::Style defaultParagraphStyle  = ooxStyles.GetDefaultStyle("paragraph");
		Odt::Logic::Property::Paragraph parProperty = Private::NamedStyle::ppr2ppr(ooxStyles.Default->ParagraphProperty);
													//Private::ParagraphProperty::paragraphProperty2paragraphProperty(ooxStyles.Default->ParagraphProperty);
		if (defaultParagraphStyle.ParagraphProperty.is_init())
		{
			Odt::Logic::Property::Paragraph pPr = Private::NamedStyle::ppr2ppr(defaultParagraphStyle.ParagraphProperty);
												//Private::ParagraphProperty::paragraphProperty2paragraphProperty(defaultParagraphStyle.ParagraphProperty);
			parProperty = Odt::Logic::Property::Paragraph::merge(pPr, parProperty);
		}
		odtDefaultStyle.Properties->Paragraph = parProperty;


		Private::NamedStyle StyleConverter(m_odtFolder, m_docxFileContainer);
		OOX::Styles::Style defaultRunStyle	= ooxStyles.GetDefaultStyle("character");
		Odt::Logic::Property::Text runProperty = StyleConverter.rpr2tpr(ooxStyles.Default->RunProperty/*, theme*/);
												//Private::TextProperty::runProperty2textProperty(ooxStyles.Default->RunProperty, theme);
		if (defaultParagraphStyle.RunProperty.is_init())
		{
			Odt::Logic::Property::Text rPr = StyleConverter.rpr2tpr(defaultParagraphStyle.RunProperty/*, theme*/);
											//Private::TextProperty::runProperty2textProperty(defaultParagraphStyle.RunProperty, theme);
			runProperty = Odt::Logic::Property::Text::merge(runProperty, rPr);
		}
		if (defaultRunStyle.RunProperty.is_init())
		{
			Odt::Logic::Property::Text rPr = StyleConverter.rpr2tpr(defaultRunStyle.RunProperty/*, theme*/);
											//Private::TextProperty::runProperty2textProperty(defaultRunStyle.RunProperty, theme);
			runProperty = Odt::Logic::Property::Text::merge(runProperty, rPr);
		}
		odtDefaultStyle.Properties->Text = runProperty;		
	}


	void Styles::convertNamedStyles(Odt::Logic::NamedStyles& odtStyles, const std::vector<OOX::Styles::Style>& ooxStyles)
	{		
		BOOST_FOREACH(const OOX::Styles::Style& ooxStyle, ooxStyles)
		{
			//if(*ooxStyle.Type == "table")
			//{
			//}
			//else
			//{
				Odt::Logic::NamedStyle odtStyle;
				odtStyle = convertNamedStyle(ooxStyle);
				odtStyles.push_back(odtStyle);
			//}
		}
	}


	const Odt::Logic::NamedStyle Styles::convertNamedStyle(const OOX::Styles::Style& ooxStyle)
	{
		Odt::Logic::NamedStyle odtStyle;
		//OOX::Theme::File theme;
		//if (m_docxFileContainer.exist<OOX::Theme::File>())
		//	theme = m_docxFileContainer.find<OOX::Theme::File>();

		if (ooxStyle.Type == "paragraph")
			odtStyle.Family = "paragraph";

		if (ooxStyle.QFormat)
		{
			odtStyle.DisplayName = ooxStyle.StyleId;
		}
		odtStyle.Name = ooxStyle.StyleId;
		odtStyle.ParentName = ooxStyle.BasedOn;


		Private::NamedStyle StyleConverter(m_odtFolder, m_docxFileContainer);
		if (ooxStyle.ParagraphProperty.is_init())
			odtStyle.Properties->Paragraph = StyleConverter.ppr2ppr(ooxStyle.ParagraphProperty);

		if (ooxStyle.RunProperty.is_init())
			odtStyle.Properties->Text = StyleConverter.rpr2tpr(ooxStyle.RunProperty/*, theme*/);

//		if (odtStyle.Properties->Text.is_init())
//			ooxStyle.RunProperty = Private::RunProperty::convert(odtStyle.Properties->Text);

		return odtStyle;
	}


	// TODO исправить после реализации стилей
	//void Styles::convert(Odt::Styles::Styles& odt, const Docx::Folder& docx)
	//{
	//	if(docx.FootNote.is_init() || docx.EndNote.is_init())
	//	{
	//		odt.NotesConfigurations->clear();
	//		odt.NotesConfigurations->push_back(foot2NotesConfiguration(docx.Document->Body->SectPr->FootnotePr));			
	//		odt.NotesConfigurations->push_back(end2NotesConfiguration(docx.Document->Body->SectPr->EndnotePr));
	//	}
	//}


	// TODO исправить после реализации стилей
/*
	void Styles::convert(Odt::Styles::Styles& odt, const OOX::Styles::File& oox)
	{		
		BOOST_FOREACH(const OOX::Styles::Style& style, *oox.Styles)
		{
			// TODO устаревшая
//			if(isLibStyle(style))
//			{
				// TODO исправить после реализации стилей
				Odt::Logic::Style odtStyle = libraryStyle2libraryStyle(style);				
				// TODO исправить после реализации стилей
				odt.getStyles()->push_back(odtStyle);
//			}
		}
	}
*/

	// TODO исправить после реализации стилей
/*
	void Styles::convert(Odt::Logic::Styles& styles, const OOX::Numbering::File& oox)
	{
		styles.ListStyles->clear();
		BOOST_FOREACH(const OOX::Numbering::AbstractNum& abstractNum, *oox.AbstractNums)
		{
			styles.ListStyles->push_back(abstractNum2ListStyle(abstractNum));
		}
	}
*/

	const Odt::Logic::ListStyle Styles::abstractNum2ListStyle(const OOX::Numbering::AbstractNum& abstractNum)
	{
		Odt::Logic::ListStyle listStyle;
		listStyle.Name = "L" + ToString(abstractNum.Id + 1);
		BOOST_FOREACH(const OOX::Numbering::Level& level, *abstractNum.Levels)
		{
			if(level.NumFmt->isBullet())
			{
				listStyle.Items->push_back(lvl2listLevelStyleBullet(level));
			}
			else
			{
				listStyle.Items->push_back(lvl2listLevelStyleNumber(level));
			}
		}
		return listStyle;
	}


	const Odt::Logic::ListLevelStyleBullet Styles::lvl2listLevelStyleBullet(const OOX::Numbering::Level& level)
	{
		Odt::Logic::ListLevelStyleBullet listLevelStyleBullet;
		listLevelStyleBullet.Style = "Standard";/***/
		listLevelStyleBullet.Level = level.Ilvl + 1;
		listLevelStyleBullet.Bullet = level.Text;
		if (level.ParagraphProperty.is_init())
			listLevelStyleBullet.Properties->ListLevel = paragraphProperty2ListLevelProperties(level.ParagraphProperty);
		if (level.RunProperty.is_init())
		{
//			OOX::Theme::File theme;
			//TODO после снятия static исправить
			//if (m_docxFileContainer.exist<OOX::Theme::File>())
			//	theme = m_docxFileContainer.find<OOX::Theme::File>();
			Private::NamedStyle StyleConverter(m_odtFolder, m_docxFileContainer);
			listLevelStyleBullet.Properties->Text = StyleConverter.rpr2tpr(level.RunProperty/*, theme*/);
													//Private::TextProperty::runProperty2textProperty(level.RunProperty, theme);
		}
		
		return listLevelStyleBullet;
	}


	const Odt::Logic::ListLevelStyleNumber Styles::lvl2listLevelStyleNumber(const OOX::Numbering::Level& level)
	{
		Odt::Logic::ListLevelStyleNumber listLevelStyleNumber;
		listLevelStyleNumber.Style = "Standard";/***/
		listLevelStyleNumber.Level = level.Ilvl + 1;
		listLevelStyleNumber.NumFormat = *level.NumFmt;
		listLevelStyleNumber.StartValue = level.Start;
		listLevelStyleNumber.Suffix = lvlText2suffix(level.Text);
		listLevelStyleNumber.Prefix = lvlText2prefix(level.Text);
		if (level.ParagraphProperty.is_init())
			listLevelStyleNumber.Properties->ListLevel = paragraphProperty2ListLevelProperties(level.ParagraphProperty);

		if (level.RunProperty.is_init())
		{
//			OOX::Theme::File theme;
			//TODO после снятия static исправить
			//if (m_docxFileContainer.exist<OOX::Theme::File>())
			//	theme = m_docxFileContainer.find<OOX::Theme::File>();
			Private::NamedStyle StyleConverter(m_odtFolder, m_docxFileContainer);
			listLevelStyleNumber.Properties->Text = StyleConverter.rpr2tpr(level.RunProperty/*, theme*/);
													//Private::TextProperty::runProperty2textProperty(level.RunProperty, theme);
		}
		
		return listLevelStyleNumber;
	}

	const Odt::Logic::Property::ListLevel Styles::paragraphProperty2ListLevelProperties(const OOX::Logic::ParagraphProperty& ooxProperty)
	{
		Odt::Logic::Property::ListLevel odtProperties;
		if (ooxProperty.Ind.is_init())
		{
			if(ooxProperty.Ind->Hanging.is_init() && ooxProperty.Ind->Left.is_init())
			{
				odtProperties.SpaceBefore	 = UniversalUnit(ooxProperty.Ind->Left->value(UniversalUnit::Emu) - ooxProperty.Ind->Hanging->value(UniversalUnit::Emu), UniversalUnit::Emu);
				odtProperties.SpaceBefore->Type = UniversalUnit::Cm;
				odtProperties.MinLabelWidth = UniversalUnit(ooxProperty.Ind->Hanging->value(UniversalUnit::Emu), UniversalUnit::Emu);
				odtProperties.MinLabelWidth->Type = UniversalUnit::Cm;
			}
			else if (ooxProperty.Ind->Left.is_init())
			{
				odtProperties.SpaceBefore	 = UniversalUnit(ooxProperty.Ind->Left->value(UniversalUnit::Emu), UniversalUnit::Emu);
				odtProperties.SpaceBefore->Type = UniversalUnit::Cm;
				odtProperties.MinLabelWidth = 0;
				odtProperties.MinLabelWidth->Type = UniversalUnit::Cm;
			}
		}
		return odtProperties;
	}


	const nullable<std::string> Styles::lvlText2suffix(const std::string& text)
	{
		nullable<std::string> suffix;
		const size_t pos = text.find_first_of("%");
		if (pos != std::string::npos)
			suffix = text.substr(pos + 2, text.size() - pos - 2);
		return suffix;
	}

	const nullable<std::string> Styles::lvlText2prefix(const std::string& text)
	{
		nullable<std::string> prefix;
		const size_t pos = text.find_first_of("%");
		if (pos != 0 && pos != std::string::npos)
			prefix = text.substr(0, pos);
		return prefix;
	}


	const Odt::Styles::NotesConfiguration Styles::foot2NotesConfiguration(const OOX::Logic::FootNoteProperty& oox)
	{
		Odt::Styles::NotesConfiguration odt;
		odt.NoteClass					= "footnote";
		odt.NumFormat					= *oox.NumFormat;
		odt.StartValue				= *oox.NumStart - 1;
		odt.FootnotesPosition	= "page";
		odt.StartNumberingAt	= "document";
		return odt;
	}


	const Odt::Styles::NotesConfiguration Styles::end2NotesConfiguration(const OOX::Logic::EndNoteProperty& oox)
	{
		Odt::Styles::NotesConfiguration odt;
		odt.NoteClass				= "endnote";
		odt.NumFormat				= *oox.NumFormat;
		odt.StartValue			= *oox.NumStart - 1;
		odt.MasterPageName	= "Endnote";
		return odt;
	}

	// TODO исправить после реализации стилей
/*
	const Odt::Logic::Style Styles::libraryStyle2libraryStyle(const OOX::Styles::Style& oox)
	{
		Odt::Logic::Style odt;

		odt.Name				= oox.StyleId;		
		odt.Family			= oox.Type;		
		odt.ParentName	= oox.BasedOn;		
		odt.Class = "text";

		if(oox.ParagraphProperty.is_init())
		{
			Odt::Logic::Property::Paragraph paragraph;
			paragraph.Align				= oox.ParagraphProperty->Align;
			paragraph.Background	= oox.ParagraphProperty->Background;
			if (oox.ParagraphProperty->Ind.is_init())
			{
				paragraph.MarginLeft  = oox.ParagraphProperty->Ind->Left;
				paragraph.MarginRight = oox.ParagraphProperty->Ind->Right;
			}
			if(oox.ParagraphProperty->Spacing.is_init())
			{
				if (!oox.ParagraphProperty->Spacing->BeforeAutospacing.is_init() || oox.ParagraphProperty->Spacing->BeforeAutospacing == 0)
					paragraph.MarginTop			= oox.ParagraphProperty->Spacing->Before;
				if (!oox.ParagraphProperty->Spacing->AfterAutospacing.is_init() || oox.ParagraphProperty->Spacing->AfterAutospacing == 0)
					paragraph.MarginBottom	= oox.ParagraphProperty->Spacing->After;
				
				if(oox.ParagraphProperty->Spacing->LineRule.is_init())
					if (oox.ParagraphProperty->Spacing->LineRule == "auto")
						paragraph.LineHeight = *oox.ParagraphProperty->Spacing->Line * 100 / 240 ;
					else if (oox.ParagraphProperty->Spacing->LineRule == "atLeast")
						paragraph.LineHeightAtLeast = oox.ParagraphProperty->Spacing->Line;
			}
			odt.Properties->Paragraph	= paragraph;	
		}

		if(oox.RunProperty.is_init())
		{
			odt.Properties->Text = runProperty2textProperties(*oox.RunProperty);
		}

		return odt;
	}
*/
} // namespace Docx2Odt