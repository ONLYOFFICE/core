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

#include "UserSViewBegin.h"
#include "../Biff_structures/CellRangeRef.h"

namespace XLS
{

UserSViewBegin::UserSViewBegin()
{
}


UserSViewBegin::~UserSViewBegin()
{
}

BaseObjectPtr UserSViewBegin::clone()
{
	return BaseObjectPtr(new UserSViewBegin(*this));
}

void UserSViewBegin::readFields(CFRecord& record)
{
	_GUID_ guid_num;
	record >> guid_num >> iTabid;
	
	guid = STR::guid2bstr(guid_num);
	
	record.skipNunBytes(2); // reserved
	record >> wScale >> icvHdr;
	record.skipNunBytes(2); // reserved
	record >> pnnSel;
	record.skipNunBytes(3); // reserved
	
	unsigned short flags;
	record >> flags;

	fShowBrks	= GETBIT(flags, 0);
	fDspFmlaSv	= GETBIT(flags, 1);
	fDspGridSv	= GETBIT(flags, 2);
	fDspRwColSv = GETBIT(flags, 3);
	fDspGutsSv	= GETBIT(flags, 4);
	fDspZerosSv = GETBIT(flags, 5);
	fHorizontal = GETBIT(flags, 6);
	fVertical	= GETBIT(flags, 7);
	fPrintRwCol = GETBIT(flags, 8);
	fPrintGrid	= GETBIT(flags, 9);
	fFitToPage	= GETBIT(flags, 10);
	fPrintArea	= GETBIT(flags, 11);
	fOnePrintArea = GETBIT(flags, 12);
	fFilterMode = GETBIT(flags, 13);
	fEzFilter	= GETBIT(flags, 14);
	fFrozen		= GETBIT(flags, 15);

	record >> flags;

	Ref8U top_left;
	record >> top_left;
	ref8TopLeft = static_cast<std::wstring >(top_left.getTopLeftCell());

	fFrozenNoSplit	= GETBIT(flags, 0);
	fSplitV			= GETBIT(flags, 1);
	fSplitH			= GETBIT(flags, 2);
	fHiddenRw		= GETBIT(flags, 3);
	fHiddenCol		= GETBIT(flags, 5);
	fFilterUnique	= GETBIT(flags, 9);
	fSheetLayoutView = GETBIT(flags, 10);
	fPageLayoutView = GETBIT(flags, 11);
	fRuler			= GETBIT(flags, 13);

    record >> operNumX >> operNumY >> colRPane >> rwBPane;

    pane_top_left_cell = CellRef(fSplitH ? (int)rwBPane : 0 , fSplitV ? (int)colRPane : 0, true, true).toString();
}

} // namespace XLS

