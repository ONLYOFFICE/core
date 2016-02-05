#pragma once

#include "BiffAttribute.h"

namespace XLS
{

class PaneType : public BiffAttributeSimple<unsigned char>
{
public:
	BiffStructurePtr clone();

	enum
	{
		REVTPNNBOTRIGHT = 0x00,	// logical bottom-right pane
		REVTPNNTOPRIGHT = 0x01,	// logical top-right pane
		REVTPNNBOTLEFT	= 0x02,	// logical bottom-left pane
		REVTPNNTOPLEFT	= 0x03	// logical top-left pane
	};
};

} // namespace XLS

