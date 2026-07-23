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

#include "ExtProp.h"

namespace XLS
{
BiffStructurePtr ExtProp::clone()
{
	return BiffStructurePtr(new ExtProp(*this));
}

void ExtProp::load(CFRecord& record)
{
	unsigned short t;
	record >> t >> cb;

	extType = (_type)t;

	if ((int)(cb - 4) > (int)(record.getDataSize() - record.getRdPtr()))
	{
		return;
	}
	switch(extType)
	{
		case 0x0004:
		case 0x0005:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
		case 0x000D:
		{
			record >> extPropData.color;
		}break;
		case 0x0006:
		{
			record >> extPropData.gradient_fill;
		}break;
		case 0x000E:
		{
			record >> extPropData.font_scheme;
		}break;
		case 0x000F:
		{
			record >> extPropData.indent_level;
		}break;
		default:
		{
			extType = None;
		}break;
	}
}

void ExtProp::save(CFRecord& record)
{
    unsigned short t = extType;
    record << t << cb;
    auto dataPos = record.getRdPtr();
    switch(extType)
    {
        case 0x0004:
        case 0x0005:
        case 0x0007:
        case 0x0008:
        case 0x0009:
        case 0x000A:
        case 0x000B:
        case 0x000C:
        case 0x000D:
        {
            record << extPropData.color;
        }break;
        case 0x0006:
        {
            record << extPropData.gradient_fill;
        }break;
        case 0x000E:
        {
            record << extPropData.font_scheme;
        }break;
        case 0x000F:
        {
            record << extPropData.indent_level;
        }break;
        default:
            break;
    }
    cb = record.getRdPtr() - dataPos;
    record.RollRdPtrBack(cb + 2);
    cb += 4;
    record << cb;
    record.skipNunBytes(cb - 4);
}

} // namespace XLS
