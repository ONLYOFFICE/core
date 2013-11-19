
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "TextItem.h"
#include <boost/foreach.hpp>
#include "Table.h"
#include "Paragraph.h"
#include "List.h"
#include "NamedStyle.h"


namespace Docx2Odt
{
	namespace Private
	{

		TextItem::TextItem(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		void TextItem::convert(const std::vector<OOX::Logic::TextItem>& ooxItems, std::vector<Odt::Content::Text>& odtItems)
		{
			odtItems.clear();
			int curlistnum = 0;
			OOX::Logic::List List;
			converterProperties.hasBukvitza = false;
			converterProperties.Lines = 0;
			converterProperties.Distance = 0;
			//OOX::Theme::File theme;
			//if (m_docxFileContainer.exist<OOX::Theme::File>())
			//		theme = m_docxFileContainer.find<OOX::Theme::File>();

			bool inField = false;
			BOOST_FOREACH(const OOX::Logic::TextItem& ooxItem, ooxItems)
			{
				if(ooxItem.is<OOX::Logic::Paragraph>())
				{
					if(ooxItem.as<OOX::Logic::Paragraph>().Property.is_init())
					{
						if(!(*converterProperties.hasBukvitza))
						{
							if(ooxItem.as<OOX::Logic::Paragraph>().Property->TextFrameProperties.is_init())
								converterProperties.hasBukvitza = ooxItem.as<OOX::Logic::Paragraph>().Property->TextFrameProperties->DropCap.get_value_or("none") != "none";
							if(*converterProperties.hasBukvitza)
							{
								converterProperties.Lines = ooxItem.as<OOX::Logic::Paragraph>().Property->TextFrameProperties->Lines.get_value_or(1);
								converterProperties.Distance = ooxItem.as<OOX::Logic::Paragraph>().Property->TextFrameProperties->HSpace.get_value_or_default();
								converterProperties.Bukvitza = ooxItem.as<OOX::Logic::Paragraph>().Items->at(0).toTxt();
								if(ooxItem.as<OOX::Logic::Paragraph>().Property->RunProperty.is_init())
									converterProperties.RunPropertyBukvitza = *ooxItem.as<OOX::Logic::Paragraph>().Property->RunProperty;
								else
									converterProperties.RunPropertyBukvitza.reset();
								continue;
							}
						}
					}

					if(ooxItem.as<OOX::Logic::Paragraph>().isInList())
					{
						int tlistnum = ooxItem.as<OOX::Logic::Paragraph>().GetListNum();
						if(curlistnum == 0)
						{
							curlistnum = tlistnum;
							List.add(ooxItem.as<OOX::Logic::Paragraph>());
						}
						else if(tlistnum != curlistnum)
						{
							addList2Items(List, odtItems, inField);
							List.Items->clear();
							curlistnum = tlistnum;
							List.add(ooxItem.as<OOX::Logic::Paragraph>());
						}
						else
						{
							List.add(ooxItem.as<OOX::Logic::Paragraph>());
						}
					}
					else
					{
						if(!List.Items->empty())
						{
							addList2Items(List, odtItems, inField);
							List.Items->clear();
							curlistnum = 0;
						}
						addItem2Items(ooxItem, odtItems, inField);
					}
				}
				else
				{
					if(!List.Items->empty())
					{
						addList2Items(List, odtItems, inField);
						List.Items->clear();
						curlistnum = 0;
					}
					addItem2Items(ooxItem, odtItems, inField);
				}
			}

			if(curlistnum != 0)
				addList2Items(List, odtItems, inField);
		}


		void TextItem::addItem2Items(const OOX::Logic::TextItem& ooxItem, std::vector<Odt::Content::Text>& odtItems, bool &inField)
		{
			if (ooxItem.is<OOX::Logic::Paragraph>())
			{
				Paragraph converter(m_odtFolder, m_docxFileContainer);
				converter.converterProperties = converterProperties;
				const OOX::Logic::Paragraph ooxParagraph = ooxItem.as<OOX::Logic::Paragraph>();
				const Odt::Content::Paragraph odtParagraph = converter.paragraph2paragraph(ooxParagraph, inField);
				converterProperties.hasBukvitza = *converter.converterProperties.hasBukvitza;
				odtItems.push_back(odtParagraph);
			}
			else if (ooxItem.is<OOX::Logic::Table>())
			{
				Table converter(m_odtFolder, m_docxFileContainer);
				converterProperties.hasBukvitza = false;
				converter.converterProperties = converterProperties;
				const OOX::Logic::Table ooxTable = ooxItem.as<OOX::Logic::Table>();
				const Odt::Content::Table odtTable = converter.table2table(ooxTable);
				odtItems.push_back(odtTable);
			}
			else if (ooxItem.is<OOX::Logic::Sdt>())
			{
				/*const OOX::Logic::Sdt ooxSdt = ooxItem.as<OOX::Logic::Sdt>();
				Private::TextItem itemConverter(m_odtFolder, m_docxFileContainer);
				itemConverter.convert(ooxSdt.Content->Items, odtItems);*/
			}
		}

		void TextItem::addList2Items(const OOX::Logic::List& ooxList, std::vector<Odt::Content::Text>& odtItems, bool &inField)
		{
			List converter(m_odtFolder, m_docxFileContainer);
			converter.converterProperties = converterProperties;
			const Odt::Content::List odtList = converter.list2list(ooxList, inField);
			converterProperties.hasBukvitza = *converter.converterProperties.hasBukvitza;
			odtItems.push_back(odtList);
		}

	} // namespace Private
} // namespace Docx2Odt