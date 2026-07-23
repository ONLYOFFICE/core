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

#include "ExtRst.h"

namespace XLS
{


BiffStructurePtr ExtRst::clone()
{
	return BiffStructurePtr(new ExtRst(*this));
}


ExtRst::ExtRst(std::list<CFRecordPtr>& cont_recs) : cont_recs_(cont_recs)
{
}


void ExtRst::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> cb;
	size_t data_start = record.getRdPtr();
	record >> phs;

	rphssub.sz = cb;

	record >> rphssub;
	for(unsigned short i = 0; i < rphssub.getRunsNumber(); ++i)
	{
		PhRuns run;
		record >> run;
		rgphruns.push_back(run);

	}
	size_t data_end = record.getRdPtr();
	if(data_end - data_start != cb)
	{
		// ERROR - not enough Continue records - need to find the exact location and load more
		if ((data_end - data_start)  < cb )
			record.skipNunBytes( cb - (data_end - data_start)); // trash for unknown reason
		else
			record.RollRdPtrBack((data_end - data_start) - cb);
	}

}

void ExtRst::save(CFRecord& record)
{
    unsigned short reserve1 = 1;
    record << reserve1;
    auto cbPose = record.getRdPtr();
    record.reserveNunBytes(2);
    record << phs;
    auto rphssubStart = record.getRdPtr();
	rphssub.crun = rgphruns.size();
    record << rphssub;
    cb = record.getRdPtr() - rphssubStart;
    record.RollRdPtrBack(record.getRdPtr() - cbPose);
    record << cb;
    record.skipNunBytes((rphssubStart - record.getRdPtr()) + cb);
    for(auto i : rgphruns)
        record << i;
}

const size_t ExtRst::getSize() const
{
	return 2/*reserved*/ + sizeof(unsigned short) /*cb*/ + sizeof(unsigned short) * 2/*phs*/ + rphssub.getSize() + (rgphruns.size() ? rgphruns.size() * sizeof(short) * 3 : 0);
}


} // namespace XLS
