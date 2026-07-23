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
#include "../Biff_structures/AFDOper.h"

namespace XLS
{

class AutoFilter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AutoFilter)
	BASE_OBJECT_DEFINE_CLASS_NAME(AutoFilter)
public:
	AutoFilter();
	~AutoFilter();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
    void writeFields(CFRecord& record);

	static const ElementType	type = typeAutoFilter;

	_UINT16 iEntry;
	
    unsigned char	wJoin = 0;
	
    bool fSimple1 = false;
    bool fSimple2 = false;
	
    bool fTopN = false;
    bool fPercent = false;
	
    unsigned char	fTop = 0;
    _UINT16 wTopN = 0;

	AFDOper doper1;
	AFDOper doper2;

    bool m_bAutoFilter12 = false;
	
    std::wstring str1 = L"";
    std::wstring str2 = L"";

//----------------------------------------
    bool	bExist = false;
    _UINT32	size = 0;
};

} // namespace XLS

