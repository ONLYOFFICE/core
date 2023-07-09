﻿/*
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

