
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FontTable.h"
#include "FileTypes.h"

namespace OOX
{
	FontTable::FontTable()
	{
	}

	FontTable::FontTable(const OOX::CPath& filename)
	{
		read(filename);
	}

	FontTable::~FontTable()
	{
	}

	void FontTable::read(const OOX::CPath& oPath)
	{
		m_fonts.clear();

		XmlUtils::CXmlNode oFonts;
		oFonts.FromXmlFile( oPath.GetPath(), true );

		if ( _T("w:fonts") == oFonts.GetName() )
		{
			XmlUtils::CXmlNodes oFontList;
			oFonts.GetNodes( _T("w:font"), oFontList );

			for ( int nFontIndex = 0; nFontIndex < oFontList.GetCount(); ++nFontIndex )
			{
				XmlUtils::CXmlNode oFontNode;
				if ( oFontList.GetAt( nFontIndex, oFontNode ) )
				{
					Font oFont;
					oFont.fromXML( oFontNode );
					m_fonts.push_back( oFont );
				}
			}
		}
	}

	void FontTable::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType FontTable::type() const
	{
		return FileTypes::FontTable;
	}

	const OOX::CPath FontTable::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath FontTable::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX