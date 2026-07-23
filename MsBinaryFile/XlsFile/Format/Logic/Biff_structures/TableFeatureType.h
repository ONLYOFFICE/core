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

#include "../Biff_records/BiffRecord.h"

#include "Feat11FieldDataItem.h"

namespace XLS
{

class TableFeatureType: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TableFeatureType)
public:
	BiffStructurePtr clone();

	TableFeatureType();
	~TableFeatureType();

	static const ElementType type = typeTableFeatureType;
	
	virtual void load(CFRecord& record);
	virtual void save(CFRecord& record);

	_UINT32	lt = 0;		// SourceType
	_UINT32	idList = 0;
	_UINT32	crwHeader = 1;
	_UINT32	crwTotals = 0;
	_UINT32	idFieldNext = 0;
	_UINT32	cbFSData = 64;
	_UINT16 rupBuild = 0x5F0C;

	bool	fAutoFilter = false;
	bool	fPersistAutoFilter = false;
	bool	fShowInsertRow = false;
	bool	fInsertRowInsCells = false;
	bool	fLoadPldwIdDeleted = false;
	bool	fShownTotalRow = false;
	bool	fNeedsCommit = false;
	bool	fSingleCell = false;
	bool	fApplyAutoFilter = false;
	bool	fForceInsertToBeVis = false;
	bool	fCompressedXml = false;
	bool	fLoadCSPName = false;
	bool	fLoadPldwIdChanged = false;
	bool	fLoadEntryId = false;
	bool	fLoadPllstclInvalid = false;
	bool	fGoodRupBld = true;
	bool	fPublished = false;
	
	unsigned char	verXL = 0xF;
	_UINT32			lPosStmCache = 0;
	_UINT32			cbStmCache = 0;
	_UINT32			cchStmCache = 0;
	_UINT32			lem = 0;		// LEMMode

	XLUnicodeString rgbName;
	_UINT16			cFieldData = 0;
	XLUnicodeString cSPName;
	XLUnicodeString entryId;

	BiffStructurePtrVector	arFieldData;

	BiffStructurePtr		idDeleted;
	BiffStructurePtr		idChanged;
	BiffStructurePtr		cellInvalid;
//-------
	_UINT32					cbFeatData = 0;
	bool					bFeature12 = false;

};

typedef boost::shared_ptr<TableFeatureType> TableFeatureTypePtr;

} // namespace XLS
