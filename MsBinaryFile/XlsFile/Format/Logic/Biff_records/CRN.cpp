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

#include "CRN.h"

namespace XLS
{

CRN::CRN()
{
}

CRN::~CRN()
{
}

BaseObjectPtr CRN::clone()
{
	return BaseObjectPtr(new CRN(*this));
}


void CRN::readFields(CFRecord& record)
{
	record >> colLast >> colFirst >> row;
	if(colFirst > colLast)
	{
		colFirst = 0;
		colLast = 0;
		return;
	}
	
	for(int i = 0; i < colLast - colFirst + 1; ++i)
	{
		unsigned char rec_type;
		record >> rec_type;
		
		SerArPtr ser(SerAr::createSerAr(rec_type));
		if(ser == nullptr)
		{
			colLast = (i > 0) ? (colFirst + i - 1) : colFirst;
			break;
		}
		record >> *ser;
		crnOper.push_back(ser);
		if(record.getRdPtr() >= record.getDataSize())
		{
			colLast = colFirst + i;
			break;
		}
	}
}

void CRN::writeFields(CFRecord& record)
{
    record << colLast << colFirst << row;
    for(auto i : crnOper)
        if(i != nullptr)
            i->save(record);
}

} // namespace XLS

