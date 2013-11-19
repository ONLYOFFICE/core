#pragma once
#ifndef DOCX_2_ODT_PRIVATE_TEXT_ITEM_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_TEXT_ITEM_INCLUDE_H_

#include "Base.h"
#include "ConverterProperties.h"


namespace Docx2Odt
{
	namespace Private
	{
		class TextItem : public Base
		{
		public:
			TextItem(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			void convert(const std::vector<OOX::Logic::TextItem>& ooxItems, std::vector<Odt::Content::Text>& odtItems);
			void addItem2Items(const OOX::Logic::TextItem& ooxItem, std::vector<Odt::Content::Text>& odtItems, bool &inField);
			void addList2Items(const OOX::Logic::List& ooxList, std::vector<Odt::Content::Text>& odtItems, bool &inField);

		public:
			ConverterProperties converterProperties;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_TEXT_ITEM_INCLUDE_H_