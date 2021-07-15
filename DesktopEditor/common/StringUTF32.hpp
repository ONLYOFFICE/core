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
#include "StringExt.h"

namespace NSStringUtils {
    class KERNEL_DECL StringUTF32 {
        std::vector<uint32_t> _vec;

    public:
        StringUTF32();
        StringUTF32(const StringUTF32 &other);
        StringUTF32(const wchar_t *other);
        StringUTF32(const std::wstring &other);
        StringUTF32(const std::vector<uint32_t> &other);
        virtual ~StringUTF32();

        virtual size_t Length() const;
        virtual void Swap(StringUTF32 &other);

        virtual std::wstring ToUTF16() const;

        virtual bool Equals(const StringUTF32 &other) const;
        virtual uint32_t &GetCharAt(size_t index);
        virtual StringUTF32 &Concatenate(const StringUTF32 &other);

        virtual bool        operator  == (const StringUTF32 &right) const;
        virtual bool        operator  != (const StringUTF32 &right) const;
        virtual uint32_t    &operator [] (size_t index);

        virtual StringUTF32 &operator  = (const StringUTF32 &right);
        virtual StringUTF32 &operator  = (const wchar_t *right);
        virtual StringUTF32 &operator  = (const std::wstring &right);
        virtual StringUTF32 &operator  = (const std::vector<uint32_t> &right);

        virtual StringUTF32 operator   + (const StringUTF32 &right) const;
        virtual StringUTF32 &operator += (const StringUTF32 &right);
    };
}

#endif // STRINGUTF32_HPP
