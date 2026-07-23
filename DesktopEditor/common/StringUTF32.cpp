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
#include "StringUTF32.h"
#include "StringExt.h"

using namespace std;
using namespace NSStringUtils;

CStringUTF32::CStringUTF32()
{
}
CStringUTF32::CStringUTF32(const CStringUTF32& other) : m_vec(other.m_vec)
{
}
CStringUTF32::CStringUTF32(const wchar_t* other)
{
	*this = other;
}
CStringUTF32::CStringUTF32(const wstring& other)
{
	*this = other;
}
CStringUTF32::CStringUTF32(const vector<uint32_t>& other) : m_vec(other)
{
}

CStringUTF32::CStringUTF32(const uint32_t* data, const size_t& len)
{
	m_vec.reserve(len);
	for (size_t i = 0; i < len; ++i)
		this->m_vec.push_back(data[i]);
}

CStringUTF32::~CStringUTF32()
{
}

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

bool CStringUTF32::operator==(const CStringUTF32& right) const
{
	return this->m_vec == right.m_vec;
}

bool CStringUTF32::operator!=(const CStringUTF32& right) const
{
	return !(this->m_vec == right.m_vec);
}

uint32_t& CStringUTF32::operator[](size_t index)
{
	return this->m_vec[index];
}

CStringUTF32& CStringUTF32::operator=(const CStringUTF32& right)
{
	this->m_vec = right.m_vec;
	return *this;
}

CStringUTF32& CStringUTF32::operator=(const wchar_t* right)
{
	if (!right)
	{
		m_vec.clear();
		return *this;
	}

	return (*this = std::wstring(right));
}

CStringUTF32& CStringUTF32::operator=(const wstring& right)
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

	delete[] utf32;
	return *this;
}

CStringUTF32& CStringUTF32::operator=(const std::vector<uint32_t>& right)
{
	this->m_vec = right;
	return *this;
}

CStringUTF32 CStringUTF32::operator+(const CStringUTF32& right) const
{
	CStringUTF32 result;
	result.m_vec.reserve(this->length() + right.length());
	result.m_vec.insert(result.m_vec.end(), m_vec.begin(), m_vec.end());
	result.m_vec.insert(result.m_vec.end(), right.m_vec.begin(), right.m_vec.end());
	return result;
}

CStringUTF32& CStringUTF32::operator+=(const CStringUTF32& right)
{
	m_vec.insert(m_vec.end(), right.m_vec.begin(), right.m_vec.end());
	return *this;
}

CStringUTF32& CStringUTF32::operator+=(const uint32_t& symbol)
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

const uint32_t& CStringUTF32::at(size_t index) const
{
	return this->m_vec.at(index);
}
uint32_t& CStringUTF32::at(size_t index)
{
	return this->m_vec.at(index);
}
