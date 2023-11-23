#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "json.h"
//#include "json_values.h"
#include "../js_internal/js_base.h"

#include <cmath>

namespace NSJSON
{
	static JSSmart<NSJSBase::CJSValue> toJS(const CValue& value)
	{
		if (value.IsUndefined())
			return NSJSBase::CJSContext::createUndefined();
		if (value.IsNull())
			return NSJSBase::CJSContext::createNull();

		JSSmart<NSJSBase::CJSValue> ret;
		if (value.IsObject())
		{
			JSSmart<NSJSBase::CJSObject> jsObj = NSJSBase::CJSContext::createObject();
			std::vector<std::string> properties = value.GetPropertyNames();
			for (const std::string& name : properties)
			{
				JSSmart<NSJSBase::CJSValue> jsValue = toJS(value[name.c_str()]);
				jsObj->set(name.c_str(), jsValue);
			}
			ret = jsObj->toValue();
		}
		else if (value.IsArray())
		{
			const int len = value.GetCount();
			JSSmart<NSJSBase::CJSArray> jsArr = NSJSBase::CJSContext::createArray(len);
			for (int i = 0; i < len; i++)
			{
				jsArr->set(i, toJS(value[i]).GetPointer());
			}
			ret = jsArr->toValue();
		}
		else if (value.IsTypedArray())
		{
			JSSmart<NSJSBase::CJSTypedArray> jsTypedArr = NSJSBase::CJSContext::createUint8Array(const_cast<BYTE*>(value.GetData()), value.GetCount());
			ret = jsTypedArr->toValue();
		}
		else
		{
			// primitive types
			if (value.IsBool())
				ret = NSJSBase::CJSContext::createBool((bool)value);
			else if (value.IsInt())
				ret = NSJSBase::CJSContext::createInt((int)value);
			else if (value.IsDouble())
				ret = NSJSBase::CJSContext::createDouble((double)value);
			else if (value.IsStringA())
				ret = NSJSBase::CJSContext::createString((std::string)value);
			else
				ret = NSJSBase::CJSContext::createString((std::wstring)value);
		}

		return ret;
	}

	static CValue fromJS(JSSmart<NSJSBase::CJSValue> jsValue)
	{
		/*
		if (jsValue->isUndefined())
			return new CPrimitive();

		if (jsValue->isNull())
		{
			CPrimitive* pValue = new CPrimitive;
			pValue->setNull();
			return pValue;
		}

		IBaseValue* ret = nullptr;
		if (jsValue->isObject())
		{
			JSSmart<NSJSBase::CJSObject> jsObj = jsValue->toObject();
			CObject* pObject = new CObject;
			// TODO: add CJSObject::getPropertyNames()
			ret = pObject;
		}
		else if (jsValue->isArray())
		{
			JSSmart<NSJSBase::CJSArray> jsArr = jsValue->toArray();
			const int len = jsArr->getCount();
			CArray* pArray = new CArray();
			for (int i = 0; i < len; i++)
			{
				JSSmart<NSJSBase::CJSValue> jsElement = jsArr->get(i);
				pArray->add(fromJS(jsElement));
			}
			ret = pArray;
		}
		else if (jsValue->isTypedArray())
		{
			JSSmart<NSJSBase::CJSTypedArray> jsTypedArr = jsValue->toTypedArray();
			const int len = jsTypedArr->getCount();
			BYTE* data = jsTypedArr->getData().Data;
			ret = new CTypedArray(data, len);
		}
		// primitives
		else if (jsValue->isBool())
		{
			ret = new CPrimitive(jsValue->toBool());
		}
		else if (jsValue->isNumber())
		{
			// TODO: this check seems redundant. Similiar to string (look further).
			// check if number is an integer or double
			double number = jsValue->toDouble();
			double integral;									// integral part
			double fractional = std::modf(number, &integral);	// fractional part
			if (fractional == 0.0 && integral >= INT_MIN && integral <= INT_MAX)
			{
				ret = new CPrimitive(static_cast<int>(integral));
			}
			else
			{
				ret = new CPrimitive(number);
			}
		}
		else if (jsValue->isString())
		{
			// TODO: how do we know whether string consist of char or wchar_t? Can we convert all strings to std::wstring?
		}
		// else ret is nullptr
		*/

		return CValue();
	}
}

#endif	// SERIALIZATION_H_
