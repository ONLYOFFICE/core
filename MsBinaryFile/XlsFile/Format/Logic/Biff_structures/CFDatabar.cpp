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

#include "CFDatabar.h"
#include "../../../../Common/Utils/simple_xml_writer.h"

namespace XLS
{


BiffStructurePtr CFDatabar::clone()
{
	return BiffStructurePtr(new CFDatabar(*this));
}

void CFDatabar::load(CFRecord& record)
{
	record.skipNunBytes(3); // unused
	
	unsigned char flags;
	record >> flags;

	fRightToLeft	= GETBIT(flags, 0);
	fShowValue		= GETBIT(flags, 1);

	record >> iPercentMin >> iPercentMax;

	record >> color;

	cfvoDB1.load(record);
	cfvoDB2.load(record);
}

int CFDatabar::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dataBar")
		{
			//todooo cfvo = num - db1 & db2
			CP_XML_NODE(L"cfvo")
			{
				if (iPercentMin > 0)
					CP_XML_ATTR(L"percent", iPercentMin);
				else
					CP_XML_ATTR(L"type", L"min");
			}
			CP_XML_NODE(L"cfvo")
			{
				if (iPercentMax < 100)
					CP_XML_ATTR(L"percent", iPercentMax);
				else
					CP_XML_ATTR(L"type", L"max");
			}
			CP_XML_NODE(L"color")
			{
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
	return 0;
}

} // namespace XLS

