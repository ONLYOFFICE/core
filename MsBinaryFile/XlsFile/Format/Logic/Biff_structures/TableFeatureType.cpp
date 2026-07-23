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

#include "TableFeatureType.h"
#include "Feat11FieldDataItem.h"

namespace XLS
{


BiffStructurePtr TableFeatureType::clone()
{
	return BiffStructurePtr(new TableFeatureType(*this));
}

TableFeatureType::TableFeatureType()
{		
}


TableFeatureType::~TableFeatureType()
{
}

void TableFeatureType::load(CFRecord& record)
{	
	record >> lt;

//LTRANGE			0x00000000 Range 
//LTSHAREPOINT		0x00000001 Read/write Web-based data provider list 
//LTXML				0x00000002 XML Mapper data
//LTEXTERNALDATA	0x00000003 External data source (query table)<180>

	record >> idList;			// Unique ID
	record >> crwHeader;		// 0 - Table has no header row, 1 - Table has a header row
	record >> crwTotals;		// 0 - Table has no total row, 1 - Table has a total row
	record >> idFieldNext;		// Next unique identifier
	record >> cbFSData;			// 64
	record >> rupBuild;			// Build number of the application

	record.skipNunBytes(2);		// unused1

	_UINT16 flags;
	record >> flags;
	
	fAutoFilter			= GETBIT(flags, 1);
	fPersistAutoFilter	= GETBIT(flags, 2);
	fShowInsertRow		= GETBIT(flags, 3);
	fInsertRowInsCells	= GETBIT(flags, 4);
	fLoadPldwIdDeleted	= GETBIT(flags, 5);
	fShownTotalRow		= GETBIT(flags, 6);
	fNeedsCommit		= GETBIT(flags, 8);
	fSingleCell			= GETBIT(flags, 9);
	fApplyAutoFilter	= GETBIT(flags, 11);
	fForceInsertToBeVis = GETBIT(flags, 12);
	fCompressedXml		= GETBIT(flags, 13);
	fLoadCSPName		= GETBIT(flags, 14);
	fLoadPldwIdChanged	= GETBIT(flags, 15);

	if (fSingleCell)
	{
		crwHeader = 0;
		crwTotals = 0;
	}

	record >> flags;
	verXL				= GETBITS(flags, 0, 4);
	fLoadEntryId		= GETBIT(flags, 4);
	fLoadPllstclInvalid = GETBIT(flags, 5);
	fGoodRupBld			= GETBIT(flags, 6);
	fPublished			= GETBIT(flags, 8);

	if (lt != 1)
	{
		record >> lPosStmCache;
		record >> cbStmCache;
		record >> cchStmCache;
	}
	record >> lem;
	if (lt != 1)
		record.skipNunBytes(16);		// rgbHashParam

	record >> rgbName;
	record >> cFieldData; //from 1 to 0x100

	if (fLoadCSPName)
		record >> cSPName;

	if (fLoadEntryId)
		record >> entryId;
	
	for (_UINT16 i = 0; i < cFieldData; i++)
	{
		if (record.getRdPtr() >= record.getDataSize())
			return;

		arFieldData.push_back(BiffStructurePtr(new Feat11FieldDataItem(lt, (crwHeader == 0x0000 && fSingleCell == 0x0000), bFeature12)));
		arFieldData.back()->load(record);
	}
	if (fLoadPldwIdDeleted)
	{
		idDeleted = BiffStructurePtr (new Feat11RgSharepointIdDel);
		idDeleted->load(record);
	}
	if (fLoadPldwIdChanged)
	{
		idChanged = BiffStructurePtr (new Feat11RgSharepointIdChange);
		idChanged->load(record);

	}
	if (fLoadPllstclInvalid)
	{
		cellInvalid = BiffStructurePtr(new Feat11RgInvalidCells);
		cellInvalid->load(record);
	}
}

void TableFeatureType::save(CFRecord& record)
{
	record << lt << idList << crwHeader << crwTotals << idFieldNext << cbFSData << rupBuild;
	record.reserveNunBytes(2);
	_UINT16 flags = 0;

	SETBIT(flags, 1, fAutoFilter)
	SETBIT(flags, 2, fPersistAutoFilter)
	SETBIT(flags, 3, fShowInsertRow)
	SETBIT(flags, 4, fInsertRowInsCells)
	SETBIT(flags, 5, fLoadPldwIdDeleted)
	SETBIT(flags, 6, fShownTotalRow)
	SETBIT(flags, 8, fNeedsCommit)
	SETBIT(flags, 9, fSingleCell)
	SETBIT(flags, 11, fApplyAutoFilter)
	SETBIT(flags, 12, fForceInsertToBeVis)
	SETBIT(flags, 13, fCompressedXml)
	SETBIT(flags, 14, fLoadCSPName)
	SETBIT(flags, 15, fLoadPldwIdChanged)
	record << flags;

	flags = 0;

	SETBITS(flags, 0, 4, verXL)
	SETBIT(flags, 4, fLoadEntryId)
	SETBIT(flags, 5, fLoadPllstclInvalid)
	SETBIT(flags, 6, fGoodRupBld)
	SETBIT(flags, 8, fPublished)
	record << flags;
	if(lt == 1)
	{
		record << lPosStmCache << cbStmCache << cchStmCache << lem;
	}
	else
		record.reserveNunBytes(16);
	record.reserveNunBytes(16); //rgbHashParam
	record << rgbName;
	cFieldData = arFieldData.size();
	record << cFieldData;
	if(fLoadCSPName)
		record << cSPName;
	if(fLoadEntryId)
		record << entryId;
	for(auto i : arFieldData)
		if(i != nullptr)
			i->save(record);

	if(fLoadPldwIdDeleted)
		idDeleted->save(record);
	if(fLoadPldwIdChanged)
		idChanged->save(record);
	if(fLoadPllstclInvalid)
		cellInvalid->save(record);
}

} // namespace XLS

