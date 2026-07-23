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

#include "SXDB.h"

namespace XLS
{

SXDB::SXDB()
{
}


SXDB::~SXDB()
{
}

BaseObjectPtr SXDB::clone()
{
	return BaseObjectPtr(new SXDB(*this));
}

void SXDB::readFields(CFRecord& record)
{
	unsigned short	flags, unused2;
	record >> crdbdb >> idstm >> flags >> unused2 >> cfdbdb >> cfdbTot >> crdbUsed >> vsType >> cchWho;	
	
	if (cchWho > 0 && cchWho < 0xffff)
	{
		rgb.setSize(cchWho);
		record >> rgb;
	}
	fSaveData			= GETBIT(flags, 0);
	fInvalid			= GETBIT(flags, 1);
	fRefreshOnLoad		= GETBIT(flags, 2);
	fOptimizeCache		= GETBIT(flags, 3);
	fBackgroundQuery	= GETBIT(flags, 4);
	fEnableRefresh		= GETBIT(flags, 5);
}

void SXDB::writeFields(CFRecord& record)
{
	cchWho = rgb.getSize();
	if(!cchWho)
		cchWho = 0xFFFF;
	unsigned short	flags = 0;
	SETBIT(flags, 0, fSaveData)
	SETBIT(flags, 1, fInvalid)
	SETBIT(flags, 2, fRefreshOnLoad)
	SETBIT(flags, 3, fOptimizeCache)
	SETBIT(flags, 4, fBackgroundQuery)
	SETBIT(flags, 5, fEnableRefresh)
	record << crdbdb << idstm << flags;
	record.reserveNunBytes(2);
	record << cfdbdb << cfdbTot << crdbUsed << vsType << cchWho;

	if (cchWho > 0 && cchWho < 0xffff)
	{
		record << rgb;
	}
}

} // namespace XLS

