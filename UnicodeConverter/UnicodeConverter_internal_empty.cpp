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
#include "./UnicodeConverter.h"
#include "../DesktopEditor/common/File.h"

namespace NSUnicodeConverter
{
    class CUnicodeConverter_Private
    {
    public:
        CUnicodeConverter_Private()
        {
        }
        ~CUnicodeConverter_Private()
        {
        }

        std::string SASLprepToUtf8(const wchar_t* sInput, const unsigned int& nInputLen)
        {
            // TODO:
            return "";
        }

        std::string fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            std::string sRes = "";
            if (sRes.empty() && nInputLen > 0)
            {
                std::wstring ws(sInput, nInputLen);
                sRes = std::string(ws.begin(), ws.end());
            }
            return sRes;
        }

        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact)
        {
            std::wstring sRes = L"";
            if (!isExact && sRes.empty() && nInputLen > 0)
            {
                std::string ws(sInput, nInputLen);
                sRes = std::wstring(ws.begin(), ws.end());
            }
            return sRes;
        }
        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact)
        {
            std::wstring sRes = L"";
            if (isExact && sRes.empty() && nInputLen > 0)
            {
                std::string ws(sInput, nInputLen);
                sRes = std::wstring(ws.begin(), ws.end());
            }
            return sRes;
        }
    };
}

namespace NSUnicodeConverter
{
    CUnicodeConverter::CUnicodeConverter()
    {
        m_pInternal = new CUnicodeConverter_Private();
    }
    CUnicodeConverter::~CUnicodeConverter()
    {
        delete m_pInternal;
    }

    std::string CUnicodeConverter::fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
    {
        return m_pInternal->fromUnicode(sInput, nInputLen, converterName);
    }
    std::string CUnicodeConverter::fromUnicode(const std::wstring &sInput, const char *converterName)
    {
        return this->fromUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
    }
    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, converterName, isExact);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sInput, const char *converterName, bool isExact)
    {
        return this->toUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName, isExact);
    }
    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, nCodePage, isExact);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sInput, int nCodePage, bool isExact)
    {
        return this->toUnicode(sInput.c_str(), (unsigned int)sInput.size(), nCodePage, isExact);
    }
    std::string CUnicodeConverter::SASLprepToUtf8(const std::wstring &sSrc)
    {
        return m_pInternal->SASLprepToUtf8(sSrc.c_str(), sSrc.length());
    }
}
