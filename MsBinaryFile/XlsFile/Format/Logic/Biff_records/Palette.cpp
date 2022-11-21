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

#include "Palette.h"

namespace XLS
{

Palette::Palette()
:	ccv(56)
{
}


Palette::Palette(const unsigned short ccv_init)
:	ccv(ccv_init)
{

}


BaseObjectPtr Palette::clone()
{
	return BaseObjectPtr(new Palette(*this));
}


const std::wstring standart_color[8] = {
										L"00000000",
										L"00FFFFFF",
										L"00FF0000",
										L"0000FF00",
										L"000000FF",
										L"00FFFF00",
										L"00FF00FF",
										L"0000FFFF"};


void Palette::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	unsigned short ccv;
	record >> ccv;
	
	for(int i = 0; i < 8; ++i)
	{	
		global_info->RegisterPaletteColor(i, standart_color[i].substr(2,6));
	}

	for(int i = 0; i < ccv; ++i)
	{
		LongRGBPtr rgb(new LongRGB);
		record >> *rgb;
		rgColor.push_back(rgb);

		global_info->RegisterPaletteColor(i+8, rgb->strRGB);
	}
}



int Palette::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"indexedColors")
		{		
			for(int i = 0; i < 8; ++i)
			{	
				CP_XML_NODE(L"rgbColor")
				{
					CP_XML_ATTR(L"rgb", standart_color[i]);
				}
			}

			for(size_t i = 0; i < rgColor.size(); ++i)
			{		
				LongRGB * rgb = dynamic_cast<LongRGB *>(rgColor[i].get());
				CP_XML_NODE(L"rgbColor")
				{
					CP_XML_ATTR(L"rgb", rgb->strARGB);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

