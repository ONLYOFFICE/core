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

#include "BiffRecord.h"
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/CellRangeRef.h"
#include "../Biff_structures/SxAxis.h"

namespace XLS
{

class SxView: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxView)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxView)
public:
	SxView()	{cchTableName = cchDataName = 0;}
	~SxView()	{}

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
    void writeFields(CFRecord& record);

	static const ElementType type = typeSxView;

	Ref8U			ref;
    RwU				rwFirstHead = 0;
    RwU				rwFirstData = 0;
    ColU			colFirstData = 0;
    short			iCache = 0;
    short			reserved = 0;
	SXAxis			sxaxis4Data;
	short			ipos4Data = -1;
    short			cDim = 0;
    unsigned short	cDimRw = 0;
    unsigned short	cDimCol = 0;
    unsigned short	cDimPg = 0;
    short			cDimData = 0;
    unsigned short	cRw = 0;
    unsigned short	cCol = 0;

	bool fRwGrand = true;
    bool fColGrand = false;
	bool fAutoFormat = true;
    bool fAtrNum = false;
    bool fAtrFnt = false;
    bool fAtrAlc = false;
    bool fAtrBdr = false;
    bool fAtrPat = false;
    bool fAtrProc = false;

	unsigned short	itblAutoFmt = 1; //enum AutoFmt8
    unsigned short	cchTableName = 0;
    unsigned short	cchDataName = 0;

	XLUnicodeStringNoCch	stTable;
	XLUnicodeStringNoCch	stData;
};

} // namespace XLS

