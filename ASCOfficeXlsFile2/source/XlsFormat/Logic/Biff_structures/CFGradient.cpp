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

#include "CFGradient.h"
#include <Binary/CFRecord.h>

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


} // namespace XLS

