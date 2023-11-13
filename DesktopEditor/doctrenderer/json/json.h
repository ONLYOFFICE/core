#ifndef JSON_H_
#define JSON_H_

#include "../js_internal/js_base.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace NSJSON
{
	class IBaseValue;
	// Transform C++ value to JS value
	JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);

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
			vtArray,
			vtTypedArray,
			vtObject
		};

	public:
		IBaseValue();
		IBaseValue(ValueType type);
		virtual ~IBaseValue();

	public:
		virtual void setNull();
		virtual bool isUndefined() const;
		virtual bool isNull() const;

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);

	protected:
		ValueType m_type;
	};

	// for primitive values
	class JS_DECL CValue : public IBaseValue
	{
	public:
		CValue();
		CValue(const CValue& other);
		CValue(CValue&& other) = delete;
		// TODO: move constructor and assignment operator ???
		CValue(const bool& value);
		CValue(const int& value);
		CValue(const double& value);
		CValue(const std::string& str);
		CValue(const std::wstring& wstr);
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

	private:
		void clear();

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);

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

	class JS_DECL CArray : public IBaseValue
	{
	public:
		CArray();
		// elements get deleted on array destruction
		virtual ~CArray();

	public:
		void add(IBaseValue* value);
		void addNull();
		void addUndefined();
		int getCount() const;

		IBaseValue* operator[](int index);
		const IBaseValue* operator[](int index) const;

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);

	private:
		std::vector<IBaseValue*> m_values;
	};

	class JS_DECL CTypedArray : public IBaseValue
	{
	public:
		CTypedArray(BYTE* data = nullptr, int len = 0);
		// elements get deleted on typed array destruction
		virtual ~CTypedArray();

	public:
		BYTE* getData();
		const BYTE* getData() const;
		int getCount() const;

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);

	private:
		BYTE* m_data;
		int m_len;
	};

	// extend this class to make custom objects serializable to JS
	class JS_DECL CObject : public IBaseValue
	{
	public:
		CObject() = default;
		virtual ~CObject() = default;

	public:
		// Add member to JS object when it will be serialized
		void addMember(const IBaseValue* pValue, const std::string& name);

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);

	private:
		std::unordered_map<std::string, const IBaseValue*> m_values;
	};
}

#endif	// JSON_H_
