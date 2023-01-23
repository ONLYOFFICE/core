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

	_UINT32	idField;
	_UINT32	lfdt;
	_UINT32	lfxidt;
	_UINT32	ilta;
	_UINT32	cbFmtAgg;
	_UINT32	istnAgg;

	bool	fAutoFilter;
	bool	fAutoFilterHidden;
	bool	fLoadXmapi;
	bool	fLoadFmla;
	bool	fLoadTotalFmla;
	bool	fLoadTotalArray;
	bool	fSaveStyleName;
	bool	fLoadTotalStr;
	bool	fAutoCreateCalcCol;
	
	_UINT32	cbFmtInsertRow;
	_UINT32	istnInsertRow;

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
	_UINT32					qsif;
	CachedDiskHeader		dskHdrCache;

//------------------------------------------------
	_UINT32					lt;
	bool					bDiskHdrCache;
	bool					bFeature12;
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

	_UINT16					cId;
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

	_UINT16					cId;
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

	_UINT16							cCellInvalid;
	std::vector<Feat11CellStruct>	rgCellInvalid;
};

typedef boost::shared_ptr<Feat11RgInvalidCells> Feat11RgInvalidCellsPtr;

} // namespace XLS

