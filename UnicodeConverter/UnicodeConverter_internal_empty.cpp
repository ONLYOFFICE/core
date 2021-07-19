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
