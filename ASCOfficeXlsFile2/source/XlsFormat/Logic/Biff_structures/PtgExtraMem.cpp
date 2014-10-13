#include "precompiled_xls.h"
#include "PtgExtraMem.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgExtraMem::PtgExtraMem()
{
}


PtgExtraMem::PtgExtraMem(CFRecord& record)
{
	load(record);
}


BiffStructurePtr PtgExtraMem::clone()
{
	return BiffStructurePtr(new PtgExtraMem(*this));
}


//void PtgExtraMem::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(array_.begin(), array_.end(), boost::bind(&Ref8U::toXML, _1, xml_tag));
//}


void PtgExtraMem::store(CFRecord& record)
{
#pragma message("####################### PtgExtraMem record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgExtraMem record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgExtraMem::load(CFRecord& record)
{
	unsigned __int16 count;
	record >> count;
	for(int i = 0; i < count; ++i)
	{
		Ref8U ref8;
		record >> ref8;
		array_.push_back(ref8);
	}
}


const std::wstring PtgExtraMem::toString() const
{
	std::wstring range;
	for(std::vector<Ref8U>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		range += it->toString() + L';';
	}
	return range + array_.back().toString();
}


} // namespace XLS

