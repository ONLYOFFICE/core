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

#include "ExtNameParsedFormula.h"
#include "ExtPtgRef3D.h"
#include "ExtPtgArea3D.h"
#include "ExtPtgRefErr3D.h"
#include "ExtPtgAreaErr3D.h"
#include "ExtPtgErr.h"

namespace XLS
{


ExtNameParsedFormula::ExtNameParsedFormula() :	ParsedFormula(CellRef())
{
}


BiffStructurePtr ExtNameParsedFormula::clone()
{
	return BiffStructurePtr(new ExtNameParsedFormula(*this));
}

void ExtNameParsedFormula::load(CFRecord& record) // Maybe this class shouldn't be a ParsedFormulas's derived?
{
    _UINT32 cb;
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        unsigned short cb_2b;
        record >> cb_2b;
        cb = cb_2b;
    }
    else
    {
        record >> cb;
    }
	if(cb && record.getDataSize() > record.getRdPtr())
	{
		unsigned char extPtg;
		record >> extPtg;
		switch(extPtg)
		{
			case 0x3A:
				val = PtgPtr(new ExtPtgRef3D(CellRef()));
				break;
			case 0x3B:
				val = PtgPtr(new ExtPtgArea3D(CellRef()));
				break;
			case 0x3C:
				val = PtgPtr(new ExtPtgRefErr3D);
				break;
			case 0x3D:
				val = PtgPtr(new ExtPtgAreaErr3D);
				break;
			case 0x1C:
				val = PtgPtr(new ExtPtgErr);
				break;
		}
		val->load(record);
		rgce.addPtg(val);
	}
	if (record.getDataSize() > record.getRdPtr())
	{
		int sz = record.getDataSize() - record.getRdPtr();
		if (sz < 100)
		{
			char *buf = new char[sz];
			memcpy(buf, record.getCurData<char>(), sz);

			record.skipNunBytes(sz);
			delete []buf;
		}
	}
}


} // namespace XLS

