#pragma once

#include "BiffAttribute.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class FontIndex : public BiffAttributeSimple<WORD>
{
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name);
	operator const _variant_t () const;
	const WORD getValue() const;
};

} // namespace XLS

