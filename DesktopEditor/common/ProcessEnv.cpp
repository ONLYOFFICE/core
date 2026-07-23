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

#include "./ProcessEnv.h"
#include <map>
#include "./StringBuilder.h"
#include "./File.h"

namespace NSProcessEnv
{
	class CProps
	{
	public:
		std::map<std::string, std::string> m_props;

	public:
		CProps()
		{
		}

		static CProps& getInstance()
		{
			static CProps props;
			return props;
		}

		void Load(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> childs;
			if (node.GetChilds(childs))
			{
				for (std::vector<XmlUtils::CXmlNode>::iterator iter = childs.begin(); iter != childs.end(); iter++)
				{
					m_props.insert(std::make_pair(iter->GetNameA(), iter->GetTextA()));
				}
			}
		}

		std::string Save()
		{
			if (0 == m_props.size())
				return "";

			NSStringUtils::CStringBuilder oBuilder;
			oBuilder.WriteString(L"<options>");

			for (std::map<std::string, std::string>::iterator iter = m_props.begin(); iter != m_props.end(); iter++)
			{
				std::wstring name = UTF8_TO_U(iter->first);
				oBuilder.AddCharSafe('<');
				oBuilder.WriteString(name);
				oBuilder.AddCharSafe('>');

				const std::string& value = iter->second;
				oBuilder.WriteEncodeXmlString(UTF8_TO_U(value));

				oBuilder.WriteString(L"</");
				oBuilder.WriteString(name);
				oBuilder.AddCharSafe('>');
			}

			oBuilder.WriteString(L"</options>");

			std::wstring sData = oBuilder.GetData();
			return U_TO_UTF8(sData);
		}

		inline bool IsPresent(const char* key)
		{
			std::map<std::string, std::string>::const_iterator iter = m_props.find(std::string(key));
			return (iter == m_props.end()) ? false : true;
		}

		inline bool GetBoolValue(const char* key)
		{
			std::map<std::string, std::string>::const_iterator iter = m_props.find(std::string(key));
			if (iter == m_props.end())
				return false;

			if ("1" == iter->second || "true" == iter->second)
				return true;
			return false;
		}
		inline int GetIntValue(const char* key)
		{
			std::map<std::string, std::string>::const_iterator iter = m_props.find(std::string(key));
			if (iter == m_props.end())
				return 0;

			return std::stoi(iter->second);
		}
		inline std::string GetStringValueA(const char* key)
		{
			std::map<std::string, std::string>::const_iterator iter = m_props.find(std::string(key));
			if (iter == m_props.end())
				return "";

			return iter->second;
		}
		inline std::wstring GetStringValue(const char* key)
		{
			std::map<std::string, std::string>::const_iterator iter = m_props.find(std::string(key));
			if (iter == m_props.end())
				return L"";

			return UTF8_TO_U(iter->second);
		}
	};

	void Load(XmlUtils::CXmlNode& node)
	{
		CProps::getInstance().Load(node);
	}
	void Load(const std::string& strUtf8)
	{
		XmlUtils::CXmlNode node;
		if (node.FromXmlStringA(strUtf8))
			Load(node);
	}
	void Load(const std::wstring& str)
	{
		XmlUtils::CXmlNode node;
		if (node.FromXmlString(str))
			Load(node);
	}

	std::string Save()
	{
		return CProps::getInstance().Save();
	}

	// GET
	bool IsPresent(const char* key)
	{
		return CProps::getInstance().IsPresent(key);
	}

	bool GetBoolValue(const char* key)
	{
		return CProps::getInstance().GetBoolValue(key);
	}
	int GetIntValue(const char* key)
	{
		return CProps::getInstance().GetIntValue(key);
	}
	std::string GetStringValueA(const char* key)
	{
		return CProps::getInstance().GetStringValueA(key);
	}
	std::wstring GetStringValue(const char* key)
	{
		return CProps::getInstance().GetStringValue(key);
	}

	// SET
	bool SetBoolValue(const char* key, const bool& value)
	{
		bool res = IsPresent(key);
		CProps::getInstance().m_props.insert(std::make_pair<std::string, std::string>(key, value ? "1" : "0"));
		return res;
	}
	bool SetIntValue(const char* key, const int& value)
	{
		bool res = IsPresent(key);
		CProps::getInstance().m_props.insert(std::make_pair<std::string, std::string>(key, std::to_string(value)));
		return res;
	}
	bool SetStringValueA(const char* key, const char* value)
	{
		bool res = IsPresent(key);
		CProps::getInstance().m_props.insert(std::make_pair<std::string, std::string>(key, value));
		return res;
	}
	bool SetStringValue(const char* key, const wchar_t* value)
	{
		bool res = IsPresent(key);
		std::wstring tmp(value);
		CProps::getInstance().m_props.insert(std::make_pair<std::string, std::string>(key, U_TO_UTF8(tmp)));
		return res;
	}
} // namespace NSProcessEnv
