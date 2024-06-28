#include "json.h"
#include "json_p.h"
#include "json_values.h"

// for working with typed arrays: Alloc() and Free()
#include "js_base.h"

namespace NSJSON
{
	CTypedValue::CTypedValue() : m_type(vtUndefined)
	{
	}

	CTypedValue::CTypedValue(IBaseValue* value, ValueType type) : m_value(value), m_type(type)
	{
	}

	CTypedValue::~CTypedValue()
	{
	}


	IValue::IValue() : m_internal(new CTypedValue())
	{
	}

	IValue::IValue(const std::shared_ptr<CTypedValue>& internal) : m_internal(internal)
	{
	}

	IValue::~IValue()
	{
	}

	bool IValue::IsUndefined() const
	{
		return m_internal->m_type == CTypedValue::vtUndefined;
	}

	bool IValue::IsNull() const
	{
		return m_internal->m_type == CTypedValue::vtNull;
	}

	bool IValue::IsInit() const
	{
		// the same as (m_internal->m_type != CTypedValue::vtUndefined && m_internal->m_type != CTypedValue::vtNull) but a little bit faster
		return m_internal->m_value.get() != nullptr;
	}

	bool IValue::IsBool() const
	{
		return (m_internal->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isBool());
	}

	bool IValue::IsInt() const
	{
		return (m_internal->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isInt());
	}

	bool IValue::IsDouble() const
	{
		return (m_internal->m_type == CTypedValue::vtPrimitive &&
				static_cast<CPrimitive*>(m_internal->m_value.get())->isDouble());
	}

	bool IValue::IsString() const
	{
		if (m_internal->m_type != CTypedValue::vtPrimitive)
			return false;

		CPrimitive* pPrimitive = static_cast<CPrimitive*>(m_internal->m_value.get());
		return (pPrimitive->isStringA() || pPrimitive->isStringW());
	}

	bool IValue::IsArray() const
	{
		return m_internal->m_type == CTypedValue::vtArray;
	}

	bool IValue::IsTypedArray() const
	{
		return m_internal->m_type == CTypedValue::vtTypedArray;
	}

	bool IValue::IsObject() const
	{
		return m_internal->m_type == CTypedValue::vtObject;
	}

	bool IValue::ToBool() const
	{
		if (m_internal->m_type != CTypedValue::vtPrimitive)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return false;
		}
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toBool();
	}

	int IValue::ToInt() const
	{
		if (m_internal->m_type != CTypedValue::vtPrimitive)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return 0;
		}
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toInt();
	}

	double IValue::ToDouble() const
	{
		if (m_internal->m_type != CTypedValue::vtPrimitive)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return 0.0;
		}
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toDouble();
	}

	std::string IValue::ToStringA() const
	{
		if (m_internal->m_type != CTypedValue::vtPrimitive)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return "";
		}
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toStringA();
	}

	std::wstring IValue::ToStringW() const
	{
		if (m_internal->m_type != CTypedValue::vtPrimitive)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return L"";
		}
		return static_cast<CPrimitive*>(m_internal->m_value.get())->toStringW();
	}

	IValue::operator bool() const
	{
		return ToBool();
	}

	IValue::operator int() const
	{
		return ToInt();
	}

	IValue::operator double() const
	{
		return ToDouble();
	}

	IValue::operator std::string() const
	{
		return ToStringA();
	}

	IValue::operator std::wstring() const
	{
		return ToStringW();
	}

	IValue::IValue(bool value) : m_internal(new CTypedValue(new CPrimitive(value), CTypedValue::vtPrimitive))
	{
	}

	IValue::IValue(int value) : m_internal(new CTypedValue(new CPrimitive(value), CTypedValue::vtPrimitive))
	{
	}

	IValue::IValue(double value) : m_internal(new CTypedValue(new CPrimitive(value), CTypedValue::vtPrimitive))
	{
	}

	IValue::IValue(const char* value) : m_internal(new CTypedValue(new CPrimitive(std::string(value)), CTypedValue::vtPrimitive))
	{
	}

	IValue::IValue(const std::string& value) : m_internal(new CTypedValue(new CPrimitive(value), CTypedValue::vtPrimitive))
	{
	}

	IValue::IValue(const wchar_t* value) : m_internal(new CTypedValue(new CPrimitive(std::wstring(value)), CTypedValue::vtPrimitive))
	{
	}

	IValue::IValue(const std::wstring& value) : m_internal(new CTypedValue(new CPrimitive(value), CTypedValue::vtPrimitive))
	{
	}

	int IValue::GetCount() const
	{
		if (m_internal->m_type == CTypedValue::vtArray)
		{
			return static_cast<CArray*>(m_internal->m_value.get())->getCount();
		}
		else if (m_internal->m_type == CTypedValue::vtTypedArray)
		{
			return static_cast<CTypedArray*>(m_internal->m_value.get())->getCount();
		}
		else
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return 0;
		}
	}

	const CValueRef IValue::Get(int index) const
	{
		if (m_internal->m_type != CTypedValue::vtArray)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return CValue();	
		}

		if (index < 0 || index >= GetCount())
		{
#ifdef JSON_DEBUG
			throw std::out_of_range("std::out_of_range");
#endif
			return CValue();
		}
		return static_cast<CArray*>(m_internal->m_value.get())->get(index);
	}

	CValueRef IValue::Get(int index)
	{
		return static_cast<const IValue&>(*this).Get(index);
	}

	const CValueRef IValue::operator[](int index) const
	{
		return Get(index);
	}

	CValueRef IValue::operator[](int index)
	{
		return Get(index);
	}

	IValue::IValue(std::initializer_list<CValue> elements) : m_internal(new CTypedValue(new CArray(elements), CTypedValue::vtArray))
	{
	}

	const BYTE* IValue::GetData() const
	{
		if (m_internal->m_type != CTypedValue::vtTypedArray)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return nullptr;
		}
		return static_cast<CTypedArray*>(m_internal->m_value.get())->getData();
	}

	BYTE* IValue::GetData()
	{
		return const_cast<BYTE*>(static_cast<const CValue&>(*this).GetData());
	}

	const CValueRef IValue::Get(const char* name) const
	{
		if (m_internal->m_type != CTypedValue::vtObject)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return CValue();
		}
		return static_cast<CObject*>(m_internal->m_value.get())->get(name);
	}

	CValueRef IValue::Get(const char* name)
	{
		return static_cast<const IValue&>(*this).Get(name);
	}

	const CValueRef IValue::operator[](const char* name) const
	{
		return Get(name);
	}

	CValueRef IValue::operator[](const char* name)
	{
		return Get(name);
	}

	std::vector<std::string> IValue::GetPropertyNames() const
	{
		if (m_internal->m_type != CTypedValue::vtObject)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return {};
		}
		return static_cast<CObject*>(m_internal->m_value.get())->getPropertyNames();
	}


	CValue::CValue() : IValue()
	{
	}

	CValue::CValue(const CValue& other) : IValue(std::make_shared<CTypedValue>(*other.m_internal))
	{
	}

	CValue::CValue(const CValueRef& ref) : IValue(std::make_shared<CTypedValue>(*ref.m_internal))
	{
	}

	CValue::~CValue()
	{
	}

	CValue& CValue::operator=(const CValue& other)
	{
		*m_internal = *other.m_internal;
		return *this;
	}

	CValue& CValue::operator=(const CValueRef& ref)
	{
		*m_internal = *ref.m_internal;
		return *this;
	}

	CValue::CValue(bool value) : IValue(value)
	{
	}

	CValue::CValue(int value) : IValue(value)
	{
	}

	CValue::CValue(double value) : IValue(value)
	{
	}

	CValue::CValue(const char* value) : IValue(value)
	{
	}

	CValue::CValue(const std::string& value) : IValue(value)
	{
	}

	CValue::CValue(const wchar_t* value) : IValue(value)
	{
	}

	CValue::CValue(const std::wstring& value) : IValue(value)
	{
	}

	CValue::CValue(std::initializer_list<CValue> elements) : IValue(elements)
	{
	}

	CValue CValue::CreateArray(int count)
	{
		CValue ret;
		if (count < 0)
			return ret;

		ret.m_internal->m_value = std::make_shared<CArray>(count);
		ret.m_internal->m_type = CTypedValue::vtArray;
		return ret;
	}

	CValue CValue::CreateTypedArray(BYTE* data, int count, bool isExternalize)
	{
		CValue ret;
		if (count <= 0)
			return ret;

		ret.m_internal->m_value = std::make_shared<CTypedArray>(data, count, isExternalize);
		ret.m_internal->m_type = CTypedValue::vtTypedArray;
		return ret;
	}

	BYTE* CValue::AllocTypedArray(size_t size)
	{
		return NSJSBase::NSAllocator::Alloc(size);
	}

	void CValue::FreeTypedArray(BYTE* data, size_t size)
	{
		NSJSBase::NSAllocator::Free(data, size);
	}

	CValue CValue::CreateObject()
	{
		CValue ret;
		ret.m_internal->m_value = std::make_shared<CObject>();
		ret.m_internal->m_type = CTypedValue::vtObject;
		return ret;
	}

	CValue CValue::CreateUndefined()
	{
		return CValue();
	}

	CValue CValue::CreateNull()
	{
		CValue ret;
		ret.m_internal->m_type = CTypedValue::vtNull;
		return ret;
	}

	CValueRef::CValueRef(const CValueRef& other) : IValue(other.m_internal)
	{
	}

	CValueRef::CValueRef(const CValue& value) : IValue(value.m_internal)
	{
	}

	CValueRef::~CValueRef()
	{
	}

	CValueRef& CValueRef::operator=(const CValueRef& other)
	{
		// not reassign the pointer, but copy its content!
		*m_internal = *other.m_internal;
		return *this;
	}

	CValueRef& CValueRef::operator=(const CValue& value)
	{
		// not reassign the pointer, but copy its content!
		*m_internal = *value.m_internal;
		return *this;
	}
}
