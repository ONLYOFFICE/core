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
#ifndef STRINGUTF32_HPP
#define STRINGUTF32_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include "../../Common/kernel_config.h"

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
		CStringUTF32(const uint32_t *data, const size_t &count);
		virtual ~CStringUTF32();

		bool empty() const;
		size_t length() const;

		std::wstring ToStdWString() const;

		bool operator==(const CStringUTF32 &right) const;
		bool operator!=(const CStringUTF32 &right) const;
		uint32_t &operator[](size_t index);

		CStringUTF32 &operator=(const CStringUTF32 &right);
		CStringUTF32 &operator=(const wchar_t *right);
		CStringUTF32 &operator=(const std::wstring &right);
		CStringUTF32 &operator=(const std::vector<uint32_t> &right);

		CStringUTF32 operator+(const CStringUTF32 &right) const;
		CStringUTF32 &operator+=(const CStringUTF32 &right);
		CStringUTF32 &operator+=(const uint32_t &symbol);

		CStringUTF32 substr(size_t start, size_t count) const;

		const uint32_t &at(size_t index) const;
		uint32_t &at(size_t index);
	};
} // namespace NSStringUtils

#endif // STRINGUTF32_HPP
