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

#include "LineFormat.h"

namespace XLS
{

LineFormat::LineFormat()
{
}


LineFormat::~LineFormat()
{
}


BaseObjectPtr LineFormat::clone()
{
	return BaseObjectPtr(new LineFormat(*this));
}

void LineFormat::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> rgb >> lns >> we >> flags >> icv;
	
	fAuto = GETBIT(flags, 0);
	fAxisOn = GETBIT(flags, 2);
	fAutoCo = GETBIT(flags, 3);
}

int LineFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"a:ln")
		{
			switch(we)
			{
				case 0xFFFF:	CP_XML_ATTR(L"w", 3175);	break; //Hairline
				case 0:			CP_XML_ATTR(L"w", 12700);	break; //single
				case 1:			CP_XML_ATTR(L"w", 25400);	break; //double
				case 2:			CP_XML_ATTR(L"w", 38100);	break; //triple
			}
			if (lns == (_UINT16) 5)
			{
				CP_XML_NODE(L"a:noFill");
			}
			else
			{
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",  rgb.strRGB);		
					}
				}

				CP_XML_NODE(L"a:prstDash")
				{			
					switch(lns)
					{
						case 1: CP_XML_ATTR(L"val", L"dash");			break;  //Dash
						case 2: CP_XML_ATTR(L"val", L"dot");			break;  //Dot
						case 3: CP_XML_ATTR(L"val", L"dashDot");		break;  //Dash-dot
						case 4: CP_XML_ATTR(L"val", L"sysDashDotDot");	break;  //Dash dot-dot
						case 0: 
						default:CP_XML_ATTR(L"val", L"solid");			break;  //Solid
					}
				}			
				switch(lns)
				{
					case 6: break; //Dark gray pattern
					case 7: break; //Medium gray pattern
					case 8: break; //Light gray patternn
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

