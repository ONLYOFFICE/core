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
#include "ReaderClasses.h"

namespace BinDocxRW {
	TrackRevision::TrackRevision()
	{
		Id = NULL;
		vMerge = NULL;
		vMergeOrigin = NULL;
		RPr = NULL;
		PPr = NULL;
		sectPr = NULL;
		tblPr = NULL;
		tblGridChange = NULL;
		trPr = NULL;
		tcPr = NULL;
		content = NULL;
		contentRun = NULL;
	}
	TrackRevision::~TrackRevision()
	{
		RELEASEOBJECT(Id);
		RELEASEOBJECT(vMerge);
		RELEASEOBJECT(vMergeOrigin);
		RELEASEOBJECT(RPr);
		RELEASEOBJECT(PPr);
		RELEASEOBJECT(sectPr);
		RELEASEOBJECT(tblPr);
		RELEASEOBJECT(tblGridChange);
		RELEASEOBJECT(trPr);
		RELEASEOBJECT(tcPr);
		RELEASEOBJECT(content);
		RELEASEOBJECT(contentRun);
	}
	void TrackRevision::Write(NSStringUtils::CStringBuilder*  pCStringWriter, std::wstring sName)
	{
		if (IsNoEmpty())
		{
			pCStringWriter->WriteString(L"<");
			pCStringWriter->WriteString(sName);
			if (NULL != Id)
			{
				pCStringWriter->WriteString(L" w:id=\"" + std::to_wstring(*Id) + L"\"");
			}
			if (!Author.empty())
			{
				pCStringWriter->WriteString(L" w:author=\"");
				pCStringWriter->WriteEncodeXmlString(Author);
				pCStringWriter->WriteString(L"\"");
			}
			if (!Date.empty())
			{
				pCStringWriter->WriteString(L" w:date=\"");
				pCStringWriter->WriteEncodeXmlString(Date);
				pCStringWriter->WriteString(L"\"");
			}
			if (!UserId.empty())
			{
				pCStringWriter->WriteString(L" oouserid=\"");
				pCStringWriter->WriteEncodeXmlString(UserId);
				pCStringWriter->WriteString(L"\"");
			}
			if (NULL != vMerge)
			{
				pCStringWriter->WriteString(L" w:vMerge=\"" + std::to_wstring(*vMerge) + L"\"");
			}
			if (NULL != vMergeOrigin)
			{
				pCStringWriter->WriteString(L" w:vMergeOrig=\"" + std::to_wstring(*vMergeOrigin) + L"\"");
			}
			if (NULL != RPr || NULL != PPr || NULL != sectPr || NULL != tblPr || NULL != tblGridChange || NULL != trPr || NULL != tcPr || NULL != content || NULL != contentRun)
			{
				pCStringWriter->WriteString(L">");
				if (NULL != RPr)
				{
					pCStringWriter->WriteString(RPr->toXML());
				}
				if (NULL != PPr)
				{
					pCStringWriter->WriteString(L"<w:pPr>");
					pCStringWriter->Write(*PPr);
					pCStringWriter->WriteString(L"</w:pPr>");
				}
				if (NULL != sectPr)
				{
					pCStringWriter->WriteString(L"<w:sectPr>");
					pCStringWriter->WriteString(sectPr->Write());
					pCStringWriter->WriteString(L"</w:sectPr>");
				}
				if (NULL != tblPr)
				{
					pCStringWriter->WriteString(tblPr->Write());
				}
				if (NULL != tblGridChange)
				{
					pCStringWriter->WriteString(L"<w:tblGrid>");
					pCStringWriter->Write(*tblGridChange);
					pCStringWriter->WriteString(L"</w:tblGrid>");
				}
				if (NULL != trPr)
				{
					pCStringWriter->WriteString(L"<w:trPr>");
					pCStringWriter->Write(*trPr);
					pCStringWriter->WriteString(L"</w:trPr>");
				}
				if (NULL != tcPr)
				{
					pCStringWriter->WriteString(L"<w:tcPr>");
					pCStringWriter->Write(*tcPr);
					pCStringWriter->WriteString(L"</w:tcPr>");
				}
				if (NULL != content)
				{
					pCStringWriter->Write(*content);
				}
				if (NULL != contentRun)
				{
					pCStringWriter->Write(*contentRun);
				}

				pCStringWriter->WriteString(L"</");
				pCStringWriter->WriteString(sName);
				pCStringWriter->WriteString(L">");
			}
			else
			{
				pCStringWriter->WriteString(L"/>");
			}
		}
	}

}
