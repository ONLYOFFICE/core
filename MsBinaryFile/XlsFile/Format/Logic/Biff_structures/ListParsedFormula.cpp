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

#include "ListParsedFormula.h"
#include "PtgList.h"

namespace XLS
{

ListParsedFormula::ListParsedFormula() : ParsedFormula(CellRef())
{
}

ListParsedFormula& ListParsedFormula::operator=(const std::wstring& value)
{
	ParsedFormula::operator = (value);
	if(!rgce.sequence.empty())
	{
		auto lastValType = GETBITS(rgce.sequence.rbegin()->get()->ptg_id.get(),5,6);
		if(lastValType == 1 || lastValType == 3)
		{
			SETBITS(rgce.sequence.rbegin()->get()->ptg_id.get(),5,6,2);
		}
		else if(rgce.sequence.rbegin()->get()->ptg_id.get() == 6424)
		{
			auto list = static_cast<XLS::PtgList*>(rgce.sequence.rbegin()->get());
			list->type_ = 1;
		}
	}
	return *this;
}

BiffStructurePtr ListParsedFormula::clone()
{
	return BiffStructurePtr(new ListParsedFormula(*this));
}

void ListParsedFormula::load(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 cce;
        record >> cce;
        record.skipNunBytes(2); // unused
        rgce.load(record, cce);
    }
    else
    {
        _UINT32 cce;
        record >> cce;

        rgce.load(record, cce);

        _UINT32 cb;
        record >> cb;

        if(cb > 0)
            rgcb.load(record, rgce.getPtgs(), false);
    }
}
void ListParsedFormula::save(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 size = 0;

		auto saving = [&](BiffStructure& rgceORrgb)
		{
			record << size;
			//record.reserveNunBytes(2); // unused

			auto rdPtr = record.getRdPtr();

			rgceORrgb.save(record);

			size = record.getRdPtr() - rdPtr;

			record.RollRdPtrBack(size + 2);
			record << size;
			record.skipNunBytes(size);
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
//-------------------------------------------------------------------------------
ListParsedArrayFormula::ListParsedArrayFormula() : ArrayParsedFormula(false, CellRef())
{
}

ListParsedArrayFormula& ListParsedArrayFormula::operator=(const std::wstring& value)
{
	ArrayParsedFormula::operator = (value);
	if(!rgce.sequence.empty())
	{
		auto lastValType = GETBITS(rgce.sequence.rbegin()->get()->ptg_id.get(),5,6);
		if(lastValType == 1 || lastValType == 3)
		{
			SETBITS(rgce.sequence.rbegin()->get()->ptg_id.get(),5,6,2);
		}
		else if(rgce.sequence.rbegin()->get()->ptg_id.get() == 6424)
		{
			auto list = static_cast<XLS::PtgList*>(rgce.sequence.rbegin()->get());
			list->type_ = 1;
		}
	}
	return *this;
}

BiffStructurePtr ListParsedArrayFormula::clone()
{
	return BiffStructurePtr(new ListParsedArrayFormula(*this));
}

void ListParsedArrayFormula::load(CFRecord& record)
{
	ArrayParsedFormula::load(record);
}

void ListParsedArrayFormula::save(CFRecord& record)
{
	ArrayParsedFormula::save(record);
}


} // namespace XLS

