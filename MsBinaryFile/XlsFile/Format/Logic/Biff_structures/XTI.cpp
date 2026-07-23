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

#include "XTI.h"
#include "../GlobalsSubstream.h"
#include "../Biff_unions/SUPBOOK.h"
#include "../Biff_records/SupBook.h"

namespace XLS
{


BiffStructurePtr XTI::clone()
{
	return BiffStructurePtr(new XTI(*this));
}

void XTI::load(CFRecord& record)
{
	if (record.checkFitReadSafe(6) == false) //Version < 0x0600
		return;

    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 iSupBook_2b;
        _INT16 itabFirst_2b;
        _INT16 itabLast_2b;

        record >> iSupBook_2b >> itabFirst_2b >> itabLast_2b;
        iSupBook = iSupBook_2b;
        itabFirst = itabFirst_2b;
        itabLast = itabLast_2b;
    }
    else
        record >> iSupBook >> itabFirst >> itabLast;
}

void XTI::save(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 iSupBook_2b = iSupBook;
		_INT16 itabFirst_2b = itabFirst;
		_INT16 itabLast_2b = itabLast;

		record << iSupBook_2b << itabFirst_2b << itabLast_2b;
	}
	else
		record << iSupBook << itabFirst << itabLast;
}

} // namespace XLS
