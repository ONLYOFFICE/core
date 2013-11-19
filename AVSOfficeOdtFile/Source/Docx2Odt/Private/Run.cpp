
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Run.h"
#include "TextItem.h"


namespace Docx2Odt
{
	namespace Private
	{

		Run::Run(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		const Odt::Content::ParagraphItem Run::endnote2run(const OOX::EndNote::Note& ooxEndnote) const
		{
			Odt::Content::ParagraphItem odtRun;
			Odt::Content::Note odtNote = endnote2note(ooxEndnote);
			odtRun.create(odtNote);
			return odtRun;
		}


		const Odt::Content::ParagraphItem Run::footnote2run(const OOX::FootNote::Note& ooxFootnote) const
		{
			Odt::Content::ParagraphItem odtRun;
			Odt::Content::Note odtNote = footnote2note(ooxFootnote);
			odtRun.create(odtNote);
			return odtRun;
		}


		const Odt::Content::Note Run::endnote2note(const OOX::EndNote::Note& ooxEndnote) const
		{
			Odt::Content::Note odtNote;
			odtNote.Class = "endnote";

			TextItem converter(m_odtFolder, m_docxFileContainer.find<OOX::EndNote>());
			converter.converterProperties = converterProperties;
			converter.convert(ooxEndnote.Items, *odtNote.Texts);

			return odtNote;
		}


		const Odt::Content::Note Run::footnote2note(const OOX::FootNote::Note& ooxFootnote) const
		{
			Odt::Content::Note odtNote;
			odtNote.Class = "footnote";

			TextItem converter(m_odtFolder, m_docxFileContainer.find<OOX::FootNote>());
			converter.converterProperties = converterProperties;
			converter.convert(ooxFootnote.Items, *odtNote.Texts);

			return odtNote;
		}

		// TODO исправить после реализации стилей
/*
		const Odt::Logic::Style Run::rpr2style(const OOX::Logic::RunProperty& ooxProperty)
		{
			Odt::Logic::Style odtStyle;
			odtStyle.notForLibrary();

			// TODO исправить после реализации стилей
//			odtStyle.Name		=  m_odtFolder.styles()->name("T");
			odtStyle.Family	= "text";

			odtStyle.Properties->Text = rpr2tpr(ooxProperty);

			// TODO исправить после реализации стилей
//			m_odtFolder.styles()->push_back(odtStyle);

			return odtStyle;
		}
*/

			// TODO исправить после реализации стилей
/*
		const Odt::Logic::Style Run::rpr2style(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Properties& odtProperties)
		{
			Odt::Logic::Style odtStyle;
			odtStyle.notForLibrary();

			// TODO исправить после реализации стилей
//			odtStyle.Name		=  m_odtFolder.styles()->name("T");
			odtStyle.Family	= "text";

			odtStyle.Properties->Text = rpr2tpr(ooxProperty, odtProperties.Text);

			// TODO исправить после реализации стилей
//			m_odtFolder.styles()->push_back(odtStyle);

			return odtStyle;
		}
*/
/*
		const Odt::Logic::Property::Text Run::rpr2tpr(const OOX::Logic::RunProperty& ooxProperty, const OOX::Theme::File& theme) const
		{
			Odt::Logic::Property::Text odtProperty;
			odtProperty.Bold		= ooxProperty.Bold;
			odtProperty.Italic	= ooxProperty.Italic;
			odtProperty.Under		= ooxProperty.Under;

			if (ooxProperty.Strike == true)
				odtProperty.LineThroughStyle = true;

			if (ooxProperty.DStrike ==true)
			{
				odtProperty.LineThroughStyle = true;
				odtProperty.LineThroughType = "double";
			}

			if(ooxProperty.RFonts.is_init())
			{
				std::string fontType = ooxProperty.RFonts->fontType();
				if (fontType == "fontName")
					odtProperty.FontName = ooxProperty.RFonts->fontName();
				else if (fontType == "minor")
					odtProperty.FontName = theme.GetMinorFont();
				else if (fontType == "major")
					odtProperty.FontName = theme.GetMajorFont();
			}

			if (ooxProperty.Caps.get_value_or(false))
				odtProperty.TextTransform = "uppercase";
			if (ooxProperty.SmallCaps.get_value_or(false))
				odtProperty.FontVariant = "small-caps";
			if(ooxProperty.Emboss.get_value_or(false))
				odtProperty.Relief = "embossed";
			if(ooxProperty.Imprint.get_value_or(false))
				odtProperty.Relief = "engraved";
			if(ooxProperty.Vanish.get_value_or(false))
				odtProperty.Display = "none";
			if(ooxProperty.Shadow.get_value_or(false))
				odtProperty.Shadow = "1pt 1pt";
			if(ooxProperty.Outline.get_value_or(false))
				odtProperty.Outline = true;

			if (ooxProperty.FontSize.is_init())
				odtProperty.FontSize = ooxProperty.FontSize.get() / 2;
			odtProperty.FontColor		= ooxProperty.FontColor;
			odtProperty.Background		= ooxProperty.Background;
			odtProperty.Index			= ooxProperty.Index;

			return odtProperty;
		}


		const Odt::Logic::Property::Text Run::rpr2tpr(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Property::Text& odtParagraphProperty, const OOX::Theme::File& theme) const
		{
			Odt::Logic::Property::Text odtRunProperty;

			odtRunProperty.Bold		= ooxProperty.Bold;
			odtRunProperty.Italic	= ooxProperty.Italic;
			odtRunProperty.Under	= ooxProperty.Under;

			if (ooxProperty.Strike == true)
				odtRunProperty.LineThroughStyle = true;

			if (ooxProperty.DStrike ==true)
			{
				odtRunProperty.LineThroughStyle = true;
				odtRunProperty.LineThroughType = "double";
			}

			if(ooxProperty.RFonts.is_init())
			{
				std::string fontType = ooxProperty.RFonts->fontType();
				if (fontType == "fontName")
					odtRunProperty.FontName = ooxProperty.RFonts->fontName();
				else if (fontType == "minor")
					odtRunProperty.FontName = theme.GetMinorFont();
				else if (fontType == "major")
					odtRunProperty.FontName = theme.GetMajorFont();
			}

			if (ooxProperty.Caps.get_value_or(false))
				odtRunProperty.TextTransform = "uppercase";
			if (ooxProperty.SmallCaps.get_value_or(false))
				odtRunProperty.FontVariant = "small-caps";
			if(ooxProperty.Emboss.get_value_or(false))
				odtRunProperty.Relief = "embossed";
			if(ooxProperty.Imprint.get_value_or(false))
				odtRunProperty.Relief = "engraved";
			if(ooxProperty.Vanish.get_value_or(false))
				odtRunProperty.Display = "none";
			if(ooxProperty.Shadow.get_value_or(false))
				odtRunProperty.Shadow = "1pt 1pt";
			if(ooxProperty.Outline.get_value_or(false))
				odtRunProperty.Outline = true;

			if (ooxProperty.FontSize.is_init())
				odtRunProperty.FontSize = ooxProperty.FontSize.get() / 2;
			odtRunProperty.FontColor	= ooxProperty.FontColor;
			odtRunProperty.Background	= ooxProperty.Background;
			odtRunProperty.Index		= ooxProperty.Index;

			return odtRunProperty;
		}
*/
	} // namespace Prviate
} // namespace Docx2Odt