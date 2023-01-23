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
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("SelectedStyle"), m_sSelectedStyle )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("StyleName"),     m_sStyleName )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("URI"),           m_sURI )
			WritingElement_ReadAttributes_End( oReader )
		}

} // namespace OOX
