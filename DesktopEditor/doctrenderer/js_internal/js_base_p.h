#ifndef _CORE_EXT_JS_BASE_P_H_
#define _CORE_EXT_JS_BASE_P_H_

#include "./js_base.h"
#include <map>

class CEmbedObjectRegistrator
{
public:
	class CEmdedClassInfo
	{
	public:
		NSJSBase::EmbedObjectCreator m_creator;
		NSJSBase::IsolateAdditionlDataType m_type;

		CEmdedClassInfo(NSJSBase::EmbedObjectCreator creator, const NSJSBase::IsolateAdditionlDataType& type = NSJSBase::iadtUndefined)
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
				  const NSJSBase::IsolateAdditionlDataType& type = NSJSBase::iadtUndefined)
	{
		m_infos.insert(std::pair<std::string, CEmdedClassInfo>(name, CEmdedClassInfo(creator, type)));
	}
};

#endif // _CORE_EXT_JS_BASE_P_H_
