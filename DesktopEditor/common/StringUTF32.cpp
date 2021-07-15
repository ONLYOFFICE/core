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
#include "StringUTF32.hpp"

using namespace std;
using namespace NSStringExt;
using namespace NSStringUtils;

StringUTF32::StringUTF32() {}

StringUTF32::StringUTF32(const StringUTF32 &other): _vec(other._vec) {}

StringUTF32::StringUTF32(const wchar_t *other) {
    *this = other;
}

StringUTF32::StringUTF32(const wstring &other) {
    *this = other;
}

StringUTF32::StringUTF32(const vector<uint32_t> &other): _vec(other) {}

StringUTF32::~StringUTF32() {}


size_t StringUTF32::Length() const {
    return this->_vec.size();
}

void StringUTF32::Swap(StringUTF32 &other) {
    StringUTF32 tmp = *this;
    *this = other;
    other = tmp;
}


wstring StringUTF32::ToUTF16() const {
    return CConverter::GetUnicodeFromUTF32(&this->_vec[0], static_cast<long>(this->Length()));
}


bool StringUTF32::Equals(const StringUTF32 &other) const {
    return this->_vec == other._vec;
}

uint32_t &StringUTF32::GetCharAt(size_t index) {
    return this->_vec[index];
}

StringUTF32 &StringUTF32::Concatenate(const StringUTF32 &other) {
    this->_vec.reserve(this->Length() + other.Length());
    for (size_t i = 0; i < other.Length(); ++i)
        this->_vec.push_back(other._vec[i]);
    return *this;
}


bool StringUTF32::operator == (const StringUTF32 &right) const {
    return this->Equals(right);
}

bool StringUTF32::operator != (const StringUTF32 &right) const {
    return !this->Equals(right);
}

uint32_t &StringUTF32::operator [] (size_t index) {
    return this->GetCharAt(index);
}


StringUTF32 &StringUTF32::operator = (const StringUTF32 &right) {
    this->_vec = right._vec;
    return *this;
}

StringUTF32 &StringUTF32::operator = (const wchar_t *right) {
    if (!right) {
        throw runtime_error("Invalid argument! \"right\" must be a valid non-NULL pointer.");
    }

    wstring tmp = L"";
    size_t length = 0;
    while (right[length] != '\0') {
        tmp += right[length++];
    }

    return (*this = tmp);
}

StringUTF32 &StringUTF32::operator = (const wstring &right) {
    unsigned int utf32_len = 0;
    uint32_t *utf32 = CConverter::GetUtf32FromUnicode(right, utf32_len);
    if (!utf32) {
        throw runtime_error("Operation aborted! Function \"GetUtf32FromUnicode\" returned NULL-pointer.");
    }

    this->_vec.clear();
    this->_vec.reserve(utf32_len);
    for (size_t i = 0; i < utf32_len; ++i) {
        this->_vec.push_back(utf32[i]);
    }

    return *this;
}

StringUTF32 &StringUTF32::operator = (const std::vector<uint32_t> &right) {
    this->_vec = right;
    return *this;
}


StringUTF32 StringUTF32::operator + (const StringUTF32 &right) const {
    StringUTF32 result;
    result._vec.reserve(this->Length() + right.Length());
    return result.Concatenate(*this).Concatenate(right);
}

StringUTF32 &StringUTF32::operator += (const StringUTF32 &right) {
    return this->Concatenate(right);
}
