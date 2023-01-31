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

#include "Feat11FieldDataItem.h"
#include "CellRangeRef.h"

namespace XLS
{
Feat11FieldDataItem::Feat11FieldDataItem(_UINT32 _lt, bool bDskHeaderCache, bool bFeat12) : lt(_lt), bDiskHdrCache(bDskHeaderCache), bFeature12(bFeat12)
{
}
BiffStructurePtr Feat11FieldDataItem::clone()
{
	return BiffStructurePtr(new Feat11FieldDataItem(*this));
}
void Feat11FieldDataItem::load(CFRecord& record)
{	
	record >> idField;
	record >> lfdt;
	record >> lfxidt;
	record >> ilta;
	record >> cbFmtAgg;
	record >> istnAgg;

	_UINT32 flags;
	record >> flags;

	fAutoFilter			= GETBIT(flags, 0);
	fAutoFilterHidden	= GETBIT(flags, 1);
	fLoadXmapi			= GETBIT(flags, 2);
	fLoadFmla			= GETBIT(flags, 3);	
	fLoadTotalFmla		= GETBIT(flags, 7);
	fLoadTotalArray		= GETBIT(flags, 8);
	fSaveStyleName		= GETBIT(flags, 9);
	fLoadTotalStr		= GETBIT(flags, 10);
	fAutoCreateCalcCol	= GETBIT(flags, 11);

	record >> cbFmtInsertRow;
	record >> istnInsertRow;
	record >> strFieldName;
	record >> strCaption;

	if (record.getRdPtr() >= record.getDataSize())
		return;//125 Planilhas de Excel.xls

	if (cbFmtAgg > 0)
	{
		dxfFmtAgg.size = cbFmtAgg;
		record >> dxfFmtAgg;
	}	
	if (cbFmtInsertRow > 0)
	{
		dxfFmtInsertRow.size = cbFmtInsertRow;
		record >> dxfFmtInsertRow;
	}	
	if (fAutoFilter)
	{
		record >> AutoFilter;
	}

	if (fLoadXmapi)
	{
		record >> rgXmap;
	}
	if (fLoadFmla)
	{
		fmla.load(record);
	}
	if (fLoadTotalFmla && bFeature12)
	{
		if (fLoadTotalArray)
		{
			totalArrayFmla.load(record);
		}	
		else
		{
			totalFmla.load(record);
		}	
	}
	if (fLoadTotalStr && bFeature12)
	{
		record >> strTotal;
	}
	if (lt == 0x00000001)
	{
		wssInfo.lfdt = lfdt;
		record >> wssInfo;
	}
	if (lt == 0x00000003)
	{
		record >> qsif;
	}
	if (bDiskHdrCache)
	{
		dskHdrCache.fSaveStyleName = fSaveStyleName;
		record >> dskHdrCache;
	}
}


//---------------------------------------------------------------------------------------------------------
BiffStructurePtr Feat11RgSharepointIdDel::clone()
{
	return BiffStructurePtr(new Feat11RgSharepointIdDel(*this));
}
void Feat11RgSharepointIdDel::load(CFRecord& record)
{
	record >> cId;

	for (_UINT16 i = 0; i < cId; i++)
	{
		_UINT32 val;
		record >> val;
		rgId.push_back(val);
	}
}
//---------------------------------------------------------------------------------------------------------
BiffStructurePtr Feat11RgSharepointIdChange::clone()
{
	return BiffStructurePtr(new Feat11RgSharepointIdChange(*this));
}
void Feat11RgSharepointIdChange::load(CFRecord& record)
{
	record >> cId;

	for (_UINT16 i = 0; i < cId; i++)
	{
		_UINT32 val;
		record >> val;
		rgId.push_back(val);
	}
}
//---------------------------------------------------------------------------------------------------------
BiffStructurePtr Feat11RgInvalidCells::clone()
{
	return BiffStructurePtr(new Feat11RgInvalidCells(*this));
}
void Feat11RgInvalidCells::load(CFRecord& record)
{
	record >> cCellInvalid;

	for (_UINT16 i = 0; i < cCellInvalid; i++)
	{
		Feat11CellStruct val;
		record >> val.idxRow >> val.idxField;
		rgCellInvalid.push_back(val);
	}
}
} // namespace XLS

