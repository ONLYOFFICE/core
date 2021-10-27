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

#include "WorkSheetStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_records/BeginSheet.h""
#include "Biff12_unions/COLINFOS.h"
#include "Biff12_records/WsDim.h"
#include "Biff12_records/Drawing.h""
#include "Biff12_records/LegacyDrawing.h"
#include "Biff12_records/LegacyDrawingHF.h"
#include "Biff12_unions/HLINKS.h"
#include "Biff12_unions/MERGECELLS.h"
#include "Biff12_unions/CELLTABLE.h"
#include "Biff12_records/WsFmtInfo.h"
#include "Biff12_unions/WSVIEWS2.h"
#include "Biff12_records/SheetProtectionIso.h"
#include "Biff12_records/SheetProtection.h"
#include "Biff12_unions/LISTPARTS.h"
#include "Biff12_unions/SORTSTATE.h"
#include "Biff12_unions/CONDITIONALFORMATTING.h"
#include "Biff12_unions/FRTWORKSHEET.h"
#include "Biff12_records/EndSheet.h"

namespace XLSB
{;

WorkSheetStream::WorkSheetStream()
{
}

WorkSheetStream::~WorkSheetStream()
{
}


BaseObjectPtr WorkSheetStream::clone()
{
        return BaseObjectPtr(new WorkSheetStream(*this));
}

const bool WorkSheetStream::loadContent(BinProcessor& proc)
{
	int	count = 0;
	
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;

		switch(type)
		{
            case rt_BeginSheet:
            {
                if (proc.optional<BeginSheet>())
                {
                    m_BrtBeginSheet = elements_.back();
                    elements_.pop_back();
                }
            }break;           

            case rt_BeginColInfos:
            {
                count = proc.repeated<COLINFOS>(0, 0);
                while(count > 0)
                {
                    m_arCOLINFOS.insert(m_arCOLINFOS.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_WsDim:
            {
                if (proc.optional<WsDim>())
                {
                    m_BrtWsDim = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_Drawing:
            {
                if (proc.optional<Drawing>())
                {
                    m_BrtDrawing = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_LegacyDrawing:
            {
                if (proc.optional<LegacyDrawing>())
                {
                    m_BrtLegacyDrawing = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_LegacyDrawingHF:
            {
                if (proc.optional<LegacyDrawingHF>())
                {
                    m_BrtLegacyDrawingHF = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_HLink:
            {
                if (proc.optional<HLINKS>())
                {
                    m_HLINKS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginMergeCells:
            {
                if (proc.optional<MERGECELLS>())
                {
                    m_MERGECELLS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginSheetData:
            {
                if (proc.optional<CELLTABLE>())
                {
                    m_CELLTABLE = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_WsFmtInfo:
            {
                if (proc.optional<WsFmtInfo>())
                {
                    m_BrtWsFmtInfo = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginWsViews:
            {
                if (proc.optional<WSVIEWS2>())
                {
                    m_WSVIEWS2 = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_SheetProtectionIso:
            {
                if (proc.optional<SheetProtectionIso>())
                {
                    m_BrtSheetProtectionIso = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_SheetProtection:
            {
                if (proc.optional<SheetProtection>())
                {
                    m_BrtSheetProtection = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginListParts:
            {
                if (proc.optional<LISTPARTS>())
                {
                    m_LISTPARTS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginSortState:
            {
                if (proc.optional<SORTSTATE>())
                {
                    m_SORTSTATE = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginConditionalFormatting:
            {
                count = proc.repeated<CONDITIONALFORMATTING>(0, 0);
                while(count > 0)
                {
                    m_arCONDITIONALFORMATTING.insert(m_arCONDITIONALFORMATTING.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;           

            case rt_FRTBegin:
            {
                if (proc.optional<FRTWORKSHEET>())
                {
                    m_FRTWORKSHEET = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_EndSheet:
            {
                if (proc.optional<EndSheet>())
                {
                    m_BrtEndSheet = elements_.back();
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

} // namespace XLSB
