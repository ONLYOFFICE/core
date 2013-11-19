#pragma once
#ifndef DOCX_2_ODT_PRIVATE_PARAGRAPH_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_PARAGRAPH_INCLUDE_H_

#include "Base.h"
#include "ConverterProperties.h"


namespace Docx2Odt
{
	namespace Private
	{
		class Paragraph : public Base
		{
		public:
			Paragraph(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Content::Paragraph paragraph2paragraph(const OOX::Logic::Paragraph& ooxParagraph, bool &inField);

		private:
			const Odt::Content::ParagraphItem paragraphItem2paragraphItem(const OOX::Logic::ParagraphItem& ooxItem, bool &inField/*, const OOX::Theme::File& theme*/) const;
			const Odt::Content::ParagraphItem item2item(const OOX::Logic::RunItem& ooxItem, bool &inField) const;

			// TODO исправить после реализации стилей
//			const nullable<Odt::Logic::Style> Paragraph::createRunStyle(const OOX::Logic::ParagraphItem& ooxItem, Odt::Content::Paragraph& odtParagraph) const;

		//private:
			// TODO исправить после реализации стилей
//			const Odt::Logic::Style paragraphProperty2style(const OOX::Logic::ParagraphProperty& ooxProperty) const;

			// TODO исправить после реализации стилей
//			const Odt::Logic::Property::Paragraph ppr2ppr(const OOX::Logic::ParagraphProperty& ooxProperty) const;

			// TODO исправить после реализации стилей
//			const std::string getDisplayStyleName(const std::string& styleName) const;

			// TODO исправить после реализации стилей
//			const Odt::Logic::Style createStyleNames() const;
		
		public:
			ConverterProperties converterProperties;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_PARAGRAPH_INCLUDE_H_