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
#ifndef _ASC_FONTCONVERTER_STRING_EXT_H_
#define _ASC_FONTCONVERTER_STRING_EXT_H_

#include <stdarg.h>

namespace NSFontConverter
{
    //---------------------------------------------------------------------------------------
    // StringExt class - CString analog
    //---------------------------------------------------------------------------------------

    class StringExt
    {
    public:

        // Create an empty string.
        StringExt();

        // Create a string from a C-style string.
        StringExt(const char *sString);

        // Create a string from <nLength> characters in <sString>. This string
        // may contain null characters.
        StringExt(const char *sString, int nLength);

        // Create a string from <nLength> characters starting at <nIndex> of string <seString>.
        StringExt(StringExt *seString, int nIndex, int nLength);

        // Copy the string.
        StringExt(StringExt *seString);
        StringExt *Copy()
        {
            return new StringExt(this);
        }

        // Concatenate two strings.
        StringExt(StringExt *seString1, StringExt *seString2);

        // Convert integer value to string.
        static StringExt *FromInt(int nValue);

        // Create a formatted string. Function is similar to printf, but without
        // buffer overflow problems. Format looks like this:
        //     {<nArg>:[<nWidth>][.<precision>]<type>}
        // where:
        // - <nArg> is argument number (numbering starts from 0).
        // -- PS: arguments must be provided in order, but can be used
        //    multiple times and in any order.
        // - <nWidth> is field width, if negative, alignment will be
        //   changed to opposite, and empty spaces will be filled with zeros.
        // - <nPrecision> is number of decimal places
        // - <nType> is one of the following types:
        //     d, x, o, b -- integer(int) in decimal, hexadecimal, octal
        //                   and binary number systems
        //     ud, ux, uo, ub -- same but unsigned integer(uint)
        //     ld, lx, lo, lb, uld, ulx, ulo, ulb -- similar for long and ulong
        //     f, g -- double
        //     c -- char
        //     s -- string (char *)
        //     t -- StringExt *
        //     w -- Spaces; argument value means number of spaces
        // To output curly braces use {{ and }}.
        static StringExt *Format (char *sFormat, ...);
        static StringExt *FormatV(char *sFormat, va_list sArgList);

        // Destructor.
        ~StringExt();

        int GetLength()
        {
            return m_nLength;
        }

        // Return string as char*.
        char *GetBuffer()
        {
            return m_sData;
        }

        char GetAt(int nIndex)
        {
            return m_sData[ nIndex ];
        }
        void SetAt(int nIndex, char nChar)
        {
            m_sData[ nIndex ] = nChar;
        }

        // Clear the string.
        StringExt *Clear();

        // Append a character or string.
        StringExt *Append(char nChar);
        StringExt *Append(StringExt *seString);
        StringExt *Append(const char *sString);
        StringExt *Append(const char *sString, int nLength);

        // Append a formatted string.
        StringExt *AppendFormat (char *sFormat, ...);
        StringExt *AppendFormatV(char *sFormat, va_list sArgList);

        // Insert a character or string.
        StringExt *Insert(int nIndex, char nChar);
        StringExt *Insert(int nIndex, StringExt  *seString);
        StringExt *Insert(int nIndex, const char *sString);
        StringExt *Insert(int nIndex, const char *sString, int nLength);

        // Delete one character or array of characters.
        StringExt *Delete(int nIndex, int nCount = 1);

        // Convert all characters in the string to uppercase/lowercase.
        StringExt *MakeUpper();
        StringExt *MakeLower();

        // Compare two strings:  -1:<  0:=  +1:>
        int Compare (StringExt *seString);
        int CompareN(StringExt *seString, int nCount);
        int Compare (const char *sString);
        int CompareN(const char *sString, int nCount);

    private:

        void Resize(int nLength);

        static void FormatInt   (         long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
        static void FormatUInt  (unsigned long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
        static void FormatDouble(       double nValue, char *sBuffer, int nBufferSize, int nPrecision, bool bTrim,            char **ppData, int *nLen);

    private:

        int   m_nLength;
        char *m_sData;

    };
}

#endif /* _ASC_FONTCONVERTER_STRING_EXT_H_ */
