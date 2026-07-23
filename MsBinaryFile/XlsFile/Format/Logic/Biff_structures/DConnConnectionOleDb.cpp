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
#include "DConnConnectionOleDb.h"
#include "BiffString.h"
#include "../Biff_records/SXStreamID.h"

namespace XLS
{

BiffStructurePtr DConnConnectionOleDb::clone()
{
	return BiffStructurePtr(new DConnConnectionOleDb(*this));
}

DConnConnectionOleDb::DConnConnectionOleDb()
{		
}

DConnConnectionOleDb::~DConnConnectionOleDb()
{
}

void DConnConnectionOleDb::load(CFRecord& record)
{
	record 	>> nDrillthroughRows >> cOleDb;
	
	for (unsigned short i = 0; i < cOleDb; i++)
	{
		unsigned short val;
		record >> val;
		rgIOleDbValid.push_back(val);
	}
	int skip_unused =  2 * (4 - cOleDb);
	
	record.skipNunBytes(skip_unused);
	
	record.skipNunBytes(2);

	for (unsigned short i = 0; i < cOleDb; i++)
	{
		XLUnicodeStringSegmented val;
		record >> val;
		rgConn.push_back(val);
	}
}

void DConnConnectionOleDb::save(CFRecord& record)
{
	record 	<< nDrillthroughRows << cOleDb;
	for(auto i : rgIOleDbValid)
		record << i;
	if(cOleDb <= 4)
		record.skipNunBytes(2 * (4 - cOleDb));
	record.reserveNunBytes(2);
	for(auto i : rgConn)
		record << i;
}


} // namespace XLS

