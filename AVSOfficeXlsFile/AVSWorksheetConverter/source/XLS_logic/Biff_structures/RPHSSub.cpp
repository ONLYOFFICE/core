#include "stdafx.h"
#include "RPHSSub.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr RPHSSub::clone()
{
	return BiffStructurePtr(new RPHSSub(*this));
}


void RPHSSub::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"crun", crun);
	xml_tag->setAttribute(L"st", st);
}

void RPHSSub::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	crun = getStructAttribute(xml_tag, L"crun");
	st = static_cast<_bstr_t>(getStructAttribute(xml_tag, L"st"));
}


void RPHSSub::store(CFRecord& record)
{
	WORD cch = st.getSize();
	record << crun << cch << st;
}


void RPHSSub::load(CFRecord& record)
{
	WORD cch;
	record >> crun >> cch >> st;
}


const size_t RPHSSub::getSize() const
{
	return sizeof(WORD)/*crun*/ + sizeof(WORD)/*cch*/ + st.getStructSizeWouldWritten();
}

const WORD RPHSSub::getRunsNumber() const
{
	return crun;
}


} // namespace XLS

