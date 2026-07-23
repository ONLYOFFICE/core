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

#include "FtLbsData.h"

namespace XLS
{

BiffStructurePtr FtLbsData::clone()
{
	return BiffStructurePtr(new FtLbsData(*this));
}

void FtLbsData::load(CFRecord& record, const unsigned short ot)
{
	unsigned short ft, cb;
	record >> ft >> cb;

	if ( ft != 0x0013)
	{
		record.RollRdPtrBack(4);
		return;
	}
	fExist = true;

	fmla.load(record);

	unsigned short flags;
	record >> cLines >> iSel >> flags >> idEdit;

	fUseCB		= GETBIT(flags, 0);
	fValidPlex	= GETBIT(flags, 1);
	fValidIds	= GETBIT(flags, 2);
	fNo3d		= GETBIT(flags, 3);
	wListSelType = GETBITS(flags, 4, 5);
	lct			= GETBITS(flags, 8, 15);

	if(0x0014 == ot)
	{
		dropData.reset(new LbsDropData);
		dropData->load(record);
	}
	if(fValidPlex)
	{
		for(int i = 0; i < cLines; ++i)
		{
			XLUnicodeString str;
			record >> str;
			rgLines.push_back(str);
		}
	}

	if (0 != wListSelType)
	{
		for(int i = 0; i < cLines; ++i)
		{
			Boolean<unsigned char> bsel;
			record >> bsel;
			bsels.push_back(bsel);
		}
	}
}

void FtLbsData::save(CFRecord& record, const unsigned short ot)
{
	{
		unsigned short ft = 0x0013;
		record << ft;
	}
	unsigned short cbFContinued = 0;
	record << cbFContinued;
	auto cbPos = record.getRdPtr();

	fmla.save(record);
	cLines = rgLines.size();

	unsigned short flags = 0;

	SETBIT(flags, 0, fUseCB)
	SETBIT(flags, 1, fValidPlex)
	SETBIT(flags, 2, fValidIds)
	SETBIT(flags, 3, fNo3d)
	SETBITS(flags, 4, 5, wListSelType)
	SETBITS(flags, 8, 15, lct)

	record << cLines << iSel << flags << idEdit;

	if(0x0014 == ot && dropData != nullptr)
	{
		dropData->save(record);
	}
	if(fValidPlex)
		for(auto i : rgLines)
			record << i;
	if (0 != wListSelType)
	{
		for(auto i : bsels)
			record << i;
	}

	//size calculation
	//todo process case with size over limit
	cbFContinued = record.getRdPtr() - cbPos;
	record.RollRdPtrBack(cbFContinued+2);
	record << cbFContinued;
	record.skipNunBytes(cbFContinued);

}

} // namespace XLS

