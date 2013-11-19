#pragma once
#ifndef DOCX_2_ODT_DOCUMENT_INCLUDE_H_
#define DOCX_2_ODT_DOCUMENT_INCLUDE_H_

#include "Docx.h"
#include "Odt.h"


namespace Docx2Odt
{
	class Document
	{
	public:
		static void convert(const OOX::Document& ooxDocument, Odt::Folder& odtFolder);

	private:
		static void sectorPropertyAndBG2PageLayoutProperties(const OOX::Document& ooxDocument, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties);
		
		static void sectorProperty2NotesProperties(const OOX::Logic::SectorProperty& ooxSectorProperty, std::vector<Odt::Styles::NotesConfiguration>& notesPr);
		static void convertNoteStyle(const OOX::Logic::NoteProperty& notePr, Odt::Styles::NotesConfiguration& noteStyle);
		static void sectorProperty2HeadersAndFooters(const OOX::Document& ooxDocument, Odt::Folder& odtFolder);
		
		static void pageSize2PageLayoutProperties(const OOX::Logic::PageSize& ooxPageSize, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties);
		static void pageMargin2PageLayoutProperties(const OOX::Logic::PageMargin& ooxPageMargin, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties);
		static const Odt::Styles::Columns columns2columns(const OOX::Logic::Columns& ooxColumns);
	};
} // namespace Docx2Odt

#endif // DOCX_2_ODT_DOCUMENT_INCLUDE_H_