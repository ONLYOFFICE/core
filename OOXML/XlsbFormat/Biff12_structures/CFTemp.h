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
#pragma once

#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BiffAttribute.h"

namespace XLSB
{

class CFTemp : public XLS::BiffAttributeSimple<_UINT32>
{
public:
    XLS::BiffStructurePtr clone();
	CFTemp& operator= (const _UINT32& other_val);
	enum
	{
            CF_TEMPLATE_EXPR                = 0x00000000,
            CF_TEMPLATE_FMLA                = 0x00000001,
            CF_TEMPLATE_GRADIENT            = 0x00000002,
            CF_TEMPLATE_DATABAR             = 0x00000003,
            CF_TEMPLATE_MULTISTATE          = 0x00000004,
            CF_TEMPLATE_FILTER              = 0x00000005,
            CF_TEMPLATE_BANDEDTABLE         = 0x00000006,
            CF_TEMPLATE_UNIQUEVALUES        = 0x00000007,
            CF_TEMPLATE_CONTAINSTEXT        = 0x00000008,
            CF_TEMPLATE_CONTAINSBLANKS      = 0x00000009,
            CF_TEMPLATE_CONTAINSNOBLANKS    = 0x0000000a,
            CF_TEMPLATE_CONTAINSERRORS      = 0x0000000b,
            CF_TEMPLATE_CONTAINSNOERRORS    = 0x0000000c,
            CF_TEMPLATE_CONTAINSFORMULAS    = 0x0000000d,
            CF_TEMPLATE_CONTAINSNOFORMULAS  = 0x0000000e,
            CF_TEMPLATE_TIMEPERIODTODAY     = 0x0000000f,
            CF_TEMPLATE_TIMEPERIODTOMORROW  = 0x00000010,
            CF_TEMPLATE_TIMEPERIODYESTERDAY = 0x00000011,
            CF_TEMPLATE_TIMEPERIODLAST7DAYS = 0x00000012,
            CF_TEMPLATE_TIMEPERIODLASTMONTH = 0x00000013,
            CF_TEMPLATE_TIMEPERIODNEXTMONTH = 0x00000014,
            CF_TEMPLATE_TIMEPERIODTHISWEEK  = 0x00000015,
            CF_TEMPLATE_TIMEPERIODNEXTWEEK  = 0x00000016,
            CF_TEMPLATE_TIMEPERIODLASTWEEK  = 0x00000017,
            CF_TEMPLATE_TIMEPERIODTHISMONTH = 0x00000018,
            CF_TEMPLATE_ABOVEAVERAGE        = 0x00000019,
            CF_TEMPLATE_BELOWAVERAGE        = 0x0000001a,
            CF_TEMPLATE_DUPLICATEVALUES     = 0x0000001b,
            CF_TEMPLATE_COMPARECOLUMNS      = 0x0000001c,
            CF_TEMPLATE_EQUALABOVEAVERAGE   = 0x0000001d,
            CF_TEMPLATE_EQUALBELOWAVERAGE   = 0x0000001e
	};
};

} // namespace XLSB

