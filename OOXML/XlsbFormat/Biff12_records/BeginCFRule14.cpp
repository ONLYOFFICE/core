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

#include "BeginCFRule14.h"

using namespace XLS;

namespace XLSB
{

    BeginCFRule14::BeginCFRule14()
    {
    }

    BeginCFRule14::~BeginCFRule14()
    {
    }

    BaseObjectPtr BeginCFRule14::clone()
    {
        return BaseObjectPtr(new BeginCFRule14(*this));
    }

    void BeginCFRule14::readFields(XLS::CFRecord& record)
    {
        record >> FRTheader >> iType >> iTemplate >> dxfId >> iPri >> iParam;

        if(iType.value().get() == CFType::CF_TYPE_CELLIS && iTemplate.value().get() == CFTemp::CF_TEMPLATE_EXPR)
            sTypeiParam = "CFOper";
        else if(iType.value().get() == CFType::CF_TYPE_EXPRIS && iTemplate.value().get() == CFTemp::CF_TEMPLATE_CONTAINSTEXT)
            sTypeiParam = "CFTextOper";
        else if(iType.value().get() == CFType::CF_TYPE_EXPRIS &&
                (iTemplate.value().get() >= CFTemp::CF_TEMPLATE_TIMEPERIODTODAY && iTemplate.value().get() <= CFTemp::CF_TEMPLATE_TIMEPERIODTHISMONTH))
            sTypeiParam = "CFDateOper";
        else if(iType.value().get() == CFType::CF_TYPE_EXPRIS && iTemplate.value().get() == CFTemp::CF_TEMPLATE_ABOVEAVERAGE)
            sTypeiParam = "above";
        else if(iType.value().get() == CFType::CF_TYPE_EXPRIS && iTemplate.value().get() == CFTemp::CF_TEMPLATE_BELOWAVERAGE)
            sTypeiParam = "below";
        else if(iType.value().get() == CFType::CF_TYPE_EXPRIS &&
                (iTemplate.value().get() == CFTemp::CF_TEMPLATE_EQUALABOVEAVERAGE || iTemplate.value().get() == CFTemp::CF_TEMPLATE_EQUALBELOWAVERAGE))
            sTypeiParam = "zero";
        else if(iType.value().get() == CFType::CF_TYPE_FILTER && iTemplate.value().get() == CFTemp::CF_TEMPLATE_FILTER)
            sTypeiParam = "cells";

        record.skipNunBytes(8);

        _UINT16 flags;
        record >> flags;
        fStopTrue   = GETBIT(flags, 1);
        fAbove      = GETBIT(flags, 2);
        fBottom     = GETBIT(flags, 3);
        fPercent    = GETBIT(flags, 4);

        record >> cbFmla1 >> cbFmla2 >> cbFmla3;

        record.skipNunBytes(4);

        _GUID_  guid_;
        record >> guid_ >> fGuid >> strParam;
        guid = STR::guid2bstr(guid_);
    }

	void BeginCFRule14::writeFields(XLS::CFRecord& record)
	{
		record << FRTheader << iType << iTemplate << dxfId << iPri << iParam;

		record.reserveNunBytes(8);

		_UINT16 flags = 0;

		SETBIT(flags, 1, fStopTrue)
		SETBIT(flags, 2, fAbove)
		SETBIT(flags, 3, fBottom)
		SETBIT(flags, 4, fPercent)

    	record << flags;

        if(cbFmla1 > 0 && !FRTheader.rgFormulas.array.empty())
            cbFmla1 = FRTheader.rgFormulas.array.at(0).formula.cce;
        else if(cbFmla3 > 0 && !FRTheader.rgFormulas.array.empty())
               cbFmla3 = FRTheader.rgFormulas.array.at(0).formula.cce;
        if(cbFmla2 > 0 && FRTheader.rgFormulas.array.size() > 1)
            cbFmla2 = FRTheader.rgFormulas.array.at(1).formula.cce;
		record << cbFmla1 << cbFmla2 << cbFmla3;

		record.reserveNunBytes(4);

		_GUID_  guid_;
        if(fGuid)
            STR::bstr2guid(guid, guid_);

		record << guid_ << fGuid << strParam;
	}

} // namespace XLSB

