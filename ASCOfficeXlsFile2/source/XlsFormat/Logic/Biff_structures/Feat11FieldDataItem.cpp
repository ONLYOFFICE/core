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

#include "Feat11FieldDataItem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Feat11FieldDataItem::clone()
{
	return BiffStructurePtr(new Feat11FieldDataItem(*this));
}



void Feat11FieldDataItem::store(CFRecord& record)
{	
}


void Feat11FieldDataItem::load(CFRecord& record)
{	

	record >> idField;
	record >> lfdt;
	record >> lfxidt;
	record >> ilta;
	record >> cbFmtAgg;
	record >> istnAgg;

	_UINT32 flags;
	record >> flags;

	fAutoFilter = static_cast<unsigned char>(GETBIT(flags, 0));
	fAutoFilterHidden = static_cast<unsigned char>(GETBIT(flags, 1));
	fLoadXmapi = static_cast<unsigned char>(GETBIT(flags, 2));
	fLoadFmla = static_cast<unsigned char>(GETBIT(flags, 3));	
	fLoadTotalFmla = static_cast<unsigned char>(GETBIT(flags, 7));
	fLoadTotalArray = static_cast<unsigned char>(GETBIT(flags, 8));
	fSaveStyleName = static_cast<unsigned char>(GETBIT(flags, 9));
	fLoadTotalStr = static_cast<unsigned char>(GETBIT(flags, 10));
	fAutoCreateCalcCol = static_cast<unsigned char>(GETBIT(flags, 11));

	record >> cbFmtInsertRow;
	record >> istnInsertRow;
	record >> strFieldName;
	record >> strCaption;

	if (record.getRdPtr() >= record.getDataSize())
		return;//125 Planilhas de Excel.xls

	if (cbFmtAgg > 0)
		record >> dxfFmtAgg;
	if (cbFmtInsertRow > 0)
		record >> dxfFmtInsertRow;
	if (fAutoFilter == BIFF_BYTE(1))
		record >> AutoFilter;
}

} // namespace XLS

