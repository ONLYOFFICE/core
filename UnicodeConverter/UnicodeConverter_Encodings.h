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
#ifndef _SERVER_COMPONENTS_UNICODE_CONVERTER_ENCODINGS_H
#define _SERVER_COMPONENTS_UNICODE_CONVERTER_ENCODINGS_H

#pragma once

#include <map>

namespace NSUnicodeConverter
{
    struct EncodindId
    {
        int Index;
        int WindowsCodePage;
        const char* Name;
        const char* DisplayName;
    };

    #define UNICODE_CONVERTER_ENCODINGS_COUNT 54
    static const EncodindId Encodings[UNICODE_CONVERTER_ENCODINGS_COUNT] =
    {
        { 0,    28596, "ISO-8859-6",       "Arabic (ISO 8859-6)" },
        { 1,    720,   "DOS-720",          "Arabic (OEM 720)" },
        { 2,    1256,  "windows-1256",     "Arabic (Windows)" },

        { 3,    28594, "ISO-8859-4",       "Baltic (ISO 8859-4)" },
        { 4,    28603, "ISO-8859-13",      "Baltic (ISO 8859-13)" },
        { 5,    775,   "IBM775",           "Baltic (OEM 775)" },
        { 6,    1257,  "windows-1257",     "Baltic (Windows)" },

        { 7,    28604, "ISO-8859-14",      "Celtic (ISO 8859-14)" },

        { 8,    28595, "ISO-8859-5",       "Cyrillic (ISO 8859-5)" },
        { 9,    20866, "KOI8-R",           "Cyrillic (KOI8-R)" },
        { 10,   21866, "KOI8-U",           "Cyrillic (KOI8-U)" },
        { 11,   10007, "x-mac-cyrillic",   "Cyrillic (Mac)" },
        { 12,   855,   "IBM855",           "Cyrillic (OEM 855)" },
        { 13,   866,   "cp866",            "Cyrillic (OEM 866)" },
        { 14,   1251,  "windows-1251",     "Cyrillic (Windows)" },

        { 15,   852,   "IBM852",           "Central European (OEM 852)" },
        { 16,   1250,  "windows-1250",     "Central European (Windows)" },

        { 17,   950,   "Big5",              "Chinese (Big5 Traditional)" },
        { 18,   936,   "GBK",               "Central (GBK Simplified)" },

        { 19,   28592, "ISO-8859-2",       "Eastern European (ISO 8859-2)" },

        { 20,   28597, "ISO-8859-7",       "Greek (ISO 8859-7)" },
        { 21,   737,   "IBM737",           "Greek (OEM 737)" },
        { 22,   869,   "IBM869",           "Greek (OEM 869)" },
        { 23,   1253,  "windows-1253",     "Greek (Windows)" },

        { 24,   28598, "ISO-8859-8",       "Hebrew (ISO 8859-8)" },
        { 25,   862,   "DOS-862",          "Hebrew (OEM 862)" },
        { 26,   1255,  "windows-1255",     "Hebrew (Windows)" },

        { 27,   932,   "Shift_JIS",        "Japanese (Shift-JIS)" },

        { 28,   949,   "KS_C_5601-1987",   "Korean (Windows)" },
        { 29,   51949, "EUC-KR",           "Korean (EUC)" },

        { 30,   861,   "IBM861",           "North European (Icelandic OEM 861)" },
        { 31,   865,   "IBM865",           "North European (Nordic OEM 865)" },

        { 32,   874,   "windows-874",      "Thai (TIS-620)" },

        { 33,   28593, "ISO-8859-3",       "Turkish (ISO 8859-3)" },
        { 34,   28599, "ISO-8859-9",       "Turkish (ISO 8859-9)" },
        { 35,   857,   "IBM857",           "Turkish (OEM 857)" },
        { 36,   1254,  "windows-1254",     "Turkish (Windows)" },

        { 37,   28591, "ISO-8859-1",       "Western European (ISO-8859-1)" },
        { 38,   28605, "ISO-8859-15",      "Western European (ISO-8859-15)" },
        { 39,   850,   "IBM850",           "Western European (OEM 850)" },
        { 40,   858,   "IBM858",           "Western European (OEM 858)" },
        { 41,   860,   "IBM860",           "Western European (OEM 860 : Portuguese)" },
        { 42,   863,   "IBM863",           "Western European (OEM 863 : French)" },
        { 43,   437,   "IBM437",           "Western European (OEM-US)" },
        { 44,   1252,  "windows-1252",     "Western European (Windows)" },

        { 45,   1258,  "windows-1258",     "Vietnamese (Windows)" },

        { 46,   65001, "UTF-8",            "Unicode (UTF-8)" },
        { 47,   65000, "UTF-7",            "Unicode (UTF-7)" },

        { 48,   1200, "UTF-16LE",          "Unicode (UTF-16)" },
        { 49,   1201, "UTF-16BE",          "Unicode (UTF-16 Big Endian)" },

        { 50,   12000, "UTF-32LE",         "Unicode (UTF-32)" },
        { 51,   12001, "UTF-32BE",          "Unicode (UTF-32 Big Endian)" },

        { 52,   20932,   "EUC-JP",          "Japanese (EUC-JP)" },
        { 53,   54936,   "GB18030",         "Chinese government standard" }
    };

    static std::map<int, std::string> create_mapEncodingsICU()
    {
        std::map<int, std::string> m;
        m[28596]	= "ISO-8859-6";
        m[720]		= "DOS-720";
        m[1256]		= "windows-1256";

        m[28594]	= "ISO-8859-4";
        m[28603]	= "ISO-8859-13";
        m[775]		= "IBM775";
        m[1257]		= "windows-1257";

        m[28604]	= "ISO-8859-14";

        m[28595]	= "ISO-8859-5";
        m[20866]	= "KOI8-R";
        m[21866]	= "KOI8-U";
        m[10007]	= "x-mac-cyrillic";
        m[855]		= "IBM855";
        m[866]		= "cp866";
        m[1251]		= "windows-1251";

        m[852]		= "IBM852";
        m[1250]		= "windows-1250";

        m[936]		= "GBK";
        m[28592]	= "ISO-8859-2";

        m[28597]	= "ISO-8859-7";
        m[737]		= "IBM737";
        m[869]		= "IBM869";
        m[1253]		= "windows-1253";

        m[28598]	= "ISO-8859-8";
        m[862]		= "DOS-862";
        m[1255]		= "windows-1255";

        m[932]		= "Shift_JIS";
        m[950]		= "Big5"; //CN

        m[949]		= "KS_C_5601-1987";
        m[51949]	= "EUC-KR";

        m[861]		= "IBM861";
        m[865]		= "IBM865";

        m[874]		= "windows-874";

        m[28593]	= "ISO-8859-3";
        m[28599]	= "ISO-8859-9";
        m[857]		= "IBM857";
        m[1254]		= "windows-1254";

        m[28591]	= "ISO-8859-1";
        m[28605]	= "ISO-8859-15";
        m[850]		= "IBM850";
        m[858]		= "IBM858";
        m[860]		= "IBM860";
        m[863]		= "IBM863";
        m[437]		= "IBM437";
        m[1252]		= "windows-1252";

        m[1258]		= "windows-1258";

        m[65001]	= "UTF-8";
        m[65000]	= "UTF-7";

        m[1200]		= "UTF-16LE";
        m[1201]		= "UTF-16BE";

        m[12000]	= "UTF-32LE";
        m[12001]	= "UTF-32BE";

        m[10000]	= "macintosh";
        m[20932]    = "EUC-JP";
        m[54936]    = "GB18030";
        return m;
    }
    static const std::map<int, std::string> mapEncodingsICU = create_mapEncodingsICU();

}

#endif // _SERVER_COMPONENTS_UNICODE_CONVERTER_ENCODINGS_H
