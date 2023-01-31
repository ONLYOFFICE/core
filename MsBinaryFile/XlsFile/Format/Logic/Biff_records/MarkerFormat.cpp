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

#include "MarkerFormat.h"
#include "../../../Converter/XlsConverter.h"
#include "../../../Converter/xlsx_conversion_context.h"

namespace XLS
{
	std::wstring default_marker_color[] =
	{	
		L"000080", 	L"FF00FF", 	L"FFFF00", 	L"00FFFF", 	L"800080", 	L"800000", 	L"808000", 	L"0000FF", 	L"00CCFF", 	L"CCFFFF", 	
		L"CCFFCC", 	L"FFFF99", 	L"99CCFF", 	L"FF99CC", 	L"CC99FF", 	L"FFCC99", 	L"3366FF", 	L"33CCCC", 	L"99CC00", 	L"FFCC00", 	
		L"FF9900", 	L"FF6600", 	L"666699", 	L"969696", 	L"336600", 	L"339966", 	L"330000", 	L"333300", 	L"993300", 	L"993366", 	
		L"333399", 	L"000000", 	L"FFFFFF", 	L"FF0000", 	L"00FF00", 	L"0000FF", 	L"FFFF00", 	L"FF00FF", 	L"00FFFF", 	L"800000", 	
		L"800000", 	L"800000", 	L"808000", 	L"800080", 	L"808000", 	L"C0C0C0", 	L"808080", 	L"9999FF", 	L"993366", 	L"FFFFCC", 
		L"CCFFFF", 	L"660066", 	L"FF8080", 	L"0066CC", 	L"CCCCFF",
		//todoooo - подглядеть какие в мс далее 
		L"000080", 	L"FF00FF", 	L"FFFF00", 	L"00FFFF", 	L"800080", 	L"800000", 	L"808000", 	L"0000FF", 	L"00CCFF", 	L"CCFFFF", 	
		L"CCFFCC", 	L"FFFF99", 	L"99CCFF", 	L"FF99CC", 	L"CC99FF", 	L"FFCC99", 	L"3366FF", 	L"33CCCC", 	L"99CC00", 	L"FFCC00", 	
		L"FF9900", 	L"FF6600", 	L"666699", 	L"969696", 	L"336600", 	L"339966", 	L"330000", 	L"333300", 	L"993300", 	L"993366", 	
		L"333399", 	L"000000", 	L"FFFFFF", 	L"FF0000", 	L"00FF00", 	L"0000FF", 	L"FFFF00", 	L"FF00FF", 	L"00FFFF", 	L"800000", 	
		L"800000", 	L"800000", 	L"808000", 	L"800080", 	L"808000", 	L"C0C0C0", 	L"808080", 	L"9999FF", 	L"993366", 	L"FFFFCC", 
		L"CCFFFF", 	L"660066", 	L"FF8080", 	L"0066CC", 	L"CCCCFF"
	};

MarkerFormat::MarkerFormat()
{
}


MarkerFormat::~MarkerFormat()
{
}


BaseObjectPtr MarkerFormat::clone()
{
	return BaseObjectPtr(new MarkerFormat(*this));
}


void MarkerFormat::readFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	unsigned short flags;
	record >> rgbFore >> rgbBack >> imk >> flags >> icvFore >> icvBack >> miSize;
	
	fAuto		= GETBIT(flags, 0);
	fNotShowInt = GETBIT(flags, 4);
	fNotShowBrd = GETBIT(flags, 5);
}
int MarkerFormat::serialize(std::wostream & _stream)
{
	return serialize(_stream, -1, BaseObjectPtr());
}
int MarkerFormat::serialize(std::wostream & _stream, int index, BaseObjectPtr _GELFRAME)
{
	std::wstring strSymbol;
	std::wstringstream strm_fill;
	int marker_fill_type = 0;
	
	if (_GELFRAME)
	{
		_GELFRAME->serialize(strm_fill);
		marker_fill_type = global_info->xls_converter->xlsx_context->get_drawing_context().get_fill_type();
	}

	CP_XML_WRITER(_stream)
	{
		CP_XML_NODE(L"c:marker")
		{
			CP_XML_NODE(L"c:symbol")
			{
				switch (imk)
				{
				case 0x0000:	strSymbol = L"none";	break;//				No marker.
				case 0x0001:	strSymbol = L"square";	break;//				Square markers.
				case 0x0002:	strSymbol = L"diamond";	break;//				Diamond-shaped markers.
				case 0x0003:	strSymbol = L"triangle";break;//				Triangular markers.
				case 0x0004:	strSymbol = L"x";		break;//				Square markers with an X.
				case 0x0005:	strSymbol = L"star";	break;//				Square markers with an asterisk.
				case 0x0006:	strSymbol = L"dot";		break;//				Short bar markers.
				case 0x0007:	strSymbol = L"dash";	break;//				Long bar markers.
				case 0x0008:	strSymbol = L"circle";	break;//				Circular markers.
				case 0x0009:	strSymbol = L"plus";	break;//				Square markers with a plus sign.
				}

				if (marker_fill_type == 3) strSymbol = L"picture";
				
				CP_XML_ATTR(L"val",  strSymbol);	
			}
			if (imk > 0)
			{
				CP_XML_NODE(L"c:size") 
				{
					CP_XML_ATTR(L"val",  (false == fAuto) ? miSize / 20 : 5);	
				}
				CP_XML_NODE(L"c:spPr")
				{
					if (_GELFRAME)
					{
						CP_XML_STREAM() << strm_fill.str();
					}
					else
					{
						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val", (false == fAuto || index < 0) ? rgbBack.strRGB : default_marker_color[index]);
							}
						}
					}
					CP_XML_NODE(L"a:ln")
					{
						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val", (false == fAuto || index < 0) ? rgbFore.strRGB : default_marker_color[index]);
							}
						}
						CP_XML_NODE(L"a:prstDash") { CP_XML_ATTR(L"val", L"solid"); }
					}
				}
			}
		}
	}
	
	return 0;
}

} // namespace XLS
