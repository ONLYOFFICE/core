﻿/*
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

#include "TableStyles.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/XLWideString.h"

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

