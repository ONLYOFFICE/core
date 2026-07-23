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

#include "BeginSXFilter.h"

using namespace XLS;

namespace XLSB
{

    BeginSXFilter::BeginSXFilter()
    {
    }

    BeginSXFilter::~BeginSXFilter()
    {
    }

    BaseObjectPtr BeginSXFilter::clone()
    {
        return BaseObjectPtr(new BeginSXFilter(*this));
    }

    void BeginSXFilter::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags;
        record >> isxvd >> isxvdMProp >> sxft;

        record.skipNunBytes(4);

        record >> dwId >> isxdiMeasure >> isxthMeasure >> flags;

        fLoadName           = GETBIT(flags, 0);
        fLoadDescription    = GETBIT(flags, 1);
        fLoadVal1           = GETBIT(flags, 2);
        fLoadVal2           = GETBIT(flags, 3);

        if(fLoadName)
            record >> stName;

        if(fLoadDescription)
            record >> stDescription;

        if(fLoadVal1)
            record >> stVal1;

        if(fLoadVal2)
            record >> stVal2;
    }

	void BeginSXFilter::writeFields(XLS::CFRecord& record)
	{
		_UINT16 flags = 0;
		record << isxvd << isxvdMProp << sxft;

		record.reserveNunBytes(4);

		SETBIT(flags, 0, fLoadName)
		SETBIT(flags, 1, fLoadDescription)
		SETBIT(flags, 2, fLoadVal1)
		SETBIT(flags, 3, fLoadVal2)

		record << dwId << isxdiMeasure << isxthMeasure << flags;

		if (fLoadName)
			record << stName;

		if (fLoadDescription)
			record << stDescription;

		if (fLoadVal1)
			record << stVal1;

		if (fLoadVal2)
			record << stVal2;
	}

} // namespace XLSB

