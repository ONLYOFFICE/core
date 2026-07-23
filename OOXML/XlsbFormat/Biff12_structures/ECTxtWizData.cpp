/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "ECTxtWizData.h"

using namespace XLS;

namespace XLSB
{

    ECTxtWizData::ECTxtWizData()
    {
    }

    ECTxtWizData::ECTxtWizData(XLS::CFRecord& record)
    {
        load(record);
    }

    ECTxtWizData::~ECTxtWizData()
    {
    }

    BiffStructurePtr ECTxtWizData::clone()
    {
        return BiffStructurePtr(new ECTxtWizData(*this));
    }

    void ECTxtWizData::load(XLS::CFRecord& record)
    {
        _UINT32 flags;
        record >> flags;

        iCpid           = GETBITS(flags, 0, 1);
        iCpidNew        = GETBITS(flags, 2, 11);
        fDelimited      = GETBIT(flags, 12);
        fTab            = GETBIT(flags, 13);
        fSpace          = GETBIT(flags, 14);
        fComma          = GETBIT(flags, 15);
        fSemiColon      = GETBIT(flags, 16);
        fConsecutive    = GETBIT(flags, 17);
        fTextDelim      = GETBITS(flags, 18, 19);
        fPromptForFile  = GETBIT(flags, 21);
        fCustom         = GETBIT(flags, 22);

        record >> chCustom >> rowStartAt >> chDecimal >> chThousSep;
    }

	void ECTxtWizData::save(XLS::CFRecord& record)
	{
		_UINT32 flags = 0;

		SETBITS(flags, 0, 1, iCpid)
		SETBITS(flags, 2, 11, iCpidNew)
		SETBIT(flags, 12, fDelimited)
		SETBIT(flags, 13, fTab)
		SETBIT(flags, 14, fSpace)
		SETBIT(flags, 15, fComma)
		SETBIT(flags, 16, fSemiColon)
		SETBIT(flags, 17, fConsecutive)
		SETBITS(flags, 18, 19, fTextDelim)
        SETBIT(flags, 20, 1)
		SETBIT(flags, 21, fPromptForFile)
		SETBIT(flags, 22, fCustom)

		record << flags;
		record << chCustom << rowStartAt << chDecimal << chThousSep;
	}

} // namespace XLSB

