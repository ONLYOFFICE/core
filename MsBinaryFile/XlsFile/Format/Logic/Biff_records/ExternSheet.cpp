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

#include "ExternSheet.h"
#include "../Biff_structures/XTI.h"
#include "../Biff_structures/BiffString.h"

namespace XLS
{

ExternSheet::ExternSheet()
{
}


ExternSheet::~ExternSheet()
{
}


BaseObjectPtr ExternSheet::clone()
{
	return BaseObjectPtr(new ExternSheet(*this));
}


void ExternSheet::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char type;
		unsigned char size;

		record >> size >> type;

		LPAnsiStringNoCch stName(size);
		record >> stName;

		name = stName.value();

		//if (!name.empty())
		//	record.getGlobalWorkbookInfo()->arExternalNames.push_back(name);

	}
    else if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
        _UINT16 cXTI_2b;
        record >> cXTI_2b;
        for(_UINT16 i = 0; i < cXTI_2b; ++i)
		{
			if (record.getRdPtr() + 6 > record.getDataSize())
				break;
			XTIPtr xti(new XTI);
			record >> *xti;
			rgXTI.push_back(xti);
		}
        cXTI = cXTI_2b;
	}
    else
    {
        record >> cXTI;
        for(_UINT32 i = 0; i < cXTI; ++i)
        {
            XTIPtr xti(new XTI);
            record >> *xti;
            rgXTI.push_back(xti);
        }
    }
}

void ExternSheet::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		LPAnsiStringNoCch stName(name);
		unsigned char type = 0;
		unsigned char size = stName.getSize();
		
		record << size << type;

		record << stName;
	}
	else if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 cXTI_2b = rgXTI.size();
		record << cXTI_2b;
		for (int i = 0; i < cXTI_2b; ++i)
		{
			record << *rgXTI[i];
		}
	}
	else
	{
		record << cXTI;
		for (int i = 0; i < cXTI; ++i)
		{
			record << *rgXTI[i];
		}
	}
}

} // namespace XLS
