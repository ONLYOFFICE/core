#pragma once

#include "Types.h"
#include "Reader.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Вспомогательные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	static unsigned int JPT_ReadVBASInfo(CReader *pStream, unsigned int nValue)
	{
		unsigned char nElement = pStream->Read(1);

		while ((nElement >> 7) == 1)
		{
			nValue = (nValue << 7);
			nValue |= (nElement & 0x7f);
			nElement = pStream->Read(1);
		}
		nValue = (nValue << 7);
		nValue |= (nElement & 0x7f);

		return nValue;
	}


	static void         JPT_ReInitMessageHeader(JPTMessageHeader *pHeader)
	{
		pHeader->nId            = 0;
		pHeader->nLastByte      = 0;
		pHeader->nMessageOffset = 0;
		pHeader->nMessageLength = 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// Основные функции
	//-------------------------------------------------------------------------------------------------------------------------------
	void JPT_InitMessageHeader(JPTMessageHeader *pHeader)
	{
		pHeader->nId            = 0;
		pHeader->nLastByte      = 0;
		pHeader->nClassId       = 0;
		pHeader->nCSnId         = 0;
		pHeader->nMessageOffset = 0;
		pHeader->nMessageLength = 0;
		pHeader->nLayerNB       = 0;
	}

	void JPT_ReadMessageHeader(PCommon pInfo, CReader *pStream, JPTMessageHeader *pHeader)
	{
		JPT_ReInitMessageHeader(pHeader);

		// VBAS : Bin-ID
		unsigned char nElement = pStream->Read(1);

		// See for Class and CSn
		unsigned char nClass = 0, nCSn = 0;
		switch ((nElement >> 5) & 0x03)
		{
			case 0:
			Event_Message(EVT_ERROR, "Forbidden value encounter in message header !!\n");
			break;
			case 1:
			nClass = 0;
			nCSn   = 0;
			break;
			case 2:
			nClass = 1;
			nCSn   = 0;
			break;
			case 3:
			nClass = 1;
			nCSn   = 1;
			break;
			default:
			break;
		}

		// [A.2.1 ISO/IEC FCD 15444-9]
		if (((nElement >> 4) & 0x01) == 1)
			pHeader->nLastByte = 1;

		// In-class identifier
		pHeader->nId |= (nElement & 0x0f);
		if ((nElement >> 7) == 1)
			pHeader->nId = JPT_ReadVBASInfo(pStream, pHeader->nId);

		// VBAS : Class
		if (nClass == 1)
		{
			pHeader->nClassId = 0;
			pHeader->nClassId = JPT_ReadVBASInfo(pStream, pHeader->nClassId);
		}

		// VBAS : CSn
		if (nCSn == 1)
		{
			pHeader->nCSnId = 0;
			pHeader->nCSnId = JPT_ReadVBASInfo(pStream, pHeader->nCSnId);
		}

		// VBAS : Msg_offset
		pHeader->nMessageOffset = JPT_ReadVBASInfo(pStream, pHeader->nMessageOffset);

		// VBAS : Msg_length
		pHeader->nMessageLength = JPT_ReadVBASInfo(pStream, pHeader->nMessageLength);

		// VBAS : Aux
		if ((pHeader->nClassId & 0x01) == 1)
		{
			pHeader->nLayerNB = 0;
			pHeader->nLayerNB = JPT_ReadVBASInfo(pStream, pHeader->nLayerNB);
		}
	}
}