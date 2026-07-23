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

#include "Bar.h"
#include "Chart3d.h"

namespace XLS
{

Bar::Bar()
{
}


Bar::~Bar()
{
}


BaseObjectPtr Bar::clone()
{
	return BaseObjectPtr(new Bar(*this));
}


void Bar::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pcOverlap >> pcGap >> flags;
	
	fTranspose	= GETBIT(flags, 0);
	fStacked	= GETBIT(flags, 1);
	f100		= GETBIT(flags, 2);
	fHasShadow	= GETBIT(flags, 3);
}

void Bar::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fTranspose)
    SETBIT(flags, 1, fStacked)
    SETBIT(flags, 2, f100)
    SETBIT(flags, 3, fHasShadow)

    record << pcOverlap << pcGap << flags;
}

int	Bar::serialize(std::wostream & _stream)
{
	Chart3d *chart3D = dynamic_cast<Chart3d *>(m_chart3D.get());
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:barDir")
		{
			if (fTranspose) CP_XML_ATTR (L"val" , L"bar");
			else			CP_XML_ATTR (L"val" , L"col");
		}
		if (fStacked || f100)
		{
			CP_XML_NODE(L"c:grouping")
			{
				if (f100)			CP_XML_ATTR (L"val" , L"percentStacked");				
				else if (fStacked)	CP_XML_ATTR (L"val" , L"stacked");
			}
		}
		else
		{
			CP_XML_NODE(L"c:grouping")
			{
				if ((chart3D) && (chart3D->fCluster))
					CP_XML_ATTR (L"val" , L"clustered"); 
				else		
					CP_XML_ATTR (L"val" , L"standard");
			}
		}

		CP_XML_NODE(L"c:gapWidth")
		{
			CP_XML_ATTR (L"val" , pcGap);
		}
		if (!chart3D && pcOverlap != 0)
		{
			CP_XML_NODE(L"c:overlap")
			{
				CP_XML_ATTR (L"val" , -pcOverlap);
			}
		}

		if ((chart3D) && (chart3D->pcGap != 150))
		{
			CP_XML_NODE(L"c:gapDepth")
			{
				CP_XML_ATTR (L"val" , chart3D->pcGap);
			}
		}
	}
	return 0;
}

} // namespace XLS

