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

#include "ExternName.h"

namespace XLS
{

ExternName::ExternName(const unsigned short supporting_link_type, bool bOle)
:	supbook_cch(supporting_link_type), cf(-1), bOleVirtualPath(bOle)
{
}


ExternName::~ExternName()
{
}


BaseObjectPtr ExternName::clone()
{
	return BaseObjectPtr(new ExternName(*this));
}

void ExternName::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;

	fBuiltIn	= GETBIT(flags, 0);
	fWantAdvise = GETBIT(flags, 1);
	fWantPict	= GETBIT(flags, 2);
	fOle		= GETBIT(flags, 3);
	fOleLink	= GETBIT(flags, 4);
	cf			= GETBITS(flags, 5, 14);
	fIcon		= GETBIT(flags, 15);

	if(0x3A01 == supbook_cch) // UDF reference on a XLL or COM add-in.
	{
		body = BiffStructurePtr(new AddinUdf);
	}
	else
	{
		if(fOle && !fOleLink)
		{
			body = BiffStructurePtr(new ExternDdeLinkNoOper);
		}
		if(!fOle && fOleLink) 
		{
			body = BiffStructurePtr(new ExternOleDdeLink);
		}
		if(!fOle && !fOleLink)
		{
			if (bOleVirtualPath)
			{
				body = BiffStructurePtr(new ExternOleDdeLink);
			}
			else
			{
				body = BiffStructurePtr(new ExternDocName);
			}
		}
	}
	body->load(record);


//cache
	switch(cf)
	{
	case 0:		// is text
		break;
	case 5:		//csv (,)
	case 6:		//Microsoft Symbolic Link (SYLK).
	case 8:		//biff8
	case 44:	//unicode text
	case 63:	//biff12
		break;
	}
}

void ExternName::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fBuiltIn)
    SETBIT(flags, 1, fWantAdvise)
    SETBIT(flags, 2, fWantPict)
    SETBIT(flags, 3, fOle)
    SETBIT(flags, 4, fOleLink)
    SETBITS(flags, 5, 14, cf)
    SETBIT(flags, 15, fIcon)
    record << flags;
    record << body;
}

} // namespace XLS

