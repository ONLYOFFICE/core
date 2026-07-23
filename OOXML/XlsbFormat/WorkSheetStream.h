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

    class WorkSheetStream;
    typedef boost::shared_ptr<WorkSheetStream>		WorkSheetStreamPtr;

    class WorkSheetStream: public XLS::CompositeObject
    {
        BASE_OBJECT_DEFINE_CLASS_NAME(WorkSheetStream)
    public:
        WorkSheetStream();
        ~WorkSheetStream();

        XLS::BaseObjectPtr clone();

        const bool loadContent(XLS::BinProcessor& proc) override;
		const bool saveContent(XLS::BinProcessor& proc) override;

        static const XLS::ElementType type = XLS::typeWorksheetSubstream;

        bool							 m_bBrtBeginSheet;
        std::vector<XLS::BaseObjectPtr>  m_arCOLINFOS;
        XLS::BaseObjectPtr               m_BrtWsDim;
        XLS::BaseObjectPtr               m_BrtDrawing;
        XLS::BaseObjectPtr               m_BrtLegacyDrawing;
        XLS::BaseObjectPtr               m_BrtLegacyDrawingHF;
        XLS::BaseObjectPtr               m_HLINKS;
        XLS::BaseObjectPtr               m_MERGECELLS;
        XLS::BaseObjectPtr               m_CELLTABLE;
        XLS::BaseObjectPtr               m_BrtWsFmtInfo;
        XLS::BaseObjectPtr               m_WSVIEWS2;
        XLS::BaseObjectPtr               m_BrtMargins;
        XLS::BaseObjectPtr               m_BrtPageSetup;
        XLS::BaseObjectPtr               m_BrtPrintOptions;
        XLS::BaseObjectPtr               m_HEADERFOOTER;
        XLS::BaseObjectPtr               m_BrtSheetProtectionIso;
        XLS::BaseObjectPtr               m_BrtSheetProtection;
        XLS::BaseObjectPtr               m_LISTPARTS;
        XLS::BaseObjectPtr               m_AUTOFILTER;
        XLS::BaseObjectPtr               m_SORTSTATE;
        std::vector<XLS::BaseObjectPtr>  m_arCONDITIONALFORMATTING;
        XLS::BaseObjectPtr               m_DVALS;
        XLS::BaseObjectPtr               m_OLEOBJECTS;
        XLS::BaseObjectPtr               m_ACTIVEXCONTROLS;
        XLS::BaseObjectPtr               m_BrtWsProp;
        XLS::BaseObjectPtr               m_BrtBkHim;
        XLS::BaseObjectPtr               m_RWBRK;
        XLS::BaseObjectPtr               m_COLBRK;
        std::vector<XLS::BaseObjectPtr>  m_arBrtRangeProtectionIso;
        std::vector<XLS::BaseObjectPtr>  m_arBrtRangeProtection;
        XLS::BaseObjectPtr               m_DCON;
        XLS::BaseObjectPtr               m_FRTWORKSHEET;        

		bool				             m_bBrtEndSheet;

        _UINT32                          m_SheetaDataPosition;

    };

}
