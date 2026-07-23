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

#include "../../OOXML/Base/Types_32.h"
#include "../../DesktopEditor/common/Types.h"
#include <string.h>

struct _GUID_
{
	_UINT32 Data1 = 0;
	_UINT16 Data2 = 0;
	_UINT16 Data3 = 0;
	_UINT64 Data4 = 0;

	BYTE* getData4()
	{
		return reinterpret_cast<BYTE*>(&Data4);
	}

	_GUID_(const _GUID_& o) : Data1(o.Data1), Data2(o.Data2), Data3(o.Data3), Data4(o.Data4)
	{
	}

	_GUID_& operator=(const _GUID_& o)
	{
		Data1 = o.Data1;
		Data2 = o.Data2;
		Data3 = o.Data3;
		Data4 = o.Data4;

		return *this;
	}

	bool operator!=(const _GUID_& oth)const
	{
		return Data1 != oth.Data1 || Data2 != oth.Data2 || Data3 != oth.Data3 || Data4 != oth.Data4;
	}

	bool operator==(const _GUID_& oth)const
	{
		return !operator!=(oth);
	}
	_GUID_() {}

	_GUID_(_UINT32 _Data1, _UINT16 _Data2, _UINT16 _Data3, _UINT16 _Data4) : Data1(_Data1), Data2(_Data2), Data3(_Data3), Data4(_Data4)
	{}
	_GUID_(int _Data1, int _Data2, int _Data3, BYTE _Data4[8]) : Data1(_Data1), Data2(_Data2), Data3(_Data3)
	{
		memcpy(getData4(), _Data4, 8);
	}
};
