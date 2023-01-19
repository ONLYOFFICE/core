/*
* (c) Copyright Ascensio System SIA 2010-2019
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
