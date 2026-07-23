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

#include "Tbkd.h"

namespace DocFileFormat
{
	Tbkd::Tbkd() : ipgd(0), itxbxs(0), dcpDepend(0), icol(0), fTableBreak(false), fColumnBreak(false), fMarked(false), fUnk(false), fTextOverflow(false)
	{
	}
	Tbkd::~Tbkd()
	{
	}

	ByteStructure* Tbkd::ConstructObject (VirtualStreamReader* reader, int length)
	{
		Tbkd* pTbkd				=	new Tbkd();

		pTbkd->ipgd				=	reader->ReadInt16();
		pTbkd->itxbxs			=	pTbkd->ipgd;
		pTbkd->dcpDepend		=	reader->ReadInt16();

		int flag				=	(int)reader->ReadInt16();

		pTbkd->icol				=	(unsigned short)FormatUtils::BitmaskToInt(flag, 0x00FF);
		pTbkd->fTableBreak		=	FormatUtils::BitmaskToBool(flag, 0x0100);
		pTbkd->fColumnBreak		=	FormatUtils::BitmaskToBool(flag, 0x0200);
		pTbkd->fMarked			=	FormatUtils::BitmaskToBool(flag, 0x0400);
		pTbkd->fUnk				=	FormatUtils::BitmaskToBool(flag, 0x0800);
		pTbkd->fTextOverflow	=	FormatUtils::BitmaskToBool(flag, 0x1000);

		return static_cast<ByteStructure*>(pTbkd);
	}

	FTXBXS::FTXBXS()
	{
	}
	FTXBXS::~FTXBXS()
	{
	}

	ByteStructure* FTXBXS::ConstructObject (VirtualStreamReader* reader, int length)
	{
		FTXBXS* pFTXBXS	=	new FTXBXS();
		if (!pFTXBXS) return NULL;

		pFTXBXS->reusable01		=	reader->ReadInt32();
		pFTXBXS->reusable02		=	reader->ReadInt32();

		pFTXBXS->fReusable		=	reader->ReadInt16();

		pFTXBXS->itxbxsDest		=	reader->ReadInt32();
		pFTXBXS->lid			=	reader->ReadInt32();
		pFTXBXS->txidUndo		=	reader->ReadInt32();

		return static_cast<ByteStructure*>(pFTXBXS);
	}
}
