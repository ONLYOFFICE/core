/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include "PRFilter.h"

using namespace XLS;

namespace XLSB
{

    PRFilter::PRFilter()
    {
    }

    PRFilter::PRFilter(XLS::CFRecord& record)
    {
        load(record);
    }

    PRFilter::~PRFilter()
    {
    }

    BiffStructurePtr PRFilter::clone()
    {
        return BiffStructurePtr(new PRFilter(*this));
    }

    void PRFilter::load(XLS::CFRecord& record)
    {
        _UINT16 flags;
        BYTE    flags1;
        record >> isxvd >> cItems >> flags >> flags1;

        itmtypeData     = GETBIT(flags, 0);
        itmtypeDEFAULT  = GETBIT(flags, 1);
        itmtypeSUM      = GETBIT(flags, 2);
        itmtypeCOUNTA   = GETBIT(flags, 3);
        itmtypeAVERAGE  = GETBIT(flags, 4);
        itmtypeMAX      = GETBIT(flags, 5);
        itmtypeMIN      = GETBIT(flags, 6);
        itmtypePRODUCT  = GETBIT(flags, 7);
        itmtypeCOUNT    = GETBIT(flags, 8);
        itmtypeSTDEV    = GETBIT(flags, 9);
        itmtypeSTDEVP   = GETBIT(flags, 10);
        itmtypeVAR      = GETBIT(flags, 11);
        itmtypeVARP     = GETBIT(flags, 12);
        fSelected       = GETBIT(flags1, 0);
    }

	void PRFilter::save(XLS::CFRecord& record)
	{
		_UINT16 flags = 0;
		BYTE    flags1 = 0;

		SETBIT(flags, 0, itmtypeData)
		SETBIT(flags, 1, itmtypeDEFAULT)
		SETBIT(flags, 2, itmtypeSUM)
		SETBIT(flags, 3, itmtypeCOUNTA)
		SETBIT(flags, 4, itmtypeAVERAGE)
		SETBIT(flags, 5, itmtypeMAX)
		SETBIT(flags, 6, itmtypeMIN)
		SETBIT(flags, 7, itmtypePRODUCT)
		SETBIT(flags, 8, itmtypeCOUNT)
		SETBIT(flags, 9, itmtypeSTDEV)
		SETBIT(flags, 10, itmtypeSTDEVP)
		SETBIT(flags, 11, itmtypeVAR)
		SETBIT(flags, 12, itmtypeVARP)
		SETBIT(flags1, 0, fSelected)

		record << isxvd << cItems << flags << flags1;
	}

} // namespace XLSB

