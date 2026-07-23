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

#include "CFGradient.h"

namespace XLS
{

BiffStructurePtr CFGradientInterpItem::clone()
{
	return BiffStructurePtr(new CFGradientInterpItem(*this));
}

void CFGradientInterpItem::load(CFRecord& record)
{
	cfvo.load(record);
	record >> numDomain;
	//record.skipNunBytes(8);
	//int val;
	//record >> val; numDomain = val << 32;
	//record >> val; numDomain = val;
}
void CFGradientInterpItem::save(CFRecord& record)
{
    cfvo.save(record);
	Xnum numDom;
	numDom.data.value = numDomain;
	record << numDom;
    //record.skipNunBytes(8);
    //int val;
    //record >> val; numDomain = val << 32;
    //record >> val; numDomain = val;
}
//---------------------------------------------------------------------------------------------
BiffStructurePtr CFGradientItem::clone()
{
	return BiffStructurePtr(new CFGradientItem(*this));
}

void CFGradientItem::load(CFRecord& record)
{
	//int val;
	//record >> val; numGrange = val << 32;
	//record >> val; numGrange = val;
	//record.skipNunBytes(8);
	record >> numGrange;
	
	color.load(record);
}

void CFGradientItem::save(CFRecord& record)
{
    record << numGrange;

    color.save(record);
}

//--------------------------------------------------------------------------------------------

BiffStructurePtr CFGradient::clone()
{
	return BiffStructurePtr(new CFGradient(*this));
}

void CFGradient::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused
	
	record.skipNunBytes(1); // reserved

	record >> cInterpCurve;

	record >> cGradientCurve;

	unsigned char flags;
	record >> flags;

	fClamp		= GETBIT(flags, 0);
	fBackground	= GETBIT(flags, 1);

	for (unsigned char i = 0; i < cInterpCurve; i++)
	{
		CFGradientInterpItemPtr item = CFGradientInterpItemPtr(new CFGradientInterpItem());
		item->load(record);
		//item.cfvo.load(record);
		//record >> val; item.numDomain = val << 32;
		//record >> val; item.numDomain += val;

		rgInterp.push_back(item);
	}

	for (unsigned char i = 0; i < cGradientCurve; i++)
	{
		CFGradientItemPtr item = CFGradientItemPtr(new CFGradientItem());
		item->load(record);
		
		//record >> val; item.numGrange = val << 32;
		//record >> val; item.numGrange += val;
		////record >> item.numGrange;
		//item.color.load(record);

		rgCurve.push_back(item);
	}
}
void CFGradient::save(CFRecord& record)
{
    record.reserveNunBytes(3);
    cInterpCurve = rgInterp.size();
    cGradientCurve = cInterpCurve;
    record << cInterpCurve << cGradientCurve;
    unsigned char flags = 0;
    SETBIT(flags, 0, fClamp);
    SETBIT(flags, 1, fBackground);
    record << flags;
    for(auto i : rgInterp)
        if(i!= nullptr)
			record << *i;
    for(auto i : rgCurve)
        if(i!= nullptr)
			record << *i;
}
int CFGradient::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"colorScale")
		{
			for (size_t i = 0; i < rgInterp.size(); i ++)
			{
				CP_XML_NODE(L"cfvo")
				{
					CFVO & cfvo = rgInterp[i]->cfvo;							
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
					else if(cfvo.numValue != 0)
						CP_XML_ATTR(L"val", cfvo.numValue);
				}
			}
			for (size_t i = 0; i < rgCurve.size(); i ++)
			{
				CP_XML_NODE(L"color")
				{
					CFColor & color = rgCurve[i]->color;
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
	}
	return 0;
}

} // namespace XLS

