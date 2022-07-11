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
#pragma once

#include "Docx.h"
#include "File.h"
#include "../Base/Nullable.h"
#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"
#include "../../../../DesktopEditor/common/SystemUtils.h"

namespace PPTX
{
	class App;
}
namespace OOX
{
	class CApp : public OOX::File
	{
	public:
		CApp(OOX::Document* pMain);
		CApp(OOX::Document* pMain, const CPath& oPath);
		virtual ~CApp()
		{
		}
		virtual void read(const CPath& oPath)
		{
			XmlUtils::CXmlNode oProperties;
			oProperties.FromXmlFile( oPath.GetPath(), true );

			if ( _T("Properties") == oProperties.GetName() )
			{
				XmlUtils::CXmlNode oItem;

				if ( oProperties.GetNode( _T("Application"), oItem ) )
					m_sApplication = oItem.GetText();

				if ( oProperties.GetNode( _T("AppVersion"), oItem ) )
					m_sAppVersion = oItem.GetText();

				if ( oProperties.GetNode( _T("Characters"), oItem ) )
					m_nCharacters = oItem.GetText();

				if ( oProperties.GetNode( _T("CharactersWithSpaces"), oItem ) )
					m_nCharactersWithSpaces = oItem.GetText();

				if ( oProperties.GetNode( _T("Company"), oItem ) )
					m_sCompany = oItem.GetText();

				if ( oProperties.GetNode( _T("DocSecurity"), oItem ) )
					m_nDocSecurity = oItem.GetText();

				if ( oProperties.GetNode( _T("HiddenSlides"), oItem ) )
					m_nHiddenSlides = oItem.GetText();

				if ( oProperties.GetNode( _T("HyperlinkBase"), oItem ) )
					m_sHyperlinkBase = oItem.GetText();

				if ( oProperties.GetNode( _T("HyperlinksChanged"), oItem ) )
					m_bHyperlinksChanged = oItem.GetText();

				if ( oProperties.GetNode( _T("Lines"), oItem ) )
					m_nLines = oItem.GetText();

				if ( oProperties.GetNode( _T("LinksUpToDate"), oItem ) )
					m_bLinksUpToDate = oItem.GetText();

				if ( oProperties.GetNode( _T("Manager"), oItem ) )
					m_sManager = oItem.GetText();

				if ( oProperties.GetNode( _T("MMClips"), oItem ) )
					m_nMMClips = oItem.GetText();

				if ( oProperties.GetNode( _T("Notes"), oItem ) )
					m_nNotes = oItem.GetText();

				if ( oProperties.GetNode( _T("Pages"), oItem ) )
					m_nPages = oItem.GetText();

				if ( oProperties.GetNode( _T("Paragraphs"), oItem ) )
					m_nParagraphs = oItem.GetText();

                if ( oProperties.GetNode( _T("ScaleCrop"), oItem ) )
					m_bScaleCrop = oItem.GetText();

				if ( oProperties.GetNode( _T("SharedDoc"), oItem ) )
					m_bSharedDoc = oItem.GetText();

				if ( oProperties.GetNode( _T("Slides"), oItem ) )
					m_nSlides = oItem.GetText();

				if ( oProperties.GetNode( _T("Template"), oItem ) )
					m_sTemplate = oItem.GetText();
				
				if ( oProperties.GetNode( _T("TotalTime"), oItem ) )
					m_nTotalTime = oItem.GetText();

				if ( oProperties.GetNode( _T("Words"), oItem ) )
					m_nWords = oItem.GetText();				
			}
		}
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
            std::wstring sXml;
			sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\">");

			if ( m_sApplication.IsInit() )
			{
				sXml += _T("<Application>");
				sXml += XmlUtils::EncodeXmlString(m_sApplication.get());
				if ( m_sAppVersion.IsInit() )
				{
					sXml += L"/";
					sXml += XmlUtils::EncodeXmlString(m_sAppVersion.get());
				}
				sXml += _T("</Application>");
			}

			//if ( m_sAppVersion.IsInit() ) - only for ms editors versions
			//{
			//	sXml += _T("<AppVersion>");
			//	sXml += m_sAppVersion.get(); // error in ms editors - "2.4.510.0"
			//	sXml += _T("</AppVersion>");
			//}

			if ( m_nCharacters.IsInit() )
			{
				sXml += _T("<Characters>");
				sXml += std::to_wstring(*m_nCharacters);
				sXml += _T("</Characters>");
			}

			if ( m_nCharactersWithSpaces.IsInit() )
			{
				sXml += _T("<CharactersWithSpaces>");
				sXml += std::to_wstring(*m_nCharactersWithSpaces);
				sXml += _T("</CharactersWithSpaces>");
			}

			if ( m_sCompany.IsInit() )
			{
				sXml += _T("<Company>");
				sXml += XmlUtils::EncodeXmlString(m_sCompany.get());
				sXml += _T("</Company>");
			}

			if ( m_nDocSecurity.IsInit() )
			{
				sXml += _T("<DocSecurity>");
				sXml += std::to_wstring(*m_nDocSecurity);
				sXml += _T("</DocSecurity>");
			}

			if ( m_nHiddenSlides.IsInit() )
			{
				sXml += _T("<HiddenSlides>");
				sXml += std::to_wstring(*m_nHiddenSlides);
				sXml += _T("</HiddenSlides>");
			}

			if ( m_sHyperlinkBase.IsInit() )
			{
				sXml += _T("<HyperlinkBase>");
				sXml += XmlUtils::EncodeXmlString(m_sHyperlinkBase.get());
				sXml += _T("</HyperlinkBase>");
			}

			if ( m_bHyperlinksChanged.IsInit() )
			{
				sXml += _T("<HyperlinksChanged>");
				sXml += *m_bHyperlinksChanged ? L"true" : L"false";
				sXml += _T("</HyperlinksChanged>");
			}

			if ( m_nLines.IsInit() )
			{
				sXml += _T("<Lines>");
				sXml += std::to_wstring(*m_nLines);
				sXml += _T("</Lines>");
			}

			if ( m_bLinksUpToDate.IsInit() )
			{
				sXml += _T("<LinksUpToDate>");
				sXml += *m_bLinksUpToDate ? L"true" : L"false";;
				sXml += _T("</LinksUpToDate>");
			}

			if ( m_sManager.IsInit() )
			{
				sXml += _T("<Manager>");
				sXml += XmlUtils::EncodeXmlString(m_sManager.get());
				sXml += _T("</Manager>");
			}

			if ( m_nMMClips.IsInit() )
			{
				sXml += _T("<MMClips>");
				sXml += std::to_wstring(*m_nMMClips);
				sXml += _T("</MMClips>");
			}

			if ( m_nNotes.IsInit() )
			{
				sXml += _T("<Notes>");
				sXml += std::to_wstring(*m_nNotes);
				sXml += _T("</Notes>");
			}

			if ( m_nPages.IsInit() )
			{
				sXml += _T("<Pages>");
				sXml += std::to_wstring(*m_nPages);
				sXml += _T("</Pages>");
			}

			if ( m_nParagraphs.IsInit() )
			{
				sXml += _T("<Paragraphs>");
				sXml += std::to_wstring(*m_nParagraphs);
				sXml += _T("</Paragraphs>");
			}

			if ( m_bScaleCrop.IsInit() )
			{
				sXml += _T("<ScaleCrop>");
				sXml += *m_bScaleCrop ? L"true" : L"false";;
				sXml += _T("</ScaleCrop>");
			}

			if ( m_bSharedDoc.IsInit() )
			{
				sXml += _T("<SharedDoc>");
				sXml += *m_bSharedDoc ? L"true" : L"false";;
				sXml += _T("</SharedDoc>");
			}

			if ( m_nSlides.IsInit() )
			{
				SimpleTypes::CDecimalNumber<> oNum;
				oNum.SetValue( m_nSlides.get() );

				sXml += _T("<Slides>");
				sXml += std::to_wstring(*m_nSlides);
				sXml += _T("</Slides>");
			}

			if ( m_sTemplate.IsInit() )
			{
				sXml += _T("<Template>");
				sXml += XmlUtils::EncodeXmlString(m_sTemplate.get());
				sXml += _T("</Template>");
			}

			if ( m_nTotalTime.IsInit() )
			{
				sXml += _T("<TotalTime>");
				sXml += std::to_wstring(*m_nTotalTime);
				sXml += _T("</TotalTime>");
			}

			if ( m_nWords.IsInit() )
			{
				sXml += _T("<Words>");
				sXml += std::to_wstring(*m_nWords);
				sXml += _T("</Words>");
			}

			sXml += _T("</Properties>");

			NSFile::CFileBinary::SaveToFile( oPath.GetPath(), sXml );
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		}

		virtual const OOX::FileType type() const
		{
			return FileTypes::App;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		void SetDefaults()
		{
			SetRequiredDefaults();
			
			m_nDocSecurity = 0;
			m_bScaleCrop = false;
			m_bLinksUpToDate = false;
			m_bSharedDoc = false;
			m_bHyperlinksChanged = false;
		}
		void SetRequiredDefaults()
		{
			m_sAppVersion.reset(NULL);
			std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
			if (sApplication.empty())
				sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
			std::string s = VALUE2STR(INTVER);
			sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif
			m_sApplication = sApplication;
		}
 
		PPTX::App* ToPptxApp();
		void FromPptxApp(PPTX::App* pApp);

		// TO DO: DigSig
		//        HeadingPairs
		//        HLinks
		//        TitlesOfParts

        nullable_string	m_sApplication;
		nullable_string	m_sAppVersion;
		nullable_int	m_nCharacters;
		nullable_int	m_nCharactersWithSpaces;
        nullable_string	m_sCompany;
		nullable_int	m_nDocSecurity;
		nullable_int	m_nHiddenSlides;
        nullable_string	m_sHyperlinkBase;
		nullable_bool	m_bHyperlinksChanged;
		nullable_int	m_nLines;
		nullable_bool	m_bLinksUpToDate;
        nullable_string	m_sManager;
		nullable_int	m_nMMClips;
		nullable_int	m_nNotes;
		nullable_int	m_nPages;
		nullable_int	m_nParagraphs;
        nullable_string	m_sPresentationForm;
		nullable_bool	m_bScaleCrop;
		nullable_bool	m_bSharedDoc;
		nullable_int	m_nSlides;
        nullable_string	m_sTemplate;
		nullable_int	m_nTotalTime;
		nullable_int	m_nWords;
	};
} // namespace OOX

