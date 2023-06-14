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
