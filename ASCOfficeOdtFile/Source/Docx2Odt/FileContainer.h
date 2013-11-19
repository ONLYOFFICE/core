#pragma once
#ifndef DOCX_2_ODT_FILE_CONTAINER_INCLUDE_H_
#define DOCX_2_ODT_FILE_CONTAINER_INCLUDE_H_

#include "Docx.h"
#include "Odt.h"
#include "IFileContainer.h"


namespace Docx2Odt
{
	class FileContainer
	{
	public:
		void convert(const OOX::IFileContainer& docxFileContainer, Odt::Folder& odtFolder) const;

	private:
		void fontTable2odtFolder(const OOX::FontTable& ooxFonts, Odt::Folder& odtFolder) const;
		const Odt::Logic::FontFace font2font(const OOX::FontTable::Font& ooxFont) const;
	};
} // namespace Docx2Odt

#endif // DOCX_2_ODT_FILE_CONTAINER_INCLUDE_H_