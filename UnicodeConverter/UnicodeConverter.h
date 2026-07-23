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
#ifndef _SERVER_COMPONENTS_UNICODE_CONVERTER_H
#define _SERVER_COMPONENTS_UNICODE_CONVERTER_H

#ifndef UNICODECONVERTER_USE_DYNAMIC_LIBRARY
#define UNICODECONVERTER_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define UNICODECONVERTER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <string>
#include "UnicodeConverter_Encodings.h"

namespace NSUnicodeConverter
{
    class CUnicodeConverter_Private;
    class UNICODECONVERTER_DECL_EXPORT CUnicodeConverter
	{
	public:
        CUnicodeConverter();
        ~CUnicodeConverter();

        std::string fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName);
        std::string fromUnicode(const std::wstring& sSrc, const char* converterName);

        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact = false);
        std::wstring toUnicode(const std::string& sSrc, const char* converterName, bool isExact = false);

        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact = false);
        std::wstring toUnicode(const std::string& sSrc, int nCodePage, bool isExact = false);

        std::string SASLprepToUtf8(const std::wstring &sSrc);

        // use this only for static icu builds
        static void setIcuDataPath(const std::wstring& sDirectory);
    private:
        CUnicodeConverter_Private* m_pInternal;
	};
}

#endif // _SERVER_COMPONENTS_UNICODE_CONVERTER_H
