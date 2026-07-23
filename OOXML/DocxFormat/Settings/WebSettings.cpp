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

#include "WebSettings.h"
#include "../../../DesktopEditor/common/File.h"
#include "../FileTypes.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// OptimizeForBrowser 17.15.2.34 (Part 1)
		//--------------------------------------------------------------------------------

		COptimizeForBrowser::COptimizeForBrowser()
		{
		}
		COptimizeForBrowser::~COptimizeForBrowser()
		{
		}
		void    COptimizeForBrowser::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:val"),    m_oVal );
			XmlMacroReadAttributeBase( oNode, _T("w:target"), m_oTarget );
		}
		void    COptimizeForBrowser::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:val"),    m_oVal )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:target"), m_oTarget )
			WritingElement_ReadAttributes_End( oReader )
		}
		std::wstring COptimizeForBrowser::ToString() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute( _T("w:val=\""),    m_oVal );
			ComplexTypes_WriteAttribute( _T("w:target=\""), m_oTarget );

			return sResult;
		}

	} // Word
} // ComplexTypes

namespace OOX
{
	// TO DO: Complete this class

	//--------------------------------------------------------------------------------
	// CWebSettings 17.11.15
	//--------------------------------------------------------------------------------

	CWebSettings::CWebSettings(OOX::Document *pMain) : OOX::File(pMain)
	{
	}
	CWebSettings::CWebSettings(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain)
	{
		read( oPath );
	}
	CWebSettings::~CWebSettings()
	{
	}
	void CWebSettings::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( _T("w:webSettings") == sName && !oReader.IsEmptyNode() )
		{
			int nStylesDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
			{
				sName = oReader.GetName();
				if ( _T("w:allowPNG") == sName )
					m_oAllowPNG = oReader;
				else if ( _T("w:optimizeForBrowser") == sName )
					m_oOptimizeForBrowser = oReader;
			}
		}
	}
	void CWebSettings::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml;
		sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">");

		if ( m_oAllowPNG.IsInit() )
		{
			sXml += _T("<w:allowPNG ");
			sXml += m_oAllowPNG->ToString();
			sXml += _T("/>");
		}

		if ( m_oOptimizeForBrowser.IsInit() )
		{
			sXml += _T("<w:optimizeForBrowser ");
			sXml += m_oOptimizeForBrowser->ToString();
			sXml += _T("/>");
		}

		sXml += _T("</w:webSettings>");

        NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), sXml );

		oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
	}
	const OOX::FileType CWebSettings::type() const
	{
        return FileTypes::WebSetting;
	}
	const CPath CWebSettings::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CWebSettings::DefaultFileName() const
		{
			return type().DefaultFileName();
		}

} // namespace OOX
