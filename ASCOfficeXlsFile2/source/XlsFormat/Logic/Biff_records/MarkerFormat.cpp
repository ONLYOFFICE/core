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

#include "MarkerFormat.h"

namespace XLS
{

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


void MarkerFormat::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 4, fNotShowInt);
	SETBIT(flags, 5, fNotShowBrd);
	record << rgbFore << rgbBack << imk << flags << icvFore << icvBack << miSize;
}

void MarkerFormat::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> rgbFore >> rgbBack >> imk >> flags >> icvFore >> icvBack >> miSize;
	
	fAuto		= GETBIT(flags, 0);
	fNotShowInt = GETBIT(flags, 4);
	fNotShowBrd = GETBIT(flags, 5);
}

int MarkerFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:marker")
		{
			CP_XML_NODE(L"c:symbol")
			{
				std::wstring strSymbol;
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
				CP_XML_ATTR(L"val",  strSymbol);	
			}
			if (imk > 0)
			{
				CP_XML_NODE(L"c:size") { CP_XML_ATTR(L"val",  miSize / 20);	}
				CP_XML_NODE(L"c:spPr")
				{
					CP_XML_NODE(L"a:solidFill")
					{
						CP_XML_NODE(L"a:srgbClr")
						{
							CP_XML_ATTR(L"val",  rgbBack.strRGB);		
						}
					}
					CP_XML_NODE(L"a:ln")
					{
						CP_XML_NODE(L"a:solidFill")
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								CP_XML_ATTR(L"val",  rgbFore.strRGB);		
							}
						}
						CP_XML_NODE(L"a:prstDash") { CP_XML_ATTR(L"val",  L"solid"); }

					}
				}
			}
		}
	}
	
	return 0;
}

} // namespace XLS
