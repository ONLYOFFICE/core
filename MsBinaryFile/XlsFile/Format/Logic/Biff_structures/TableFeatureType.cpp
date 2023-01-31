/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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


} // namespace XLS

