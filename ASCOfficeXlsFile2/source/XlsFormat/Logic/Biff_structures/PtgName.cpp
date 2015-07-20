
#include "PtgName.h"
#include "RevNameTabid.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgName::PtgName()
{
}


PtgName::PtgName(const unsigned int index, const PtgDataType data_type)
:	nameindex(index),
	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
}


BiffStructurePtr PtgName::clone()
{
	return BiffStructurePtr(new PtgName(*this));
}


//void PtgName::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"nameindex", nameindex);
//}


void PtgName::storeFields(CFRecord& record)
{
	record << nameindex;
}


void PtgName::loadFields(CFRecord& record)
{
	record >> nameindex;
}


void PtgName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	RevNameTabidPtr tab_id;
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevNameTabid>(extra_data.front())))
	{
#pragma message("####################### PtgName struct for revisions is not checked")
		Log::info("PtgName struct for revisions is not checked.");
		ptg_stack.push(tab_id->toString());
		extra_data.pop();
		return;
	}

	//std::wstring query = L"root/WorkbookStreamObject/GlobalsSubstream/LBL[position() = " + 
	//	STR::int2wstr(nameindex, 10) + L"]/Lbl/@Name";
	//MSXML2::IXMLDOMNodePtr node = parent->GetownerDocument()->selectSingleNode(query.c_str());

	//if(node)
	//{
	//	ptg_stack.push(static_cast<wchar_t*>(node->Gettext()));
	//}
	//else
	//{
	//	ptg_stack.push(L"#UNDEFINED_NAME(" + STR::int2wstr(nameindex) + L")!");
	//}
}


} // namespace XLS

