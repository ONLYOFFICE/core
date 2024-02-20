#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "json.h"
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
		// handle primitive types first, as they are most commonly used
		if (value.IsBool())
		{
			ret = NSJSBase::CJSContext::createBool((bool)value);
		}
		else if (value.IsInt())
		{
			ret = NSJSBase::CJSContext::createInt((int)value);
		}
		else if (value.IsDouble())
		{
			ret = NSJSBase::CJSContext::createDouble((double)value);
		}
		else if (value.IsStringA())
		{
			ret = NSJSBase::CJSContext::createString((std::string)value);
		}
		else if (value.IsStringW())
		{
			ret = NSJSBase::CJSContext::createString((std::wstring)value);
		}
		// arrays
		else if (value.IsArray())
		{
			const int len = value.GetCount();
			JSSmart<NSJSBase::CJSArray> jsArr = NSJSBase::CJSContext::createArray(len);
			for (int i = 0; i < len; i++)
			{
				jsArr->set(i, toJS(value[i]));
			}
			ret = jsArr->toValue();
		}
		// typed arrays
		else if (value.IsTypedArray())
		{
			JSSmart<NSJSBase::CJSTypedArray> jsTypedArr = NSJSBase::CJSContext::createUint8Array(const_cast<BYTE*>(value.GetData()), value.GetCount());
			ret = jsTypedArr->toValue();
		}
		// objects (there is no need for IsObject())
		else
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

		return ret;
	}

	static CValue fromJS(JSSmart<NSJSBase::CJSValue> jsValue)
	{
		if (jsValue->isUndefined())
			return CValue::CreateUndefined();
		if (jsValue->isNull())
			return CValue::CreateNull();

		CValue ret;
		// handle primitive types first, as they are most commonly used
		if (jsValue->isBool())
		{
			ret = CValue(jsValue->toBool());
		}
		else if (jsValue->isNumber())
		{
			double number = jsValue->toDouble();
			if (std::isfinite(number))
			{
				// check if number is an integer or double
				double integral;									// integral part
				double fractional = std::modf(number, &integral);	// fractional part
				// TODO: this may not work for non-32 bit integers
				if (fractional == 0.0 && integral >= INT_MIN && integral <= INT_MAX)
					ret = (int)integral;
				else
					ret = number;
			}
			else
			{
				// handle NaN, inf, -inf
				ret = number;
			}
		}
		else if (jsValue->isString())
		{
			// convert all strings to std::wstring, because in JS all strings are encoded in UTF-16
			ret = jsValue->toStringW();
		}
		// arrays
		else if (jsValue->isArray())
		{
			JSSmart<NSJSBase::CJSArray> jsArr = jsValue->toArray();
			const int len = jsArr->getCount();
			ret = CValue::CreateArray(len);
			for (int i = 0; i < len; i++)
			{
				JSSmart<NSJSBase::CJSValue> jsElement = jsArr->get(i);
				ret[i] = fromJS(jsElement);
			}
		}
		// typed arrays
		else if (jsValue->isTypedArray())
		{
			JSSmart<NSJSBase::CJSTypedArray> jsTypedArr = jsValue->toTypedArray();
			const int len = jsTypedArr->getCount();
			BYTE* data = jsTypedArr->getData().Data;
			ret = CValue::CreateTypedArray(data, len);
		}
		// objects
		else if (jsValue->isObject())
		{
			JSSmart<NSJSBase::CJSObject> jsObj = jsValue->toObject();
			std::vector<std::string> properties = jsObj->getPropertyNames();
			ret = CValue::CreateObject();
			for (const std::string& name : properties)
			{
				JSSmart<NSJSBase::CJSValue> jsPropertyValue = jsObj->get(name.c_str());
				ret[name.c_str()] = fromJS(jsPropertyValue);
			}
		}
		// else the type is not supported and will be converted as undefined value

		return ret;
	}
}

#endif	// SERIALIZATION_H_
