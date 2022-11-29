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
	// TO DO: Доделать данный класс

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
