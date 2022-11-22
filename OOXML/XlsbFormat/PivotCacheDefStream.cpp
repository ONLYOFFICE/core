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

#include "PivotCacheDefStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_unions/ACUID.h"
#include "Biff12_records/BeginPivotCacheDef.h"
#include "Biff12_unions/PCDSOURCE.h"
#include "Biff12_unions/PCDFIELDS.h"
#include "Biff12_unions/PCDCALCITEMS.h"
#include "Biff12_unions/PCDHIERARCHIES.h"
#include "Biff12_unions/PCDKPIS.h"
#include "Biff12_unions/PCDCALCMEMS.h"
#include "Biff12_unions/PCDSDTUPLECACHE.h"
#include "Biff12_unions/DIMS.h"
#include "Biff12_unions/MGS.h"
#include "Biff12_unions/MGMAPS.h"
#include "Biff12_unions/FRTPIVOTCACHEDEF.h"
#include "Biff12_records/EndPivotCacheDef.h"

using namespace XLS;

namespace XLSB
{;

PivotCacheDefStream::PivotCacheDefStream()
{
}

PivotCacheDefStream::~PivotCacheDefStream()
{
}


BaseObjectPtr PivotCacheDefStream::clone()
{
        return BaseObjectPtr(new PivotCacheDefStream(*this));
}

const bool PivotCacheDefStream::loadContent(BinProcessor& proc)
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

                case rt_BeginPivotCacheDef:
                {
                    if (proc.optional<BeginPivotCacheDef>())
                    {
                        m_BrtBeginPivotCacheDef = elements_.back();
                        elements_.pop_back();
                    }
                }break;


                case rt_BeginPCDSource:
                {
                    if (proc.optional<PCDSOURCE>())
                    {
                        m_PCDSOURCE = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginPCDFields:
                {
                    if (proc.optional<PCDFIELDS>())
                    {
                        m_PCDFIELDS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginPCDCalcItems:
                {
                    if (proc.optional<PCDCALCITEMS>())
                    {
                        m_PCDCALCITEMS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginPCDHierarchies:
                {
                    if (proc.optional<PCDHIERARCHIES>())
                    {
                        m_PCDHIERARCHIES = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginPCDKPIs:
                {
                    if (proc.optional<PCDKPIS>())
                    {
                        m_PCDKPIS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginPCDCalcMems:
                {
                    if (proc.optional<PCDCALCMEMS>())
                    {
                        m_PCDCALCMEMS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginPCDSDTupleCache:
                {
                    if (proc.optional<PCDSDTUPLECACHE>())
                    {
                        m_PCDSDTUPLECACHE = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginDims:
                {
                    if (proc.optional<DIMS>())
                    {
                        m_DIMS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginMGs:
                {
                    if (proc.optional<MGS>())
                    {
                        m_MGS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_BeginMGMaps:
                {
                    if (proc.optional<MGMAPS>())
                    {
                        m_MGMAPS = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_FRTBegin:
                {
                    if (proc.optional<FRTPIVOTCACHEDEF>())
                    {
                        m_FRTPIVOTCACHEDEF = elements_.back();
                        elements_.pop_back();
                    }
                }break;

                case rt_EndPivotCacheDef:
                {
                    if (proc.optional<EndPivotCacheDef>())
                    {
                        m_BrtEndPivotCacheDef = elements_.back();
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
