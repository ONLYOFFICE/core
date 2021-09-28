#ifndef STRINGUTF32_HPP
#define STRINGUTF32_HPP
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

#include <string>
#include <vector>
#include <stdint.h>
#include "StringExt.h"

namespace NSStringUtils
{
    class KERNEL_DECL CStringUTF32
    {
        std::vector<uint32_t> m_vec;
    public:
        CStringUTF32();
        CStringUTF32(const CStringUTF32 &other);
        CStringUTF32(const wchar_t *other);
        CStringUTF32(const std::wstring &other);
        CStringUTF32(const std::vector<uint32_t> &other);
        CStringUTF32(const uint32_t* data, const size_t& count);
        virtual ~CStringUTF32();

        bool empty() const;
        size_t length() const;

        std::wstring ToStdWString() const;

        bool        operator  == (const CStringUTF32 &right) const;
        bool        operator  != (const CStringUTF32 &right) const;
        uint32_t    &operator [] (size_t index);

        CStringUTF32 &operator  = (const CStringUTF32 &right);
        CStringUTF32 &operator  = (const wchar_t *right);
        CStringUTF32 &operator  = (const std::wstring &right);
        CStringUTF32 &operator  = (const std::vector<uint32_t> &right);

        CStringUTF32 operator   + (const CStringUTF32 &right) const;
        CStringUTF32 &operator += (const CStringUTF32 &right);
        CStringUTF32 &operator += (const uint32_t& symbol);

        CStringUTF32 substr(size_t start, size_t count) const;
    };
}

#endif // STRINGUTF32_HPP
