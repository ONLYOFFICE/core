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

#include "DVParsedFormula.h"

namespace XLS
{

DVParsedFormula::DVParsedFormula() : cce(0), ParsedFormula(CellRef())
{
}

DVParsedFormula& DVParsedFormula::operator=(const std::wstring& value)
{
	ParsedFormula::operator = (value);
	return *this;
}

BiffStructurePtr DVParsedFormula::clone()
{
	return BiffStructurePtr(new DVParsedFormula(*this));
}

void DVParsedFormula::load(CFRecord& record, bool bLoad)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 cce_;
        record >> cce_;
        cce = cce_;
        record.skipNunBytes(2); // unused
    }
    else
    {
        record >> cce;
    }

	if (bLoad) 
		return load(record);
	else
	{
		cce = 0;
	}
}
	
void DVParsedFormula::load(CFRecord& record)
{	
    rgce.load(record, cce);

    if (record.getGlobalWorkbookInfo()->Version == 0x0800)
    {
        _UINT32 cb;
        record >> cb;
        record.skipNunBytes(cb);

        //rgcb.load(record, rgce.getPtgs(), true);
    }
}

void DVParsedFormula::save(CFRecord& record, bool bSave)
{
	if (bSave)
		return save(record);
	else
	{
		cce = 0;
		record << cce;
	}
}

void DVParsedFormula::save(CFRecord& record)
{
	auto saving = [&](BiffStructure& rgceORrgb)
	{
		record << cce;

		auto rdPtr = record.getRdPtr();

		rgceORrgb.save(record);

		cce = record.getRdPtr() - rdPtr;

		record.RollRdPtrBack(cce + 4);
		record << cce;
		record.skipNunBytes(cce);
	};

	saving(rgce);

	if (record.getGlobalWorkbookInfo()->Version == 0x0800)
	{
		saving(rgcb);
	}
}



} // namespace XLS

