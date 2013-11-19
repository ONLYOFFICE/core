
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "List.h"
#include <vector>
#include "TextItem.h"
#include "Paragraph.h"


namespace Docx2Odt
{
	namespace Private
	{

		List::List(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
			m_currentListLevel = 0;
			m_currentListStyle = "";
			m_currentElement = 0;
		}


		const Odt::Content::List List::list2list(const OOX::Logic::List& ooxList, bool &inField)
		{
			Odt::Content::List odtList;
			odtList.Style	= listStyleName(ooxList.Num);
			++m_currentListLevel;
			convert(*odtList.Items, *ooxList.Items, inField);
			--m_currentListLevel;

			int start = *ooxList.Items->at(0).CountInList;
			int stop = *ooxList.Items->at(ooxList.Items->size() - 1).CountInList;
			if(start > 1)
			{
				odtList.ContinueNumbering = true;
				odtList.ContinueList = "list_" + m_currentListStyle + "_" + ToString(start - 1);
			}
			odtList.id = "list_" + m_currentListStyle + "_" + ToString(stop);

			return odtList;
		}


		void List::convert(std::vector<Odt::Content::ListItem>& odt, const std::vector<OOX::Logic::Paragraph>& oox, bool &inField)
		{
			//BOOST_FOREACH(const OOX::Logic::Paragraph& listItem, oox)
			//{
			//	odt.push_back(listItem2listItem(listItem));
			//}
			while((m_currentElement < oox.size()) && (oox.at(m_currentElement).GetLevel() >= m_currentListLevel))
			{
				odt.push_back(listItem2listItem(oox, inField));
			}
		}


		const Odt::Content::ListItem List::listItem2listItem(const std::vector<OOX::Logic::Paragraph>& ooxListItems, bool &inField)
		{
			Odt::Content::ListItem odtListItem;
			if(ooxListItems.at(m_currentElement).GetLevel() == m_currentListLevel)
			{
				ParagraphInListItem(*odtListItem.Texts, ooxListItems.at(m_currentElement), inField);
				m_currentElement++;
			}
			else
			{
				Odt::Content::List list;

				if(ooxListItems.at(m_currentElement).GetLevel() > m_currentListLevel)
				{
					m_currentListLevel++;
					convert(*list.Items, ooxListItems, inField);
					//list.Items->push_back(listItem2listItem(ooxListItem));
					odtListItem.Texts->push_back(list);
					m_currentListLevel--;
				}
				else
				{
					m_currentListLevel--;
				}
				//m_currentListLevel++;
				//list.Items->push_back(listItem2listItem(ooxListItem));
				//odtListItem.Texts->push_back(list);
				//m_currentListLevel--;
			}
			return odtListItem;
/*
			Odt::Content::ListItem odtListItem;
			TextItem converter(m_odtFolder, m_docxFileContainer);

			// TODO реализовать как for
			int count = 0;
			BOOST_FOREACH(const OOX::Logic::Paragraph& textItem, *ooxListItem.Items)
			{				
				++count;
				if (textItem.is<OOX::Logic::Paragraph>())
				{		
					if (count != 1)
						ParagraphInListItem(*odtListItem.Texts, textItem);
					else
						firstParagraphInListItem(*odtListItem.Texts, textItem);
				}
				else
				{
					converter.addItem2Items(textItem, *odtListItem.Texts);
				}
			}
			return odtListItem;
*/
		}


		const nullable<std::string> List::listStyleName(const int NumList)
		{
			nullable<std::string> name;
			if (m_currentListLevel == 0)
			{
				//const int abstractNum = m_docxFileContainer.find<OOX::Document>().find<OOX::Numbering>().getAbstractNumId(NumList);
				const int abstractNum = m_docxFileContainer.find<OOX::Numbering>().getAbstractNumId(NumList);
				name = "L" + ToString(abstractNum + 1);
				m_currentListStyle = name.get();
			}
			return name;
		}


		void List::ParagraphInListItem(std::vector<Odt::Content::Text>& odtTexts, const OOX::Logic::Paragraph& textItem, bool &inField)
		{
			Paragraph converter(m_odtFolder, m_docxFileContainer);
			converter.converterProperties = converterProperties;
//*			const OOX::Logic::Paragraph ooxParagraph = textItem.as<OOX::Logic::Paragraph>();
			const Odt::Content::Paragraph odtParagraph = converter.paragraph2paragraph(textItem, inField);//ooxParagraph);
			converterProperties.hasBukvitza = *converter.converterProperties.hasBukvitza;

			// _TODO исправить после реализации стилей
			//Odt::Logic::Properties listProperties = m_odtFolder.styles()->listLevelStyleProperties(m_currentListStyle, m_currentListLevel);
			//Odt::Logic::Properties properties = m_odtFolder.styles()->properties(odtParagraph.StyleName);//извлечение properties
			//changePr(properties, listProperties.ListLevel);																							 //изменение properties
			//m_odtFolder.styles()->setProperties(odtParagraph.StyleName, properties);										 //внедрение properties

			odtTexts.push_back(odtParagraph);
		}


		void List::firstParagraphInListItem(std::vector<Odt::Content::Text>& odtTexts, const OOX::Logic::Paragraph& textItem, bool &inField)
		{
			Paragraph converter(m_odtFolder, m_docxFileContainer);
			converter.converterProperties = converterProperties;
//*			const OOX::Logic::Paragraph ooxParagraph = textItem.as<OOX::Logic::Paragraph>();
			const Odt::Content::Paragraph odtParagraph = converter.paragraph2paragraph(textItem, inField);//ooxParagraph);
			converterProperties.hasBukvitza = *converter.converterProperties.hasBukvitza;

			// _TODO исправить после реализации стилей
			//Odt::Logic::Properties listProperties = m_odtFolder.styles()->listLevelStyleProperties(m_currentListStyle, 1);
			//Odt::Logic::Properties properties = m_odtFolder.styles()->properties(odtParagraph.StyleName);//извлечение properties
			//changePr(properties, listProperties.ListLevel);																							 //изменение properties
			//m_odtFolder.styles()->setProperties(odtParagraph.StyleName, properties);										 //внедрение properties

			odtTexts.push_back(odtParagraph);
		}


		void List::changePr(Odt::Logic::Properties& properties, const Odt::Logic::Property::ListLevel& listLevelPr) const
		{
			if (properties.Paragraph.is_init())
				properties.Paragraph->MarginLeft = properties.Paragraph->MarginLeft.get_value_or(0) - listLevelPr.SpaceBefore.get_value_or(0) - *listLevelPr.MinLabelWidth;
		}

	} // namespace Private
} // namespace Docx2Odt