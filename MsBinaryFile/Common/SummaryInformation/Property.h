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
