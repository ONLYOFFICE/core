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

#include "PivotTableStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_unions/ACUID.h"
#include "Biff12_records/BeginSXView.h"
#include "Biff12_unions/SXLOCATION.h"
#include "Biff12_unions/SXVDS.h"
#include "Biff12_unions/ISXVDRWS.h"
#include "Biff12_unions/SXLIRWS.h"
#include "Biff12_unions/ISXVDCOLS.h"
#include "Biff12_unions/SXLICOLS.h"
#include "Biff12_unions/SXPIS.h"
#include "Biff12_unions/SXDIS.h"
#include "Biff12_unions/SXFORMATS.h"
#include "Biff12_unions/SXCONDFMTS.h"
#include "Biff12_unions/SXCRTFORMATS.h"
#include "Biff12_unions/SXTHS.h"
#include "Biff12_records/TableStyleClient.h"
#include "Biff12_unions/SXFILTERS.h"
#include "Biff12_unions/ISXTHRWS.h"
#include "Biff12_unions/ISXTHCOLS.h"
#include "Biff12_unions/FRTSXVIEW.h"
#include "Biff12_records/EndSXView.h"

using namespace XLS;

namespace XLSB
{;

PivotTableStream::PivotTableStream()
{
}

PivotTableStream::~PivotTableStream()
{
}


BaseObjectPtr PivotTableStream::clone()
{
        return BaseObjectPtr(new PivotTableStream(*this));
}

const bool PivotTableStream::loadContent(BinProcessor& proc)
{    
	while (true)
	{
            CFRecordType::TypeId type = proc.getNextRecordType();

            if (type == rt_NONE) break;

            switch(type)
            {
                case rt_ACBegin:
                {
                    if (proc.optional<ACUID>())
                    {
                        //m_ACUID = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXView:
                {
                    if (proc.optional<BeginSXView>())
                    {
                        m_BrtBeginSXView = elements_.back();
                        elements_.pop_back();
                    }
                }break;


                case rt_BeginSXLocation:
                {
                    if (proc.optional<SXLOCATION>())
                    {
                        m_SXLOCATION = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXVDs:
                {
                    if (proc.optional<SXVDS>())
                    {
                        m_SXVDS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginISXVDRws:
                {
                    if (proc.optional<ISXVDRWS>())
                    {
                        m_ISXVDRWS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXLIRws:
                {
                    if (proc.optional<SXLIRWS>())
                    {
                        m_SXLIRWS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginISXVDCols:
                {
                    if (proc.optional<ISXVDCOLS>())
                    {
                        m_ISXVDCOLS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXLICols:
                {
                    if (proc.optional<SXLICOLS>())
                    {
                        m_SXLICOLS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXPIs:
                {
                    if (proc.optional<SXPIS>())
                    {
                        m_SXPIS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXDIs:
                {
                    if (proc.optional<SXDIS>())
                    {
                        m_SXDIS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXFormats:
                {
                    if (proc.optional<SXFORMATS>())
                    {
                        m_SXFORMATS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXCondFmts:
                {
                    if (proc.optional<SXCONDFMTS>())
                    {
                        m_SXCONDFMTS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXCrtFormats:
                {
                    if (proc.optional<SXCRTFORMATS>())
                    {
                        m_SXCRTFORMATS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXTHs:
                {
                    if (proc.optional<SXTHS>())
                    {
                        m_SXTHS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_TableStyleClient:
                {
                    if (proc.optional<TableStyleClient>())
                    {
                        m_BrtTableStyleClient = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginSXFilters:
                {
                    if (proc.optional<SXFILTERS>())
                    {
                        m_SXFILTERS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginISXTHRws:
                {
                    if (proc.optional<ISXTHRWS>())
                    {
                        m_ISXTHRWS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginISXTHCols:
                {
                    if (proc.optional<ISXTHCOLS>())
                    {
                        m_ISXTHCOLS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_FRTBegin:
                {
                    if (proc.optional<FRTSXVIEW>())
                    {
                        m_FRTSXVIEW = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_EndSXView:
                {
                    if (proc.optional<EndSXView>())
                    {
                        m_BrtEndSXView = elements_.back();
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
