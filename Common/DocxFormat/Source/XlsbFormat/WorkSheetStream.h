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


#include "../../../../DesktopEditor/common/Types.h"
#include "../Base/Types_32.h"
#include "../XlsxFormat/WritingElement.h"
#include <string>
#include <memory.h>
#include <iostream>
#include "../../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/CompositeObject.h"
typedef BYTE *LPBYTE;
using namespace XLS;
namespace XLSB
{
    class StreamCacheReader;

    class WorkSheetStream;
    typedef std::shared_ptr<WorkSheetStream>		WorkSheetStreamPtr;

    class WorkSheetStream: public CompositeObject
    {
        BASE_OBJECT_DEFINE_CLASS_NAME(WorkBookStream)
    public:
        WorkSheetStream(const unsigned short code_page);
        virtual ~WorkSheetStream();

        BaseObjectPtr clone();

        virtual const bool loadContent(BinProcessor& proc);

        static const XLS::ElementType type = typeWorksheetSubstream;

        int serialize_format(std::wostream & _stream);
        int serialize_protection(std::wostream & _stream);

        BaseObjectPtr               m_BrtBeginSheet;
        std::vector<BaseObjectPtr>  m_arCOLINFOS;
        BaseObjectPtr               m_BrtWsDim;
        BaseObjectPtr               m_BrtDrawing;
        BaseObjectPtr               m_BrtLegacyDrawing;
        BaseObjectPtr               m_BrtLegacyDrawingHF;
        BaseObjectPtr               m_HLINKS;
        BaseObjectPtr               m_MERGECELLS;
        BaseObjectPtr               m_CELLTABLE;
        BaseObjectPtr               m_BrtWsFmtInfo;
        BaseObjectPtr               m_WSVIEWS2;
        BaseObjectPtr               m_BrtSheetProtectionIso;
        BaseObjectPtr               m_BrtSheetProtection;
        BaseObjectPtr               m_LISTPARTS;
        BaseObjectPtr               m_SORTSTATE;
        std::vector<BaseObjectPtr>  m_arCONDITIONALFORMATTING;
        BaseObjectPtr               m_FRTWORKSHEET;

        BaseObjectPtr               m_BrtEndSheet;


        unsigned short              code_page_;
        GlobalWorkbookInfoPtr		global_info_;


    };

}

#endif // WORKSHEETSTREAM_H

