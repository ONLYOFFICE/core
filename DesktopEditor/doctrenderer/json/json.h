#ifndef JSON_H_
#define JSON_H_

#include "../js_internal/js_base.h"

#include <string>
#include <unordered_map>
#include <vector>

// enable to see debug output and exceptions
//#define JSON_DEBUG

namespace NSJSON
{
	class IBaseValue;
	// Transform C++ value to JS value
	JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);
	// Transform JS value to C++ value
	IBaseValue* fromJS(JSSmart<NSJSBase::CJSValue> jsValue);

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
		friend IBaseValue* fromJS(JSSmart<NSJSBase::CJSValue> jsValue);

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
		// TODO: constructor and assignment operator from (const char*/wchar_t*)
		CValue(const std::string& str);
		CValue(const std::wstring& wstr);
		virtual ~CValue();

		// getters
		bool toBool() const;
		// TODO: should cast to int handle the case when current value is double and vise-versa ?
		//		 If so, then cast to bool can handle other values too ?
		int toInt() const;
		double toDouble() const;
		std::string toStringA() const;
		std::wstring toStringW() const;

		// TODO: setters
		CValue& operator=(const CValue& other);
		CValue& operator=(CValue&& other) = delete;
		CValue& operator=(const bool& value);
		CValue& operator=(const int& value);
		CValue& operator=(const double& value);
		CValue& operator=(const std::string& str);
		CValue& operator=(const std::wstring& wstr);

	public:
		virtual void setNull() override;

	private:
		void clear();

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);
		friend IBaseValue* fromJS(JSSmart<NSJSBase::CJSValue> jsValue);

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
		friend IBaseValue* fromJS(JSSmart<NSJSBase::CJSValue> jsValue);

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
		friend IBaseValue* fromJS(JSSmart<NSJSBase::CJSValue> jsValue);

	private:
		BYTE* m_data;
		int m_len;
	};

	class JS_DECL CObject : public IBaseValue
	{
	private:
		// Need to store smart pointers to IBaseValue instead of raw pointers, because user can reset members with new values
		using storage_t = std::unordered_map<std::string, IBaseValue*>;

	public:
		CObject();
		virtual ~CObject();

	public:
		void set(const std::string& name, IBaseValue* pValue);
		// TODO: implement getValue, getObject, getArray, getTypedArray so there is no need to cast to needed type explicitly
		IBaseValue* get(const std::string& name);
		IBaseValue* operator[](const std::string& name);

		friend JSSmart<NSJSBase::CJSValue> toJS(const IBaseValue* pValue);
		friend IBaseValue* fromJS(JSSmart<NSJSBase::CJSValue> jsValue);

	private:
		storage_t m_values;
	};
}

#endif	// JSON_H_
