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

#include "Bibliography.h"
#include "../../DesktopEditor/common/File.h"
#include "FileTypes.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CBibliography 22.6.2.60 (Part 1)
	//--------------------------------------------------------------------------------

	CBibliography::CBibliography(OOX::Document* pMain) : OOX::File(pMain)
	{

	}
	CBibliography::CBibliography(OOX::Document* pMain, const CPath& oPath) : OOX::File(pMain)
	{
		read( oPath );
	}
	void CBibliography::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( _T("b:Sources") == sName && !oReader.IsEmptyNode() )
		{
			ReadAttributes( oReader );
		}
	}
	void CBibliography::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml;
		sXml = _T("<b:Sources");

		if ( m_sSelectedStyle.IsInit() )
		{
			sXml += _T(" SelectedStyle=\"");
			sXml += m_sSelectedStyle.get2();
			sXml += _T("\"");
		}

		if ( m_sStyleName.IsInit() )
		{
			sXml += _T(" StyleName=\"");
			sXml += m_sStyleName.get2();
			sXml += _T("\"");
		}

		if ( m_sURI.IsInit() )
		{
			sXml += _T(" URI=\"");
			sXml += m_sURI.get2();
			sXml += _T("\"");
		}

		sXml += _T(" xmlns:b=\"http://schemas.openxmlformats.org/officeDocument/2006/bibliography\" xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/bibliography\">");


		sXml += _T("</a:Sources>");

        NSFile::CFileBinary::SaveToFile( oFilePath.GetPath(), sXml );
		oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
	}
	const OOX::FileType CBibliography::type() const
	{
		return FileTypes::Bibliography;
	}
	const CPath CBibliography::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CBibliography::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CBibliography::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("SelectedStyle"), m_sSelectedStyle )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("StyleName"),     m_sStyleName )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("URI"),           m_sURI )
			WritingElement_ReadAttributes_End( oReader )
		}

} // namespace OOX
