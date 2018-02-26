/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "CF12.h"
#include "CFEx.h"

#include <Logic/Biff_structures/CFMultistate.h>
#include <Logic/Biff_structures/CFDatabar.h>
#include <Logic/Biff_structures/CFGradient.h>
#include <utils.h>

namespace XLS
{

CF12::CF12(const CellRef& cell_base_ref)
:	rgce1(cell_base_ref),
	rgce2(cell_base_ref),
	fmlaActive(cell_base_ref)
{
	dxfId_ = 0;
	ipriority_ = 0;
}


CF12::~CF12()
{
}


BaseObjectPtr CF12::clone()
{
	return BaseObjectPtr(new CF12(*this));
}

void CF12::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	record >> frtRefHeader;
	record >> ct >> cp;
	
	unsigned short cce1;
	unsigned short cce2;
	
	record >> cce1 >> cce2;
	record >> dxf;
	
	rgce1.load(record, cce1);
	rgce2.load(record, cce2);
	fmlaActive.load(record);
	
	unsigned char flags;
	record >> flags;
	fStopIfTrue = GETBIT(flags, 1);
	
	record >> ipriority >> icfTemplate;
	
	unsigned char cbTemplateParm;
	
	record >> cbTemplateParm;
	record >> rgbTemplateParms;

	switch(ct)
	{
		case 0x03:
			rgbCT = BiffStructurePtr(new CFGradient);
			rgbCT->load(record);
			break;
		case 0x04:
			rgbCT = BiffStructurePtr(new CFDatabar);
			rgbCT->load(record);
			break;
		case 0x05:
			//todooo
			//rgbCT = BiffStructurePtr(new CFFilter);
			//rgbCT->load(record);
			break;
		case 0x06:
			rgbCT = BiffStructurePtr(new CFMultistate);
			rgbCT->load(record);
			break;
	}
	
	ipriority_	= ++record.getGlobalWorkbookInfo()->cmt_rules;
	
	if  ( 0 == dxf.serialize(record.getGlobalWorkbookInfo()->users_Dxfs_stream))
		dxfId_ = global_info->cellStyleDxfs_count++;
	else 
		dxfId_ = -1;
}
int CF12::serialize(std::wostream & stream)
{
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
				case 3:	CP_XML_ATTR(L"type", L"colorScale");	break;
				case 4:	CP_XML_ATTR(L"type", L"dataBar");		break;
				case 6:	CP_XML_ATTR(L"type", L"iconSet");		break;
			}
			if (ct == 1)
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
			CP_XML_ATTR(L"priority", ipriority_);
			CP_XML_ATTR(L"stopIfTrue", fStopIfTrue);

			if (dxfId_ >= 0)
				CP_XML_ATTR(L"dxfId", dxfId_);

			if (ct == 3)
			{
				CFGradient *gradient = dynamic_cast<CFGradient*>(rgbCT.get());
				CP_XML_NODE(L"colorScale")
				{
					for (size_t i = 0; i < gradient->rgInterp.size(); i ++)
					{
						CP_XML_NODE(L"cfvo")
						{
							CFVO & cfvo = gradient->rgInterp[i]->cfvo;							
							switch(cfvo.cfvoType)
							{
								case 2:	CP_XML_ATTR(L"type", L"min");			break;
								case 3:	CP_XML_ATTR(L"type", L"max");			break;
								case 7:	CP_XML_ATTR(L"type", L"formule");		break;
								case 4:	CP_XML_ATTR(L"type", L"percent");		break;
								case 5:	CP_XML_ATTR(L"type", L"percentile");	break;
								default:
									CP_XML_ATTR(L"type", L"num");				break;
							}	
							if (cfvo.cfvoType == 7)
								CP_XML_ATTR(L"val", cfvo.fmla.getAssembledFormula()); 
							else
								CP_XML_ATTR(L"val", cfvo.numValue);
						}
					}
					for (size_t i = 0; i < gradient->rgCurve.size(); i ++)
					{
						CP_XML_NODE(L"color")
						{
							CFColor & color = gradient->rgCurve[i]->color;
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
			}
			if (ct == 4)
			{
				CFDatabar *dataBar = dynamic_cast<CFDatabar*>(rgbCT.get());

				CP_XML_NODE(L"dataBar")
				{
					//todooo cfvo = num - db1 & db2
					CP_XML_NODE(L"cfvo")
					{
						if (dataBar->iPercentMin > 0)
							CP_XML_ATTR(L"percent", dataBar->iPercentMin);
						else
							CP_XML_ATTR(L"type", L"min");
					}
					CP_XML_NODE(L"cfvo")
					{
						if (dataBar->iPercentMax < 100)
							CP_XML_ATTR(L"percent", dataBar->iPercentMax);
						else
							CP_XML_ATTR(L"type", L"max");
					}
					CP_XML_NODE(L"color")
					{
						switch(dataBar->color.xclrType.type)
						{
						case 1: CP_XML_ATTR(L"indexed",	dataBar->color.icv);			break;
						case 2:	CP_XML_ATTR(L"rgb",		dataBar->color.rgb.strARGB);	break;
						case 3: CP_XML_ATTR(L"theme",	dataBar->color.theme);
								CP_XML_ATTR(L"tint",	dataBar->color.numTint);		break;
						default: CP_XML_ATTR(L"auto", true);
						}
					}
				}
			}
			//что использовать зависит от типа ... todooo
			std::wstring s	= fmlaActive.getAssembledFormula();
			std::wstring s1 = rgce1.getAssembledFormula();
			std::wstring s2 = rgce2.getAssembledFormula();
			
			if (!s1.empty() || !s2.empty())
			{
				CP_XML_NODE(L"formula")
				{
					if (!s1.empty()) 
						CP_XML_STREAM() << xml::utils::replace_text_to_xml(s1);
					else if(!s2.empty()) 
						CP_XML_STREAM() << xml::utils::replace_text_to_xml(s2);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

