/*
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

        unsigned short flags;
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

} // namespace XLSB

