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

#include "BeginPCDFAtbl.h"

using namespace XLS;

namespace XLSB
{

    BeginPCDFAtbl::BeginPCDFAtbl()
    {
    }

    BeginPCDFAtbl::~BeginPCDFAtbl()
    {
    }

    BaseObjectPtr BeginPCDFAtbl::clone()
    {
        return BaseObjectPtr(new BeginPCDFAtbl(*this));
    }

    void BeginPCDFAtbl::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags;        
        record >> flags;

        fTextEtcField               = GETBIT(flags, 0);
        fNonDates                   = GETBIT(flags, 1);
        fDateInField                = GETBIT(flags, 2);
        fHasTextItem                = GETBIT(flags, 3);
        fHasBlankItem               = GETBIT(flags, 4);
        fMixedTypesIgnoringBlanks   = GETBIT(flags, 5);
        fNumField                   = GETBIT(flags, 6);
        fIntField                   = GETBIT(flags, 7);
        fNumMinMaxValid             = GETBIT(flags, 8);
        fHasLongTextItem            = GETBIT(flags, 9);

        record >> citems;

        if(fNumMinMaxValid)
            record >> xnumMin;

        if(fNumMinMaxValid)
            record >> xnumMax;

        if(record.getGlobalWorkbookInfo()->pivotCacheRecordType.find(record.getGlobalWorkbookInfo()->currentPivotCacheRecord) == record.getGlobalWorkbookInfo()->pivotCacheRecordType.end())
            record.getGlobalWorkbookInfo()->pivotCacheRecordType.insert({record.getGlobalWorkbookInfo()->currentPivotCacheRecord, std::vector<XLS::ElementType>()});

        if(citems > 0)
            record.getGlobalWorkbookInfo()->pivotCacheRecordType[record.getGlobalWorkbookInfo()->currentPivotCacheRecord].push_back(XLS::typePCDIIndex);
        else if(fNumField)
            record.getGlobalWorkbookInfo()->pivotCacheRecordType[record.getGlobalWorkbookInfo()->currentPivotCacheRecord].push_back(XLS::typePCDINumber);
        else if(fDateInField && !fHasTextItem)
            record.getGlobalWorkbookInfo()->pivotCacheRecordType[record.getGlobalWorkbookInfo()->currentPivotCacheRecord].push_back(XLS::typePCDIDatetime);
        else
            record.getGlobalWorkbookInfo()->pivotCacheRecordType[record.getGlobalWorkbookInfo()->currentPivotCacheRecord].push_back(XLS::typePCDIString);
    }

} // namespace XLSB

