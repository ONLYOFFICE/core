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

#include "DataLabExtContents.h"
#include "../Biff_structures/FrtHeader.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

DataLabExtContents::DataLabExtContents()
{
	is_area = false;
}


DataLabExtContents::~DataLabExtContents()
{
}


BaseObjectPtr DataLabExtContents::clone()
{
	return BaseObjectPtr(new DataLabExtContents(*this));
}

void DataLabExtContents::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	unsigned short flags;
	record >> flags >> rgchSep;

	fSerName	= GETBIT(flags, 0);
	fCatName	= GETBIT(flags, 1);
	fValue		= GETBIT(flags, 2);
	fPercent	= GETBIT(flags, 3);
	fBubSizes	= GETBIT(flags, 4);
}

int DataLabExtContents::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")
		{
			CP_XML_ATTR (L"val" , fValue); 
		}
		CP_XML_NODE(L"c:showPercent")
		{
			CP_XML_ATTR (L"val" , fPercent); 
		}
		CP_XML_NODE(L"c:showBubbleSize")
		{
			CP_XML_ATTR (L"val" , fBubSizes); 
		}

		if (rgchSep.value().empty() == false)
		{
			CP_XML_NODE(L"c:separator")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(rgchSep.value());
			}
		}
		if (is_area == false)
		{
			CP_XML_NODE(L"c:showCatName")
			{
				CP_XML_ATTR (L"val", fCatName); 
			}		
			CP_XML_NODE(L"c:showSerName") {	CP_XML_ATTR (L"val" , 0); }	
		}
		else
		{
			CP_XML_NODE(L"c:showCatName") {	CP_XML_ATTR (L"val" , 0); }	
			CP_XML_NODE(L"c:showSerName")
			{
				CP_XML_ATTR (L"val" , fSerName); 
			}		
		}


	}
	return 0;
}

} // namespace XLS

