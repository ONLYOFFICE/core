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

#include "CF.h"
#include "CFEx.h"

#include <utils.h>

namespace XLS
{

CF::CF(const CellRef& cell_base_ref)
:	rgce1(cell_base_ref),
	rgce2(cell_base_ref)
{
	dxfId_ = 0;
	ipriority_ = 0;
}


CF::~CF()
{
}


BaseObjectPtr CF::clone()
{
	return BaseObjectPtr(new CF(*this));
}


void CF::writeFields(CFRecord& record)
{
	record << ct << cp;
	
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	
	record << rgbdxf;
	
	rgce1.store(record);
	rgce2.store(record);
	
	record.registerDelayedDataSource(rgce1.getCCE(), rt_CF);
	record.registerDelayedDataSource(rgce2.getCCE(), rt_CF);
}


void CF::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	unsigned short cce1;
	unsigned short cce2;
	
	record >> ct >> cp >> cce1 >> cce2;
	record >> rgbdxf;
	
	rgce1.load(record, cce1);
	rgce2.load(record, cce2);
	
	ipriority_	= ++record.getGlobalWorkbookInfo()->cmt_rules;

	rgbdxf.serialize(record.getGlobalWorkbookInfo()->users_Dxfs_stream);
	dxfId_ = global_info->cellStyleDxfs_count++;
}

int CF::serialize(std::wostream & stream)
{
	if (ct != 1 && ct != 2 && ct !=6) 
		return 0;
	
	CFEx * cfEx = dynamic_cast<CFEx *>(m_CFEx.get());
	if (cfEx)
	{
	}

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cfRule")
		{
			switch(ct)
			{
				case 1:	CP_XML_ATTR(L"type", L"cellIs");		break;
				case 2:	CP_XML_ATTR(L"type", L"expression");	break;
				case 6:	CP_XML_ATTR(L"type", L"iconSet");		break;
			}

			if (ct == (unsigned char)1)
			{
				switch(cp)
				{
					case 1:	CP_XML_ATTR(L"operator", L"between");			break;
					case 2:	CP_XML_ATTR(L"operator", L"notBetween");		break;
					case 3:	CP_XML_ATTR(L"operator", L"equal");				break;
					case 4:	CP_XML_ATTR(L"operator", L"notEqual");			break;
					case 5:	CP_XML_ATTR(L"operator", L"greaterThan");		break;
					case 6:	CP_XML_ATTR(L"operator", L"lessThan");			break;
					case 7:	CP_XML_ATTR(L"operator", L"greaterThanOrEqual");break;
					case 8:	CP_XML_ATTR(L"operator", L"lessThanOrEqual");	break;
				}
			}

			CP_XML_ATTR(L"priority", cfEx ? cfEx->content.ipriority_ : ipriority_);
			
			CP_XML_ATTR(L"stopIfTrue", 1);

			if ((cfEx) && (cfEx->content.fHasDXF))
			{
				cfEx->content.dxf.serialize(CP_XML_STREAM());
			}
			else
			{
				CP_XML_ATTR(L"dxfId", dxfId_);	
			}

			std::wstring s1 = rgce1.getAssembledFormula();
			std::wstring s2 = rgce2.getAssembledFormula();
			
			CP_XML_NODE(L"formula")
			{
				if (!s1.empty()) 
					CP_XML_STREAM() << xml::utils::replace_text_to_xml(s1);
				else if(!s2.empty()) 
					CP_XML_STREAM() << xml::utils::replace_text_to_xml(s2);
			}
		}
	}
	return 0;
}


} // namespace XLS

