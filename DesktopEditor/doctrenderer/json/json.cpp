#include "json.h"
#include "json_p.h"
#include "json_values.h"

// for working with typed arrays: Alloc() and Free()
#include "js_base.h"

// for converting primitive types to JSON-string
#include <sstream>
#include <iomanip>
#include <limits>
// for strtod()
#include <cstdlib>
// for modf()
#include <cmath>

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

	bool IValue::IsImage() const
	{
		return m_internal->m_type == CTypedValue::vtImage;
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

	namespace
	{
		std::string doubleToJsonString(double value)
		{
			// handle special cases
			if (std::isnan(value) || std::isinf(value))
				return "null";

			// convert to string with full precision
			std::ostringstream oss;
			oss.precision(std::numeric_limits<double>::digits10);
			oss << std::noshowpoint << value;

			return oss.str();
		}

		std::string stringToJsonString(const std::string& value)
		{
			std::string result;
			for (char ch : value)
			{
				if (ch <= 0x1F)
				{
					result += '\\';
					if (ch == '\b')
					{
						result += 'b';
					}
					else if (ch == '\t')
					{
						result += 't';
					}
					else if (ch == '\n')
					{
						result += 'n';
					}
					else if (ch == '\f')
					{
						result += 'f';
					}
					else if (ch == '\r')
					{
						result += 'r';
					}
					else
					{
						result += "u00";
						std::ostringstream oss;
						oss << std::setfill('0') << std::setw(2) << std::hex << (int)ch;
						result += oss.str();
					}
				}
				else if (ch == '\\')
				{
					result += "\\\\";
				}
				else if (ch == '\"')
				{
					result += "\\\"";
				}
				else
				{
					result += ch;
				}
			}

			return result;
		}
	}

	std::string IValue::ToJSON()
	{
		std::string strRes;
		CTypedValue::ValueType type = m_internal->m_type;
		switch (type)
		{
		case CTypedValue::vtUndefined:
		{
			break;
		}
		case CTypedValue::vtNull:
		{
			strRes = "null";
			break;
		}
		case CTypedValue::vtPrimitive:
		{
			CPrimitive* pPrimitiveValue = static_cast<CPrimitive*>(m_internal->m_value.get());
			if (pPrimitiveValue->isBool())
			{
				strRes = pPrimitiveValue->toBool() ? "true" : "false";
			}
			else if (pPrimitiveValue->isInt())
			{
				strRes = std::to_string(pPrimitiveValue->toInt());
			}
			else if (pPrimitiveValue->isDouble())
			{
				strRes = doubleToJsonString(pPrimitiveValue->toDouble());
			}
			else
			{
				strRes = "\"" + stringToJsonString(pPrimitiveValue->toStringA()) + "\"";
			}
			break;
		}
		case CTypedValue::vtArray:
		{
			CArray* pArrayValue = static_cast<CArray*>(m_internal->m_value.get());
			strRes = "[";
			const int len = pArrayValue->getCount();
			for (int i = 0; i < len; i++)
			{
				CValue& value = pArrayValue->get(i);
				if (value.IsUndefined())
					strRes += "null";
				else
					strRes += value.ToJSON();
				strRes += ',';
			}
			// remove last ','
			if (strRes.back() == ',')
				strRes.pop_back();
			strRes += "]";
			break;
		}
		case CTypedValue::vtTypedArray:
		{
			CTypedArray* pTypedArrayValue = static_cast<CTypedArray*>(m_internal->m_value.get());
			strRes += "{";
			const int size = pTypedArrayValue->getCount();
			BYTE* data = pTypedArrayValue->getData();
			for (int i = 0; i < size; i++)
			{
				strRes += "\"" + std::to_string(i) + "\":";
				strRes += std::to_string((int)data[i]);
				strRes += ',';
			}
			// remove last ','
			if (strRes.back() == ',')
				strRes.pop_back();
			strRes += "}";
			break;
		}
		case CTypedValue::vtObject:
		{
			CObject* pObjectValue = static_cast<CObject*>(m_internal->m_value.get());
			strRes += "{";
			std::vector<std::string> propertyNames = pObjectValue->getPropertyNames();
			const int count = propertyNames.size();
			for (int i = 0; i < count; i++)
			{
				CValue& value = pObjectValue->get(propertyNames[i]);
				if (value.IsUndefined())
					continue;

				strRes += "\"" + propertyNames[i] + "\":";
				strRes += value.ToJSON();
				strRes += ',';
			}
			// remove last ','
			if (strRes.back() == ',')
				strRes.pop_back();
			strRes += "}";
			break;
		}
		case CTypedValue::vtImage:
		{
			// TODO: implement like typed array?
			break;
		}
		}

		return strRes;
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

	const BYTE* IValue::GetImageBits() const
	{
		if (m_internal->m_type != CTypedValue::vtImage)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return nullptr;
		}
		return static_cast<CImage*>(m_internal->m_value.get())->getBits();
	}

	BYTE* IValue::GetImageBits()
	{
		return const_cast<BYTE*>(static_cast<const CValue&>(*this).GetImageBits());
	}

	int IValue::GetImageWidth() const
	{
		if (m_internal->m_type != CTypedValue::vtImage)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return 0;
		}
		return static_cast<CImage*>(m_internal->m_value.get())->getWidth();
	}

	int IValue::GetImageHeight() const
	{
		if (m_internal->m_type != CTypedValue::vtImage)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return 0;
		}
		return static_cast<CImage*>(m_internal->m_value.get())->getHeight();
	}

	ImageFormat IValue::GetImageFormat() const
	{
		if (m_internal->m_type != CTypedValue::vtImage)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return ImageFormat::ifInvalid;
		}
		return static_cast<CImage*>(m_internal->m_value.get())->getFormat();
	}

	void IValue::ImageExternalize()
	{
		if (m_internal->m_type != CTypedValue::vtImage)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return;
		}
		static_cast<CImage*>(m_internal->m_value.get())->externalize();
	}

	void IValue::ImageAlloc(const int& width, const int& height, const ImageFormat& format)
	{
		if (m_internal->m_type != CTypedValue::vtImage)
		{
#ifdef JSON_DEBUG
			throw std::bad_cast();
#endif
			return;
		}
		static_cast<CImage*>(m_internal->m_value.get())->alloc(width, height, format);
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

	CValue CValue::CreateImage(BYTE* bits, int width, int height, ImageFormat format, bool isExternalize)
	{
		CValue ret;
		if (width <= 0 || height <= 0)
			return ret;

		ret.m_internal->m_value = std::make_shared<CImage>(bits, width, height, format, isExternalize);
		ret.m_internal->m_type = CTypedValue::vtImage;
		return ret;
	}

	CValue CValue::CreateEmptyImage(ImageFormat format)
	{
		CValue ret;
		ret.m_internal->m_value = std::make_shared<CImage>((BYTE*)NULL, 0, 0, format, false);
		ret.m_internal->m_type = CTypedValue::vtImage;
		return ret;
	}

	BYTE* CValue::AllocImageBits(int width, int height)
	{
		return new BYTE[4 * width * height];
	}

	void CValue::FreeImageBits(BYTE* bits)
	{
		delete[] bits;
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

	namespace
	{
		// moves pos to first non-space symbol and returns false if end of the string was reached
		bool skipWhitespaces(const std::string& str, int& pos)
		{
			for (; pos < str.size(); pos++)
			{
				char ch = str[pos];
				if (ch != '\t' && ch != '\n' && ch != '\r' && ch != ' ')
					break;
			}
			return pos < str.size();
		}

		CValue parseNumberFromJSON(const std::string& str, int& pos)
		{
			const char* startPos = str.c_str() + pos;
			char* newPos;
			double number = std::strtod(startPos, &newPos);
			if (newPos == startPos)
				return CValue();

			CValue value;
			if (std::isfinite(number))
			{
				// check if number can be represented as an integer type without loss of precision
				double integral;									// integral part
				double fractional = std::modf(number, &integral);	// fractional part
				if (fractional == 0.0 && integral >= std::numeric_limits<int>::min() && integral <= std::numeric_limits<int>::max())
					value = (int)integral;
				else
					value = number;
			}
			else
			{
				value = number;
			}

			pos += newPos - startPos;
			return value;
		}

		CValue parseStringFromJSON(const std::string& str, int& pos)
		{
			// skip '\"' cause it has already been checked
			pos++;
			std::string result;
			while (pos < str.size() && str[pos] != '\"')
			{
				// any non-escaped control characters are not allowed
				if (str[pos] < 0x20)
					return CValue();

				if (str[pos] != '\\')
				{
					result += str[pos++];
				}
				else
				{
					pos++;
					if (pos >= str.size())
						return CValue();

					char ch = str[pos++];
					if (ch == '\\' || ch == '\"' || ch == '/')
					{
						result += ch;
					}
					else if (ch == 'b')
					{
						result += '\b';
					}
					else if (ch == 't')
					{
						result += '\t';
					}
					else if (ch == 'n')
					{
						result += '\n';
					}
					else if (ch == 'f')
					{
						result += '\f';
					}
					else if (ch == 'r')
					{
						result += '\r';
					}
					else if (ch == 'u')
					{
						if (str.size() - pos < 4)
							return CValue();
						// TODO: support unicode symbols with codes > U+0020
						if (str[pos] != '0' || str[pos + 1] != '0')
							return CValue();
						pos += 2;
						std::string strHex = str.substr(pos, 2);
						pos += 2;
						// std::stoi throws std::invalid_argument if string is invalid
						try
						{
							std::size_t count;
							char symbol = (char)std::stoi(strHex, &count, 16);
							if (count < 2)
								return CValue();

							result += symbol;
						}
						catch (std::invalid_argument& e)
						{
							return CValue();
						}
					}
					else
					{
						return CValue();
					}
				}
			}
			// if did not encounter closing qoutes, return undefined
			if (pos == str.size())
				return CValue();
			pos++;

			return result;
		}

		// parse one value of JSON-string `str` starting from `pos`
		CValue valueFromJSON(const std::string& str, int& pos);

		CValue parseArrayFromJSON(const std::string& str, int& pos)
		{
			// skip opening bracket '[' cause it has already been checked
			pos++;
			// handle first element separately to get pattern: [(firstValue)(,value)(,value)...]
			CValue firstValue = valueFromJSON(str, pos);
			if (firstValue.IsUndefined())
			{
				if (pos < str.size() && str[pos] == ']')
				{
					pos++;
					return CValue::CreateArray(0);
				}
				else
				{
					return CValue();
				}
			}

			std::vector<CValue> values;
			values.emplace_back(firstValue);
			while (skipWhitespaces(str, pos) && str[pos] != ']')
			{
				// expect ','
				if (str[pos] != ',')
					return CValue();
				pos++;
				// expect value
				CValue value = valueFromJSON(str, pos);
				if (value.IsUndefined())
					return CValue();
				values.emplace_back(value);
			}
			// if did not encounter closing bracket ']', return undefined
			if (pos == str.size())
				return CValue();
			pos++;

			// copy values from vector to CValue array
			CValue result = CValue::CreateArray(values.size());
			for (int i = 0; i < values.size(); i++)
			{
				result[i] = values[i];
			}

			return result;
		}

		std::pair<std::string, CValue> parseKeyValuePair(const std::string& str, int& pos)
		{
			std::pair<std::string, CValue> result;

			if (!skipWhitespaces(str, pos) || str[pos] != '\"')
				return result;
			// parse key
			CValue key = parseStringFromJSON(str, pos);
			if (key.IsUndefined())
				return result;
			result.first = key.ToStringA();
			// expect ':'
			if (!skipWhitespaces(str, pos) || str[pos] != ':')
				return result;
			pos++;
			// parse value
			CValue value = valueFromJSON(str, pos);
			if (value.IsUndefined())
				return result;
			result.second = value;

			return result;
		}

		CValue parseObjectFromJSON(const std::string& str, int& pos)
		{
			CValue result = CValue::CreateObject();
			// skip opening curly brace '{' cause it has already been checked
			pos++;
			// handle first key-value pair separately to get pattern: [(firstKey:firstValue)(,key:value)(,key:value)...]
			std::pair<std::string, CValue> keyValue = parseKeyValuePair(str, pos);
			// if value is undefined, then something went wrong or object is empty
			if (keyValue.second.IsUndefined())
			{
				if (pos < str.size() && str[pos] == '}' && keyValue.first.empty())
				{
					pos++;
					return result;
				}
				else
				{
					return CValue();
				}
			}

			result[keyValue.first.c_str()] = keyValue.second;
			while (skipWhitespaces(str, pos) && str[pos] != '}')
			{
				// expect ','
				if (str[pos] != ',')
					return CValue();
				pos++;
				// expect key-value pair
				std::pair<std::string, CValue> keyValue = parseKeyValuePair(str, pos);
				if (keyValue.second.IsUndefined())
					return CValue();
				result[keyValue.first.c_str()] = keyValue.second;
			}
			// if did not encounter closing curly brace '}', return undefined
			if (pos == str.size())
				return CValue();
			pos++;

			return result;
		}

		CValue valueFromJSON(const std::string& str, int& pos)
		{
			if (!skipWhitespaces(str, pos))
				return CValue();

			CValue value;
			char ch = str[pos];
			if (ch == '\"')
			{
				// string
				value = parseStringFromJSON(str, pos);
			}
			else if (ch == '[')
			{
				// array
				value = parseArrayFromJSON(str, pos);
			}
			else if (ch == '{')
			{
				// object
				value = parseObjectFromJSON(str, pos);
			}
			else if (ch == 'n')
			{
				// null
				if (str.substr(pos, 4) == "null")
					value = CValue::CreateNull();
				else
					return CValue();
				pos += 4;
			}
			else if (ch == 't')
			{
				// true (bool)
				if (str.substr(pos, 4) == "true")
					value = true;
				else
					return CValue();
				pos += 4;
			}
			else if (ch == 'f')
			{
				// false (bool)
				if (str.substr(pos, 5) == "false")
					value = false;
				else
					return CValue();
				pos += 5;
			}
			else if (ch == '-' || (ch >= '0' && ch <= '9'))
			{
				value = parseNumberFromJSON(str, pos);
			}
			else
			{
				return CValue();
			}

			return value;
		}
	}

	CValue CValue::FromJSON(const std::string& jsonString)
	{
		int pos = 0;
		CValue value = valueFromJSON(jsonString, pos);
		// if there are still some non-space characters after the value has been parsed,
		//  the JSON-string is considered invalid
		if (skipWhitespaces(jsonString, pos))
			return CValue();

		return value;
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
