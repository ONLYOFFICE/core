/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
}

#endif	// JSON_VALUES_H_
