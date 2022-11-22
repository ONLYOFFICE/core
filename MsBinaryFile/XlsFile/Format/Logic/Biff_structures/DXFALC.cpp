/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

