#pragma once

#include "BiffAttribute.h"

namespace XLS
{;

class PaneType : public BiffAttributeSimple<BYTE>
{
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);

	enum
	{
		REVTPNNBOTRIGHT = 0x00,	// logical bottom-right pane
		REVTPNNTOPRIGHT = 0x01,	// logical top-right pane
		REVTPNNBOTLEFT = 0x02,	// logical bottom-left pane
		REVTPNNTOPLEFT = 0x03	// logical top-left pane
	};
};

} // namespace XLS

