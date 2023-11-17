#include "json.h"
#include "json_p.h"
#include "json_values.h"

namespace NSJSON
{
	CValueContainer::CValueContainer() : m_type(vtUndefined)
	{
	}

	CValueContainer::CValueContainer(ValueType type, IBaseValue* pValue) : m_value(pValue), m_type(type)
	{
	}

	CValueContainer CValueContainer::DeepCopy(const CValueContainer& other)
	{
		CValueContainer ret;
		ValueType type = other.m_type;
		ret.m_type = type;
		switch (type)
		{
		case vtPrimitive:
			ret.m_value = std::make_shared<CPrimitive>(*static_cast<CPrimitive*>(other.m_value.get()));
			break;
		case vtArray:
			ret.m_value = std::make_shared<CArray>(*static_cast<CArray*>(other.m_value.get()));
			break;
		case vtTypedArray:
			ret.m_value = std::make_shared<CTypedArray>(*static_cast<CTypedArray*>(other.m_value.get()));
			break;
		case vtObject:
			ret.m_value = std::make_shared<CObject>(*static_cast<CObject*>(other.m_value.get()));
			break;
		default:
			// vtUndefined of vtNull
			break;
		}
		return ret;
	}

	CValue::CValue() : m_internal(new CValueContainer())
	{
	}

	bool CValue::IsUndefined() const
	{
		return m_internal->m_type == CValueContainer::vtUndefined;
	}

	bool CValue::IsNull() const
	{
		return m_internal->m_type == CValueContainer::vtNull;
	}

	bool CValue::IsBool() const
	{
		return (m_internal->m_type == CValueContainer::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isBool());
	}

	bool CValue::IsInt() const
	{
		return (m_internal->m_type == CValueContainer::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isInt());
	}

	bool CValue::IsDouble() const
	{
		return (m_internal->m_type == CValueContainer::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isDouble());
	}

	bool CValue::IsStringA() const
	{
		return (m_internal->m_type == CValueContainer::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isStringA());
	}

	bool CValue::IsStringW() const
	{
		return (m_internal->m_type == CValueContainer::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isStringW());
	}

	bool CValue::IsArray() const
	{
		return m_internal->m_type == CValueContainer::vtArray;
	}

	bool CValue::IsTypedArray() const
	{
		return m_internal->m_type == CValueContainer::vtTypedArray;
	}

	bool CValue::IsObject() const
	{
		return m_internal->m_type == CValueContainer::vtObject;
	}

	bool CValue::ToBool() const
	{
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toBool();
	}

	int CValue::ToInt() const
	{
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toInt();
	}

	double CValue::ToDouble() const
	{
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toDouble();
	}

	std::string CValue::ToStringA() const
	{
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toStringA();
	}

	std::wstring CValue::ToStringW() const
	{
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toStringW();
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

	CValue::CValue(bool value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(value)))
	{
	}

	CValue::CValue(int value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(value)))
	{
	}

	CValue::CValue(double value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(value)))
	{
	}

	CValue::CValue(const char* value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(std::string(value))))
	{
	}

	CValue::CValue(const std::string& value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(value)))
	{
	}

	CValue::CValue(const wchar_t* value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(std::wstring(value))))
	{
	}

	CValue::CValue(const std::wstring& value) : m_internal(new CValueContainer(CValueContainer::vtPrimitive, new CPrimitive(value)))
	{
	}

	// Helper function to reduce code duplication for further assignment operators
	template<typename T>
	static inline void setPrimitive(const std::shared_ptr<CValueContainer>& value, T primitive)
	{
		if (value->m_type != CValueContainer::vtPrimitive)
		{
			value->m_type = CValueContainer::vtPrimitive;
			value->m_value = std::make_shared<CPrimitive>(primitive);
		}
		else
		{
			static_cast<CPrimitive*>(value->m_value.get())->set(primitive);
		}
	}

	CValue& CValue::operator=(bool value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	CValue& CValue::operator=(int value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	CValue& CValue::operator=(double value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	CValue& CValue::operator=(const char* value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	CValue& CValue::operator=(const std::string& value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	CValue& CValue::operator=(const wchar_t* value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	CValue& CValue::operator=(const std::wstring& value)
	{
		setPrimitive(m_internal, value);
		return *this;
	}

	int CValue::GetCount() const
	{
		if (m_internal->m_type != CValueContainer::vtArray)
			return 0;
		return static_cast<CArray*>(m_internal->m_value.get())->getCount();
	}

	const CValue CValue::Get(int index) const
	{
		if (m_internal->m_type != CValueContainer::vtArray)
			return CValue();
		return static_cast<CArray*>(m_internal->m_value.get())->get(index);
	}

	CValue CValue::Get(int index)
	{
		return static_cast<const CValue&>(*this).Get(index);
	}

	const CValue CValue::operator[](int index) const
	{
		return Get(index);
	}

	CValue CValue::operator[](int index)
	{
		return Get(index);
	}

	CValue::CValue(std::initializer_list<CValue> elements) : m_internal(new CValueContainer(CValueContainer::vtArray, new CArray(elements)))
	{
	}

	CValue CValue::CreateArray(int count)
	{
		CValue ret;
		ret.m_internal = std::make_shared<CValueContainer>(CValueContainer::vtArray, new CArray(count));
		return ret;
	}

	const BYTE* CValue::GetData() const
	{
		if (m_internal->m_type != CValueContainer::vtTypedArray)
			return nullptr;
		return static_cast<CTypedArray*>(m_internal->m_value.get())->getData();
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
		if (m_internal->m_type != CValueContainer::vtObject)
			return CValue();
		return static_cast<CObject*>(m_internal->m_value.get())->get(name);
	}

	CValue CValue::Get(const char* name)
	{
		return static_cast<const CValue&>(*this).Get(name);
	}

	CValue CValue::operator[](const char* name)
	{
		return Get(name);
	}

	const CValue CValue::operator[](const char* name) const
	{
		return Get(name);
	}

	CValue CValue::CreateObject()
	{
		CValue ret;
		ret.m_internal = std::make_shared<CValueContainer>(CValueContainer::vtObject, new CObject());
		return ret;
	}

	CValue CValue::CreateUndefined()
	{
		return CValue();
	}

	CValue CValue::CreateNull()
	{
		CValue ret;
		ret.m_internal->m_type = CValueContainer::vtNull;
		return ret;
	}
}
