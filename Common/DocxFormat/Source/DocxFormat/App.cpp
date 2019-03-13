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
#include "../../../../ASCOfficePPTXFile/PPTXFormat/App.h"

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
} // namespace OOX
