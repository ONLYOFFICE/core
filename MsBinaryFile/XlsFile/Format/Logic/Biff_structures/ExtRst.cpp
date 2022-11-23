/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
		//ОШИБКА - нехватило Continue records - нужно найти место где именно и подзагрузить
		if ((data_end - data_start)  < cb )
			record.skipNunBytes( cb - (data_end - data_start)); // trash for unknown reason
		else
			record.RollRdPtrBack((data_end - data_start) - cb);
	}

}


const size_t ExtRst::getSize() const
{
	return 2/*reserved*/ + sizeof(unsigned short) /*cb*/ + sizeof(unsigned short) * 2/*phs*/ + rphssub.getSize() + (rgphruns.size() ? rgphruns.size() * sizeof(short) * 3 : 0);
}


} // namespace XLS
