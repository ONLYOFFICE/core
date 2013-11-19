#include "stdafx.h"
#include "DXFNumUsr.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


DXFNumUsr::DXFNumUsr()
{
}


BiffStructurePtr DXFNumUsr::clone()
{
	return BiffStructurePtr(new DXFNumUsr(*this));
}


void DXFNumUsr::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	parent->setAttribute(L"fmt", fmt);
}


const bool DXFNumUsr::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
	fmt = static_cast<_bstr_t>(getStructAttribute(parent, L"fmt"));
	return true;
}


void DXFNumUsr::store(CFRecord& record)
{
	size_t start_ptr = record.getDataSize();
	record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*cb*/, rt_STRUCT_DXFNumUsr);
	record << fmt;
	record.registerDelayedDataSource(record.getDataSize() - start_ptr, rt_STRUCT_DXFNumUsr);

}


void DXFNumUsr::load(CFRecord& record)
{
	record >> cb >> fmt;
}


} // namespace XLS

