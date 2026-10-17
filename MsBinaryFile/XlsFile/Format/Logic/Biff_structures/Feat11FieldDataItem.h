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
#pragma once

#include "BiffString.h"
#include "DXFN12List.h"
#include "ListParsedFormula.h"

#include "Feat11FdaAutoFilter.h"
#include "Feat11Fmla.h"
#include "Feat11XMap.h"
#include "Feat11WSSListInfo.h"
#include "CachedDiskHeader.h"

namespace XLS
{

class CFRecord;

class Feat11FieldDataItem : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11FieldDataItem)
public:
	Feat11FieldDataItem(_UINT32 lt, bool bDskHeaderCache, bool bFeat12);
	BiffStructurePtr clone();

	static const ElementType type = typeFeat11FieldDataItem;
	
	virtual void load(CFRecord& record);
	virtual void save(CFRecord& record);

	_UINT32	idField = 1;
	_UINT32	lfdt = 0;
	_UINT32	lfxidt = 0;
	_UINT32	ilta = 0;
	_UINT32	cbFmtAgg = 0;
	_UINT32	istnAgg = 0xFFFFFFFF;

	bool	fAutoFilter = false;
	bool	fAutoFilterHidden = false;
	bool	fLoadXmapi = false;
	bool	fLoadFmla = false;
	bool	fLoadTotalFmla = false;
	bool	fLoadTotalArray = false;
	bool	fSaveStyleName = false;
	bool	fLoadTotalStr = false;
	bool	fAutoCreateCalcCol = false;
	
	_UINT32	cbFmtInsertRow = 0;
	_UINT32	istnInsertRow = 0;

	XLUnicodeString strFieldName;
	XLUnicodeString strCaption;

	DXFN12List dxfFmtAgg;
	DXFN12List dxfFmtInsertRow;

	Feat11FdaAutoFilter AutoFilter;

	Feat11XMap				rgXmap;
	Feat11Fmla				fmla;
	ListParsedFormula		totalFmla;
	ListParsedArrayFormula	totalArrayFmla;
	XLUnicodeString			strTotal;
	Feat11WSSListInfo		wssInfo;
	_UINT32					qsif = 1;
	CachedDiskHeader		dskHdrCache;

//------------------------------------------------
	_UINT32					lt = 0;
	bool					bDiskHdrCache = false;
	bool					bFeature12 = false;
};

typedef boost::shared_ptr<Feat11FieldDataItem> Feat11FieldDataItemPtr;
//--------------------------------------------------------------------------------------------------
class Feat11RgSharepointIdDel : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11RgSharepointIdDel)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFeat11RgSharepointIdDel;
	
	virtual void load(CFRecord& record);
	virtual void save(CFRecord& record);

	_UINT16					cId = 0;
	std::vector<_UINT32>	rgId;
};

typedef boost::shared_ptr<Feat11RgSharepointIdDel> Feat11RgSharepointIdDelPtr;
//--------------------------------------------------------------------------------------------------
class Feat11RgSharepointIdChange : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11RgSharepointIdChange)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFeat11RgSharepointIdChange;
	
	virtual void load(CFRecord& record);
	virtual void save(CFRecord& record);

	_UINT16					cId = 0;
	std::vector<_UINT32>	rgId;
};

typedef boost::shared_ptr<Feat11RgSharepointIdChange> Feat11RgSharepointIdChangePtr;
//--------------------------------------------------------------------------------------------------

struct Feat11CellStruct
{
	_UINT32 idxRow		= 0;
	_UINT32 idxField	= 0;
};
class Feat11RgInvalidCells : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11RgInvalidCells)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFeat11RgInvalidCells;
	
	virtual void load(CFRecord& record);
	virtual void save(CFRecord& record);

	_UINT16							cCellInvalid;
	std::vector<Feat11CellStruct>	rgCellInvalid;
};

typedef boost::shared_ptr<Feat11RgInvalidCells> Feat11RgInvalidCellsPtr;

} // namespace XLS

