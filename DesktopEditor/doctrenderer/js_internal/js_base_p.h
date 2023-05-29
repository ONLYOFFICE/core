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
		NSJSBase::IsolateAdditionalDataType m_type;

		CEmdedClassInfo(NSJSBase::EmbedObjectCreator creator, const NSJSBase::IsolateAdditionalDataType& type = NSJSBase::iadtUndefined)
		{
			m_creator = creator;
			m_type = type;
		}
	};

public:
	using store_t = std::map<std::string, CEmdedClassInfo>;

	store_t m_infos;

private:
	CEmbedObjectRegistrator() = default;

public:
	void Register(const std::string& name,
				  NSJSBase::EmbedObjectCreator creator,
				  const NSJSBase::IsolateAdditionalDataType& type = NSJSBase::iadtUndefined)
	{
		m_infos.insert(std::pair<std::string, CEmdedClassInfo>(name, CEmdedClassInfo(creator, type)));
	}

	static CEmbedObjectRegistrator& getInstance()
	{
		static CEmbedObjectRegistrator oRegistrator;
		return oRegistrator;
	}
};

#endif // _CORE_EXT_JS_BASE_P_H_
