/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#pragma once

#include "Types.h"
#include "Reader.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Helper functions
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
	// Main functions
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