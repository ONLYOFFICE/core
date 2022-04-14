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

#include "ChartSheetStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_records/BeginSheet.h"
#include "Biff12_records/Drawing.h"
#include "Biff12_records/LegacyDrawing.h"
#include "Biff12_records/LegacyDrawingHF.h"
#include "Biff12_unions/CSVIEWS.h"
#include "Biff12_records/Margins.h"
#include "Biff12_unions/HEADERFOOTER.h"
#include "Biff12_records/CsProp.h"
#include "Biff12_records/CsProtectionIso.h"
#include "Biff12_records/CsProtection.h"
#include "Biff12_unions/USERCSVIEWS.h"
#include "Biff12_records/BkHim.h"
#include "Biff12_unions/WEBPUBITEMS.h"
#include "Biff12_records/EndSheet.h"

using namespace XLS;

namespace XLSB
{;

ChartSheetStream::ChartSheetStream()
{
}

ChartSheetStream::~ChartSheetStream()
{
}


BaseObjectPtr ChartSheetStream::clone()
{
        return BaseObjectPtr(new ChartSheetStream(*this));
}

const bool ChartSheetStream::loadContent(BinProcessor& proc)
{
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

            case rt_BeginCsViews:
            {
                if (proc.optional<CSVIEWS>())
                {
                    m_CSVIEWS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_Margins:
            {
                if (proc.optional<Margins>())
                {
                    m_BrtMargins = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_CsPageSetup:
            {
                if (proc.optional<CsPageSetup>())
                {
                    m_BrtCsPageSetup = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_CsProp:
            {
                if (proc.optional<CsProp>())
                {
                    m_BrtCsProp = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginHeaderFooter:
            {
                if (proc.optional<HEADERFOOTER>())
                {
                    m_HEADERFOOTER = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_CsProtectionIso:
            {
                if (proc.optional<CsProtectionIso>())
                {
                    m_BrtCsProtectionIso = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_CsProtection:
            {
                if (proc.optional<CsProtection>())
                {
                    m_BrtCsProtection = elements_.back();
                    elements_.pop_back();
                }
            }break;           

            case rt_BkHim:
            {
                if (proc.optional<BkHim>())
                {
                    m_BrtBkHim = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginUserCsViews:
            {
                if (proc.optional<USERCSVIEWS>())
                {
                    m_USERCSVIEWS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginWebPubItems:
            {
                if (proc.optional<WEBPUBITEMS>())
                {
                    m_WEBPUBITEMS = elements_.back();
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
