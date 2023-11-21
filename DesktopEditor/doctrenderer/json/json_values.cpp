#include "json_values.h"

namespace NSJSON
{
	IBaseValue::IBaseValue()
	{
	}

	IBaseValue::~IBaseValue()
	{
	}

	CPrimitive::CPrimitive()
	{
	}

	CPrimitive::CPrimitive(bool value) : m_type(ptBoolean)
	{
		m_bool = value;
	}

	CPrimitive::CPrimitive(int value) : m_type(ptInteger)
	{
		m_int = value;
	}

	CPrimitive::CPrimitive(double value) : m_type(ptDouble)
	{
		m_double = value;
	}

	CPrimitive::CPrimitive(const std::string& str) : m_type(ptStringA)
	{
		new (&m_string) std::string(str);
	}

	CPrimitive::CPrimitive(const std::wstring& wstr) : m_type(ptStringW)
	{
		new (&m_wstring) std::wstring(wstr);
	}

	CPrimitive::CPrimitive(const CPrimitive& other)
	{
		this->set(other);
	}

	CPrimitive::~CPrimitive()
	{
		clear();
	}

	void CPrimitive::set(const CPrimitive& other)
	{
		clear();
		m_type = other.m_type;
		switch (m_type)
		{
		case ptBoolean:
			m_bool = other.m_bool;
			break;
		case ptInteger:
			m_int = other.m_int;
			break;
		case ptDouble:
			m_double = other.m_double;
			break;
		case ptStringA:
			new (&m_string) std::string(other.m_string);
			break;
		case ptStringW:
			new (&m_wstring) std::wstring(other.m_wstring);
			break;
		}
	}

	bool CPrimitive::isBool() const
	{
		return m_type == ptBoolean;
	}

	bool CPrimitive::isInt() const
	{
		return m_type == ptInteger;
	}

	bool CPrimitive::isDouble() const
	{
		return m_type == ptDouble;
	}

	bool CPrimitive::isStringA() const
	{
		return m_type == ptStringA;
	}

	bool CPrimitive::isStringW() const
	{
		return m_type == ptStringW;
	}

	bool CPrimitive::toBool() const
	{
		if (m_type == ptBoolean)
			return m_bool;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return false;
	}

	int CPrimitive::toInt() const
	{
		if (m_type == ptInteger)
			return m_int;
		if (m_type == ptDouble)
			return (int)m_double;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return 0;
	}

	double CPrimitive::toDouble() const
	{
		if (m_type == ptDouble)
			return m_double;
		if (m_type == ptInteger)
			return (double)m_double;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return 0.0;
	}

	std::string CPrimitive::toStringA() const
	{
		if (m_type == ptStringA)
			return m_string;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return "";
	}

	std::wstring CPrimitive::toStringW() const
	{
		if (m_type == ptStringW)
			return m_wstring;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return L"";
	}

	void CPrimitive::set(bool value)
	{
		clear();
		m_type = ptBoolean;
		m_bool = value;
	}

	void CPrimitive::set(int value)
	{
		clear();
		m_type = ptInteger;
		m_int = value;
	}

	void CPrimitive::set(double value)
	{
		clear();
		m_type = ptDouble;
		m_double = value;
	}

	void CPrimitive::set(const std::string& str)
	{
		clear();
		m_type = ptStringA;
		new (&m_string) std::string(str);
	}

	void CPrimitive::set(const std::wstring& wstr)
	{
		clear();
		m_type = ptStringW;
		new (&m_wstring) std::wstring(wstr);
	}

	void CPrimitive::clear()
	{
		switch (m_type)
		{
		case ptStringA:
			m_string.~basic_string<char>();
			break;
		case ptStringW:
			m_wstring.~basic_string<wchar_t>();
			break;
		default:
			break;
		}
	}

	CArray::CArray(int count) : m_values(count)
	{
	}

	CArray::CArray(std::initializer_list<CValue> elements) : m_values(elements)
	{
	}

	CArray::~CArray()
	{
	}

	int CArray::getCount() const
	{
		return (int)m_values.size();
	}

	CValue& CArray::get(int index)
	{
		return m_values[index];
	}

	CTypedArray::CTypedArray(BYTE* data, int len) : m_data(data), m_len(len)
	{
	}

	CTypedArray::~CTypedArray()
	{
		// TODO:
	}

	BYTE* CTypedArray::getData()
	{
		return m_data;
	}

	int CTypedArray::getCount() const
	{
		return m_len;
	}

	CObject::CObject()
	{
	}

	CObject::~CObject()
	{
	}

	CValue& CObject::get(const std::string& name)
	{
		return m_values[name];
	}

	std::vector<std::string> CObject::getPropertyNames()
	{
		std::vector<std::string> ret;
		for (const storage_t::value_type& entry : m_values)
		{
			if (!entry.second.IsUndefined())
				ret.push_back(entry.first);
		}
		return ret;
	}
}
