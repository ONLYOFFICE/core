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
#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/Feat11FieldDataItem.h>


namespace XLS
{

class TableFeatureType: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TableFeatureType)
public:
	BiffStructurePtr clone();

	TableFeatureType();
	~TableFeatureType();

	static const ElementType	type = typeTableFeatureType;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	_UINT32	lt;		// SourceType
	_UINT32	idList;
	_UINT32	crwHeader;
	_UINT32	crwTotals;
	_UINT32	idFieldNext;
	_UINT32	cbFSData;
	_UINT16 rupBuild;

	unsigned char	fAutoFilter;
	unsigned char	fPersistAutoFilter;
	unsigned char	fShowInsertRow;
	unsigned char	fInsertRowInsCells;
	unsigned char	fLoadPldwIdDeleted;
	unsigned char	fShownTotalRow;
	unsigned char	fNeedsCommit;
	unsigned char	fSingleCell;
	unsigned char	fApplyAutoFilter;
	unsigned char	fForceInsertToBeVis;
	unsigned char	fCompressedXml;
	unsigned char	fLoadCSPName;
	unsigned char	fLoadPldwIdChanged;
	unsigned char	verXL;
	unsigned char	fLoadEntryId;
	unsigned char	fLoadPllstclInvalid;
	unsigned char	fGoodRupBld;
	unsigned char	fPublished;
	
	_UINT32	lPosStmCache;
	_UINT32	cbStmCache;
	_UINT32	cchStmCache;
	_UINT32	lem;		// LEMMode

	XLUnicodeString rgbName;
	_UINT16 cFieldData;
	XLUnicodeString cSPName;
	XLUnicodeString entryId;

	BiffStructurePtrVector fieldData;
};

typedef boost::shared_ptr<TableFeatureType> TableFeatureTypePtr;

} // namespace XLS