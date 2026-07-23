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

