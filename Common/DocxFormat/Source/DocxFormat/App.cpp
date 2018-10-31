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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

		return res;
	}
} // namespace OOX
