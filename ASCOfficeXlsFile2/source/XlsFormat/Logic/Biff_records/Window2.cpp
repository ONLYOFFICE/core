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

#include "Window2.h"

namespace XLS
{

Window2::Window2()
{
	is_contained_in_chart_substream = false;
	
	fSelected		= false;
	fDspFmlaRt		= false;
	fDspGridRt		= false;
	fDspRwColRt		= false;
	fFrozenRt		= false;
	fDspZerosRt		= false;
	fDefaultHdr		= false;
	fRightToLeft	= false;
	fDspGuts		= false;
	fFrozenNoSplit	= false;
	fPaged			= false;
	fSLV			= false;

	wScaleSLV = wScaleNormal = 0;
}


Window2::~Window2()
{
}


BaseObjectPtr Window2::clone()
{
	return BaseObjectPtr(new Window2(*this));
}


void Window2::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 9, fSelected);

	if(is_contained_in_chart_substream)
	{
		record << flags;
		record.reserveNunBytes(8); // must be ignored
		return;
	}

	SETBIT(flags, 0, fDspFmlaRt);
	SETBIT(flags, 1, fDspGridRt);
	SETBIT(flags, 2, fDspRwColRt);
	SETBIT(flags, 3, fFrozenRt);
	SETBIT(flags, 4, fDspZerosRt);
	SETBIT(flags, 5, fDefaultHdr);
	SETBIT(flags, 6, fRightToLeft);
	SETBIT(flags, 7, fDspGuts);
	SETBIT(flags, 8, fFrozenNoSplit);
	SETBIT(flags, 10, fPaged);
	SETBIT(flags, 11, fSLV);
	
	record << flags;

	CellRef topLeftCellRef(topLeftCell);
	rwTop = static_cast<unsigned short>(topLeftCellRef.getRow());
	colLeft = static_cast<unsigned short>(topLeftCellRef.getColumn());
	record << rwTop << colLeft << icvHdr;

	record.reserveNunBytes(2); // reserved 
	record << wScaleSLV << wScaleNormal;
	record.reserveNunBytes(4); // unused / reserved
}


void Window2::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version >= 0x0600)
	{
		is_contained_in_chart_substream = (10 == record.getDataSize());
	}

	unsigned short flags;
	record >> flags;

	fSelected		= GETBIT(flags, 9);

	if(is_contained_in_chart_substream)
	{
		record.skipNunBytes(8); // must be ignored
		return;
	}
	fDspFmlaRt		= GETBIT(flags, 0);
	fDspGridRt		= GETBIT(flags, 1);
	fDspRwColRt		= GETBIT(flags, 2);
	fFrozenRt		= GETBIT(flags, 3);
	fDspZerosRt		= GETBIT(flags, 4);
	fDefaultHdr		= GETBIT(flags, 5);
	fRightToLeft	= GETBIT(flags, 6);
	fDspGuts		= GETBIT(flags, 7);
	fFrozenNoSplit	= GETBIT(flags, 8);
	fPaged			= GETBIT(flags, 10);
	fSLV			= GETBIT(flags, 11);
	
	record >> rwTop >> colLeft >> icvHdr;
	
	topLeftCell = static_cast<std::wstring >(CellRef(rwTop, colLeft, true, true));
	record.skipNunBytes(2); // reserved

	if (10 > record.getDataSize())
	{
		record >> wScaleSLV >> wScaleNormal;
		record.skipNunBytes(4); // unused / reserved
	}
}


} // namespace XLS

