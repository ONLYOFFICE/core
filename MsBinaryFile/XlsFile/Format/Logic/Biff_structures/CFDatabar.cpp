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

#include "CFDatabar.h"
#include "../../../../Common/Utils/simple_xml_writer.h"

namespace XLS
{


BiffStructurePtr CFDatabar::clone()
{
	return BiffStructurePtr(new CFDatabar(*this));
}

void CFDatabar::load(CFRecord& record)
{
	record.skipNunBytes(3); // unused
	
	unsigned char flags;
	record >> flags;

	fRightToLeft	= GETBIT(flags, 0);
	fShowValue		= GETBIT(flags, 1);

	record >> iPercentMin >> iPercentMax;

	record >> color;

	cfvoDB1.load(record);
	cfvoDB2.load(record);
}

void CFDatabar::save(CFRecord& record)
{
    record.reserveNunBytes(3); // unused

    unsigned char flags = 0;
    SETBIT(flags, 0, fRightToLeft);
    SETBIT(flags, 1, fShowValue);
    record << flags;
    record << iPercentMin << iPercentMax << color;
    cfvoDB1.save(record);
    cfvoDB2.save(record);
}

int CFDatabar::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dataBar")
		{
			//todooo cfvo = num - db1 & db2
			CP_XML_NODE(L"cfvo")
			{
				if (iPercentMin > 0)
					CP_XML_ATTR(L"percent", iPercentMin);
				else
					CP_XML_ATTR(L"type", L"min");
			}
			CP_XML_NODE(L"cfvo")
			{
				if (iPercentMax < 100)
					CP_XML_ATTR(L"percent", iPercentMax);
				else
					CP_XML_ATTR(L"type", L"max");
			}
			CP_XML_NODE(L"color")
			{
				switch(color.xclrType.type)
				{
				case 1: CP_XML_ATTR(L"indexed",	color.icv);			break;
				case 2:	CP_XML_ATTR(L"rgb",		color.rgb.strARGB);	break;
				case 3: CP_XML_ATTR(L"theme",	color.theme);
						CP_XML_ATTR(L"tint",	color.numTint);		break;
				default: CP_XML_ATTR(L"auto", true);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

