#pragma once
#ifndef DOCX_2_ODT_STYLES_INCLUDE_H_
#define DOCX_2_ODT_STYLES_INCLUDE_H_

#include "Docx.h"
#include "Odt.h"
#include "Private/Base.h"


namespace Docx2Odt
{
	class Styles : public Private::Base
	{
	public:
		Styles(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);
	public:
		void convert(const OOX::Styles& ooxStyles, Odt::Folder& odtFolder);
		// TODO видимо, надо выделить класс Numbering
		/*static*/ void convert(const OOX::Numbering& ooxNumbering, Odt::Folder& odtFolder);

	private:
		void convertDefaultStyles(Odt::Logic::DefaultStyles& odtDefaultStyles, const OOX::Styles& ooxStyles);
		void convertNamedStyles(Odt::Logic::NamedStyles& odtStyles, const std::vector<OOX::Styles::Style>& ooxStyles);

	private:
		const Odt::Logic::NamedStyle convertNamedStyle(const OOX::Styles::Style& ooxStyle);

	private:
		// TODO исправить после реализации стилей
		//static void convert(Odt::Styles::Styles& odt, const Docx::Folder& docx);

		// TODO исправить после реализации стилей
		//static void convert(Odt::Styles::Styles& odt, const OOX::Styles::File& oox);

		// TODO исправить после реализации стилей
		//static void convert(Odt::Logic::Styles& styles, const OOX::Numbering::File& oox);

	private:
		/*static*/ const Odt::Logic::ListStyle abstractNum2ListStyle(const OOX::Numbering::AbstractNum& abstractNum);
		/*static*/ const Odt::Logic::ListLevelStyleBullet lvl2listLevelStyleBullet(const OOX::Numbering::Level& lvl);
		/*static*/ const Odt::Logic::ListLevelStyleNumber lvl2listLevelStyleNumber(const OOX::Numbering::Level& lvl);
		/*static*/ const Odt::Styles::NotesConfiguration foot2NotesConfiguration(const OOX::Logic::FootNoteProperty& oox);
		/*static*/ const Odt::Styles::NotesConfiguration end2NotesConfiguration(const OOX::Logic::EndNoteProperty& oox);

		// TODO исправить после реализации стилей
		///*static*/ const Odt::Logic::Style libraryStyle2libraryStyle(const OOX::Styles::Style& oox);

		// TODO устаревшая
		/*static*/ const Odt::Logic::Property::ListLevel paragraphProperty2ListLevelProperties(const OOX::Logic::ParagraphProperty& oox);
		/*static*/ const nullable<std::string> lvlText2suffix(const std::string& text);
		/*static*/ const nullable<std::string> lvlText2prefix(const std::string& text);
	};
} // namespace OOX2Odt

#endif // DOCX_2_ODT_STYLES_INCLUDE_H_