#pragma once

#include "BiffAttribute.h"

namespace XLS
{

class PaneType : public BiffAttributeSimple<unsigned char>
{
public:
	BiffStructurePtr clone();

	////virtual void toXML(BiffStructurePtr & parent, const std::wstring & attrib_name);
	////virtual const bool fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name);

	enum
	{
		REVTPNNBOTRIGHT = 0x00,	// logical bottom-right pane
		REVTPNNTOPRIGHT = 0x01,	// logical top-right pane
		REVTPNNBOTLEFT = 0x02,	// logical bottom-left pane
		REVTPNNTOPLEFT = 0x03	// logical top-left pane
	};
};

} // namespace XLS

