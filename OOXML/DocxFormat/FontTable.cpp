/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "FontTable.h"

#include "../../DesktopEditor/common/Directory.h"
#include "FileTypes.h"
#include "Docx.h"

namespace OOX
{
	CFontTable::CFontTable(OOX::Document *pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.fontTable = this;
			else						docx->m_oMain.fontTable = this;
		}
	}
	CFontTable::CFontTable(OOX::Document *pMain, const CPath &oFilePath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.fontTable = this;
			else						docx->m_oMain.fontTable = this;
		}
		read( oFilePath );
	}
	CFontTable::CFontTable(XmlUtils::CXmlNode& oNode) : File(NULL)
	{
		fromXML( oNode );
	}
	CFontTable::CFontTable(XmlUtils::CXmlLiteReader& oReader) : File(NULL)
	{
		fromXML( oReader );
	}
	CFontTable::~CFontTable()
	{
		for ( size_t nIndex = 0; nIndex < m_arrFonts.size(); nIndex++ )
		{
			if (m_arrFonts[nIndex] )
				delete m_arrFonts[nIndex];

			m_arrFonts[nIndex] = NULL;
		}

		m_arrFonts.clear();
	}
	const CFontTable& CFontTable::operator =(const XmlUtils::CXmlNode& oNode)
	{
		fromXML( (XmlUtils::CXmlNode&)oNode );
		return *this;
	}
	const CFontTable& CFontTable::operator =(const XmlUtils::CXmlLiteReader& oReader)
	{
		fromXML( (XmlUtils::CXmlLiteReader&)oReader );
		return *this;
	}
	bool CFontTable::Find(std::wstring &sFontName, CFont *oFont)
	{
		for (unsigned int nIndex = 0; nIndex < m_arrFonts.size(); nIndex++ )
		{
			if ((m_arrFonts[nIndex]) && ( sFontName == m_arrFonts[nIndex]->m_sName ))
			{
				oFont = m_arrFonts[nIndex];
				return true;
			}
		}

		return false;
	}
	void CFontTable::read(const CPath &oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( _T("w:fonts") == sName)
		{
			fromXML(oReader);
		}
	}
	void CFontTable::write(const CPath &oFilePath, const CPath &oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = toXML();

		CDirectory::SaveToFile( oFilePath.GetPath(), sXml );

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const FileType CFontTable::type() const
	{
		return FileTypes::FontTable;
	}
	const CPath CFontTable::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CFontTable::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CFontTable::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	void CFontTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		int nFontsDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nFontsDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:font" == sName )
			{
				CFont *pFont = new CFont(oReader);
				if (pFont)
				{
					m_arrFonts.push_back( pFont );
				}
			}
			else if (L"w:defaultFonts")
			{
				m_oDefaultFonts = oReader;
			}
		}
	}
	std::wstring CFontTable::toXML() const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<w:fonts xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">";

		for (size_t nIndex = 0; nIndex < m_arrFonts.size(); nIndex++ )
		{
			if (m_arrFonts[nIndex])
				sXml += m_arrFonts[nIndex]->toXML();
		}
		sXml += _T("</w:fonts>");

		return sXml;
	}
	EElementType CFontTable::getType() const
		{
			return et_w_fonts;
		}
} // namespace OOX
