#include "stdafx.h"
#include "FontIndex.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FontIndex::clone()
{
	return BiffStructurePtr(new FontIndex(*this));
}


void FontIndex::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	parent->setAttribute(attrib_name, getValue());
}


const bool FontIndex::fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name)
{
	WORD index = getStructAttribute(xml_tag, attrib_name);
	if(index >= 4)
	{
		++index;
	}
	val = index;
	return true;
}

FontIndex::operator const _variant_t () const
{
	return getValue();
}

const WORD FontIndex::getValue() const
{	
	WORD index = static_cast<WORD>(val);
	return index < 4 ? index : index - 1;
}


} // namespace XLS

