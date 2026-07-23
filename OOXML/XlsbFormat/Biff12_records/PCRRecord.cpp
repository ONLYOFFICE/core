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

#include "PCRRecord.h"
#include "../Biff12_structures/PCDIDateTime.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/Xnum.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/XLWideString.h"

using namespace XLS;

namespace XLSB
{

    PCRRecord::PCRRecord()
    {
    }

    PCRRecord::~PCRRecord()
    {
    }

    BaseObjectPtr PCRRecord::clone()
    {
        return BaseObjectPtr(new PCRRecord(*this));
    }

    void PCRRecord::readFields(XLS::CFRecord& record)
    {
        _UINT32 index;
        Xnum xnum;
        PCDIDateTime dateTime;
        XLWideString string;

        if (record.checkFitReadSafe(1))
        {
            auto arrPivotCacheRecordType = record.getGlobalWorkbookInfo()->pivotCacheRecordType.find(record.getGlobalWorkbookInfo()->currentPivotCacheRecord - 1);
            if (arrPivotCacheRecordType != record.getGlobalWorkbookInfo()->pivotCacheRecordType.end())
            {
                for(const auto& item : arrPivotCacheRecordType->second)
                switch (item)
                {
                    case XLS::typePCDIIndex:
                        record >> index;
                        data.push_back({XLS::typePCDIIndex, index});
                        break;
                    case XLS::typePCDINumber:
                        record >> xnum;
                        data.push_back({XLS::typePCDINumber, xnum.data.value});
                        break;
                    case XLS::typePCDIDatetime:
                        record >> dateTime;
                        data.push_back({XLS::typePCDIDatetime, dateTime.value()});
                        break;
                    case XLS::typePCDIString:
                        record >> string;
                        data.push_back({XLS::typePCDIString, string.value()});
                        break;
                }
            }
        }
    }

	void PCRRecord::writeFields(XLS::CFRecord& record)
	{
		_UINT32 index;
		Xnum xnum;
		PCDIDateTime dateTime;
		XLWideString string;

		for (const auto& item : data)
			switch (item.first)
			{
			case XLS::typePCDIIndex:
				index = *boost::any_cast<_UINT32>(&item.second);
				record << index;
				break;
			case XLS::typePCDINumber:
				xnum.data.value = *boost::any_cast<double>(&item.second);
				record << xnum;
				break;
			case XLS::typePCDIDatetime:
				dateTime.fromString(*boost::any_cast<std::wstring>(&item.second));
				record << dateTime;
				break;
			case XLS::typePCDIString:
				string = *boost::any_cast<std::wstring>(&item.second);
				record << string;
				break;
			}			
	}

} // namespace XLSB

