#pragma once
#ifndef DOCX_2_ODT_PRIVATE_RUN_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_RUN_INCLUDE_H_

#include "Base.h"
#include "ConverterProperties.h"

namespace Docx2Odt
{
	namespace Private
	{
		class Run : public Base
		{
		public:
			Run(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Content::ParagraphItem endnote2run(const OOX::EndNote::Note& ooxEndnote) const;
			const Odt::Content::ParagraphItem footnote2run(const OOX::FootNote::Note& ooxFootnote) const;

			//const Odt::Logic::Property::Text rpr2tpr(const OOX::Logic::RunProperty& ooxProperty, const OOX::Theme::File& theme) const;
			//const Odt::Logic::Property::Text rpr2tpr(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Property::Text& odtParagraphProperty, const OOX::Theme::File& theme) const;

			// TODO исправить после реализации стилей
//			const Odt::Logic::Style rpr2style(const OOX::Logic::RunProperty& ooxProperty);

			// TODO исправить после реализации стилей
//			const Odt::Logic::Style rpr2style(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Properties& odtProperties);

		private:
			const Odt::Content::Note Run::endnote2note(const OOX::EndNote::Note& ooxEndnote) const;
			const Odt::Content::Note Run::footnote2note(const OOX::FootNote::Note& ooxFootnote) const;

		public:
			ConverterProperties converterProperties;
		};
	} // namespace Prviate
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_RUN_INCLUDE_H_