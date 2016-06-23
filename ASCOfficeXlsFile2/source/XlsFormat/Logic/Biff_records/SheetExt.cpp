/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "SheetExt.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

SheetExt::SheetExt()
{
}


SheetExt::~SheetExt()
{
}


BaseObjectPtr SheetExt::clone()
{
	return BaseObjectPtr(new SheetExt(*this));
}


void SheetExt::writeFields(CFRecord& record)
{
// 	FrtHeader frtHeader(rt_SheetExt);
// 	record << frtHeader;

#pragma message("####################### SheetExt record is not implemented")
	Log::error("SheetExt record is not implemented.");
	//record << some_value;
}


void SheetExt::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_SheetExt);
	record >> frtHeader;

	record >> cb;
	
	_UINT32 flags;
	record >> flags;
	
	icvPlain = static_cast<unsigned char>(GETBITS(flags, 0, 6));

	if(0x00000028 == cb)
	{
		record >> sheetExtOptional;
	}
}

int SheetExt::serialize(std::wostream & stream)
{
	if (!sheetExtOptional.bEnabled) return 0;
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"sheetPr")
		{	
			if (!sheetExtOptional.fCondFmtCalc)	
				CP_XML_ATTR(L"enableFormatConditionsCalculation", false);
			if (!sheetExtOptional.fNotPublished)	
				CP_XML_ATTR(L"published" ,false);

			if (sheetExtOptional.color.xclrType.type == XColorType::XCLRRGB)
			{
				CP_XML_NODE(L"tabColor")
				{
					CP_XML_ATTR(L"rgb", sheetExtOptional.color.rgb.strARGB);
				}
			}

		}
	}
	return 0;
}

} // namespace XLS

