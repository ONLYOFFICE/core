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
