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

#include "Cell.h"
#include "CellRef.h"

namespace XLS
{


Cell::Cell()
{
}


Cell::Cell(const unsigned short rw_init, const unsigned short col_init, const unsigned short ixfe_init)
{
	rw = rw_init;
	col = col_init;
	ixfe = ixfe_init;
}


BiffStructurePtr Cell::clone()
{
	return BiffStructurePtr(new Cell(*this));
}

void Cell::load(CFRecord& record)
{
	record >> rw >> col;

	if (record.getGlobalWorkbookInfo()->Version == 0x0200)
	{
		unsigned char flags1, flags2, flags3;

		record >> flags1 >> flags2 >> flags3;

		ixfe = GETBITS(flags1, 0, 5);
		
		bool fLocked = GETBIT(flags1, 6);
		bool fHidden = GETBIT(flags1, 7);

		short ifmt_index = GETBITS(flags2, 0, 5);
		short ifnt_index = GETBITS(flags2, 6, 7);		

		short alc = GETBITS(flags3, 0, 2);

		short border_dgLeft = GETBIT(flags3, 3) ? 1 : 0;
		short border_dgRight = GETBIT(flags3, 4) ? 1 : 0;
		short border_dgTop = GETBIT(flags3, 5) ? 1 : 0;
		short border_dgBottom = GETBIT(flags3, 6) ? 1 : 0;
	}
	else
	{
		record >> ixfe;
	}
}


const CellRef Cell::getLocation() const
{
	return RgceLoc(rw, col, true, true);
}


} // namespace XLS
