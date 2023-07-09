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

#include "Window2.h"

namespace XLS
{

Window2::Window2(bool isChart)
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

    _isChart = isChart;
}


Window2::~Window2()
{
}


BaseObjectPtr Window2::clone()
{
	return BaseObjectPtr(new Window2(*this));
}

void Window2::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version >= 0x0600)
	{
		is_contained_in_chart_substream = (10 == record.getDataSize());
	}

    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 flags;
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

        _UINT16		rwTop_2b;
        _UINT16		colLeft_2b;

        record >> rwTop_2b >> colLeft_2b >> icvHdr;

        rwTop   = rwTop_2b;
        colLeft = colLeft_2b;

        topLeftCell = static_cast<std::wstring >(CellRef(rwTop_2b, colLeft_2b, true, true));
        record.skipNunBytes(2); // reserved

        if (10 > record.getDataSize())
        {
            record >> wScaleSLV >> wScaleNormal;
        }
        int sz = record.getDataSize() - record.getRdPtr();

        record.skipNunBytes(sz); // unused / reserved
    }

    else
    {
        if(_isChart)
        {
            _UINT16 flags;
            _UINT32 wScale_4b;

            record >> flags;

            fSelected           = GETBIT(flags, 0);

            record >> wScale_4b >> iWbkView;
            wScale = wScale_4b;
        }
        else
        {
            _UINT16 flags;
            record >> flags;

            fWnProt             = GETBIT(flags, 0);
            fDspFmlaRt          = GETBIT(flags, 1);
            fDspGridRt          = GETBIT(flags, 2);
            fDspRwColRt         = GETBIT(flags, 3);
            fDspZerosRt         = GETBIT(flags, 4);
            fRightToLeft        = GETBIT(flags, 5);
            fSelected           = GETBIT(flags, 6);
            fDspRuler           = GETBIT(flags, 7);
            fDspGuts            = GETBIT(flags, 8);
            fDefaultHdr         = GETBIT(flags, 9);
            fWhitespaceHidden   = GETBIT(flags, 10);

            record >> xlView >> rwTop >> colLeft;

            topLeftCell = static_cast<std::wstring >(CellRef(rwTop, colLeft, true, true));

            BYTE	icvHdr_1b;
            record >> icvHdr_1b;
            icvHdr = icvHdr_1b;

            record.skipNunBytes(3); // reserved

            record >> wScale >> wScaleNormal >> wScaleSLV >> wScalePLV >> iWbkView;
        }

    }
}

void Window2::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 flags = 0;

		SETBIT(flags, 9, fSelected);

		if (is_contained_in_chart_substream)
		{
			record.reserveNunBytes(8); // must be ignored
			return;
		}
		SETBIT(flags, 0, fDspFmlaRt)
		SETBIT(flags, 1, fDspGridRt)
		SETBIT(flags, 2, fDspRwColRt)
		SETBIT(flags, 3, fFrozenRt)
		SETBIT(flags, 4, fDspZerosRt)
		SETBIT(flags, 5, fDefaultHdr)
		SETBIT(flags, 6, fRightToLeft)
		SETBIT(flags, 7, fDspGuts)
		SETBIT(flags, 8, fFrozenNoSplit)
		SETBIT(flags, 10, fPaged)
		SETBIT(flags, 11, fSLV)

		CellRef ref(topLeftCell);

		_UINT16		rwTop_2b = ref.row == -1 ? 0: ref.row;
		_UINT16		colLeft_2b = ref.column == -1 ? 0 : ref.column;
		
		record << flags;
		record << rwTop_2b << colLeft_2b << icvHdr;
		
		record.reserveNunBytes(2); // reserved

		record << wScaleSLV << wScaleNormal;
	}

	else
	{
		if (_isChart)
		{
			_UINT16 flags = 0;
			_UINT32 wScale_4b = 0;

			SETBIT(flags, 0, fSelected)
			wScale_4b = wScale;

			record << flags;

			record << wScale_4b << iWbkView;
		}
		else
		{
			_UINT16 flags = 0;

			SETBIT(flags, 0, fWnProt)
			SETBIT(flags, 1, fDspFmlaRt)
			SETBIT(flags, 2, fDspGridRt)
			SETBIT(flags, 3, fDspRwColRt)
			SETBIT(flags, 4, fDspZerosRt)
			SETBIT(flags, 5, fRightToLeft)
			SETBIT(flags, 6, fSelected)
			SETBIT(flags, 7, fDspRuler)
			SETBIT(flags, 8, fDspGuts)
			SETBIT(flags, 9, fDefaultHdr)
			SETBIT(flags, 10, fWhitespaceHidden)

			record << flags;

			CellRef ref(topLeftCell);

			rwTop = ref.row == -1 ? 0 : ref.row;
			colLeft = ref.column == -1 ? 0 : ref.column;

			record << xlView << rwTop << colLeft;

			BYTE	icvHdr_1b = icvHdr;
			record << icvHdr_1b;

			record.reserveNunBytes(3); // reserved

			record << wScale << wScaleNormal << wScaleSLV << wScalePLV << iWbkView;
		}

	}
}


} // namespace XLS

