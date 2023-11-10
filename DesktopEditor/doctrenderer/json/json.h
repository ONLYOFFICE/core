#ifndef JSON_H_
#define JSON_H_

#include "../js_internal/js_base.h"

#include <string>
#include <unordered_map>

namespace NSJSON
{
	class JS_DECL IBaseValue
	{
	protected:
		enum ValueType
		{
			vtUndefined,
			vtNull,
			vtBoolean,
			vtInteger,
			vtDouble,
			vtStringA,
			vtStringW,
			vtObject
		};

	public:
		IBaseValue();
		virtual ~IBaseValue();

	protected:
		ValueType m_type;

	public:
		virtual void setNull();
		virtual bool isUndefined() const;
		virtual bool isNull() const;
		// Transform C++ value to JS value
		virtual JSSmart<NSJSBase::CJSValue> toJS() const = 0;
	};

	class JS_DECL CValue : public IBaseValue
	{
	public:
		CValue();
		CValue(const CValue& other);
		CValue(CValue&& other) = delete;
		// TODO: move constructor and assignment operator ???
		// TODO: constructors from value ???
		virtual ~CValue();

		CValue& operator=(const CValue& other);
		CValue& operator=(CValue&& other) = delete;
		CValue& operator=(const bool& value);
		CValue& operator=(const int& value);
		CValue& operator=(const double& value);
		CValue& operator=(const std::string& str);
		CValue& operator=(const std::wstring& wstr);

		// TODO: type cast operators ???

	public:
		virtual void setNull() override;
		// Transform C++ value to JS value
		virtual JSSmart<NSJSBase::CJSValue> toJS() const override;

	private:
		void clear();

	private:
		union
		{
			bool m_bool;
			int m_int;
			double m_double;
			std::string m_string;
			std::wstring m_wstring;
		};
	};

	// extend this class to make custom objects serializable to JS
	class JS_DECL CObject : public IBaseValue
	{
	public:
		CObject();
		virtual ~CObject();

	public:
		// Add member to JS object when it will be serialized
		void addMember(const IBaseValue* pValue, const std::string& name);
		// Transform C++ object to JS object
		virtual JSSmart<NSJSBase::CJSValue> toJS() const override;

	private:
		std::unordered_map<std::string, const IBaseValue*> m_values;
	};
}

#endif	// JSON_H_
