#pragma once

#include "Types.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	//  Events
	//-------------------------------------------------------------------------------------------------------------------------------

	bool   Event_Message(int nEventType, const char *sFormatString, ...)
	{
		if (sFormatString != NULL)
		{
			// TODO: print sMessage/*, pCodecInfo->pClientData*/
		}

		return true;
	}

}