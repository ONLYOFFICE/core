#include "precompiled_xls.h"
#include "RPHSSub.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr RPHSSub::clone()
{
	return BiffStructurePtr(new RPHSSub(*this));
}


//void RPHSSub::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"crun", crun);
//	xml_tag->setAttribute(L"st", st);
//}
//
//void RPHSSub::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	crun = getStructAttribute(xml_tag, L"crun");
//	st = static_cast<std::wstring >(getStructAttribute(xml_tag, L"st"));
//}


void RPHSSub::store(CFRecord& record)
{
	unsigned __int16 cch = st.getSize();
	record << crun << cch << st;
}


void RPHSSub::load(CFRecord& record)
{
	unsigned __int16 cch;
	record >> crun >> cch >> st;
}


const size_t RPHSSub::getSize() const
{
	return sizeof(unsigned __int16)/*crun*/ + sizeof(unsigned __int16)/*cch*/ + st.getStructSizeWouldWritten();
}

const unsigned __int16 RPHSSub::getRunsNumber() const
{
	return crun;
}


} // namespace XLS

