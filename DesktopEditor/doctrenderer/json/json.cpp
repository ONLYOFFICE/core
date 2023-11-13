#include "json.h"

namespace NSJSON
{
	IBaseValue::IBaseValue() : m_type(vtUndefined)
	{
	}

	IBaseValue::~IBaseValue()
	{
	}

	void IBaseValue::setNull()
	{
		m_type = vtNull;
	}

	bool IBaseValue::isUndefined() const
	{
		return m_type == vtUndefined;
	}

	bool IBaseValue::isNull() const
	{
		return m_type == vtNull;
	}

	CValue::CValue()
	{
	}

	CValue::CValue(const CValue& other)
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
		m_type = other.m_type;
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

	void CValue::setNull()
	{
		clear();
		m_type = vtNull;
	}

	void CValue::clear()
	{
		switch (m_type)
		{
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

	CObject::CObject()
	{
	}

	CObject::~CObject()
	{
	}

	void CObject::addMember(const IBaseValue* pValue, const std::string& name)
	{
		if (m_type != vtObject)
			m_type = vtObject;
		m_values[name] = pValue;
	}
}
