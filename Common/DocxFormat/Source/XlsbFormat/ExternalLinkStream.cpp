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
