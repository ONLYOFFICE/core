#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

#include "json.h"
#include "../js_internal/js_base.h"
#include "../embed/GraphicsEmbed.h"

#include <cmath>

class CAppImageTo : public CGraphicsAppImage
{
private:
	NSJSON::CValueRef* m_image;

public:
	CAppImageTo(const NSJSON::CValue& image) : CGraphicsAppImage()
	{
		m_image = new NSJSON::CValueRef(image);
	}
	virtual ~CAppImageTo()
	{
		if (m_image)
			delete m_image;
	}

public:
	virtual unsigned char* GetBits(int& w, int& h)
	{
		unsigned char* bits = m_image->GetImageBits();
		if (NULL != bits)
		{
			w = m_image->GetImageWidth();
			h = m_image->GetImageHeight();
		}
		return bits;
	}
	virtual unsigned char* AllocBits(const int& w, const int& h)
	{
		m_image->ImageAlloc(w, h, GetRgba() ? NSJSON::ImageFormat::ifRGBA : NSJSON::ImageFormat::ifBGRA);
		return m_image->GetImageBits();
	}
};

class CAppImageFrom : public CGraphicsAppImage
{
public:
	unsigned char* m_pData;
	int m_nW;
	int m_nH;
public:
	CAppImageFrom() : CGraphicsAppImage()
	{
		m_pData = NULL;
		m_nW = 0;
		m_nH = 0;
	}
	virtual ~CAppImageFrom()
	{
	}

public:
	virtual unsigned char* GetBits(int& w, int& h)
	{
		return m_pData;
	}
	virtual unsigned char* AllocBits(const int& w, const int& h)
	{
		m_nW = w;
		m_nH = h;
		m_pData = NSJSON::CValue::AllocImageBits(w, h);
		return m_pData;
	}
};

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
		else if (value.IsString())
		{
			ret = NSJSBase::CJSContext::createString(value.ToStringA());
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
		else if (value.IsImage())
		{
			JSSmart<CJSObject> wrap = CJSContext::createEmbedObject("CGraphicsEmbed");
			((CGraphicsEmbed*)wrap->getNative())->SetAppImage(new CAppImageTo(value));
			ret = wrap->toValue();
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
		if (!jsValue.is_init())
			return CValue::CreateUndefined();
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

			CJSEmbedObject* pNative = jsObj->getNative();
			if (pNative != NULL)
			{
				CGraphicsEmbed* pGrEmbed = dynamic_cast<CGraphicsEmbed*>(pNative);
				if (pGrEmbed)
				{
					CAppImageFrom* pAppImage = dynamic_cast<CAppImageFrom*>(pGrEmbed->GetAppImage());
					if (pAppImage)
					{
						return NSJSON::CValue::CreateImage(pAppImage->m_pData,
														   pAppImage->m_nW,
														   pAppImage->m_nH,
														   pAppImage->GetRgba() ? ImageFormat::ifRGBA : ImageFormat::ifBGRA,
														   false);
					}
				}
			}

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
