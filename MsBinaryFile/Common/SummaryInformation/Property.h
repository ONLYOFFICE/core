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

#include <boost/shared_ptr.hpp>
#include "../../XlsFile/Format/Binary/BinSmartPointers.h"
#include <string>

namespace OLEPS
{
	enum Property_Identifier
	{
//---------- ext -  2.3.3.2.1.1 PIDSI		
		CODEPAGE = 0x0001,
		TITLE = 0x0002,
		SUBJECT = 0x0003,
		AUTHOR = 0x0004,
		KEYWORDS = 0x0005,
		COMMENTS = 0x0006,
		TEMPLATE = 0x0007,
		LASTAUTHOR = 0x0008,
		REVNUMBER = 0x0009,
		EDITTIME = 0x000A,
		LASTPRINTED = 0x000B,
		CREATE_DTM = 0x000C,
		LASTSAVE_DTM = 0x000D,
		PAGECOUNT = 0x000E,
		WORDCOUNT = 0x000F,
		CHARCOUNT = 0x0010,
		THUMBNAIL = 0x0011,
		APPNAME = 0x0012,
		DOC_SECURITY = 0x0013,
//---------- ext -  2.3.3.2.2.1 PIDDSI
		CODEPAGEEXT = 0x1001,
		CATEGORY = 0x1002,
		PRESFORMAT = 0x1003,
		BYTECOUNT = 0x1004,
		LINECOUNT = 0x1005,
		PARACOUNT = 0x1006,
		SLIDECOUNT = 0x1007,
		NOTECOUNT = 0x1008,
		HIDDENCOUNT = 0x1009,
		MMCLIPCOUNT = 0x100A,
		SCALE = 0x100B,
		HEADINGPAIR = 0x100C,
		DOCPARTS = 0x100D,
		MANAGER = 0x100E,
		COMPANY = 0x100F,
		LINKSDIRTY = 0x1010,
		CCHWITHSPACES = 0x1011,
		SHAREDDOC = 0x1013,
		LINKBASE = 0x1014,
		HLINKS = 0x1015,
		HYPERLINKSCHANGED = 0x1016,
		VERSION = 0x1017,
		DIGSIG = 0x1018,
		CONTENTTYPE = 0x101A,
		CONTENTSTATUS = 0x101B,
		LANGUAGE = 0x101C,
		DOCVERSION = 0x101D
	};
	class Property
	{
	public:
		Property(unsigned int prop_type_, unsigned short value_type_) : prop_type(prop_type_), value_type(value_type_) {}
		virtual ~Property()
		{
		}
		virtual bool Read(XLS::CFStreamPtr stream) = 0;
		virtual std::wstring toString() = 0;

		virtual bool IsEmpty() { return false; }

		unsigned int	prop_type;
		unsigned short	value_type;

		enum ValueTypes
		{
			VT_I2 = 0x0002,
			VT_I4 = 0x0003,
			VT_R4 = 0x0004,
			VT_R8 = 0x0005,
			VT_CY = 0x0006,
			VT_DATE = 0x0007,
			VT_BSTR = 0x0008,
			VT_ERROR = 0x000A,
			VT_BOOL = 0x000B,
			VT_DECIMAL = 0x000E,
			VT_I1 = 0x0010,
			VT_UI1 = 0x0011,
			VT_UI2 = 0x0012,
			VT_UI4 = 0x0013,
			VT_I8 = 0x0014,
			VT_UI8 = 0x0015,
			VT_INT = 0x0016,
			VT_UINT = 0x0017,
			VT_LPSTR = 0x001E,
			VT_LPWSTR = 0x001F,
			VT_FILETIME = 0x0040,
			VT_CF = 0x0047,
			VT_CLSID = 0x0047,
		};
	};
	typedef boost::shared_ptr<Property> PropertyPtr;
} // namespace OLEPS
