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
			char sMessage[MSG_SIZE]={};
			va_list sArg;

			memset(sMessage, 0, MSG_SIZE);
			va_start(sArg, sFormatString);
			int nLength = (strlen(sFormatString) > MSG_SIZE) ? MSG_SIZE : strlen(sFormatString);
			vsprintf(sMessage, sFormatString, sArg);
			va_end(sArg);

			// TODO: print sMessage/*, pCodecInfo->pClientData*/
		}

		return true;
	}

}