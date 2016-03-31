
#include "CF.h"
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
			CP_XML_ATTR(L"stopIfTrue", 1);
			CP_XML_ATTR(L"dxfId", dxfId_);	

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

