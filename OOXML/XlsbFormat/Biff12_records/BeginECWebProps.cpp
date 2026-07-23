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

#include "BeginECWebProps.h"

using namespace XLS;

namespace XLSB
{

    BeginECWebProps::BeginECWebProps()
    {
    }

    BeginECWebProps::~BeginECWebProps()
    {
    }

    BaseObjectPtr BeginECWebProps::clone()
    {
        return BaseObjectPtr(new BeginECWebProps(*this));
    }

    void BeginECWebProps::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags;

        record >> wHTMLFmt >> flags;

        fSrcIsXML              = GETBIT(flags, 0);
        fImportSourceData      = GETBIT(flags, 1);
        fParsePreFormatted     = GETBIT(flags, 2);
        fConsecDelim           = GETBIT(flags, 3);
        fSameSettings          = GETBIT(flags, 4);
        fXL97Format            = GETBIT(flags, 5);
        fNoDateRecog           = GETBIT(flags, 6);
        fRefreshedInXL9        = GETBIT(flags, 7);
        fTablesOnlyHTML        = GETBIT(flags, 8);

        fLoadWebPost           = GETBIT(flags, 24);
        fLoadEditWebPage       = GETBIT(flags, 25);
        fLoadURL               = GETBIT(flags, 26);

        if(fLoadURL)
            record >> stURL;

        if(fLoadWebPost)
            record >> stWebPost;

        if(fLoadEditWebPage)
            record >> stEditWebPage;
    }

	void BeginECWebProps::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags = 0;

		SETBIT(flags, 0, fSrcIsXML)
		SETBIT(flags, 1, fImportSourceData)
		SETBIT(flags, 2, fParsePreFormatted)
		SETBIT(flags, 3, fConsecDelim)
		SETBIT(flags, 4, fSameSettings)
		SETBIT(flags, 5, fXL97Format)
		SETBIT(flags, 6, fNoDateRecog)
		SETBIT(flags, 7, fRefreshedInXL9)
		SETBIT(flags, 8, fTablesOnlyHTML)

		SETBIT(flags, 24, fLoadWebPost)
		SETBIT(flags, 25, fLoadEditWebPage)
		SETBIT(flags, 26, fLoadURL)

		record << wHTMLFmt << flags;
		
		if (fLoadURL)
			record << stURL;

		if (fLoadWebPost)
			record << stWebPost;

		if (fLoadEditWebPage)
			record << stEditWebPage;
	}

} // namespace XLSB

