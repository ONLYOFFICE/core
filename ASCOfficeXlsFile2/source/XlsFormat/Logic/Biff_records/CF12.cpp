
#include "CF12.h"
#include <Logic/Biff_structures/CFMultistate.h>
#include <Logic/Biff_structures/CFDatabar.h>
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


void CF12::writeFields(CFRecord& record)
{

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
			//rgbCT = BiffStructurePtr(new CFGradient);
			//rgbCT->load(record);
			break;
		case 0x04:
			rgbCT = BiffStructurePtr(new CFDatabar);
			rgbCT->load(record);
			break;
		case 0x05:
			//rgbCT = BiffStructurePtr(new CFFilter);
			//rgbCT->load(record);
			break;
		case 0x06:
			rgbCT = BiffStructurePtr(new CFMultistate);
			rgbCT->load(record);
			break;
	}
	
	ipriority_	= ++record.getGlobalWorkbookInfo()->cmt_rules;
	
	dxf.serialize(record.getGlobalWorkbookInfo()->users_Dxfs_stream);
	dxfId_ = global_info->cellStyleDxfs_count++;

}
int CF12::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cfRule")
		{
			switch(ct)
			{
				case 1:	CP_XML_ATTR(L"type", L"cellIs");		break;
				case 2:	CP_XML_ATTR(L"type", L"expression");	break;
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

			CP_XML_ATTR(L"dxfId", dxfId_);

			if (ct == 4)
			{
				CFDatabar *dataBar = dynamic_cast<CFDatabar*>(rgbCT.get());

				CP_XML_NODE(L"dataBar")
				{
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

			std::wstring s1 = rgce1.getAssembledFormula();
			std::wstring s2 = rgce1.getAssembledFormula();
			
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

