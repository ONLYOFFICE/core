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

#ifndef JSON_VALUES_H_
#define JSON_VALUES_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "json.h"

namespace NSJSON
{
	class IBaseValue
	{
	public:
		IBaseValue();
		virtual ~IBaseValue();
	};

	class CPrimitive : public IBaseValue
	{
	private:
		enum PrimitiveType
		{
			ptBoolean,
			ptInteger,
			ptDouble,
			ptStringA,
			ptStringW
		};

	public:
		CPrimitive(bool value);
		CPrimitive(int value);
		CPrimitive(double value);
		CPrimitive(const std::string& str);
		CPrimitive(const std::wstring& wstr);
		~CPrimitive();

		// disable copy
		CPrimitive(const CPrimitive& other) = delete;
		CPrimitive& operator=(const CPrimitive& other) = delete;

		// type check
		bool isBool() const;
		bool isInt() const;
		bool isDouble() const;
		bool isStringA() const;
		bool isStringW() const;

		// getters
		bool toBool() const;
		int toInt() const;
		double toDouble() const;
		std::string toStringA() const;
		std::wstring toStringW() const;

	private:
		union
		{
			bool m_bool;
			int m_int;
			double m_double;
			std::string m_string;
			std::wstring m_wstring;
		};
		PrimitiveType m_type;
	};

	class CArray : public IBaseValue
	{
	public:
		CArray(int count);
		CArray(std::initializer_list<CValue> elements);
		~CArray();

	public:
		int getCount() const;
		CValue& get(int index);

	private:
		std::vector<CValue> m_values;
	};

	class CTypedArray : public IBaseValue
	{
	public:
		CTypedArray(BYTE* data, int len, bool isExternalize = true);
		~CTypedArray();

	public:
		BYTE* getData();
		int getCount() const;

	private:
		BYTE* m_data;
		int m_len;
		bool m_isExternalize;
	};

	class CObject : public IBaseValue
	{
	private:
		using storage_t = std::unordered_map<std::string, CValue>;

	public:
		CObject();
		~CObject();

	public:
		CValue& get(const std::string& name);
		std::vector<std::string> getPropertyNames();

	private:
		storage_t m_values;
	};

	class CImage : public IBaseValue
	{
	public:
		CImage(BYTE* bits, const int& width, const int& height, const ImageFormat& format, const bool& isExternalize = true);
		~CImage();

	public:
		BYTE* getBits();
		int getWidth();
		int getHeight();
		ImageFormat getFormat();
		void externalize();
		void alloc(const int& width, const int& height, const ImageFormat& format);

	private:
		BYTE* m_bits;
		int m_width;
		int m_height;
		ImageFormat m_format;
		bool m_isExternalize;
	};
} // namespace

#endif	// JSON_VALUES_H_
