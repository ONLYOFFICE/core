/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
	_UINT32	crwHeader = 0;
	_UINT32	crwTotals = 0;
	_UINT32	idFieldNext = 0;
	_UINT32	cbFSData = 64;
	_UINT16 rupBuild = 0;

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
	bool	fGoodRupBld = false;
	bool	fPublished = false;
	
	unsigned char	verXL;
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
