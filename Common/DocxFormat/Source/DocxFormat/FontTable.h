#pragma once

#ifndef OOX_FONTTABLE_INCLUDE_H_
#define OOX_FONTTABLE_INCLUDE_H_

#include "File.h"
#include "Font.h"
#include "FileTypes.h"

#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"

namespace OOX
{
	class CFontTable : public OOX::File
	{
	public:

		CFontTable()
		{
		}
		CFontTable(const CPath &oFilePath)
		{
			read( oFilePath );
		}

		virtual ~CFontTable()
		{
		}

		bool Find(CString &sFontName, CFont &oFont)
		{
			for ( int nIndex = 0; nIndex < m_arrFonts.GetSize(); nIndex++ )
			{
				if ( sFontName == m_arrFonts[nIndex].m_sName )
				{
					oFont = m_arrFonts[nIndex];
					return true;
				}
			}

			return false;
		}


	public:

		virtual void read(const CPath &oFilePath)
		{
			XmlUtils::CXmlNode oFonts;
			oFonts.FromXmlFile( oFilePath.GetPath(), true );

			if ( _T("w:fonts") == oFonts.GetName() )
			{
				XmlUtils::CXmlNodes oFontList;
				oFonts.GetNodes( _T("w:font"), oFontList );

				for ( int nFontIndex = 0; nFontIndex < oFontList.GetCount(); nFontIndex++ )
				{
					XmlUtils::CXmlNode oFontNode;
					if ( oFontList.GetAt( nFontIndex, oFontNode ) )
					{
						CFont oFont;
						oFont.fromXML( oFontNode );
						m_arrFonts.Add( oFont );
					}
				}
			}
		}
		virtual void write(const CPath &oFilePath, const CPath &oDirectoryPath, CContentTypes& content) const
		{
			CString sXml;
			sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">");
			for ( int nIndex = 0; nIndex < m_arrFonts.GetSize(); nIndex++ )
			{
				const CFont &oFont = m_arrFonts[nIndex];
				sXml += oFont.toXML();
			}
			sXml += _T("</w:fonts>");

			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
		}

	public:

		virtual const FileType type() const
		{
			return FileTypes::FontTable;
		}

		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:

		CSimpleArray<CFont> m_arrFonts;

	};

} // namespace OOX

#endif // OOX_FONTTABLE_INCLUDE_H_