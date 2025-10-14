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

#include "SxFilt.h"

namespace XLS
{

SxFilt::SxFilt()
{
	isxvd = iDim = cisxvi = grbitSbt = 0;
}


SxFilt::~SxFilt()
{
}

BaseObjectPtr SxFilt::clone()
{
	return BaseObjectPtr(new SxFilt(*this));
}


void SxFilt::readFields(CFRecord& record)
{
	unsigned short flags1, flags2;

	record >> flags1 >> flags2 >> grbitSbt >> cisxvi;
	
	sxaxisRw	= GETBIT(flags1, 0);
	sxaxisCol	= GETBIT(flags1, 1);
	sxaxisPage	= GETBIT(flags1, 2);
	sxaxisData	= GETBIT(flags1, 3);
	iDim		= GETBITS(flags1, 6, 15);

	isxvd		= GETBITS(flags2, 0, 9);
	fSelected	= GETBIT(flags2, 10);
}

void SxFilt::writeFields(CFRecord& record)
{
    unsigned short flags1 = 0, flags2 = 0;
    SETBIT(flags1, 0, sxaxisRw)
    SETBIT(flags1, 1, sxaxisCol)
    SETBIT(flags1, 2, sxaxisPage)
    SETBIT(flags1, 3, sxaxisData)
    SETBITS(flags1, 6, 15, iDim)

    SETBITS(flags2, 0, 9, isxvd)
    SETBIT(flags2, 10, fSelected)
    record << flags1 << flags2 << grbitSbt << cisxvi;
}

} // namespace XLS

