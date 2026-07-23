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

#include "ExternalLinkStream.h"

#include "Biff12_unions/EXTERNALLINK.h"
#include "Biff12_records/BeginSupBook.h"


using namespace XLS;

namespace XLSB
{;

ExternalLinkStream::ExternalLinkStream()
{
}

ExternalLinkStream::~ExternalLinkStream()
{
}


BaseObjectPtr ExternalLinkStream::clone()
{
        return BaseObjectPtr(new ExternalLinkStream(*this));
}

const bool ExternalLinkStream::loadContent(BinProcessor& proc)
{
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;

		switch(type)
        {
            case rt_BeginSupBook:
            {
                if (proc.optional<EXTERNALLINK>())
                {
                    m_EXTERNALLINK = elements_.back();
                    elements_.pop_back();
                }
            }break;

			default://skip					
			{
				proc.SkipRecord();	
			}break;
		}
	}

	return true;
}

const bool ExternalLinkStream::saveContent(XLS::BinProcessor & proc)
{
	if (m_EXTERNALLINK != nullptr)
		proc.mandatory(*m_EXTERNALLINK);

	return true;
}

void ExternalLinkStream::UpdateXti(XLS::GlobalWorkbookInfo* global_info_, const std::wstring & rId)
{
    for (size_t i = 0; i < global_info_->arXti_External.size(); i++)
    {
       if(global_info_->arXti_External[i].link == rId)
       {
           auto & xti = global_info_->arXti_External[i];
           if (xti.itabFirst >= 0)
           {
               std::wstring strRange;
               if(-1 == xti.itabFirst)
               {
                   strRange = L"#REF";
               }
               else if (xti.itabFirst < global_info_->external_sheets_info.size())
               {
                   strRange = XMLSTUFF::name2sheet_name(global_info_->external_sheets_info[xti.itabFirst], L"");
                   if (xti.itabFirst != xti.itabLast)
                   {
                       strRange += std::wstring(L":") + XMLSTUFF::name2sheet_name(global_info_->external_sheets_info[xti.itabLast], L"");
                   }
               }
               xti.link = std::wstring(L"[") + std::to_wstring(xti.iSup) + std::wstring(L"]") + strRange;
           }
       }
    }
}

} // namespace XLSB
