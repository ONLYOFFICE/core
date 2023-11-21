#include "json.h"
#include "json_p.h"
#include "json_values.h"

namespace NSJSON
{
	CTypedValue::CTypedValue() : m_type(vtUndefined)
	{
	}

	CTypedValueContainer::CTypedValueContainer() : m_typedValue(new CTypedValue()), m_isReference(false)
	{
	}

	CTypedValueContainer::CTypedValueContainer(const CTypedValueContainer& other) : CTypedValueContainer()
	{
		*this = other;
	}

	CTypedValueContainer& CTypedValueContainer::operator=(const CTypedValueContainer& other)
	{
		if (other.m_isReference)
		{
			// set pointer to the other's typed value
			m_typedValue = other.m_typedValue;
		}
		else
		{
			// copy typed value without changing pointer to it
			*m_typedValue = *other.m_typedValue;
		}
		return *this;
	}

	CValue::CValue() : m_internal(new CTypedValueContainer())
	{
	}

	CValue::CValue(const CValue& other) : m_internal(new CTypedValueContainer(*other.m_internal))
	{
	}

	CValue::~CValue()
	{
		delete m_internal;
	}

	CValue& CValue::operator=(const CValue& other)
	{
		*m_internal = *other.m_internal;
		return *this;
	}

	bool CValue::IsUndefined() const
	{
		return m_internal->m_typedValue->m_type == CTypedValue::vtUndefined;
	}

	bool CValue::IsNull() const
	{
		return m_internal->m_typedValue->m_type == CTypedValue::vtNull;
	}

	bool CValue::IsBool() const
	{
		return (m_internal->m_typedValue->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->isBool());
	}

	bool CValue::IsInt() const
	{
		return (m_internal->m_typedValue->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->isInt());
	}

	bool CValue::IsDouble() const
	{
		return (m_internal->m_typedValue->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->isDouble());
	}

	bool CValue::IsStringA() const
	{
		return (m_internal->m_typedValue->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->isStringA());
	}

	bool CValue::IsStringW() const
	{
		return (m_internal->m_typedValue->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->isStringW());
	}

	bool CValue::IsArray() const
	{
		return m_internal->m_typedValue->m_type == CTypedValue::vtArray;
	}

	bool CValue::IsTypedArray() const
	{
		return m_internal->m_typedValue->m_type == CTypedValue::vtTypedArray;
	}

	bool CValue::IsObject() const
	{
		return m_internal->m_typedValue->m_type == CTypedValue::vtObject;
	}

	bool CValue::ToBool() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtPrimitive)
			return false;
		return static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->toBool();
	}

	int CValue::ToInt() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtPrimitive)
			return 0;
		return static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->toInt();
	}

	double CValue::ToDouble() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtPrimitive)
			return 0.0;
		return static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->toDouble();
	}

	std::string CValue::ToStringA() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtPrimitive)
			return "";
		return static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->toStringA();
	}

	std::wstring CValue::ToStringW() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtPrimitive)
			return L"";
		return static_cast<CPrimitive*>(m_internal->m_typedValue->m_value.get())->toStringW();
	}

	CValue::operator bool() const
	{
		return ToBool();
	}

	CValue::operator int() const
	{
		return ToInt();
	}

	CValue::operator double() const
	{
		return ToDouble();
	}

	CValue::operator std::string() const
	{
		return ToStringA();
	}

	CValue::operator std::wstring() const
	{
		return ToStringW();
	}

	CValue::CValue(bool value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(value);
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	CValue::CValue(int value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(value);
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	CValue::CValue(double value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(value);
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	CValue::CValue(const char* value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(std::string(value));
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	CValue::CValue(const std::string& value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(value);
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	CValue::CValue(const wchar_t* value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(std::wstring(value));
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	CValue::CValue(const std::wstring& value) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CPrimitive>(value);
		m_internal->m_typedValue->m_type = CTypedValue::vtPrimitive;
	}

	int CValue::GetCount() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtArray)
			return 0;
		return static_cast<CArray*>(m_internal->m_typedValue->m_value.get())->getCount();
	}

	const CValue CValue::Get(int index) const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtArray)
			return CValue();
		// don't set reference mode, because this is a const method
		return static_cast<CArray*>(m_internal->m_typedValue->m_value.get())->get(index);
	}

	CValue CValue::Get(int index)
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtArray)
			return CValue();
		CValue& value = static_cast<CArray*>(m_internal->m_typedValue->m_value.get())->get(index);
		value.m_internal->m_isReference = true;
		return value;
	}

	const CValue CValue::operator[](int index) const
	{
		return Get(index);
	}

	CValue CValue::operator[](int index)
	{
		CValue ret = Get(index);
		ret.m_internal->m_isReference = true;
		return ret;
	}

	CValue::CValue(std::initializer_list<CValue> elements) : CValue()
	{
		m_internal->m_typedValue->m_value = std::make_shared<CArray>(elements);
		m_internal->m_typedValue->m_type = CTypedValue::vtArray;
	}

	CValue CValue::CreateArray(int count)
	{
		CValue ret;
		ret.m_internal->m_typedValue->m_value = std::make_shared<CArray>(count);
		ret.m_internal->m_typedValue->m_type = CTypedValue::vtArray;
		return ret;
	}

	const BYTE* CValue::GetData() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtTypedArray)
			return nullptr;
		return static_cast<CTypedArray*>(m_internal->m_typedValue->m_value.get())->getData();
	}

	BYTE* CValue::GetData()
	{
		return const_cast<BYTE*>(static_cast<const CValue&>(*this).GetData());
	}

	CValue CValue::CreateTypedArray(BYTE* data, int count, bool isExternalize)
	{
		// TODO:
		return CValue();
	}

	BYTE* CValue::AllocTypedArray(size_t size)
	{
		// TODO:
		return nullptr;
	}

	void CValue::FreeTypedArray(BYTE* data, size_t size)
	{
		// TODO:
	}

	const CValue CValue::Get(const char* name) const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtObject)
			return CValue();
		// don't set reference mode, because this is a const method
		return static_cast<CObject*>(m_internal->m_typedValue->m_value.get())->get(name);
	}

	CValue CValue::Get(const char* name)
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtObject)
			return CValue();
		CValue& value = static_cast<CObject*>(m_internal->m_typedValue->m_value.get())->get(name);
		value.m_internal->m_isReference = true;
		return value;
	}

	const CValue CValue::operator[](const char* name) const
	{
		return Get(name);
	}

	CValue CValue::operator[](const char* name)
	{
		CValue ret = Get(name);
		ret.m_internal->m_isReference = true;
		return ret;
	}

	std::vector<std::string> CValue::GetPropertyNames() const
	{
		if (m_internal->m_typedValue->m_type != CTypedValue::vtObject)
			return {};
		return static_cast<CObject*>(m_internal->m_typedValue->m_value.get())->getPropertyNames();
	}

	CValue CValue::CreateObject()
	{
		CValue ret;
		ret.m_internal->m_typedValue->m_value = std::make_shared<CObject>();
		ret.m_internal->m_typedValue->m_type = CTypedValue::vtObject;
		return ret;
	}

	CValue CValue::CreateUndefined()
	{
		return CValue();
	}

	CValue CValue::CreateNull()
	{
		CValue ret;
		ret.m_internal->m_typedValue->m_type = CTypedValue::vtNull;
		return ret;
	}
}
