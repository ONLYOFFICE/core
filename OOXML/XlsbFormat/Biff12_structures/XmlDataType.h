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

#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BiffAttribute.h"

namespace XLSB
{

class XmlDataType : public XLS::BiffAttributeSimple<unsigned int>
{
public:
    XLS::BiffStructurePtr clone();

	enum
	{
            MSOXIDTSTRING               = 0x00000001,
            MSOXIDTNORMALIZEDSTRING     = 0x00000002,
            MSOXIDTTOKEN                = 0x00000003,
            MSOXIDTBYTE                 = 0x00000004,
            MSOXIDTUNSIGNEDBYTE         = 0x00000005,
            MSOXIDTBASE64BINARY         = 0x00000006,
            MSOXIDTHEXBINARY            = 0x00000007,
            MSOXIDTINTEGER              = 0x00000008,
            MSOXIDTPOSITIVEINTEGER      = 0x00000009,
            MSOXIDTNEGATIVEINTEGER      = 0x0000000A,
            MSOXIDTNONPOSITIVEINTEGER   = 0x0000000B,
            MSOXIDTNONNEGATIVEINTEGER   = 0x0000000C,
            MSOXIDTINT                  = 0x0000000D,
            MSOXIDTUNSIGNEDINT          = 0x0000000E,
            MSOXIDTLONG                 = 0x0000000F,
            MSOXIDTUNSIGNEDLONG         = 0x00000010,
            MSOXIDTSHORT                = 0x00000011,
            MSOXIDTUNSIGNEDSHORT        = 0x00000012,
            MSOXIDTDECIMAL              = 0x00000013,
            MSOXIDTFLOAT                = 0x00000014,
            MSOXIDTDOUBLE               = 0x00000015,
            MSOXIDTBOOLEAN              = 0x00000016,
            MSOXIDTTIME                 = 0x00000017,
            MSOXIDTDATETIME             = 0x00000018,
            MSOXIDTDURATION             = 0x00000019,
            MSOXIDTDATE                 = 0x0000001A,
            MSOXIDTMONTH                = 0x0000001B,
            MSOXIDTYEAR                 = 0x0000001C,
            MSOXIDTYEARMONTH            = 0x0000001D,
            MSOXIDTDAY                  = 0x0000001E,
            MSOXIDTMONTHDAY             = 0x0000001F,
            MSOXIDTNAME                 = 0x00000020,
            MSOXIDTQNAME                = 0x00000021,
            MSOXIDTNCNAME               = 0x00000022,
            MSOXIDTANYURI               = 0x00000023,
            MSOXIDTLANGUAGE             = 0x00000024,
            MSOXIDTID                   = 0x00000025,
            MSOXIDTIDREF                = 0x00000026,
            MSOXIDTIDREFS               = 0x00000027,
            MSOXIDTENTITY               = 0x00000028,
            MSOXIDTENTITIES             = 0x00000029,
            MSOXIDTNOTATION             = 0x0000002A,
            MSOXIDTNMTOKEN              = 0x0000002B,
            MSOXIDTNMTOKENS             = 0x0000002C,
            MSOXIDTANYTYPE              = 0x0000002D
	};
};

} // namespace XLSB

