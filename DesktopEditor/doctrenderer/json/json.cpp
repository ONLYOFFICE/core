#include "json.h"

namespace NSJSON
{
	CValue::CValue() : m_type(vtUndefined)
	{
	}

	CValue::CValue(const CValue& other) : m_type(other.m_type)
	{
		*this = other;
	}

	CValue::~CValue()
	{
		clear();
	}

	CValue& CValue::operator=(const CValue& other)
	{
		clear();
		switch (m_type)
		{
		case vtBoolean:
			m_bool = other.m_bool;
			break;
		case vtInteger:
			m_int = other.m_int;
			break;
		case vtDouble:
			m_double = other.m_double;
			break;
		case vtStringA:
			new (&m_string) std::string(other.m_string);
			break;
		case vtStringW:
			new (&m_wstring) std::wstring(other.m_wstring);
			break;
		default:
			break;
		}
		return *this;
	}

	CValue& CValue::operator=(const bool& value)
	{
		clear();
		m_type = vtBoolean;
		m_bool = value;
		return *this;
	}

	CValue& CValue::operator=(const int& value)
	{
		clear();
		m_type = vtInteger;
		m_int = value;
		return *this;
	}

	CValue& CValue::operator=(const double& value)
	{
		clear();
		m_type = vtDouble;
		m_double = value;
		return *this;
	}

	CValue& CValue::operator=(const std::string& str)
	{
		clear();
		m_type = vtStringA;
		new (&m_string) std::string(str);
		return *this;
	}

	CValue& CValue::operator=(const std::wstring& wstr)
	{
		clear();
		m_type = vtStringW;
		new (&m_wstring) std::wstring(wstr);
		return *this;
	}

	void CValue::setUndefined()
	{
		clear();
		m_type = vtUndefined;
	}

	void CValue::setNull()
	{
		clear();
		m_type = vtNull;
	}

	bool CValue::isUndefined() const
	{
		return m_type == vtUndefined;
	}

	bool CValue::isNull() const
	{
		return m_type == vtNull;
	}

	JSSmart<NSJSBase::CJSValue> CValue::toJS() const
	{
		JSSmart<NSJSBase::CJSValue> ret;
		switch (m_type) {
		case vtUndefined:
			ret = NSJSBase::CJSContext::createUndefined();
			break;
		case vtNull:
			ret = NSJSBase::CJSContext::createNull();
			break;
		case vtBoolean:
			ret = NSJSBase::CJSContext::createBool(m_bool);
			break;
		case vtInteger:
			ret = NSJSBase::CJSContext::createInt(m_int);
			break;
		case vtDouble:
			ret = NSJSBase::CJSContext::createDouble(m_double);
			break;
		case vtStringA:
			ret = NSJSBase::CJSContext::createString(m_string);
			break;
		case vtStringW:
			ret = NSJSBase::CJSContext::createString(m_wstring);
			break;
		}
		return ret;
	}

	void CValue::clear()
	{
		switch (m_type)
		{
		case vtBoolean:
			m_bool = false;
			break;
		case vtInteger:
			m_int = 0;
			break;
		case vtDouble:
			m_double = 0.0;
			break;
		case vtStringA:
			m_string.~basic_string<char>();
			break;
		case vtStringW:
			m_wstring.~basic_string<wchar_t>();
			break;
		default:
			break;
		}
	}

	void CObject::addMember(const CValue* pValue, const std::string& name)
	{
		m_values[name] = pValue;
	}

	void CObject::addMember(const CObject* pObject, const std::string& name)
	{
		m_objects[name] = pObject;
	}

	JSSmart<NSJSBase::CJSObject> CObject::toJS() const
	{
		JSSmart<NSJSBase::CJSObject> ret = NSJSBase::CJSContext::createObject();
		// set values
		for (const auto& entry : m_values)
		{
			if (entry.second)
			{
				JSSmart<NSJSBase::CJSValue> jsValue = entry.second->toJS();
				ret->set(entry.first.c_str(), jsValue);
			}
		}
		// set inner objects
		for (const auto& entry : m_objects)
		{
			if (entry.second)
			{
				JSSmart<NSJSBase::CJSObject> jsObject = entry.second->toJS();
				ret->set(entry.first.c_str(), jsObject);
			}
		}
		return ret;
	}
}
