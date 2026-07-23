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

#include "TableStyles.h"
#include "../Biff_structures/BIFF12/XLWideString.h"

namespace XLS
{

TableStyles::TableStyles()
{
}

TableStyles::~TableStyles()
{
}

BaseObjectPtr TableStyles::clone()
{
	return BaseObjectPtr(new TableStyles(*this));
}

void TableStyles::readFields(CFRecord& record)
{	
    if(record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 cchDefTableStyle;
        _UINT16 cchDefPivotStyle;
        record >> frtHeader >> cts >> cchDefTableStyle >> cchDefPivotStyle;

        LPWideStringNoCch	rgchDefTableStyle_;
        LPWideStringNoCch	rgchDefPivotStyle_;
        rgchDefTableStyle_.setSize(cchDefTableStyle);
        rgchDefPivotStyle_.setSize(cchDefPivotStyle);

        record >> rgchDefTableStyle_ >> rgchDefPivotStyle_;

        rgchDefTableStyle = rgchDefTableStyle_.value();
        rgchDefPivotStyle = rgchDefPivotStyle_.value();
    }
    else
    {
        XLSB::XLNullableWideString    strDefList;
        XLSB::XLNullableWideString    strDefPivot;
        record >> cts >> strDefList >> strDefPivot;

        rgchDefTableStyle = strDefList.value();
        rgchDefPivotStyle = strDefPivot.value();
    }
}

void TableStyles::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 cchDefTableStyle;
		_UINT16 cchDefPivotStyle;

		LPWideStringNoCch	rgchDefTableStyle_(rgchDefTableStyle);
		LPWideStringNoCch	rgchDefPivotStyle_(rgchDefPivotStyle);

		cchDefTableStyle = rgchDefTableStyle_.getSize();
		cchDefPivotStyle = rgchDefPivotStyle_.getSize();
        frtHeader.rt = 0x088E;
		record << frtHeader << cts << cchDefTableStyle << cchDefPivotStyle;
		record << rgchDefTableStyle_ << rgchDefPivotStyle_;
	}
	else
	{
		XLSB::XLNullableWideString    strDefList(rgchDefTableStyle);
		XLSB::XLNullableWideString    strDefPivot(rgchDefPivotStyle);

		record << cts << strDefList << strDefPivot;
	}
}

} // namespace XLS

