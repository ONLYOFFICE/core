#ifndef JSON_H_
#define JSON_H_

#include "../js_internal/js_base.h"

#include <string>
#include <unordered_map>

namespace NSJSON
{
	class CObject;
	class JS_DECL CValue
	{
	private:
		enum ValueType
		{
			vtUndefined,
			vtNull,
			vtBoolean,
			vtInteger,
			vtDouble,
			vtStringA,
			vtStringW,
		};

	public:
		CValue();
		CValue(const CValue& other);
		CValue(CValue&& other) = delete;
		// TODO: move constructor and assignment operator ???
		// TODO: constructors from value ???
		~CValue();

		CValue& operator=(const CValue& other);
		CValue& operator=(CValue&& other) = delete;
		CValue& operator=(const bool& value);
		CValue& operator=(const int& value);
		CValue& operator=(const double& value);
		CValue& operator=(const std::string& str);
		CValue& operator=(const std::wstring& wstr);

		// TODO: type cast operators ???

	public:
		void setUndefined();
		void setNull();
		bool isUndefined() const;
		bool isNull() const;
		// Transform C++ value to JS value
		JSSmart<NSJSBase::CJSValue> toJS() const;

	private:
		void clear();

	private:
		ValueType m_type;
		union
		{
			bool m_bool;
			int m_int;
			double m_double;
			std::string m_string;
			std::wstring m_wstring;
		};
	};

	class JS_DECL CObject
	{
	public:
		void addMember(const CValue* pValue, const std::string& name);
		void addMember(const CObject* pObject, const std::string& name);
		// Transform C++ object to JS object
		JSSmart<NSJSBase::CJSObject> toJS() const;

	private:
		std::unordered_map<std::string, const CValue*> m_values;
		std::unordered_map<std::string, const CObject*> m_objects;
	};
}

#endif	// JSON_H_
