#include "precompiled_xls.h"
#include "FontIndex.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FontIndex::clone()
{
	return BiffStructurePtr(new FontIndex(*this));
}


//void FontIndex::toXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	parent->setAttribute(attrib_name, getValue());
//}
//
//
//const bool FontIndex::fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const std::wstring & attrib_name)
//{
//	unsigned short index = getStructAttribute(xml_tag, attrib_name);
//	if(index >= 4)
//	{
//		++index;
//	}
//	val = index;
//	return true;
//}

FontIndex::operator const _variant_t () const
{
	return getValue();
}

const unsigned short FontIndex::getValue() const
{	
	unsigned short index = static_cast<unsigned short>(val.get_value_or(0));
	return index < 4 ? index : index - 1;
}


} // namespace XLS

