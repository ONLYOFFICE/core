#include "json/serialization.h"

#ifdef JSON_GOOGLE_TEST
#include "gtest/gtest.h"
#include <algorithm>
#else
#include <iostream>
#endif

using namespace NSJSBase;
using namespace NSJSON;

#ifdef JSON_GOOGLE_TEST

class CJSONTest : public testing::Test
{
public:
	void SetUp() override
	{
		// create and enter context
		m_pContext = new CJSContext();
		m_pContext->Enter();
	}

	void TearDown() override
	{
		m_pContext->Exit();
	}

	bool compare(const CValue& value, JSSmart<CJSValue> jsValue, bool makeExpects = true)
	{
		if (value.IsUndefined())
		{
			if (makeExpects)
				EXPECT_TRUE(jsValue->isUndefined());
			return jsValue->isUndefined();
		}
		if (value.IsNull())
		{
			if (makeExpects)
				EXPECT_TRUE(jsValue->isNull());
			return jsValue->isNull();
		}

		if (value.IsArray())
		{
			if (makeExpects)
				EXPECT_TRUE(jsValue->isArray());
			if (!jsValue->isArray())
				return false;

			JSSmart<CJSArray> jsArr = jsValue->toArray();
			const int len = value.GetCount();

			if (makeExpects)
				EXPECT_EQ(len, jsArr->getCount());
			if (len != jsArr->getCount())
				return false;

			for (int i = 0; i < len; i++)
			{
				if (!compare(value[i], jsArr->get(i), makeExpects))
				{
					if (makeExpects)
						ADD_FAILURE() << "Array values at index [" << i << "] are different!";
					return false;
				}
			}
		}
		else if (value.IsTypedArray())
		{
			if (makeExpects)
				EXPECT_TRUE(jsValue->isTypedArray());
			if (!jsValue->isTypedArray())
				return false;

			JSSmart<CJSTypedArray> jsTypedArr = jsValue->toTypedArray();

			if (makeExpects)
				EXPECT_EQ(value.GetCount(), jsTypedArr->getCount());
			if (value.GetCount() != jsTypedArr->getCount())
				return false;

			// compare pointers, not values
			if (makeExpects)
				EXPECT_EQ(value.GetData(), jsTypedArr->getData().Data);
			if (value.GetData() != jsTypedArr->getData().Data)
				return false;
		}
		else if (value.IsObject())
		{
			if (makeExpects)
				EXPECT_TRUE(jsValue->isObject());
			if (!jsValue->isObject())
				return false;

			JSSmart<CJSObject> jsObj = jsValue->toObject();
			std::vector<std::string> properties = value.GetPropertyNames();
			std::vector<std::string> jsProperties = jsObj->getPropertyNames();

			const int len = properties.size();
			if (makeExpects)
				EXPECT_EQ(len, jsProperties.size());
			if (len != jsProperties.size())
				return false;

			// sort both vectors since the order of properties may vary in them
			std::sort(properties.begin(), properties.end());
			std::sort(jsProperties.begin(), jsProperties.end());

			for (int i = 0; i < len; i++)
			{
				if (makeExpects)
					EXPECT_EQ(properties[i], jsProperties[i]);
				if (properties[i] != jsProperties[i])
					return false;

				const char* sProperty = properties[i].c_str();
				if (!compare(value[sProperty], jsObj->get(sProperty), makeExpects))
				{
					if (makeExpects)
						ADD_FAILURE() << "Object property values for property \"" << sProperty << "\" are different!";
					return false;
				}
			}
		}
		else
		{
			// primitive types
			if (value.IsBool())
			{
				if (makeExpects)
					EXPECT_TRUE(jsValue->isBool());
				if (!jsValue->isBool())
					return false;

				bool val = (bool)value;
				bool jsVal = jsValue->toBool();
				if (makeExpects)
					EXPECT_EQ(val, jsVal);
				if (val != jsVal)
					return false;
			}
			else if (value.IsInt())
			{
				if (makeExpects)
					EXPECT_TRUE(jsValue->isNumber());
				if (!jsValue->isNumber())
					return false;

				int val = (int)value;
				int jsVal = jsValue->toInt32();
				if (makeExpects)
					EXPECT_EQ(val, jsVal);
				if (val != jsVal)
					return false;
			}
			else if (value.IsDouble())
			{
				if (makeExpects)
					EXPECT_TRUE(jsValue->isNumber());
				if (!jsValue->isNumber())
					return false;

				double val = (double)value;
				double jsVal = jsValue->toDouble();
				// strict check without tolerance
				if (makeExpects)
					EXPECT_EQ(val, jsVal);
				if (val != jsVal)
					return false;
			}
			else if (value.IsStringA())
			{
				if (makeExpects)
					EXPECT_TRUE(jsValue->isString());
				if (!jsValue->isString())
					return false;

				std::string val = value.ToStringA();
				std::string jsVal = jsValue->toStringA();
				if (makeExpects)
					EXPECT_EQ(val, jsVal);
				if (val != jsVal)
					return false;
			}
			else
			{
				if (makeExpects)
				{
					EXPECT_TRUE(value.IsStringW());
					EXPECT_TRUE(jsValue->isString());
				}
				if (!jsValue->isString())
					return false;

				std::wstring val = value.ToStringW();
				std::wstring jsVal = jsValue->toStringW();
				if (makeExpects)
					EXPECT_EQ(val, jsVal);
				if (val != jsVal)
					return false;
			}
		}

		return true;
	}

	JSSmart<CJSObject> getObject(const std::string& objLiteral)
	{
		JSSmart<CJSObject> jsObj = m_pContext->runScript("(() => { return " + objLiteral + ";})();")->toObject();
		return jsObj;
	}

public:
	JSSmart<CJSContext> m_pContext;
};

TEST_F(CJSONTest, undefined_from_default_constructor)
{
	CValue val;
	JSSmart<CJSValue> jsVal = CJSContext::createUndefined();
	EXPECT_TRUE(compare(val, jsVal));
	val = 0;
	EXPECT_FALSE(compare(val, jsVal, false));
}

TEST_F(CJSONTest, undefined_from_static_method)
{
	CValue val = CValue::CreateUndefined();
	JSSmart<CJSValue> jsVal = CJSContext::createUndefined();
	EXPECT_TRUE(compare(val, jsVal));
	val = 5;
	EXPECT_FALSE(compare(val, jsVal, false));
}

TEST_F(CJSONTest, null_)
{
	CValue val = CValue::CreateNull();
	JSSmart<CJSValue> jsVal = CJSContext::createNull();
	EXPECT_TRUE(compare(val, jsVal));
	val = CValue();
	EXPECT_FALSE(compare(val, jsVal, false));
}

TEST_F(CJSONTest, bool_)
{
	CValue val = true;
	JSSmart<CJSValue> jsVal = CJSContext::createBool(true);
	EXPECT_TRUE(compare(val, jsVal));
	val = false;
	EXPECT_FALSE(compare(val, jsVal, false));
}

TEST_F(CJSONTest, int_)
{
	CValue val = 42;
	EXPECT_FALSE(val.IsDouble());
	JSSmart<CJSValue> jsVal = CJSContext::createInt(42);
	EXPECT_TRUE(compare(val, jsVal));
	val = 100;
	EXPECT_FALSE(compare(val, jsVal, false));
	val = 3;
	jsVal = CJSContext::createDouble(3.0);
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, double_)
{
	CValue val = 3.141592;
	EXPECT_FALSE(val.IsInt());
	JSSmart<CJSValue> jsVal = CJSContext::createDouble(3.141592);
	EXPECT_TRUE(compare(val, jsVal));
	val = 2.81828;
	jsVal = CJSContext::createDouble(3.0);
	EXPECT_FALSE(compare(val, jsVal, false));
	val = 3.0;
	jsVal = CJSContext::createInt(3);
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, string_char_constructor)
{
	CValue val = "test";
	JSSmart<CJSValue> jsVal = CJSContext::createString("test");
	EXPECT_TRUE(compare(val, jsVal));
	jsVal = CJSContext::createString(L"test");
	EXPECT_TRUE(compare(val, jsVal));
	val = "";
	EXPECT_FALSE(compare(val, jsVal, false));
}

TEST_F(CJSONTest, string_string_constructor)
{
	CValue val = std::string("test");
	JSSmart<CJSValue> jsVal = CJSContext::createString("test");
	EXPECT_TRUE(compare(val, jsVal));
	jsVal = CJSContext::createString(L"test");
	EXPECT_TRUE(compare(val, jsVal));
	val = "";
	EXPECT_FALSE(compare(val, jsVal, false));
}

TEST_F(CJSONTest, wstring_wchar_constructor)
{
	CValue val = L"тест";
	JSSmart<CJSValue> jsVal = CJSContext::createString(L"тест");
	EXPECT_TRUE(compare(val, jsVal));
	val = "test";
	EXPECT_FALSE(compare(val, jsVal, false));
	jsVal = CJSContext::createString(L"test");
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, wstring_wstring_constructor)
{
	CValue val = std::wstring(L"тест");
	JSSmart<CJSValue> jsVal = CJSContext::createString(L"тест");
	EXPECT_TRUE(compare(val, jsVal));
	val = "test";
	EXPECT_FALSE(compare(val, jsVal, false));
	jsVal = CJSContext::createString(L"test");
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, array_from_static_method)
{
	CValue arr = CValue::CreateArray(4);
	arr[0] = CValue::CreateNull();
	arr[1] = 42;
	arr[2] = CValue::CreateArray(2);
	arr[2][0] = true;
	arr[2][1] = 2.5;
	EXPECT_TRUE(arr[3].IsUndefined());
	EXPECT_TRUE(arr[100].IsUndefined());
	EXPECT_TRUE(arr[-1].IsUndefined());
	arr[5] = 1;
	JSSmart<CJSArray> jsArr = CJSContext::createArray(4);
	jsArr->set(0, CJSContext::createNull());
	jsArr->set(1, CJSContext::createInt(42));
	jsArr->set(2, CJSContext::createArray(2));
	jsArr->get(2)->toArray()->set(0, CJSContext::createBool(true));
	jsArr->get(2)->toArray()->set(1, CJSContext::createDouble(2.5));
	EXPECT_TRUE(compare(arr, jsArr->toValue()));
	arr[1] = 41;
	EXPECT_FALSE(compare(arr, jsArr->toValue(), false));
}

TEST_F(CJSONTest, array_from_initializer_list)
{
	CValue arr = {CValue::CreateNull(), 42, {true, 2.5}, CValue::CreateUndefined()};
	EXPECT_TRUE(arr[3].IsUndefined());
	EXPECT_TRUE(arr[100].IsUndefined());
	EXPECT_TRUE(arr[-1].IsUndefined());
	arr[5] = 1;
	JSSmart<CJSArray> jsArr = CJSContext::createArray(4);
	jsArr->set(0, CJSContext::createNull());
	jsArr->set(1, CJSContext::createInt(42));
	jsArr->set(2, CJSContext::createArray(2));
	jsArr->get(2)->toArray()->set(0, CJSContext::createBool(true));
	jsArr->get(2)->toArray()->set(1, CJSContext::createDouble(2.5));
	EXPECT_TRUE(compare(arr, jsArr->toValue()));
	arr[1] = 41;
	EXPECT_FALSE(compare(arr, jsArr->toValue(), false));
}

TEST_F(CJSONTest, array_empty)
{
	CValue arr = CValue::CreateArray(0);
	EXPECT_TRUE(arr.IsArray());
	EXPECT_EQ(arr.GetCount(), 0);
	EXPECT_TRUE(arr[0].IsUndefined());
}

TEST_F(CJSONTest, array_negative_size)
{
	CValue arr = CValue::CreateArray(-1);
	EXPECT_TRUE(arr.IsUndefined());
	EXPECT_TRUE(arr[100].IsUndefined());
}

TEST_F(CJSONTest, typed_array_externalize)
{
	BYTE* data = CValue::AllocTypedArray(10);
	CValue typedArr = CValue::CreateTypedArray(data, 10);
	JSSmart<CJSTypedArray> jsTypedArr = CJSContext::createUint8Array(data, 10);
	EXPECT_TRUE(compare(typedArr, jsTypedArr->toValue()));
	typedArr = CValue::CreateTypedArray(data, 11);
	EXPECT_FALSE(compare(typedArr, jsTypedArr->toValue(), false));
	BYTE* data2 = CValue::AllocTypedArray(10);
	typedArr = CValue::CreateTypedArray(data2, 10);
	EXPECT_FALSE(compare(typedArr, jsTypedArr->toValue(), false));

	CValue::FreeTypedArray(data, 10);
	CValue::FreeTypedArray(data2, 10);
}

TEST_F(CJSONTest, typed_array_not_externalize)
{
	BYTE* data = CValue::AllocTypedArray(10);
	CValue typedArr = CValue::CreateTypedArray(data, 10, false);
	JSSmart<CJSTypedArray> jsTypedArr = CJSContext::createUint8Array(data, 10);
	EXPECT_TRUE(compare(typedArr, jsTypedArr->toValue()));
}

TEST_F(CJSONTest, typed_array_empty)
{
	BYTE* data = CValue::AllocTypedArray(0);
	CValue typedArr = CValue::CreateTypedArray(data, 0, false);
	EXPECT_TRUE(typedArr.IsUndefined());
}

TEST_F(CJSONTest, typed_array_negative_size)
{
	BYTE* data = CValue::AllocTypedArray(10);
	CValue typedArr = CValue::CreateTypedArray(nullptr, -10);
	EXPECT_TRUE(typedArr.IsUndefined());
	CValue::FreeTypedArray(data, 10);
}

TEST_F(CJSONTest, object)
{
	CValue obj = CValue::CreateObject();
	JSSmart<CJSObject> jsObj = CJSContext::createObject();
	EXPECT_TRUE(compare(obj, jsObj->toValue()));
	obj["name"] = "Foo";
	jsObj->set("name", CJSContext::createString("Foo"));
	EXPECT_TRUE(compare(obj, jsObj->toValue()));
	obj["name"] = "Bar";
	EXPECT_FALSE(compare(obj, jsObj->toValue(), false));
	obj["number"] = 42;
	jsObj->set("name", CJSContext::createString("Bar"));
	jsObj->set("number", CJSContext::createInt(42));
	EXPECT_TRUE(compare(obj, jsObj->toValue()));
	obj["extra"] = CValue::CreateUndefined();
	EXPECT_TRUE(compare(obj, jsObj->toValue()));
	obj["extra"] = CValue::CreateNull();
	EXPECT_FALSE(compare(obj, jsObj->toValue(), false));
}

TEST_F(CJSONTest, references)
{
	CValue val = 42;
	CValueRef ref = val;
	EXPECT_EQ((int)val, 42);
	EXPECT_EQ((int)ref, 42);
	ref = 10;
	EXPECT_EQ((int)val, 10);
	EXPECT_EQ((int)ref, 10);
	// CValue from CValueRef
	CValue val2 = ref;
	val2 = 12;
	EXPECT_EQ((int)val2, 12);
	EXPECT_EQ((int)ref, 10);
	EXPECT_EQ((int)val, 10);
	// chaining CValueRef-s
	CValueRef ref2 = ref;
	val = "foo";
	EXPECT_EQ((std::string)ref, "foo");
	EXPECT_EQ((std::string)ref2, "foo");
	val = {1, 2, 3};
	EXPECT_EQ((int)ref[1], 2);
	// CValue assignment to CValueRef
	val2 = ref2;
	EXPECT_EQ((int)val2[0], 1);
	val2 = true;
	EXPECT_EQ((bool)val2, true);
	EXPECT_TRUE(ref.IsArray());
	EXPECT_TRUE(ref2.IsArray());
	// CValueRef from operator[]
	CValueRef ref3 = val[2];
	ref3 = ref2[0];
	EXPECT_EQ((int)val[2], 1);
}

TEST_F(CJSONTest, constants)
{
	const CValue val = 42;
	// you can't change constant value explicitly, but you can do it using references:
	CValueRef ref = val;
	ref = 10;
	EXPECT_EQ((int)val, 10);
	EXPECT_EQ((int)ref, 10);
	// const references saves its const properties
	const CValueRef ref2 = ref;
	// you can't do that:
	// ref2 = 100;
	EXPECT_EQ((int)ref2, 10);
}

TEST_F(CJSONTest, wrong_usage)
{
	CValue val = 42;
	EXPECT_TRUE(val["name"].IsUndefined());
	EXPECT_TRUE(val[0].IsUndefined());
	EXPECT_TRUE(val.GetPropertyNames().empty());
	EXPECT_EQ(val.GetCount(), 0);
	EXPECT_EQ(val.GetData(), nullptr);
	EXPECT_DOUBLE_EQ(val.ToDouble(), 42.0);
#ifdef JSON_DEBUG
	EXPECT_THROW((bool)val, std::bad_cast);
	EXPECT_THROW((std::string)val, std::bad_cast);
	EXPECT_THROW((std::wstring)val, std::bad_cast);
#else
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((std::string)val, "");
	EXPECT_EQ((std::wstring)val, L"");
#endif

	val = 3.1415926535;
	EXPECT_EQ((int)val, 3);

	val = "test";
#ifdef JSON_DEBUG
	EXPECT_THROW((bool)val, std::bad_cast);
	EXPECT_THROW((int)val, std::bad_cast);
	EXPECT_THROW((double)val, std::bad_cast);
	EXPECT_THROW((std::wstring)val, std::bad_cast);
#else
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((int)val, 0);
	EXPECT_EQ((double)val, 0.0);
	EXPECT_EQ((std::wstring)val, L"");
#endif

	val = L"тест";
#ifdef JSON_DEBUG
	EXPECT_THROW((bool)val, std::bad_cast);
	EXPECT_THROW((int)val, std::bad_cast);
	EXPECT_THROW((double)val, std::bad_cast);
	EXPECT_THROW((std::string)val, std::bad_cast);
#else
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((int)val, 0);
	EXPECT_EQ((double)val, 0.0);
	EXPECT_EQ((std::string)val, "");
#endif

	val = CValue::CreateObject();
#ifdef JSON_DEBUG
	EXPECT_THROW((bool)val, std::bad_cast);
	EXPECT_THROW((int)val, std::bad_cast);
	EXPECT_THROW((double)val, std::bad_cast);
	EXPECT_THROW((std::string)val, std::bad_cast);
	EXPECT_THROW((std::wstring)val, std::bad_cast);
#else
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((int)val, 0);
	EXPECT_EQ((double)val, 0.0);
	EXPECT_EQ((std::string)val, "");
	EXPECT_EQ((std::wstring)val, L"");
#endif
}

#else
int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	CJSContextScope scope(pContext);

	// top object with some text parameters
	CValue textPr = CValue::CreateObject();

	CValue colorRGB = CValue::CreateObject();
	colorRGB["r"] = 12;
	colorRGB["g"] = 34;
	colorRGB["b"] = 56;

	CValue colorRGBA = CValue::CreateObject();
	colorRGBA["rgb"] = colorRGB;
	colorRGBA["alpha"] = 80;

	// CValueRef behaves as reference
	CValueRef name = textPr["name"];
	// from now on, name - is a reference to the property "name" of textPr. Changing it will affect this object property:
	name = "Foo";
	// Also you can chain references
	CValueRef name3 = name;
	name3 = "FooBar";	// this change applies to textPr["name"]
	// The same but with CValue wouldnt' work:
	CValue name2 = textPr["name"];
	name2 = "Bar";		// this change doesn't affect the property "name" of textPr

	textPr["size"] = 4.2;
	textPr["color"] = colorRGBA;
	textPr["font"] = CValue::CreateObject();
	textPr["font"]["fontName"] = L"Times New Roman";
	textPr["font"]["bold"] = true;
	// undefined member:
	textPr["font"]["italic"] = CValue::CreateUndefined();
	// or just
//	textPr["font"]["italic"];
	// null member:
	textPr["extras"] = CValue::CreateNull();
	// array
	CValue numbers = {10000, 12, 42, 0, 147};
	// inner array
	CValue innerArray = {true, "abc", 3.1415926535, L"ABC", 4};
	numbers[3] = innerArray;
	textPr["numbers"] = numbers;
	// create typed array
	BYTE* pData = CValue::AllocTypedArray(4);
	pData[0] = 11;
	pData[1] = 23;
	pData[2] = 58;
	pData[3] = 13;
	// add typed array
	CValue typedArr = CValue::CreateTypedArray(pData, 4, false);
	textPr["typedArr"] = typedArr;

	// convert to JS
	JSSmart<CJSObject> jsObj = toJS(textPr)->toObject();
	JSSmart<CJSObject> global = pContext->GetGlobal();
	global->set("textPr", jsObj);
	JSSmart<CJSValue> ret = pContext->runScript("(function () { return JSON.stringify(textPr, null, 4); })();");
	if (ret.IsInit())
	{
		std::cout << ret->toStringA() << std::endl;
	}

	// convert fromJS() the same object:
	CValue textPr2 = fromJS(jsObj->toValue());
	// and then back toJS() to see if result is the same
	JSSmart<CJSObject> jsObj2 = toJS(textPr2)->toObject();
	global->set("textPr2", jsObj2);
	ret = pContext->runScript("(function () { return JSON.stringify(textPr2, null, 4); })();");
	if (ret.IsInit())
	{
		std::cout << ret->toStringA() << std::endl;
	}

	return 0;
}
#endif	// JSON_GOOGLE_TEST
