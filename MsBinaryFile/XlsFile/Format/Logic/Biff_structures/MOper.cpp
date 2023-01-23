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

#include "MOper.h"
#include "../../../../Common/Utils/simple_xml_writer.h"

namespace XLS
{

BiffStructurePtr MOper::clone()
{
	return BiffStructurePtr(new MOper(*this));
}

void MOper::load(CFRecord& record)
{
	record >> colLast >> rowLast;
	
	for(int i = 0; i < (colLast + 1) * (rowLast + 1); ++i)
	{
		unsigned char rec_type;
		record >> rec_type;
		
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		
		extOper.push_back(ser);
	}
}

int MOper::serialize(std::wostream & strm)
{
	if (extOper.empty()) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"values")
		{
			for(size_t i = 0; i < extOper.size(); ++i)
			{		
				CP_XML_NODE(L"value")
				{
					switch(extOper[i]->fixed_type)
					{
					case SerAr::typeSerNil:		CP_XML_ATTR(L"t", L"nil");	break;
					case SerAr::typeSerNum:		CP_XML_ATTR(L"t", L"n");	break;
					case SerAr::typeSerStr:		CP_XML_ATTR(L"t", L"str");	break;
					case SerAr::typeSerBool:	CP_XML_ATTR(L"t", L"b");	break;
					case SerAr::typeSerErr:		CP_XML_ATTR(L"t", L"e");	break;
					}
					CP_XML_NODE(L"val")
					{			
						if (extOper[i]->fixed_type == SerAr::typeSerStr)
						{
							CP_XML_ATTR(L"xml:space", L"preserve"); 
						}
						CP_XML_STREAM() << extOper[i]->toString();
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

