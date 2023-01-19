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

#include "BiffRecordContinued.h"

#include "../Biff_structures/FrtHeaderOld.h"
#include "../Biff_structures/SxAxis.h"
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/HiddenMemberSet.h"

namespace XLS
{

class SXTH: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXTH)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXTH)
public:
	SXTH();
	~SXTH();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
	int serialize(std::wostream & strm);

	static const ElementType type = typeSXTH;

	FrtHeaderOld	frtHeaderOld;
	bool			fMeasure;
	bool			fOutlineMode;
	bool			fEnableMultiplePageItems;
	bool			fSubtotalAtTop;
	bool			fSet;
	bool			fDontShowFList;
	bool			fAttributeHierarchy;
	bool			fTimeHierarchy;
	bool			fFilterInclusive;
	bool			fKeyAttributeHierarchy;
	bool			fKPI;	
	SXAxis			sxaxis;		
	_INT32			isxvd;
	_INT32			csxvdXl;
	bool			fDragToRow;
	bool			fDragToColumn;
	bool			fDragToPage;
	bool			fDragToData;
	bool			fDragToHide;

	XLUnicodeString	stUnique;
	XLUnicodeString	stDisplay;
	XLUnicodeString	stDefault;
	XLUnicodeString	stAll;
	XLUnicodeString	stDimension;
	
	_UINT32				cisxvd;
	std::vector<_INT32>	rgisxvd;

	_UINT32							cHiddenMemberSets;
	std::vector<HiddenMemberSet>	rgHiddenMemberSets;
};

} // namespace XLS

