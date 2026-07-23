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

	void BeginPCDFAtbl::writeFields(XLS::CFRecord& record)
	{
		_UINT16 flags = 0;

		SETBIT(flags, 0, fTextEtcField)
		SETBIT(flags, 1, fNonDates)
		SETBIT(flags, 2, fDateInField)
		SETBIT(flags, 3, fHasTextItem)
		SETBIT(flags, 4, fHasBlankItem)
		SETBIT(flags, 5, fMixedTypesIgnoringBlanks)
		SETBIT(flags, 6, fNumField)
		SETBIT(flags, 7, fIntField)
		SETBIT(flags, 8, fNumMinMaxValid)
		SETBIT(flags, 9, fHasLongTextItem)

		record << flags;
		record << citems;

		if (fNumMinMaxValid)
			record << xnumMin;

		if (fNumMinMaxValid)
			record << xnumMax;		
	}

} // namespace XLSB

