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
#include "StringUTF32.h"

using namespace std;
using namespace NSStringUtils;

CStringUTF32::CStringUTF32() {}
CStringUTF32::CStringUTF32(const CStringUTF32 &other): m_vec(other.m_vec) {}
CStringUTF32::CStringUTF32(const wchar_t *other) { *this = other; }
CStringUTF32::CStringUTF32(const wstring &other) { *this = other; }
CStringUTF32::CStringUTF32(const vector<uint32_t> &other): m_vec(other) {}

CStringUTF32::CStringUTF32(const uint32_t* data, const size_t& len)
{
    m_vec.reserve(len);
    for (size_t i = 0; i < len; ++i)
        this->m_vec.push_back(data[i]);
}

CStringUTF32::~CStringUTF32() {}

bool CStringUTF32::empty() const
{
    return this->m_vec.empty();
}

size_t CStringUTF32::length() const
{
    return this->m_vec.size();
}

wstring CStringUTF32::ToStdWString() const
{
    if (0 == length())
        return L"";
    return NSStringExt::CConverter::GetUnicodeFromUTF32(&this->m_vec[0], static_cast<long>(this->length()));
}

bool CStringUTF32::operator == (const CStringUTF32 &right) const
{
    return this->m_vec == right.m_vec;
}

bool CStringUTF32::operator != (const CStringUTF32 &right) const
{
    return !(this->m_vec == right.m_vec);
}

uint32_t& CStringUTF32::operator [] (size_t index)
{
    return this->m_vec[index];
}

CStringUTF32& CStringUTF32::operator = (const CStringUTF32 &right)
{
    this->m_vec = right.m_vec;
    return *this;
}

CStringUTF32& CStringUTF32::operator = (const wchar_t *right)
{
    if (!right)
    {
        m_vec.clear();
        return *this;
    }

    return (*this = std::wstring(right));
}

CStringUTF32& CStringUTF32::operator = (const wstring &right)
{
    if (right.empty())
    {
        m_vec.clear();
        return *this;
    }

    unsigned int utf32_len = 0;
    unsigned int* utf32 = NSStringExt::CConverter::GetUtf32FromUnicode(right, utf32_len);
    this->m_vec.clear();
    this->m_vec.reserve(utf32_len);
    for (size_t i = 0; i < utf32_len; ++i)
        this->m_vec.push_back(utf32[i]);

    delete [] utf32;
    return *this;
}

CStringUTF32& CStringUTF32::operator = (const std::vector<uint32_t> &right)
{
    this->m_vec = right;
    return *this;
}


CStringUTF32 CStringUTF32::operator + (const CStringUTF32 &right) const
{
    CStringUTF32 result;
    result.m_vec.reserve(this->length() + right.length());
    result.m_vec.insert(result.m_vec.end(), m_vec.begin(), m_vec.end());
    result.m_vec.insert(result.m_vec.end(), right.m_vec.begin(), right.m_vec.end());
    return result;
}

CStringUTF32& CStringUTF32::operator += (const CStringUTF32 &right)
{
    m_vec.insert(m_vec.end(), right.m_vec.begin(), right.m_vec.end());
    return *this;
}

CStringUTF32& CStringUTF32::operator += (const uint32_t& symbol)
{
    m_vec.push_back(symbol);
    return *this;
}

CStringUTF32 CStringUTF32::substr(size_t start, size_t count) const
{
    CStringUTF32 result;
    result.m_vec.reserve(count);
    result.m_vec.insert(result.m_vec.end(), m_vec.begin() + start, m_vec.begin() + start + count);
    return result;
}
