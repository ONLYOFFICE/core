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

#include "HeaderFooter.h"
#include "../Biff_structures/FrtHeader.h"

namespace XLS
{

HeaderFooter::HeaderFooter()
{}
HeaderFooter::~HeaderFooter()
{}
BaseObjectPtr HeaderFooter::clone()
{
	return BaseObjectPtr(new HeaderFooter(*this));
}

void HeaderFooter::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record >> frtHeader;
	
	unsigned short	flags;
	_GUID_			guid_num;
	
	record >> guid_num >> flags;
	guidSView = STR::guid2bstr(guid_num);
	
	fHFDiffOddEven	= GETBIT(flags, 0);
	fHFDiffFirst	= GETBIT(flags, 1);
	fHFScaleWithDoc = GETBIT(flags, 2);
	fHFAlignMargins = GETBIT(flags, 3);

	record >> cchHeaderEven >> cchFooterEven >> cchHeaderFirst >> cchFooterFirst;
	if(cchHeaderEven)
	{
		record >> strHeaderEven;
	}
	if(cchFooterEven)
	{
		record >> strFooterEven;
	}
	if(cchHeaderFirst)
	{
		record >> strHeaderFirst;
	}
	if(cchFooterFirst)
	{
		record >> strFooterFirst;
	}
}

void HeaderFooter::writeFields(CFRecord& record)
{
    FrtHeader frtHeader(rt_HeaderFooter);
    frtHeader.grbitFrt.fFrtRef = 0;
    frtHeader.grbitFrt.fFrtAlert = 0;
    record << frtHeader;
	_GUID_ guid_num;
	STR::bstr2guid(guidSView ,guid_num);
    record << guid_num;
    _UINT16 flags = 0;
    SETBIT(flags, 0, fHFDiffOddEven);
    SETBIT(flags, 1, fHFDiffFirst);
    SETBIT(flags, 2, fHFScaleWithDoc);
    SETBIT(flags, 3, fHFAlignMargins);
    record << flags;
    record << cchHeaderEven << cchFooterEven << cchHeaderFirst << cchFooterFirst;
    if(cchHeaderEven && strHeaderEven.getSize())
    {
        record << strHeaderEven;
    }
    if(cchFooterEven && strFooterEven.getSize())
    {
        record << strFooterEven;
    }
    if(cchHeaderFirst && strHeaderFirst.getSize())
    {
        record << strHeaderFirst;
    }
    if(cchFooterFirst && strFooterFirst.getSize())
    {
        record << strFooterFirst;
    }

}

} // namespace XLS

