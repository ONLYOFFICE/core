
#include "CF12.h"
#include <Logic/Biff_structures/CFMultistate.h>
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
#pragma message("####################### CF12 record is not implemented")
	Log::error("CF12 record is not implemented.");
	//record << some_value;
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
#pragma message("####################### CFGradient structure is not implemented")
			Log::info("CFGradient structure is not implemented.");
			//rgbCT = BiffStructurePtr(new CFGradient);
			//rgbCT->load(record);
			break;
		case 0x04:
#pragma message("####################### CFDatabar structure is not implemented")
			Log::info("CFDatabar structure is not implemented.");
			//rgbCT = BiffStructurePtr(new CFDatabar);
			//rgbCT->load(record);
			break;
		case 0x05:
#pragma message("####################### CFFilter structure is not implemented")
			Log::info("CFFilter structure is not implemented.");
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
			CP_XML_ATTR(L"priority", ipriority_);
			CP_XML_ATTR(L"stopIfTrue", fStopIfTrue);

			CP_XML_ATTR(L"dxfId", dxfId_);	

			std::wstring s1 = rgce1.getAssembledFormula();
			std::wstring s2 = rgce1.getAssembledFormula();
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

