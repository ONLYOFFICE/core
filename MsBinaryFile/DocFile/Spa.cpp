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
#include "Spa.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{


	Spa::Spa() : spid(0), fHdr(0), bx(MARGIN), by(MARGIN), wr(0), wrk(0), 
			xaLeft(0), yaTop(0), xaRight(0), yaBottom(0), 				
			fRcaSimple(0), fBelowText(0), fAnchorLock(0), cTxbx(0)
	{

	}

	Spa::~Spa()
	{
		for (int i = 0 ; i < primitives.size(); i++)
		{
			if (primitives[i]) delete primitives[i];
			primitives[i] = NULL;
		}
	}

	void Spa::read_primitives(Spa* pSpa, VirtualStreamReader* reader, int length)
	{
		if (!pSpa) return;
		if (!reader || length < 1) return;

		int stream_pos = reader->GetPosition();
		int pos = 0;
		while (pos < length)
		{
			reader->Seek(stream_pos + pos, 0);

			unsigned short dpk	= reader->ReadUInt16();
			
			int shape_type = FormatUtils::BitmaskToInt (dpk, 0x00ff);
			
			short cb_primitive	= reader->ReadInt16();

			if ((cb_primitive < 1) || (pos + cb_primitive > length))
			{
				reader->Seek(reader->GetPosition() - 4, 0);
				break;
			}

			DrawingPrimitive *dp = DrawingPrimitive::Create(reader, cb_primitive - 4, shape_type);
			if 	(dp) 
				pSpa->primitives.push_back(dp);
			
			if (shape_type == 0x00)
			{
				//start group
				read_primitives(pSpa, reader, cb_primitive - 4);
			}
			else if (shape_type == 0x08)
			{
				//end group
				break;
			}

			pos += cb_primitive;
		}

	}

	ByteStructure* Spa::ConstructObject(VirtualStreamReader* reader, int length)
	{
		Spa* pSpa =	new Spa();  
		if (!pSpa) return NULL;
		
		if (reader->nWordVersion > 0)
		{
			int fc				=	reader->ReadInt32();
			pSpa->cTxbx			=	reader->ReadUInt16();

			int pos_keep = reader->GetPosition();

			reader->Seek(fc, 0);

			unsigned short dok = reader->ReadUInt16();			
			short cb = reader->ReadInt16();

			pSpa->bx			=	(AnchorType)reader->ReadByte();
			pSpa->by			=	(AnchorType)reader->ReadByte();
			short height		=	reader->ReadInt16();
			pSpa->fAnchorLock	=	reader->ReadUInt16();

			int pos = 10; //fixed size
			
			read_primitives(pSpa, reader, cb - 10/*_primitive*/);

			reader->Seek(pos_keep, 0);
		}
		else
		{
			pSpa->spid			=	reader->ReadInt32();
			pSpa->xaLeft		=	reader->ReadInt32();
			pSpa->yaTop			=	reader->ReadInt32();
			pSpa->xaRight		=	reader->ReadInt32();
			pSpa->yaBottom		=	reader->ReadInt32();

			unsigned short flag	=	reader->ReadUInt16();

			pSpa->fHdr			=	FormatUtils::BitmaskToBool(flag, 0x0001);
			pSpa->bx			=	(AnchorType)FormatUtils::BitmaskToInt(flag, 0x0006);
			pSpa->by			=	(AnchorType)FormatUtils::BitmaskToInt(flag, 0x0018);
			pSpa->wr			=	(unsigned short)FormatUtils::BitmaskToInt(flag, 0x01E0);
			pSpa->wrk			=	(unsigned short)FormatUtils::BitmaskToInt(flag, 0x1E00);
			pSpa->fRcaSimple	=	FormatUtils::BitmaskToBool(flag, 0x2000);
			pSpa->fBelowText	=	FormatUtils::BitmaskToBool(flag, 0x4000);
			pSpa->fAnchorLock	=	FormatUtils::BitmaskToBool(flag, 0x8000);

			pSpa->cTxbx			=	reader->ReadInt32();
		}

		return static_cast<ByteStructure*>(pSpa);
	}



}
