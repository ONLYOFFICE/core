﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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

