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

#include "SXVDTEx.h"

namespace XLS
{

SXVDTEx::SXVDTEx()
{
}


SXVDTEx::~SXVDTEx()
{
}


BaseObjectPtr SXVDTEx::clone()
{
	return BaseObjectPtr(new SXVDTEx(*this));
}

void SXVDTEx::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> frtHeaderOld >> flags >> isxth >> isxtl >> csxvi;

	for (_INT32 i = 0; i < csxvi; i++)
	{
		SXVIFlags val;
		record >> val;
		rgsxvi.push_back(val);
	}
	fTensorSort					= GETBIT(flags, 0);
	fDrilledLevel				= GETBIT(flags, 1);
	fItemsDrilledByDefault		= GETBIT(flags, 2);
	fMemPropDisplayInReport		= GETBIT(flags, 3);
	fMemPropDisplayInTip		= GETBIT(flags, 4);
	fMemPropDisplayInCaption	= GETBIT(flags, 5);
}

} // namespace XLS

