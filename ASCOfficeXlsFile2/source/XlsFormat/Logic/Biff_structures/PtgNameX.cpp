
#include "PtgNameX.h"
#include "RevName.h"
#include <Binary/CFRecord.h>
//#include <Exception/WrongIntermediateXMLFormat.h>

namespace XLS
{;


PtgNameX::PtgNameX()
{
}


PtgNameX::PtgNameX(const std::wstring& word, BiffStructurePtr & parent, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
	fromString(word, parent);
}


BiffStructurePtr PtgNameX::clone()
{
	return BiffStructurePtr(new PtgNameX(*this));
}


//void PtgNameX::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ixti", ixti);
//	xml_tag->setAttribute(L"nameindex", nameindex);
//}


void PtgNameX::storeFields(CFRecord& record)
{
	record << ixti << nameindex;
}


void PtgNameX::loadFields(CFRecord& record)
{
	record >> ixti >> nameindex;
	global_info = record.getGlobalWorkbookInfo();
}


void PtgNameX::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	RevNamePtr tab_id;
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevName>(extra_data.front())))
	{
#pragma message("####################### PtgNameX struct for revisions is not implemented")
		Log::error("PtgNameX struct for revisions is not implemented.");
		ptg_stack.push(L"#REF!");
		extra_data.pop();
		return;
	}

	std::wstring udfName;
	if(nameindex > 0 && nameindex <= global_info->AddinUdfs.size() && !(udfName = global_info->AddinUdfs[nameindex - 1]).empty())
	{
		ptg_stack.push(udfName);
	}
	else
	{
#pragma message("####################### PtgNameX struct is not implemented")
 		Log::warning("PtgNameX structure is not implemented.");
		//ptg_stack.push(L"#UNDEFINED_EXTERN_NAME(" + STR::int2wstr(nameindex) + L")!");
		ptg_stack.push(L""); // This would let us to continue without an error
	}


	// Example of result: "[1]!range"
	// in the formula window it looks like: "'D:\Projects\AVSWorksheetConverter\bin\InFiles\Blank2003_range.xls'!range"
}


void PtgNameX::fromString(const std::wstring& word, BiffStructurePtr & parent)
{
 	std::wstring  query = L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK";
	
//	MSXML2::IXMLDOMNodeListPtr nodes = parent->GetownerDocument()->selectNodes(query);
//	MSXML2::IXMLDOMElementPtr supbook;
//	size_t scounter = 0;
//	while(supbook = nodes->nextNode())
//	{
//		MSXML2::IXMLDOMNodeListPtr names = supbook->selectNodes(L"ExternName");
//		MSXML2::IXMLDOMElementPtr name;
//		size_t ncounter = 1;
//		while(name = names->nextNode())
//		{
//			MSXML2::IXMLDOMElementPtr addinf = name->selectSingleNode((L"AddinUdf[@udfName = '" + word + L"']").c_str());
//			if(addinf)
//			{
//				nameindex = ncounter;
//// 				std::wstring query = std::wstring(L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK/ExternSheet/XTI[@iSupBook = '") + STR::int2wstr(ncounter) + L"']";
//				MSXML2::IXMLDOMNodeListPtr xtis = parent->GetownerDocument()->selectNodes(L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK/ExternSheet/XTI");
//				MSXML2::IXMLDOMElementPtr xti;
//				size_t xcounter = 0;
//				while(xti = xtis->nextNode())
//				{
//					if(_variant_t(std::wstring (L"-2")) == xti->getAttribute(L"itabFirst"))
//					{
//						ixti = xcounter;
//						return;
//					}
//					++xcounter;
//				}
//				throw;// EXCEPT::LE::WrongIntermediateXMLFormat("Necessary 'XTI' tag was not found.", __FUNCTION__);
//			}
//			++ncounter;
//		}
//		++scounter;
//	}	
	throw;// EXCEPT::LE::WrongIntermediateXMLFormat("Necessary 'ExternName' tag was not found.", __FUNCTION__);
}


} // namespace XLS

