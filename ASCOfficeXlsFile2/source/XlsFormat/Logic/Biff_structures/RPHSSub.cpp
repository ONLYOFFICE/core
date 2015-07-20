
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
	unsigned short cch = st.getSize();
	record << crun << cch << st;
}


void RPHSSub::load(CFRecord& record)
{
	unsigned short cch;
	record >> crun >> cch >> st;
}


const size_t RPHSSub::getSize() const
{
	return sizeof(unsigned short)/*crun*/ + sizeof(unsigned short)/*cch*/ + st.getStructSizeWouldWritten();
}

const unsigned short RPHSSub::getRunsNumber() const
{
	return crun;
}


} // namespace XLS

