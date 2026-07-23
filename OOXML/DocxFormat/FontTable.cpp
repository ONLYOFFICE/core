/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "FontTable.h"

#include "../../DesktopEditor/common/Directory.h"
#include "FileTypes.h"
#include "Docx.h"

namespace OOX
{
	CFontTable::CFontTable() {}
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
				CFont *pFont = new CFont();
				*pFont = oReader;
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
