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

#include "CFMultistate.h"
#include "../../../../Common/Utils/simple_xml_writer.h"

namespace XLS
{

BiffStructurePtr CFMultistate::clone()
{
	return BiffStructurePtr(new CFMultistate(*this));
}

void CFMultistate::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused
	record.skipNunBytes(1); // reserved
	record >> cStates >> iIconSet;
	
	unsigned char flags;
	record >> flags;
	fIconOnly = GETBIT(flags, 0);
	fReverse = GETBIT(flags, 2);

	for(unsigned char i = 0; i < cStates; ++i)
	{
		CFMStateItemPtr item(new CFMStateItem);
		item->load(record);
		rgStates.push_back(item);
	}
}

void CFMultistate::save(CFRecord& record)
{
    record.reserveNunBytes(3);
    cStates = rgStates.size();
    record << cStates << iIconSet;
    unsigned char flags = 0;
    SETBIT(flags, 0, fIconOnly);
    SETBIT(flags, 2, fReverse);
    record << flags;
    for (auto i : rgStates)
        if(i != nullptr)
            i->save(record);
}

int CFMultistate::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"iconSet")
		{
			switch(iIconSet)
			{
				case 0:		CP_XML_ATTR(L"iconSet", L"3Arrows");		break;
				case 1:		CP_XML_ATTR(L"iconSet", L"3ArrowsGray");	break;
				case 2:		CP_XML_ATTR(L"iconSet", L"3Flags");			break;
				case 3:		CP_XML_ATTR(L"iconSet", L"3TrafficLights1");break;
				case 4:		CP_XML_ATTR(L"iconSet", L"3Signs");			break;
				case 5:		CP_XML_ATTR(L"iconSet", L"3TrafficLights2");break;
				case 6:		CP_XML_ATTR(L"iconSet", L"3Symbols");		break;
				case 7:		CP_XML_ATTR(L"iconSet", L"3Symbols2");		break;
				case 8:		CP_XML_ATTR(L"iconSet", L"4Arrows");		break;
				case 9:		CP_XML_ATTR(L"iconSet", L"4ArrowsGray");	break;
				case 10:	CP_XML_ATTR(L"iconSet", L"4RedToBlack");	break;
				case 11:	CP_XML_ATTR(L"iconSet", L"4Rating");		break;
				case 12:	CP_XML_ATTR(L"iconSet", L"4TrafficLights");	break;
				case 13:	CP_XML_ATTR(L"iconSet", L"5Arrows");		break;
				case 14:	CP_XML_ATTR(L"iconSet", L"5ArrowsGray");	break;
				case 15:	CP_XML_ATTR(L"iconSet", L"5Rating");		break;
				case 16:	CP_XML_ATTR(L"iconSet", L"5Quarters");		break;
				default:
					CP_XML_ATTR(L"iconSet", L"3Symbols");				break;
			}
			CP_XML_ATTR(L"showValue", false == fIconOnly);	
			
			if (fReverse)
				CP_XML_ATTR(L"reverse", 1);	

			for (size_t i = 0; i < rgStates.size(); i ++)
			{
				CP_XML_NODE(L"cfvo")
				{
					CFVO & cfvo = rgStates[i]->cfvo;							
					switch(cfvo.cfvoType)
					{
						case 2:	CP_XML_ATTR(L"type", L"min");			break;
						case 3:	CP_XML_ATTR(L"type", L"max");			break;
						case 7:	CP_XML_ATTR(L"type", L"formule");		break;
						case 4:	CP_XML_ATTR(L"type", L"percent");		break;
						case 5:	CP_XML_ATTR(L"type", L"percentile");	break;
						default:
							CP_XML_ATTR(L"type", L"num");				break;
					}	
					if (cfvo.cfvoType == 7)
						CP_XML_ATTR(L"val", cfvo.fmla.getAssembledFormula()); 
					else
						CP_XML_ATTR(L"val", cfvo.numValue);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

