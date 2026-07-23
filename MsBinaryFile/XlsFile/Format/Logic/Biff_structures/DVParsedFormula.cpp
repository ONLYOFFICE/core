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

#include "DVParsedFormula.h"

namespace XLS
{

DVParsedFormula::DVParsedFormula() : cce(0), ParsedFormula(CellRef())
{
}

DVParsedFormula& DVParsedFormula::operator=(const std::wstring& value)
{
	ParsedFormula::operator = (value);
	return *this;
}

BiffStructurePtr DVParsedFormula::clone()
{
	return BiffStructurePtr(new DVParsedFormula(*this));
}

void DVParsedFormula::load(CFRecord& record, bool bLoad)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 cce_;
        record >> cce_;
        cce = cce_;
        record.skipNunBytes(2); // unused
    }
    else
    {
        record >> cce;
    }

	if (bLoad && record.getRdPtr() + cce <= record.getDataSize())
		return load(record);
	else
	{
		cce = 0;
	}
}
	
void DVParsedFormula::load(CFRecord& record)
{	
    rgce.load(record, cce);

    if (record.getGlobalWorkbookInfo()->Version == 0x0800)
    {
        _UINT32 cb;
        record >> cb;
        record.skipNunBytes(cb);

        //rgcb.load(record, rgce.getPtgs(), true);
    }
}

void DVParsedFormula::save(CFRecord& record, bool bSave)
{
	if (bSave)
		return save(record);
	else
	{
		cce = 0;
        _UINT32 cb = 0;
		record << cce;
		if(record.getGlobalWorkbookInfo()->Version == 0x0800)
			record << cb;
	}
}

void DVParsedFormula::save(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		auto saving = [&](BiffStructure& rgceORrgb)
		{
			record << cce;

			auto rdPtr = record.getRdPtr();

			rgceORrgb.save(record);

			cce = record.getRdPtr() - rdPtr;

			record.RollRdPtrBack(cce + 4);
			record << cce;
			record.skipNunBytes(cce);
		};

		saving(rgce);
	}
	else 
	{
		_UINT32 size = 0;
		auto saving = [&](BiffStructure& rgceORrgb)
		{
			record << size;

			auto rdPtr = record.getRdPtr();

			rgceORrgb.save(record);

			size = record.getRdPtr() - rdPtr;

			record.RollRdPtrBack(size + 4);
			record << size;
			record.skipNunBytes(size);
		};

		saving(rgce);
		saving(rgcb);
	}
}



} // namespace XLS

