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

#include "DBQueryExt.h"
#include "../Biff_structures/ConnGrbitDbtWeb.h"
#include "../Biff_structures/ConnGrbitDbtOledb.h"
#include "../Biff_structures/ConnGrbitDbtAdo.h"

namespace XLS
{

DBQueryExt::DBQueryExt()
{
}


DBQueryExt::~DBQueryExt()
{
}


BaseObjectPtr DBQueryExt::clone()
{
	return BaseObjectPtr(new DBQueryExt(*this));
}

void DBQueryExt::readFields(CFRecord& record)
{
	unsigned short	flags1, flags2;
	record >> frtHeaderOld >> dbt >> flags1;		
	
	fMaintain			= GETBIT(flags1, 0);
	fNewQuery			= GETBIT(flags1, 1);
	fImportXmlSource	= GETBIT(flags1, 2);
	fSPListSrc			= GETBIT(flags1, 3);
	fSPListReinitCache	= GETBIT(flags1, 4);
	fSrcIsXml			= GETBIT(flags1, 7);

	switch(dbt)
	{
		case 4:	grbitDbt.reset(new ConnGrbitDbtWeb);	break;
		case 5:	grbitDbt.reset(new ConnGrbitDbtOledb);	break;
		case 7:	grbitDbt.reset(new ConnGrbitDbtAdo);	break;
		default:
			record.skipNunBytes(2); break;	//unused
			
	}
	if (grbitDbt)
	{
		grbitDbt->load(record);
	}

	record >> flags2 >> bVerDbqueryEdit >> bVerDbqueryRefreshed >> bVerDbqueryRefreshableMin;
	
	fTxtWiz			= GETBIT(flags2, 0);
	fTableNames		= GETBIT(flags2, 1);

	record.skipNunBytes(3); //unused

	record >> coledb >> cstFuture >> wRefreshInterval >> wHtmlFmt >> cwParamFlags;

	PBT val1;
	for (unsigned short i = 0; i < cwParamFlags; i++)
	{
		record >> val1;
		rgPbt.push_back(val1);
	}

	char val2;
	for (unsigned short i = 0; i < cstFuture; i++)
	{
		record >> val2;
		rgbFutureBytes += val2;
	}
}

void DBQueryExt::writeFields(CFRecord& record)
{
	unsigned short	flags1 = 0, flags2 = 0;
	frtHeaderOld.rt = rt_DBQueryExt;
	record << frtHeaderOld << dbt << flags1;

	SETBIT(flags1, 0, fMaintain)
	SETBIT(flags1, 1, fNewQuery)
	SETBIT(flags1, 2, fImportXmlSource)
	SETBIT(flags1, 3, fSPListSrc)
	SETBIT(flags1, 4, fSPListReinitCache)
	SETBIT(flags1, 7, fSrcIsXml)

	record << frtHeaderOld << dbt << flags1;
	if(dbt == 4 || dbt == 5 || dbt == 7)
	{
		grbitDbt->save(record);
	}
	else
		record.reserveNunBytes(2);

	SETBIT(flags2, 0, fTxtWiz)
	SETBIT(flags2, 1, fTableNames)
	record << flags2 << bVerDbqueryEdit << bVerDbqueryRefreshed << bVerDbqueryRefreshableMin;
	record.reserveNunBytes(3);
	record << coledb << cstFuture << wRefreshInterval << wHtmlFmt << cwParamFlags;
	for(auto i : rgPbt)
		i.save(record);
	record.reserveNunBytes(cstFuture);
}

} // namespace XLS

