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

#include "Types.h"
#include "../../OOXML/Base/Base.h"

#include <string>

enum ASC_VARENUM
    {
        ASC_VT_EMPTY	= 0,
        ASC_VT_I2       = 2,
        ASC_VT_I4       = 3,
        ASC_VT_R4       = 4,
        ASC_VT_R8       = 5,
        ASC_VT_STR      = 8,
        ASC_VT_BOOL     = 11,
        ASC_VT_I1       = 16,
        ASC_VT_UI1      = 17,
        ASC_VT_UI2      = 18,
        ASC_VT_UI4      = 19,
        ASC_VT_I8       = 20,
        ASC_VT_UI8      = 21
    } ;


typedef unsigned short ASC_VARTYPE;

struct ASC_VARIANT
{
    ASC_VARIANT() : vt(ASC_VT_EMPTY), llVal(0), lVal(0), bVal(0), iVal(0), fltVal(0), dblVal(0), boolVal(false) {}

    ASC_VARTYPE     vt;

	_INT64          llVal;
	_INT32          lVal;
    BYTE            bVal;
    SHORT           iVal;
    FLOAT           fltVal;
    std::wstring    strVal;
    bool            boolVal;
    double          dblVal;
};
