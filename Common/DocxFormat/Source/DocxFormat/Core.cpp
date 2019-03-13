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
#include "Core.h"
#include "../XlsxFormat/Xlsx.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Core.h"

namespace OOX
{
	CCore::CCore(OOX::Document* pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCore = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pCore = this;
	}
	CCore::CCore(OOX::Document* pMain, const CPath& oPath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pCore = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pCore = this;

		read( oPath );
	}

	PPTX::Core* CCore::ToPptxCore()
	{
		PPTX::Core* res = new PPTX::Core(NULL);
		if (m_sCategory.IsInit())
			res->category = m_sCategory.get();
		if (m_sContentStatus.IsInit())
			res->contentStatus = m_sContentStatus.get();
		if (m_sCreated.IsInit())
			res->created = m_sCreated.get();
		if (m_sCreator.IsInit())
			res->creator = m_sCreator.get();
		if (m_sDescription.IsInit())
			res->description = m_sDescription.get();
		if (m_sIdentifier.IsInit())
			res->identifier = m_sIdentifier.get();
		if (m_sKeywords.IsInit())
			res->keywords = m_sKeywords.get();
		if (m_sLanguage.IsInit())
			res->language = m_sLanguage.get();
		if (m_sLastModifiedBy.IsInit())
			res->lastModifiedBy = m_sLastModifiedBy.get();
		if (m_sLastPrinted.IsInit())
			res->lastPrinted = m_sLastPrinted.get();
		if (m_sModified.IsInit())
			res->modified = m_sModified.get();
		if (m_sRevision.IsInit())
			res->revision = m_sRevision.get();
		if (m_sSubject.IsInit())
			res->subject = m_sSubject.get();
		if (m_sTitle.IsInit())
			res->title = m_sTitle.get();
		if (m_sVersion.IsInit())
			res->version = m_sVersion.get();
		return res;
	}
	void CCore::FromPptxCore(PPTX::Core* pCore)
	{
		if(pCore->category.IsInit())
			m_sCategory = pCore->category.get();
		if(pCore->contentStatus.IsInit())
			m_sContentStatus = pCore->contentStatus.get();
		if(pCore->created.IsInit())
			m_sCreated = pCore->created.get();
		if(pCore->creator.IsInit())
			m_sCreator = pCore->creator.get();
		if(pCore->description.IsInit())
			m_sDescription = pCore->description.get();
		if(pCore->identifier.IsInit())
			m_sIdentifier = pCore->identifier.get();
		if(pCore->keywords.IsInit())
			m_sKeywords = pCore->keywords.get();
		if(pCore->language.IsInit())
			m_sLanguage = pCore->language.get();
		if(pCore->lastModifiedBy.IsInit())
			m_sLastModifiedBy = pCore->lastModifiedBy.get();
		if(pCore->lastPrinted.IsInit())
			m_sLastPrinted = pCore->lastPrinted.get();
		if(pCore->modified.IsInit())
			m_sModified = pCore->modified.get();
		if(pCore->revision.IsInit())
			m_sRevision = pCore->revision.get();
		if(pCore->subject.IsInit())
			m_sSubject = pCore->subject.get();
		if(pCore->title.IsInit())
			m_sTitle = pCore->title.get();
		if(pCore->version.IsInit())
			m_sVersion = pCore->version.get();
	}
} // namespace OOX
