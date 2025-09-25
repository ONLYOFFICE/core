/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
