#pragma once
#ifndef DOCX_2_ODT_PRIVATE_LIST_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_LIST_INCLUDE_H_

#include "Base.h"
#include "Logic\Paragraph.h"
#include "ConverterProperties.h"


namespace Docx2Odt
{
	namespace Private
	{
		class List : public Base
		{
		public:
			List(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Content::List list2list(const OOX::Logic::List& ooxList, bool &inField);

		private:
			void convert(std::vector<Odt::Content::ListItem>& odt, const std::vector<OOX::Logic::Paragraph>& oox, bool &inField);
			void ParagraphInListItem(std::vector<Odt::Content::Text>& odtTexts, const OOX::Logic::Paragraph& textItem, bool &inField);
			void firstParagraphInListItem(std::vector<Odt::Content::Text>& odtTexts, const OOX::Logic::Paragraph& textItem, bool &inField);
			void changePr(Odt::Logic::Properties& properties, const Odt::Logic::Property::ListLevel& listLevelPr) const;
		private:
			const Odt::Content::ListItem listItem2listItem(const std::vector<OOX::Logic::Paragraph>& ooxListItems, bool &inField);

		private:
			const nullable<std::string> listStyleName(const int NumList);
						
		public:
			ConverterProperties converterProperties;

		private:
			int			m_currentListLevel;
			std::string	m_currentListStyle;
			int			m_currentElement;

		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_LIST_INCLUDE_H_