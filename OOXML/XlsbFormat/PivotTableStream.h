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

#pragma once

#include "../../DesktopEditor/common/Types.h"
#include "../Base/Base.h"
#include "../XlsxFormat/WritingElement.h"
#include <string>
#include "../../MsBinaryFile/XlsFile/Format/Logic/CompositeObject.h"

typedef BYTE *LPBYTE;

namespace XLSB
{
    class PivotTableStream;
    typedef boost::shared_ptr<PivotTableStream>		PivotTableStreamPtr;

    class PivotTableStream: public XLS::CompositeObject
    {
        BASE_OBJECT_DEFINE_CLASS_NAME(PivotTableStream)
    public:
        PivotTableStream();
        ~PivotTableStream();

        XLS::BaseObjectPtr clone();

		const bool loadContent(XLS::BinProcessor& proc) override;
		const bool saveContent(XLS::BinProcessor& proc) override;

        //XLS::BaseObjectPtr               m_ACUID;
        XLS::BaseObjectPtr               m_BrtBeginSXView;
        XLS::BaseObjectPtr               m_SXLOCATION;
        XLS::BaseObjectPtr               m_SXVDS;
        XLS::BaseObjectPtr               m_ISXVDRWS;
        XLS::BaseObjectPtr               m_SXLIRWS;
        XLS::BaseObjectPtr               m_ISXVDCOLS;
        XLS::BaseObjectPtr               m_SXLICOLS;
        XLS::BaseObjectPtr               m_SXPIS;
        XLS::BaseObjectPtr               m_SXDIS;
        XLS::BaseObjectPtr               m_SXFORMATS;
        XLS::BaseObjectPtr               m_SXCONDFMTS;
        XLS::BaseObjectPtr               m_SXCRTFORMATS;
        XLS::BaseObjectPtr               m_SXTHS;
        XLS::BaseObjectPtr               m_BrtTableStyleClient;
        XLS::BaseObjectPtr               m_SXFILTERS;
        XLS::BaseObjectPtr               m_ISXTHRWS;
        XLS::BaseObjectPtr               m_ISXTHCOLS;
        XLS::BaseObjectPtr               m_FRTSXVIEW;
		bool			                 m_bBrtEndSXView;

    };

}


