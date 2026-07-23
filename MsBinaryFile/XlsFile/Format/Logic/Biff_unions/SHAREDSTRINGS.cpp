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

#include "SHAREDSTRINGS.h"
#include "..//Biff_records/SST.h"
#include "../Biff_records/Continue.h"

namespace XLS
{


SHAREDSTRINGS::SHAREDSTRINGS(const unsigned short code_page)
: code_page_(code_page)
{
	size_ = 0 ;
}


SHAREDSTRINGS::~SHAREDSTRINGS()
{
}


BaseObjectPtr SHAREDSTRINGS::clone()
{
	return BaseObjectPtr(new SHAREDSTRINGS(*this));
}


// SHAREDSTRINGS = SST *Continue
const bool SHAREDSTRINGS::loadContent(BinProcessor& proc)
{
    SST sst(code_page_);
    if(!proc.mandatory(sst))
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	size_ = sst.rgb.size();
	return true;
}

const bool SHAREDSTRINGS::saveContent(BinProcessor& proc)
{
    if(sstPtr == nullptr)
        return false;
    proc.mandatory(*sstPtr);
    auto castedSst = static_cast<SST*>(sstPtr.get());
    if(!castedSst->rgb.empty())
    {
		const auto MaxRecordSize = 8000;
        while(!castedSst->rgb.empty())
        {
            CFRecordPtr tempRecord(new CFRecord(rt_Continue, proc.getGlobalWorkbookInfo()));
            Continue continueRecord;
            while(!castedSst->rgb.empty())
            {
                auto oldPose = tempRecord->getRdPtr();
				castedSst->rgb.at(0)->save(*tempRecord);
				if(tempRecord->getRdPtr() >= MaxRecordSize)
                {
                    tempRecord->RollRdPtrBack(tempRecord->getRdPtr() - oldPose);
                    break;
                }
                castedSst->rgb.erase(castedSst->rgb.begin());
            }
            continueRecord.m_iDataSize = tempRecord->getRdPtr();
            continueRecord.m_pData = new char[continueRecord.m_iDataSize];
            auto copyData = tempRecord->getCurStaticData<char>() - continueRecord.m_iDataSize;
            memcpy(continueRecord.m_pData, copyData, continueRecord.m_iDataSize);
            proc.mandatory(continueRecord);
        }

    }
    return true;
}

int SHAREDSTRINGS::serialize(std::wostream & stream)
{
	for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); ++it)
	{
		(*it)->serialize(stream);
	}
	return 0;
}


} // namespace XLS

