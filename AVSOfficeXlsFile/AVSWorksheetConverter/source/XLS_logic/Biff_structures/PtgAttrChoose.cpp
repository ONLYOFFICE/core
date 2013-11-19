#include "stdafx.h"
#include "PtgAttrChoose.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrChoose::clone()
{
	return BiffStructurePtr(new PtgAttrChoose(*this));
}


void PtgAttrChoose::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"cOffset", cOffset);

	for(size_t i = 0; i < rgOffset.size(); ++i)
	{
		MSXML2::IXMLDOMElementPtr own_tag2 = XMLSTUFF::makeXMLNode(L"offset", xml_tag);
		own_tag2->Puttext(STR::int2str(rgOffset[i], 10).c_str());
	}
}


void PtgAttrChoose::storeFields(CFRecord& record)
{
#pragma message("####################### PtgAttrChoose record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgAttrChoose record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgAttrChoose::loadFields(CFRecord& record)
{
	record >> cOffset;
	for(int i = 0; i < cOffset + 1; ++i)
	{
		WORD val;
		record >> val;
		rgOffset.push_back(val);
	}
}


void PtgAttrChoose::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	// All control tokens shall be skipped
}


} // namespace XLS

