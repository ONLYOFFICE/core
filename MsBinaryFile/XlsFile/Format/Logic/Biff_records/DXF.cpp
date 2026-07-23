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

#include "DXF.h"

namespace XLS
{

DXF::DXF()
{
}


DXF::~DXF()
{
}


BaseObjectPtr DXF::clone()
{
	return BaseObjectPtr(new DXF(*this));
}

void DXF::readFields(CFRecord& record)
{
    if(record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> frtRefHeaderU;
        _UINT16 flags;

        record >> flags >> xfprops;

        xfprops.fNewBorder = GETBIT(flags, 1);
    }
    else
    {
        _UINT16 flags;

        record >> flags >> xfprops;

        xfprops.fNewBorder = GETBIT(flags, 15);
    }
}

void DXF::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		frtRefHeaderU.rt = 2189;
		record << frtRefHeaderU;
		_UINT16 flags = 0;
		SETBIT(flags, 0, 1)
		SETBIT(flags, 1, xfprops.fNewBorder)
		record << flags << xfprops;
	}
	else
	{
		_UINT16 flags = 0;

		SETBIT(flags, 15, xfprops.fNewBorder)
		record << flags << xfprops;
	}
}

int DXF::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dxf")
		{
			xfprops.serialize(CP_XML_STREAM(), true);
		}
	}
	return 0;
}

int DXF::deserialize(XmlUtils::CXmlLiteReader & oReader)
{
	xfprops.deserialize(oReader);

	return 0;
}

} // namespace XLS

