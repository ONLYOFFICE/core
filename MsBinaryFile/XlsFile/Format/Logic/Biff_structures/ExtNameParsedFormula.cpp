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

#include "ExtNameParsedFormula.h"
#include "ExtPtgRef3D.h"
#include "ExtPtgArea3D.h"
#include "ExtPtgRefErr3D.h"
#include "ExtPtgAreaErr3D.h"
#include "ExtPtgErr.h"

namespace XLS
{


ExtNameParsedFormula::ExtNameParsedFormula() :	ParsedFormula(CellRef())
{
}

ExtNameParsedFormula& ExtNameParsedFormula::operator=(const std::wstring& value)
{
	ParsedFormula::operator = (value);
	return *this;
}

BiffStructurePtr ExtNameParsedFormula::clone()
{
	return BiffStructurePtr(new ExtNameParsedFormula(*this));
}

void ExtNameParsedFormula::load(CFRecord& record) // Maybe this class shouldn't be a ParsedFormulas's derived?
{
    _UINT32 cb;
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        unsigned short cb_2b;
        record >> cb_2b;
        cb = cb_2b;
    }
    else
    {
        record >> cb;
    }
	if(cb && record.getDataSize() > record.getRdPtr())
	{
		unsigned char extPtg;
		record >> extPtg;
		switch(extPtg)
		{
			case 0x3A:
				val = PtgPtr(new ExtPtgRef3D(CellRef()));
				break;
			case 0x3B:
				val = PtgPtr(new ExtPtgArea3D(CellRef()));
				break;
			case 0x3C:
				val = PtgPtr(new ExtPtgRefErr3D);
				break;
			case 0x3D:
				val = PtgPtr(new ExtPtgAreaErr3D);
				break;
			case 0x1C:
				val = PtgPtr(new ExtPtgErr);
				break;
		}
		val->load(record);
		rgce.addPtg(val);
	}
	if (record.getDataSize() > record.getRdPtr())
	{
		int sz = record.getDataSize() - record.getRdPtr();
		if (sz < 100)
		{
			char *buf = new char[sz];
			memcpy(buf, record.getCurData<char>(), sz);

			record.skipNunBytes(sz);
			delete []buf;
		}
	}
}

void ExtNameParsedFormula::save(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 size;

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
		//saving(rgcb);
	}
}


} // namespace XLS

