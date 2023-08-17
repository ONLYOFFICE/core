﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

