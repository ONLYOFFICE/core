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

#ifndef WORKSHEETSTREAM_H
#define WORKSHEETSTREAM_H


#include "../../DesktopEditor/common/Types.h"
#include "../Base/Base.h"
#include "../XlsxFormat/WritingElement.h"
#include <string>
#include <memory.h>
#include <iostream>
#include "../../MsBinaryFile/XlsFile/Format/Logic/CompositeObject.h"
typedef BYTE *LPBYTE;

namespace XLSB
{
    class StreamCacheReader;

    class WorkSheetStream;
    typedef boost::shared_ptr<WorkSheetStream>		WorkSheetStreamPtr;

    class WorkSheetStream: public XLS::CompositeObject
    {
        BASE_OBJECT_DEFINE_CLASS_NAME(WorkSheetStream)
    public:
        WorkSheetStream();
        ~WorkSheetStream();

        XLS::BaseObjectPtr clone();

        virtual const bool loadContent(XLS::BinProcessor& proc);

        static const XLS::ElementType type = XLS::typeWorksheetSubstream;

        XLS::BaseObjectPtr               m_BrtBeginSheet;
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

        XLS::BaseObjectPtr               m_BrtEndSheet;

        _UINT32                          m_SheetaDataPosition;

    };

}

#endif // WORKSHEETSTREAM_H

