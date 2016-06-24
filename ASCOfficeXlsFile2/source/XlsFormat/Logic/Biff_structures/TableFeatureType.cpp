/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include <Binary/CFRecord.h>

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


void TableFeatureType::store(CFRecord& record)
{	
}


void TableFeatureType::load(CFRecord& record)
{	
	record >> lt;				// SourceType
	record >> idList;			// Unique ID
	record >> crwHeader;		// 0 - Table has no header row, 1 - Table has a header row
	record >> crwTotals;		// 0 - Table has no total row, 1 - Table has a total row
	record >> idFieldNext;		// Next unique identifier
	record >> cbFSData;			// 64
	record >> rupBuild;			// Build number of the application

	_UINT32 _lt = lt;
	_UINT32 _idList = idList;
	_UINT32 _crwHeader = crwHeader;
	_UINT32 _crwTotals = crwTotals;
	_UINT32 _idFieldNext = idFieldNext;
	_UINT32 _cbFSData = cbFSData;
	_UINT32 _rupBuild = rupBuild;

	record.skipNunBytes(2);		// unused1

	_UINT16 flags;
	record >> flags;
	
	fAutoFilter = static_cast<unsigned char>(GETBIT(flags, 1));
	fPersistAutoFilter = static_cast<unsigned char>(GETBIT(flags, 2));
	fShowInsertRow = static_cast<unsigned char>(GETBIT(flags, 3));
	fInsertRowInsCells = static_cast<unsigned char>(GETBIT(flags, 4));
	fLoadPldwIdDeleted = static_cast<unsigned char>(GETBIT(flags, 5));
	fShownTotalRow = static_cast<unsigned char>(GETBIT(flags, 6));
	fNeedsCommit = static_cast<unsigned char>(GETBIT(flags, 8));
	fSingleCell = static_cast<unsigned char>(GETBIT(flags, 9));
	fApplyAutoFilter = static_cast<unsigned char>(GETBIT(flags, 11));
	fForceInsertToBeVis = static_cast<unsigned char>(GETBIT(flags, 12));
	fCompressedXml = static_cast<unsigned char>(GETBIT(flags, 13));
	fLoadCSPName = static_cast<unsigned char>(GETBIT(flags, 14));
	fLoadPldwIdChanged = static_cast<unsigned char>(GETBIT(flags, 15));

	record >> flags;
	verXL = static_cast<unsigned char>(GETBITS(flags, 0, 4));
	fLoadEntryId = static_cast<unsigned char>(GETBIT(flags, 4));
	fLoadPllstclInvalid = static_cast<unsigned char>(GETBIT(flags, 5));
	fGoodRupBld = static_cast<unsigned char>(GETBIT(flags, 6));
	fPublished = static_cast<unsigned char>(GETBIT(flags, 8));

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
	record >> cFieldData;

	int r = cFieldData;

	if (fLoadCSPName)
		record >> cSPName;

	if (fLoadEntryId == BIFF_BYTE(1))
		record >> entryId;
	
	for (size_t i = 0; i < cFieldData; i++)
	{
		if (record.getRdPtr() >= record.getDataSize())
			return;
		
		Feat11FieldDataItemPtr item(new Feat11FieldDataItem);
		item->load(record);
		fieldData.push_back(item);
	}
}


} // namespace XLS

