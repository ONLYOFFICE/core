
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "FileContainer.h"
#include <boost_filesystem_version.h>
#include "Styles.h"


namespace Docx2Odt
{

	void FileContainer::convert(const OOX::IFileContainer& docxFileContainer, Odt::Folder& odtFolder) const
	{
		BOOST_FOREACH(const OOX::IFileContainer::RIdFilePair& pair, docxFileContainer)
		{
			if (dynamic_cast<const OOX::FontTable*>(pair.second.get()) != 0)
			{
				const OOX::FontTable& fontTable = static_cast<const OOX::FontTable&>(*pair.second);
				fontTable2odtFolder(fontTable, odtFolder);
			}
			else if (dynamic_cast<const OOX::Styles*>(pair.second.get()) != 0)
			{
				const OOX::Styles& ooxStyles = static_cast<const OOX::Styles&>(*pair.second);				
				Styles converter(odtFolder, docxFileContainer);
				converter.convert(ooxStyles, odtFolder);
			}
			else if (dynamic_cast<const OOX::Numbering*>(pair.second.get()) != 0)
			{
				const OOX::Numbering& ooxNumbering = static_cast<const OOX::Numbering&>(*pair.second);
				Styles converter(odtFolder, docxFileContainer);
				converter.convert(ooxNumbering, odtFolder);
			}
		}
	}


	void FileContainer::fontTable2odtFolder(const OOX::FontTable& ooxFonts, Odt::Folder& odtFolder) const
	{
		BOOST_FOREACH(const OOX::FontTable::Font& ooxFont, *ooxFonts.Fonts)
		{
			const Odt::Logic::FontFace odtFont = font2font(ooxFont);
			odtFolder.Content->FontFaceDecls->Add(odtFont);
			odtFolder.Styles->FontFaceDecls->Add(odtFont);
		}
	}


	const Odt::Logic::FontFace FileContainer::font2font(const OOX::FontTable::Font& ooxFont) const
	{
		Odt::Logic::FontFace odtFont;
		odtFont.Name		= ooxFont.Name;
		odtFont.Family	= ooxFont.Name;
		odtFont.Generic	= ooxFont.Family;
		odtFont.Pitch		= ooxFont.Pitch;
		return odtFont;
	}

} // namespace Docx2Odt