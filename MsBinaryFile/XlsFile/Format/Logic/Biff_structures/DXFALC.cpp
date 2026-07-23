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

#include "DXFN.h"

namespace XLS
{
BiffStructurePtr DXFALC::clone()
{
	return BiffStructurePtr(new DXFALC(*this));
}
void DXFALC::load(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;
	
	alc				= static_cast<unsigned char>(GETBITS(flags, 0, 2));
	fWrap			= GETBIT(flags, 3);
	alcv			= static_cast<unsigned char>(GETBITS(flags, 4, 6));
	fJustLast		= GETBIT(flags, 7);
	trot			= static_cast<unsigned char>(GETBITS(flags, 8, 15));
	cIndent			= static_cast<unsigned char>(GETBITS(flags, 16, 19));
	fShrinkToFit	= GETBIT(flags, 20);
	fMergeCell		= GETBIT(flags, 21);
	iReadingOrder	= static_cast<unsigned char>(GETBITS(flags, 22, 23));

	record >> iIndent;
}

void DXFALC::save(CFRecord& record)
{
    _UINT32 flags = 0;
    SETBITS(flags, 0, 2, alc)
    SETBIT(flags, 3, fWrap)
    SETBITS(flags, 4, 6, alcv)
    SETBIT(flags, 7, fJustLast)
    SETBITS(flags, 8, 15, trot)
    SETBITS(flags, 16, 19, cIndent)
    SETBIT(flags, 20, fShrinkToFit)
    SETBIT(flags, 21, fMergeCell)
    SETBITS(flags, 22, 23, iReadingOrder)

    record << flags << iIndent;
}

int DXFALC::serialize(std::wostream & stream)
{
	if (parent->iReadingOrderNinch && parent->alchNinch && parent->alcvNinch && parent->wrapNinch && 
		parent->trotNinch && parent->kintoNinch && parent->cIndentNinch && parent->fShrinkNinch && 
		parent->fMergeCellNinch ) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"alignment")
		{
			if (!parent->iReadingOrderNinch)
			{
				CP_XML_ATTR(L"readingOrder", iReadingOrder);
			}
			if (!parent->alchNinch && alc != 0xFF)
			{
				switch(alc)
				{
				case 0:	CP_XML_ATTR(L"horizontal", L"general");	break;
				case 1:	CP_XML_ATTR(L"horizontal", L"left");	break;
				case 2:	CP_XML_ATTR(L"horizontal", L"center");	break;
				case 3:	CP_XML_ATTR(L"horizontal", L"right");	break;
				case 4:	CP_XML_ATTR(L"horizontal", L"fill");	break;
				case 5:	CP_XML_ATTR(L"horizontal", L"justify");	break;
				case 6:	CP_XML_ATTR(L"horizontal", L"centerContinuous");break;
				case 7:	CP_XML_ATTR(L"horizontal", L"distributed");	break;
				}
			}
			if (!parent->alcvNinch)
			{
				switch(alcv)
				{
				case 0:	CP_XML_ATTR(L"vertical", L"top");		break;
				case 1:	CP_XML_ATTR(L"vertical", L"center");	break;
				case 2:	CP_XML_ATTR(L"vertical", L"bottom");	break;
				case 3:	CP_XML_ATTR(L"vertical", L"justify");	break;
				case 4:	CP_XML_ATTR(L"vertical", L"distributed");break;
				}			
			}
			if (!parent->trotNinch)
			{
				CP_XML_ATTR(L"textRotation", trot);
			}
			if (!parent->wrapNinch)
			{
				CP_XML_ATTR(L"wrapText", fWrap);
			}
			if (!parent->cIndentNinch)
			{
				CP_XML_ATTR(L"indent",			cIndent);
				CP_XML_ATTR(L"relativeIndent",	iIndent);
			}
			if (!parent->kintoNinch)
			{
				CP_XML_ATTR(L"justifyLastLine", fJustLast);
			}
			if (!parent->fShrinkNinch)
			{
  				CP_XML_ATTR(L"shrinkToFit", fShrinkToFit);
			}
		}
	}
	return 0;
}


} // namespace XLS

