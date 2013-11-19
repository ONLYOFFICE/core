#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class HideObjEnum : public BiffAttributeSimple<WORD>
{
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);

	enum
	{
		SHOWALL = 0x0000,
		SHOWPLACEHOLDER = 0x0001,
		HIDEALL = 0x0002
	};
};

} // namespace XLS

