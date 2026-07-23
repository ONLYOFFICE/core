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

#include "List12BlockLevel.h"

namespace XLS
{


BiffStructurePtr List12BlockLevel::clone()
{
	return BiffStructurePtr(new List12BlockLevel(*this));
}

List12BlockLevel::List12BlockLevel()
{		
	cbdxfHeader			= 0xffffffff;
	istnHeader			= 0xffffffff;
	cbdxfData			= 0xffffffff;
	istnData			= 0xffffffff;
	cbdxfAgg			= 0xffffffff;
	istnAgg				= 0xffffffff;
	cbdxfBorder			= 0xffffffff;
	cbdxfHeaderBorder	= 0xffffffff;
	cbdxfAggBorder		= 0xffffffff;
}


List12BlockLevel::~List12BlockLevel()
{
}

void List12BlockLevel::load(CFRecord& record)
{	
	record >> cbdxfHeader;
	record >> istnHeader;
	record >> cbdxfData;
	record >> istnData;
	record >> cbdxfAgg;
	record >> istnAgg;
	record >> cbdxfBorder;
	record >> cbdxfHeaderBorder;
	record >> cbdxfAggBorder;
	
	if (cbdxfHeader)
	{
		dxfHeader.size = cbdxfHeader;
		record >> dxfHeader;
	}
	if (cbdxfData)
	{
		dxfData.size = cbdxfData;
		record >> dxfData;
	}
	if (cbdxfAgg)
	{
		dxfAgg.size = cbdxfAgg;
		record >> dxfAgg;
	}
	if (cbdxfBorder)
	{
		dxfBorder.size = cbdxfBorder;
		record >> dxfBorder;
	}
	if (cbdxfHeaderBorder)
	{
		dxfHeaderBorder.size = cbdxfHeaderBorder;
		record >> dxfHeaderBorder;
	}
	if (cbdxfAggBorder)
	{
		dxfAggBorder.size = cbdxfAggBorder;
		record >> dxfAggBorder;
	}
	if (istnHeader != -1)
		record >> stHeader;
	if (istnData != -1)
		record >> stData;
	if (istnAgg != -1)
		record >> stAgg;
}
void saveDXFN(CFRecord& record, const size_t cbPos, DXFN12List & dxfnList)
{
	_INT32 listSize = 0;
	auto dxfnStart = record.getRdPtr();
	dxfnList.save(record);
	listSize = record.getRdPtr() - dxfnStart;
	record.RollRdPtrBack(record.getRdPtr() - cbPos);
	record << listSize;
	record.skipNunBytes(dxfnStart - record.getRdPtr() + listSize);
}
void List12BlockLevel::save(CFRecord& record)
{
	auto headerSizePos = record.getRdPtr();
	record << cbdxfHeader << istnHeader;
	auto dataSizePos = record.getRdPtr();
	record << cbdxfData << istnData;
	auto aggPos = record.getRdPtr();
	record << cbdxfAgg << istnAgg;
	auto BorderPos = record.getRdPtr();
	record << cbdxfBorder;
	auto HeaderBorderPos = record.getRdPtr();
	record << cbdxfHeaderBorder;
	auto aggBorderPos = record.getRdPtr();
	record << cbdxfAggBorder;

	if(dxfHeader.bExist)
		saveDXFN(record, headerSizePos, dxfHeader);
	if(dxfData.bExist)
		saveDXFN(record, dataSizePos, dxfData);
	if(dxfAgg.bExist)
		saveDXFN(record, aggPos, dxfAgg);
	if(dxfBorder.bExist)
		saveDXFN(record, BorderPos, dxfBorder);
	if(dxfHeaderBorder.bExist)
		saveDXFN(record, HeaderBorderPos, dxfHeaderBorder);
	if(dxfAggBorder.bExist)
		saveDXFN(record, aggBorderPos, dxfAggBorder);
	if(istnHeader != -1)
		record << stHeader;
	if(istnData != -1)
		record << stData;
	if(istnAgg != -1)
		record << stAgg;
}


} // namespace XLS

