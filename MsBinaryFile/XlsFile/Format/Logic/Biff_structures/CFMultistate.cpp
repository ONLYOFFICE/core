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
int CFMultistate::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"iconSet")
		{
			switch(iIconSet)
			{
				case 0:		CP_XML_ATTR(L"iconSet", L"3Symbols");		break;
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

