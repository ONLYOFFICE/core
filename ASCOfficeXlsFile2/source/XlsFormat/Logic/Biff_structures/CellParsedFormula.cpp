/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "CellParsedFormula.h"
#include <Binary/CFRecord.h>

namespace XLS
{


CellParsedFormula::CellParsedFormula(const bool is_part_of_a_revision)
:	is_part_of_a_revision_(is_part_of_a_revision),
	ParsedFormula(CellRef())
{
}


BiffStructurePtr CellParsedFormula::clone()
{
	return BiffStructurePtr(new CellParsedFormula(*this));
}


void CellParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Formula);
	rgcb.store(record);
}


void CellParsedFormula::load(CFRecord& record)
{
	unsigned short cce;
	record >> cce;
	
	//if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	//{
	//	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
	//}
	//else
	{
		rgce.load(record, cce);
		rgcb.load(record, rgce.getPtgs(), is_part_of_a_revision_);
	}
}


} // namespace XLS

