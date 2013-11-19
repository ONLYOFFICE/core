
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Paragraph.h"
#include "TextItem.h"
#include "Run.h"
#include "Frame.h"
#include "NamedStyle.h"
#include "Pict.h"
#include "Content/PageNumber.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>


namespace Docx2Odt
{
	namespace Private
	{

		Paragraph::Paragraph(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		const Odt::Content::Paragraph Paragraph::paragraph2paragraph(const OOX::Logic::Paragraph& ooxParagraph, bool &inField)
		{
			//OOX::Theme::File theme;
			//	if (m_docxFileContainer.exist<OOX::Theme::File>())
			//		theme = m_docxFileContainer.find<OOX::Theme::File>();
			Odt::Content::Paragraph odtParagraph;
			std::string typeBreak;
			if(*converterProperties.hasBukvitza)
			{
				odtParagraph.AddText(*converterProperties.Bukvitza);
				if(converterProperties.RunPropertyBukvitza.is_init())
				{
					NamedStyle converter(m_odtFolder, m_docxFileContainer);
					Odt::Logic::NamedStyle style = converter.runProperty2style(*converterProperties.RunPropertyBukvitza/*, theme*/);
					if (converterProperties.getDestStyleIsContent())
						converter.addStyle2Content(style);
					else 
						converter.addStyle2Styles(style);
					odtParagraph.Items->at(0).as<Odt::Content::Span>().Style = style.Name;
				}
			}
			
			BOOST_FOREACH(const OOX::Logic::ParagraphItem& ooxItem, *ooxParagraph.Items)
			{
				const Odt::Content::ParagraphItem odtItem = paragraphItem2paragraphItem(ooxItem, inField/*, theme*/);
				odtParagraph.Items->push_back(odtItem);

				if (ooxItem.is<OOX::Logic::Run>())
				{
					const OOX::Logic::Run run = ooxItem.as<OOX::Logic::Run>();
					BOOST_FOREACH(const OOX::Logic::RunItem& runItem, *run.Items)
					{
						if (runItem.is<OOX::Logic::Break>())
						{
							OOX::Logic::Break ooxBreak = runItem.as<OOX::Logic::Break>();
							if (ooxBreak.Type.is_init())
							{
								typeBreak = ooxBreak.Type;
							}
						}
					}					
				}
			}

			if (ooxParagraph.Property.is_init() && !ooxParagraph.Property->isSimple() || typeBreak != "" || (*converterProperties.hasBukvitza))
			{
				NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
				Odt::Logic::NamedStyle style;
				if (ooxParagraph.Property.is_init() && !ooxParagraph.Property->isSimple())
					style = styleConverter.paragraphProperty2style(ooxParagraph.Property);
				else
					style = styleConverter.createEmptyParagraphStyle();

				if (!style.Properties->Paragraph.is_init())
					style.Properties->Paragraph.init();
				if (typeBreak != "")
					style.Properties->Paragraph->BreakAfter = typeBreak;					
				if(ooxParagraph.Property.is_init() && ooxParagraph.Property->SectorProperty.is_init())
					style.Properties->Paragraph->BreakBefore = "page";

				if(*converterProperties.hasBukvitza)
				{
					Odt::Logic::DropCap dropCap;
					dropCap.Lines = *converterProperties.Lines;
					dropCap.Distance = UniversalUnit(*converterProperties.Distance);
					dropCap.Distance->Type = UniversalUnit::Cm;
					style.Properties->Paragraph->DropCap = dropCap;
				}

				if (converterProperties.getDestStyleIsContent())
					styleConverter.addStyle2Content(style);
				else 
					styleConverter.addStyle2Styles(style);
				odtParagraph.Style = style.Name;
			}
			else
			{			
				//??? odtParagraph.Style = "Standard";
			}
			converterProperties.hasBukvitza = false;
			return odtParagraph;
		}


		const Odt::Content::ParagraphItem Paragraph::paragraphItem2paragraphItem(const OOX::Logic::ParagraphItem& ooxItem, bool &inField/*, const OOX::Theme::File& theme*/) const
		{
			Odt::Content::ParagraphItem odtItem;

			if (ooxItem.is<OOX::Logic::Run>())
			{
				const OOX::Logic::Run run = ooxItem.as<OOX::Logic::Run>();
				Odt::Content::Span span;
				BOOST_FOREACH(const OOX::Logic::RunItem& runItem, *run.Items)
				{
                    try 
                    {
					    span.Items->push_back(item2item(runItem, inField));
                    }
                    catch(std::exception & )
                    {
                    }
				}
				if (run.Property.is_init() && !run.Property->isSimple())
				{
					NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
					Odt::Logic::NamedStyle style = styleConverter.runProperty2style(run.Property/*, theme*/);
					if (converterProperties.getDestStyleIsContent())
						styleConverter.addStyle2Content(style);
					else 
						styleConverter.addStyle2Styles(style);
					span.Style = style.Name;
				}	
				odtItem.create(span);
			}
			else if (ooxItem.is<OOX::Logic::Hyperlink>())
			{
				const OOX::Logic::Hyperlink hyperlink = ooxItem.as<OOX::Logic::Hyperlink>();
				Odt::Content::Ahref href;
				if (hyperlink.rId.is_init())
				{
					const OOX::RId rId = hyperlink.rId;
					const OOX::HyperLink ooxHyperLink = m_docxFileContainer.hyperlink(rId);
					href.Href = Encoding::unicode2utf8(ooxHyperLink.Uri().string());
				}
				BOOST_FOREACH(const OOX::Logic::Run& ooxItem, *hyperlink.Runs)
				{
					const Odt::Content::ParagraphItem odtItem = paragraphItem2paragraphItem(ooxItem, inField/*, theme*/);
					href.Items->push_back(odtItem);
				}

				odtItem.create(href);
			}
			else if (ooxItem.is<OOX::Logic::BookmarkStart>())
			{
			}
			else if (ooxItem.is<OOX::Logic::BookmarkEnd>())
			{
			}
			else if (ooxItem.is<OOX::Logic::FldSimple>())
			{
			}
			else
			{
				odtItem.create<Odt::Content::NullRun>();
			}

			return odtItem;
		}


		const Odt::Content::ParagraphItem Paragraph::item2item(const OOX::Logic::RunItem& ooxItem, bool &inField) const
		{
			Odt::Content::ParagraphItem odtItem;

			if (ooxItem.is<OOX::Logic::Text>())
			{
				Odt::Content::Span span(ooxItem.toTxt());
				odtItem.create(span);
			}
			else if (ooxItem.is<OOX::Logic::Tab>())
			{
				odtItem.create<Odt::Content::Tab>();
			}
			else if (ooxItem.is<OOX::Logic::Break>())
			{
				OOX::Logic::Break ooxBreak = ooxItem.as<OOX::Logic::Break>();
				if (!ooxBreak.Type.is_init())
					odtItem.create<Odt::Content::LineBreak>();
			}
			else if (ooxItem.is<OOX::Logic::Drawing>())
			{
				Frame converter(m_odtFolder, m_docxFileContainer);
				OOX::Logic::Drawing ooxDrawing = ooxItem.as<OOX::Logic::Drawing>();
				if (ooxDrawing.Inline->Graphic->Pic->rId.is_init())
					odtItem.create(converter.drawing2frame(ooxDrawing));
			}
			else if (ooxItem.is<OOX::Logic::Pict>())
			{
				Pict converter(m_odtFolder, m_docxFileContainer);
				OOX::Logic::Pict ooxPict = ooxItem.as<OOX::Logic::Pict>();
				if (ooxPict.hasPictures() || ooxPict.isOle())
				{
					Odt::Content::Frame odtFrame = converter.pict2frame(ooxPict);
					odtItem.create(odtFrame);
				}
			}
			else if (ooxItem.is<OOX::Logic::FootnoteRef>())
			{
			}
			else if (ooxItem.is<OOX::Logic::EndnoteRef>())
			{
			}
			else if (ooxItem.is<OOX::Logic::ContinuationSeparator>())
			{
			}
			else if (ooxItem.is<OOX::Logic::Separator>())
			{
			}
			else if (ooxItem.is<OOX::Logic::FootnoteReference>())
			{
				const OOX::Logic::FootnoteReference reference = ooxItem.as<OOX::Logic::FootnoteReference>();
				const OOX::FootNote::Note ooxFootnote = m_docxFileContainer.find<OOX::FootNote>().find(reference);
				Run converter(m_odtFolder, m_docxFileContainer);
				converter.converterProperties = converterProperties;
				odtItem = converter.footnote2run(ooxFootnote);
			}
			else if (ooxItem.is<OOX::Logic::EndnoteReference>())
			{
				const OOX::Logic::EndnoteReference reference = ooxItem.as<OOX::Logic::EndnoteReference>();
				const OOX::EndNote::Note ooxEndnote = m_docxFileContainer.find<OOX::EndNote>().find(reference);
				Run converter(m_odtFolder, m_docxFileContainer);
				converter.converterProperties = converterProperties;
				odtItem = converter.endnote2run(ooxEndnote);
			}			
			else if (ooxItem.is<OOX::Logic::FldChar>())
			{
				const OOX::Logic::FldChar& fldChar = ooxItem.as<OOX::Logic::FldChar>();
				if (fldChar.FldCharType == "begin")
				{
					inField = true;
				}
				else 
				{
					if (fldChar.FldCharType == "separate")	
						inField = false;
				}
			}
			else if (ooxItem.is<OOX::Logic::InstrText>())
			{
				if(!inField)
				{
					Odt::Content::Span span(ooxItem.toTxt());
					odtItem.create(span);
				}
                else
                {
                    const std::string t = ooxItem.toTxt();
                    if (boost::algorithm::contains(t, "PAGE"))
                    {
                        Odt::Content::PageNumber pageNumber;
                        odtItem.create(pageNumber);                                                
                    }                
                }
			}
			else
			{
				odtItem.create<Odt::Content::NullRun>();
			}

			return odtItem;
		}


		// TODO исправить после реализации стилей
/*
		const nullable<Odt::Logic::Style> Paragraph::createRunStyle(const OOX::Logic::ParagraphItem& ooxItem, Odt::Content::Paragraph& odtParagraph) const
		{
			const OOX::Logic::Run ooxRun = ooxItem.as<OOX::Logic::Run>();

			nullable<Odt::Logic::Style> odtRunStyle;
			const std::string odtParagraphStyleName = odtParagraph.StyleName;			

			if (odtParagraphStyleName == "Standard")
			{
				if (ooxRun.Property.is_init() && !ooxRun.Property->isSimple())
				{
					Run converter(m_odtFolder, m_docxFolder);		
					odtRunStyle = converter.rpr2style(ooxRun.Property);
				}
			}
			else
			{
				if (getDisplayStyleName(odtParagraphStyleName) == "Standard")
				{
					// TODO исправить после реализации стилей
//					Odt::Logic::Properties odtParagraphProperties = m_odtFolder.properties(odtParagraphStyleName);
//					Run converter(m_odtFolder, m_docxFolder);
//					odtRunStyle = converter.rpr2style(ooxRun.Property.get_value_or_default(), odtParagraphProperties);
				}
				else if(ooxRun.Property.is_init() && !ooxRun.Property->isSimple())
				{
					// TODO исправить после реализации стилей
//					Odt::Logic::Properties odtParagraphProperties = m_odtFolder.properties(odtParagraphStyleName);
//					Run converter(m_odtFolder, m_docxFolder);
//					odtRunStyle = converter.rpr2style(ooxRun.Property, odtParagraphProperties);
				}
			}
			return odtRunStyle;
		}
*/

		// TODO исправить после реализации стилей
/*
		const Odt::Logic::Style Paragraph::paragraphProperty2style(const OOX::Logic::ParagraphProperty& ooxProperty) const
		{
			Odt::Logic::Style odtStyle = createStyleNames();

			if (ooxProperty.PStyle.is_init())
				odtStyle.ParentName = ooxProperty.PStyle;
			else
				odtStyle.ParentName = "Standard";

			odtStyle.Properties->Paragraph = ppr2ppr(ooxProperty);

			if (ooxProperty.RunProperty.is_init() && odtStyle.ParentName == "Standard")
			{
				Run converter(m_odtFolder, m_docxFolder);
				odtStyle.Properties->Text = converter.rpr2tpr(ooxProperty.RunProperty);
			}			
			return odtStyle;
		}
*/

		// TODO исправить после реализации стилей
/*
		const Odt::Logic::Property::Paragraph Paragraph::ppr2ppr(const OOX::Logic::ParagraphProperty& ooxProperty) const
		{
			Odt::Logic::Property::Paragraph odtProperty;
			odtProperty.Align				= ooxProperty.Align;
			odtProperty.Background	= ooxProperty.Background;
			if(ooxProperty.Ind.is_init())
			{
				odtProperty.MarginLeft	= ooxProperty.Ind->Left;
				odtProperty.MarginRight	= ooxProperty.Ind->Right;
			}
			if(ooxProperty.Spacing.is_init())
			{
				if (!ooxProperty.Spacing->BeforeAutospacing.is_init() || ooxProperty.Spacing->BeforeAutospacing == 0)
					odtProperty.MarginTop			= ooxProperty.Spacing->Before;
				if (!ooxProperty.Spacing->AfterAutospacing.is_init() || ooxProperty.Spacing->AfterAutospacing == 0)
					odtProperty.MarginBottom	= ooxProperty.Spacing->After;

				if(ooxProperty.Spacing->LineRule.is_init())
					if (ooxProperty.Spacing->LineRule == "auto")
						odtProperty.LineHeight = *ooxProperty.Spacing->Line * 100 / 240 ;
					else if (ooxProperty.Spacing->LineRule == "atLeast")
						odtProperty.LineHeightAtLeast = ooxProperty.Spacing->Line;
					else if (ooxProperty.Spacing->LineRule == "exact")
						;//odtProperty.LineHeight = ooxProperty.Spacing->Line;
			}
			return odtProperty;
		}
*/

		// TODO исправить после реализации стилей
/*
		const std::string Paragraph::getDisplayStyleName(const std::string& styleName) const
		{
			std::string displayName;
			// TODO исправить после реализации стилей
//			displayName = m_odtFolder.styles()->displayName(styleName);
			return displayName;
		}
*/


			// TODO исправить после реализации стилей
/*
		const Odt::Logic::Style Paragraph::createStyleNames() const
		{
			Odt::Logic::Style odtStyle;
			odtStyle.notForLibrary();

			// TODO исправить после реализации стилей
//			odtStyle.Name		= m_odtFolder.styles()->name("P");
			odtStyle.Family = "paragraph";
			return odtStyle;
		}
*/
	} // namespace Private
} // namespace Docx2Odt