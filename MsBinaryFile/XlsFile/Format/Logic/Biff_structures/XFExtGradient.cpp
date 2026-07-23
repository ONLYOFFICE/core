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

#include "XFExtGradient.h"

namespace XLS
{


BiffStructurePtr XFExtGradient::clone()
{
	return BiffStructurePtr(new XFExtGradient(*this));
}

void XFExtGradient::load(CFRecord& record)
{
	record >> gradient >> cGradStops;

	if (cGradStops > 256) cGradStops = 256;

	for(size_t i = 0; !record.isEOF() && i < cGradStops; ++i)
	{
		GradStop grad_stop;
		record >> grad_stop;
		rgGradStops.push_back(grad_stop);
	}
}

void XFExtGradient::save(CFRecord& record)
{
    cGradStops = rgGradStops.size();
    record << gradient << cGradStops;
    for(auto i : rgGradStops)
        record << i;
}

int XFExtGradient::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)
	{
		CP_XML_NODE(L"gradientFill")
		{
			for (size_t i = 0; i < rgGradStops.size(); ++i)
			{
				CP_XML_NODE(L"stop")
				{
					CP_XML_ATTR(L"position", rgGradStops[i].numPosition.data.value);
					CP_XML_NODE(L"color")
					{
						switch (rgGradStops[i].xclrType)
						{
							case 0: CP_XML_ATTR(L"auto", 1); break;
							case 1: CP_XML_ATTR(L"indexed", rgGradStops[i].xclrValue);	break;
							case 3: CP_XML_ATTR(L"theme", rgGradStops[i].xclrValue);	break;
							default:
								CP_XML_ATTR(L"rgb", rgGradStops[i].xclrValue);	break;
						}
						CP_XML_ATTR(L"tint", rgGradStops[i].numTint.data.value);
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS
