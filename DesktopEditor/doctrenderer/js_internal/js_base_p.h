#ifndef _CORE_EXT_JS_BASE_P_H_
#define _CORE_EXT_JS_BASE_P_H_

#include "./js_base.h"
#include <map>
#include <unordered_map>

class CEmbedObjectRegistrator
{
public:
	class CEmdedClassInfo
	{
	public:
		NSJSBase::EmbedObjectCreator m_creator;
		NSJSBase::IsolateAdditionalDataType m_type;

		CEmdedClassInfo(NSJSBase::EmbedObjectCreator creator, const NSJSBase::IsolateAdditionalDataType& type = NSJSBase::iadtUndefined)
		{
			m_creator = creator;
			m_type = type;
		}
	};

public:
	std::map<std::string, CEmdedClassInfo> m_infos;

public:
	void Register(const std::string& name,
				  NSJSBase::EmbedObjectCreator creator,
				  const NSJSBase::IsolateAdditionalDataType& type = NSJSBase::iadtUndefined)
	{
		m_infos.insert(std::pair<std::string, CEmdedClassInfo>(name, CEmdedClassInfo(creator, type)));
	}
};

class CEmbedObjectRegistratorPool
{
private:
	std::unordered_map<void*, CEmbedObjectRegistrator> m_registrators;

public:
	CEmbedObjectRegistrator& getRegistrator(void* ptr)
	{
		return m_registrators[ptr];
	}
};

#endif // _CORE_EXT_JS_BASE_P_H_
