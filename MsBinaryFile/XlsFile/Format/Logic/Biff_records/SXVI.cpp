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

#include "SXVI.h"

namespace XLS
{

SXVI::SXVI()
{
}


SXVI::~SXVI()
{
}


BaseObjectPtr SXVI::clone()
{
	return BaseObjectPtr(new SXVI(*this));
}

void SXVI::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> itmType>> flags >> iCache >> cchName;

	fHidden		= GETBIT(flags, 0);
	fHideDetail	= GETBIT(flags, 1);
	fFormula	= GETBIT(flags, 3);
	fMissing	= GETBIT(flags, 4);
	
	if (cchName > 0 && cchName < 0xFFFF)
	{
		stName.setSize(cchName);
		record >> stName;

	}
}
int SXVI::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"item")
		{
			if (fHidden)	CP_XML_ATTR(L"h", 1); 
			
			if (fMissing)	CP_XML_ATTR(L"m", 1); 
			if (fHideDetail)CP_XML_ATTR(L"sd", 0); 
			if (fFormula)	CP_XML_ATTR(L"f", 1);

			if (!stName.value().empty())
			{
				CP_XML_ATTR(L"n", stName.value()); 
			}

			if (itmType == 0)
			{
				CP_XML_ATTR(L"x", iCache); 
			}
			switch(itmType)
			{
			case 0x0001: CP_XML_ATTR(L"t", L"default"); break;
			case 0x0002: CP_XML_ATTR(L"t", L"sum");		break;
			case 0x0003: CP_XML_ATTR(L"t", L"countA");	break;
			case 0x0004: CP_XML_ATTR(L"t", L"avg");		break;
			case 0x0005: CP_XML_ATTR(L"t", L"max");		break;
			case 0x0006: CP_XML_ATTR(L"t", L"min");		break;
			case 0x0007: CP_XML_ATTR(L"t", L"product"); break;
			case 0x0008: CP_XML_ATTR(L"t", L"count");	break;
			case 0x0009: CP_XML_ATTR(L"t", L"stdDev");	break;
			case 0x000a: CP_XML_ATTR(L"t", L"stdDevP"); break;
			case 0x000b: CP_XML_ATTR(L"t", L"var");		break;
			case 0x000c: CP_XML_ATTR(L"t", L"varP");	break;
			}
		}
	}
	return 0;
}
} // namespace XLS

