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
#include "BiffString.h"
#include "DXFN12List.h"

namespace XLS
{

class List12BlockLevel: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(List12BlockLevel)
public:
	BiffStructurePtr clone();

	List12BlockLevel();
	~List12BlockLevel();

	static const ElementType	type = typeList12BlockLevel;
	
	virtual void load(CFRecord& record);
	virtual void save(CFRecord& record);

	_INT32	cbdxfHeader = 0;
	_INT32	istnHeader = -1;
	_INT32	cbdxfData = 0;
	_INT32	istnData = -1;
	_INT32	cbdxfAgg = 0;
	_INT32	istnAgg= -1;
	_INT32	cbdxfBorder = 0;
	_INT32	cbdxfHeaderBorder = 0;
	_INT32	cbdxfAggBorder = 0;

	DXFN12List dxfHeader;
	DXFN12List dxfData;
	DXFN12List dxfAgg;
	DXFN12List dxfBorder;
	DXFN12List dxfHeaderBorder;
	DXFN12List dxfAggBorder;
	
	XLUnicodeString stHeader;
	XLUnicodeString stData;
	XLUnicodeString stAgg;
};

typedef boost::shared_ptr<List12BlockLevel> List12BlockLevelPtr;

} // namespace XLS
