/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "App.h"
#include "../XlsxFormat/Xlsx.h"
#include "../PPTXFormat/App.h"

namespace OOX
{
	CApp::CApp(OOX::Document* pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pApp = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pApp = this;
	}
	CApp::CApp(OOX::Document* pMain, const CPath& oPath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pApp = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pApp = this;

		read( oPath );
	}
	CApp::~CApp()
	{
	}
	PPTX::App* CApp::ToPptxApp()
	{
		PPTX::App* res = new PPTX::App(NULL);
		if(m_sTemplate.IsInit())
			res->Template = m_sTemplate.get();
		if(m_nTotalTime.IsInit())
			res->TotalTime = m_nTotalTime.get();
		if(m_nWords.IsInit())
			res->Words = m_nWords.get();
		if(m_sApplication.IsInit())
			res->Application = m_sApplication.get();
		if(m_sPresentationForm.IsInit())
			res->PresentationFormat = m_sPresentationForm.get();
		if(m_nParagraphs.IsInit())
			res->Paragraphs = m_nParagraphs.get();
		if(m_nSlides.IsInit())
			res->Slides = m_nSlides.get();
		if(m_nNotes.IsInit())
			res->Notes = m_nNotes.get();
		if(m_nHiddenSlides.IsInit())
			res->HiddenSlides = m_nHiddenSlides.get();
		if(m_nMMClips.IsInit())
			res->MMClips = m_nMMClips.get();
		if(m_bScaleCrop.IsInit())
			res->ScaleCrop = m_bScaleCrop.get();
		if(m_sCompany.IsInit())
			res->Company = m_sCompany.get();
		if(m_bLinksUpToDate.IsInit())
			res->LinksUpToDate = m_bLinksUpToDate.get();
		if(m_bSharedDoc.IsInit())
			res->SharedDoc = m_bSharedDoc.get();
		if(m_bHyperlinksChanged.IsInit())
			res->HyperlinksChanged = m_bHyperlinksChanged.get();
		if(m_sAppVersion.IsInit())
			res->AppVersion = m_sAppVersion.get();
		if(m_nCharacters.IsInit())
			res->Characters = m_nCharacters.get();
		if(m_nCharactersWithSpaces.IsInit())
			res->CharactersWithSpaces = m_nCharactersWithSpaces.get();
		if(m_nDocSecurity.IsInit())
			res->DocSecurity = m_nDocSecurity.get();
		if(m_sHyperlinkBase.IsInit())
			res->HyperlinkBase = m_sHyperlinkBase.get();
		if(m_nLines.IsInit())
			res->Lines = m_nLines.get();
		if(m_sManager.IsInit())
			res->Manager = m_sManager.get();
		if(m_nPages.IsInit())
			res->Pages = m_nPages.get();
		return res;
	}
	void CApp::FromPptxApp(PPTX::App* pApp)
	{
		if(pApp->Template.IsInit())
			m_sTemplate = pApp->Template.get();
		if(pApp->TotalTime.IsInit())
			m_nTotalTime = pApp->TotalTime.get();
		if(pApp->Words.IsInit())
			m_nWords = pApp->Words.get();
		if(pApp->Application.IsInit())
			m_sApplication = pApp->Application.get();
		if(pApp->PresentationFormat.IsInit())
			m_sPresentationForm = pApp->PresentationFormat.get();
		if(pApp->Paragraphs.IsInit())
			m_nParagraphs = pApp->Paragraphs.get();
		if(pApp->Slides.IsInit())
			m_nSlides = pApp->Slides.get();
		if(pApp->Notes.IsInit())
			m_nNotes = pApp->Notes.get();
		if(pApp->HiddenSlides.IsInit())
			m_nHiddenSlides = pApp->HiddenSlides.get();
		if(pApp->MMClips.IsInit())
			m_nMMClips = pApp->MMClips.get();
		if(pApp->ScaleCrop.IsInit())
			m_bScaleCrop = pApp->ScaleCrop.get();
		if(pApp->Company.IsInit())
			m_sCompany = pApp->Company.get();
		if(pApp->LinksUpToDate.IsInit())
			m_bLinksUpToDate = pApp->LinksUpToDate.get();
		if(pApp->SharedDoc.IsInit())
			m_bSharedDoc = pApp->SharedDoc.get();
		if(pApp->HyperlinksChanged.IsInit())
			m_bHyperlinksChanged = pApp->HyperlinksChanged.get();
		if(pApp->AppVersion.IsInit())
			m_sAppVersion = pApp->AppVersion.get();
		if(pApp->Characters.IsInit())
			m_nCharacters = pApp->Characters.get();
		if(pApp->CharactersWithSpaces.IsInit())
			m_nCharactersWithSpaces = pApp->CharactersWithSpaces.get();
		if(pApp->DocSecurity.IsInit())
			m_nDocSecurity = pApp->DocSecurity.get();
		if(pApp->HyperlinkBase.IsInit())
			m_sHyperlinkBase = pApp->HyperlinkBase.get();
		if(pApp->Lines.IsInit())
			m_nLines = pApp->Lines.get();
		if(pApp->Manager.IsInit())
			m_sManager = pApp->Manager.get();
		if(pApp->Pages.IsInit())
			m_nPages = pApp->Pages.get();
	}
	void CApp::read(const CPath& oPath)
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
	void CApp::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
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
			SimpleTypes::CDecimalNumber oNum;
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
	const OOX::FileType CApp::type() const
	{
		return FileTypes::App;
	}
	const CPath CApp::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CApp::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CApp::SetDefaults()
	{
		SetRequiredDefaults();

		m_nDocSecurity = 0;
		m_bScaleCrop = false;
		m_bLinksUpToDate = false;
		m_bSharedDoc = false;
		m_bHyperlinksChanged = false;
	}
	void CApp::SetRequiredDefaults()
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

} // namespace OOX
