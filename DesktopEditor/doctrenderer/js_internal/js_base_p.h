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

#ifndef _CORE_EXT_JS_BASE_P_H_
#define _CORE_EXT_JS_BASE_P_H_

#include "./js_base.h"
#include <map>
#include <mutex>

class CEmbedObjectRegistrator
{
public:
	class CEmdedClassInfo
	{
	public:
		NSJSBase::EmbedObjectCreator m_creator;
		bool m_bIsCreationAllowed;

		CEmdedClassInfo(NSJSBase::EmbedObjectCreator creator, const bool& bIsCreationAllowed = true)
		{
			m_creator = creator;
			m_bIsCreationAllowed = bIsCreationAllowed;
		}
	};

	using store_t = std::map<std::string, CEmdedClassInfo>;

	class CAllowedCreationScope
	{
	private:
		store_t::iterator m_oIter;
		bool m_bPrev;
		std::lock_guard<std::mutex> m_guard;

	public:
		CAllowedCreationScope(const store_t::iterator& iter) : m_oIter(iter), m_guard(getInstance().m_mutex)
		{
			m_bPrev = m_oIter->second.m_bIsCreationAllowed;
			m_oIter->second.m_bIsCreationAllowed = true;
		}

		~CAllowedCreationScope()
		{
			m_oIter->second.m_bIsCreationAllowed = m_bPrev;
		}
	};

public:
	store_t m_infos;
	std::mutex m_mutex;

private:
	CEmbedObjectRegistrator() = default;

public:
	void Register(const std::string& name,
				  NSJSBase::EmbedObjectCreator creator,
				  const bool& bIsCreationAllowed = true)
	{
		m_infos.insert(std::pair<std::string, CEmdedClassInfo>(name, CEmdedClassInfo(creator, bIsCreationAllowed)));
	}

	CAllowedCreationScope* AllowCreationInScope(const std::string& name)
	{
		store_t::iterator iter = m_infos.find(name);
		if (iter == m_infos.end())
			return nullptr;
		return new CAllowedCreationScope(iter);
	}

	static CEmbedObjectRegistrator& getInstance()
	{
		static CEmbedObjectRegistrator oRegistrator;
		return oRegistrator;
	}
};

#endif // _CORE_EXT_JS_BASE_P_H_
