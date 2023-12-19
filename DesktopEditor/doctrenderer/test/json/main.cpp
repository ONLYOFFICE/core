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
				if (!std::isnan(val))
				{
					// strict check without tolerance
					if (makeExpects)
						EXPECT_EQ(val, jsVal);
					if (val != jsVal)
						return false;
				}
				else
				{
					if (makeExpects)
						EXPECT_TRUE(std::isnan(jsVal));
					if (!std::isnan(jsVal))
						return false;
				}
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

public:
	JSSmart<CJSContext> m_pContext;
};

// --------- CValue basic functinality tests ----------

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
#ifdef JSON_DEBUG
	EXPECT_THROW(arr[100].IsUndefined(), std::out_of_range);
	EXPECT_THROW(arr[-1].IsUndefined(), std::out_of_range);
	EXPECT_THROW(arr[5] = 1, std::out_of_range);
#else
	EXPECT_TRUE(arr[100].IsUndefined());
	EXPECT_TRUE(arr[-1].IsUndefined());
	arr[5] = 1;
#endif
	EXPECT_EQ(arr.GetCount(), 4);
	JSSmart<CJSArray> jsArr = CJSContext::createArray(4);
	jsArr->set(0, CJSContext::createNull());
	jsArr->set(1, CJSContext::createInt(42));
	jsArr->set(2, CJSContext::createArray(2));
	jsArr->get(2)->toArray()->set(0, CJSContext::createBool(true));
	jsArr->get(2)->toArray()->set(1, CJSContext::createDouble(2.5));
	jsArr->set(3, CJSContext::createUndefined());
	EXPECT_TRUE(compare(arr, jsArr->toValue()));
	arr[1] = 41;
	EXPECT_FALSE(compare(arr, jsArr->toValue(), false));
}

TEST_F(CJSONTest, array_from_initializer_list)
{
	CValue arr = {CValue::CreateNull(), 42, {true, 2.5}, CValue::CreateUndefined()};
	EXPECT_TRUE(arr[3].IsUndefined());
#ifdef JSON_DEBUG
	EXPECT_THROW(arr[100].IsUndefined(), std::out_of_range);
	EXPECT_THROW(arr[-1].IsUndefined(), std::out_of_range);
	EXPECT_THROW(arr[5] = 1, std::out_of_range);
#else
	EXPECT_TRUE(arr[100].IsUndefined());
	EXPECT_TRUE(arr[-1].IsUndefined());
	arr[5] = 1;
#endif
	EXPECT_EQ(arr.GetCount(), 4);
	JSSmart<CJSArray> jsArr = CJSContext::createArray(4);
	jsArr->set(0, CJSContext::createNull());
	jsArr->set(1, CJSContext::createInt(42));
	jsArr->set(2, CJSContext::createArray(2));
	jsArr->get(2)->toArray()->set(0, CJSContext::createBool(true));
	jsArr->get(2)->toArray()->set(1, CJSContext::createDouble(2.5));
	jsArr->set(3, CJSContext::createUndefined());
	EXPECT_TRUE(compare(arr, jsArr->toValue()));
	arr[1] = 41;
	EXPECT_FALSE(compare(arr, jsArr->toValue(), false));
}

TEST_F(CJSONTest, array_empty)
{
	CValue arr = CValue::CreateArray(0);
	EXPECT_TRUE(arr.IsArray());
	EXPECT_EQ(arr.GetCount(), 0);
#ifdef JSON_DEBUG
	EXPECT_THROW(arr[0].IsUndefined(), std::out_of_range);
#else
	EXPECT_TRUE(arr[0].IsUndefined());
#endif
}

TEST_F(CJSONTest, array_negative_size)
{
	CValue arr = CValue::CreateArray(-1);
	EXPECT_TRUE(arr.IsUndefined());
#ifdef JSON_DEBUG
	EXPECT_THROW(arr[0].IsUndefined(), std::bad_cast);
#else
	EXPECT_TRUE(arr[0].IsUndefined());
#endif
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

TEST_F(CJSONTest, typed_array_copy)
{
	BYTE* data = CValue::AllocTypedArray(10);
	CValue typedArr = CValue::CreateTypedArray(data, 10, false);
	{
		CValue typedArr2 = typedArr;
		typedArr2.GetData()[4] = 0x42;
	}
	EXPECT_EQ(typedArr.GetData()[4], 0x42);
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
	// NOTE: you can't change constant value explicitly, but you can do it using references:
	CValueRef ref = val;
	ref = 10;
	EXPECT_EQ((int)val, 10);
	EXPECT_EQ((int)ref, 10);
	// NOTE: const references saves its const properties
	const CValueRef ref2 = ref;
	// you can't do that:
//	ref2 = 100;
	EXPECT_EQ((int)ref2, 10);
}

TEST_F(CJSONTest, wrong_usage)
{
	CValue val = 42;
#ifdef JSON_DEBUG
	EXPECT_THROW(val["name"].IsUndefined(), std::bad_cast);
	EXPECT_THROW(val[0].IsUndefined(), std::bad_cast);
	EXPECT_THROW(val.GetPropertyNames(), std::bad_cast);
	EXPECT_THROW(val.GetCount(), std::bad_cast);
	EXPECT_THROW(val.GetData(), std::bad_cast);
#else
	EXPECT_TRUE(val["name"].IsUndefined());
	EXPECT_TRUE(val[0].IsUndefined());
	EXPECT_TRUE(val.GetPropertyNames().empty());
	EXPECT_EQ(val.GetCount(), 0);
	EXPECT_EQ(val.GetData(), nullptr);
#endif
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
	EXPECT_THROW(val[0].IsUndefined(), std::bad_cast);
	EXPECT_THROW((bool)val, std::bad_cast);
	EXPECT_THROW((int)val, std::bad_cast);
	EXPECT_THROW((double)val, std::bad_cast);
	EXPECT_THROW((std::string)val, std::bad_cast);
	EXPECT_THROW((std::wstring)val, std::bad_cast);
#else
	EXPECT_TRUE(val[0].IsUndefined());
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((int)val, 0);
	EXPECT_EQ((double)val, 0.0);
	EXPECT_EQ((std::string)val, "");
	EXPECT_EQ((std::wstring)val, L"");
#endif
}

TEST_F(CJSONTest, is_init)
{
	CValue val;
	EXPECT_FALSE(val.IsInit());
	val = CValue::CreateNull();
	EXPECT_FALSE(val.IsInit());
	val = 42;
	EXPECT_TRUE(val.IsInit());
	CValue val2 = val;
	val2 = CValue::CreateUndefined();
	EXPECT_TRUE(val.IsInit());
	EXPECT_FALSE(val2.IsInit());
	CValueRef ref = val;
	ref = CValue::CreateUndefined();
	EXPECT_FALSE(val.IsInit());
	EXPECT_FALSE(ref.IsInit());
	ref = 3;
	EXPECT_TRUE(val.IsInit());
	EXPECT_TRUE(ref.IsInit());
}

// ----------- toJS() tests -----------

TEST_F(CJSONTest, toJS_undefined)
{
	CValue val;
	JSSmart<CJSValue> jsVal = toJS(val);
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, toJS_null)
{
	CValue val = CValue::CreateNull();
	JSSmart<CJSValue> jsVal = toJS(val);
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, toJS_typed_arrays)
{
	BYTE* data = CValue::AllocTypedArray(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	CValue typedArr = CValue::CreateTypedArray(data, 4, false);
	JSSmart<CJSValue> jsTypedArr = toJS(typedArr);
	// NOTE: BE CAREFUL WHEN CALLLING toJS() MULTIPLE TIMES WITH THE SAME TYPED ARRAY!
	// second typed array will NOT be initialized properly, since there is another `CJSTypedArray` containing this memory!
//	JSSmart<CJSValue> jsTypedArr2 = toJS(typedArr);
	EXPECT_TRUE(compare(typedArr, jsTypedArr));
}

TEST_F(CJSONTest, toJS_arrays)
{
	BYTE* data = CValue::AllocTypedArray(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	CValue typedArr = CValue::CreateTypedArray(data, 4, false);

	// can't add typed array to this inner array (see test above), only to external array
	CValue arrInner = {true, 42, L"тест функции toJS()", 2.71828, CValue(), "abc de f", L"test"};
	CValue arr = {0, arrInner, arrInner, CValue::CreateNull(), arrInner, CValue::CreateArray(4), typedArr};

	JSSmart<CJSValue> jsArr = toJS(arr);
	EXPECT_TRUE(compare(arr, jsArr));
}

TEST_F(CJSONTest, toJS_arrays_circular)
{
	// NOTE: BE CAREFULL WHEN CREATING CIRCULAR REFERENCE DEPENDENCY IN YOUR ARRAY OR OBJECT!
	CValue arr = CValue::CreateArray(2);
	CValueRef ref = arr;
	arr[0] = 3;
	arr[1] = ref;
	// or simply:
//	arr[1] = arr;

	EXPECT_EQ((int)arr[0], 3);
	EXPECT_EQ((int)arr[1][0], 3);
	EXPECT_EQ((int)arr[1][1][1][1][1][1][1][1][1][1][0], 3);
	EXPECT_TRUE(arr[1][1][1][1].IsArray());
	// here you will get stack overflow, because each inner array reference will be transformed to a new copy of `CJSArray`
//	JSSmart<CJSValue> jsArr = toJS(arr);

	// keep only 2 inner recursions
	CValue arrRec = arr[1][1];
	arrRec[1] = 42;
	EXPECT_TRUE(arr[1].IsInt());

	JSSmart<CJSValue> jsArr = toJS(arr);
	EXPECT_TRUE(compare(arr, jsArr));
}

TEST_F(CJSONTest, toJS_objects)
{
	CValue obj = CValue::CreateObject();
	obj["name"] = L"Foo";
	obj["parameters"] = CValue::CreateObject();
	CValueRef parameters = obj["parameters"];
	parameters["size"] = 42;
	parameters["arr"] = {CValue::CreateNull(), CValue::CreateArray(0), {42, L"тест функции toJS()", 2.71828}, CValue::CreateObject(), CValue(), "abc de f", L"test"};
	parameters["0"] = 0;

	BYTE* data = CValue::AllocTypedArray(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	CValue typedArr = CValue::CreateTypedArray(data, 4, false);
	obj["typed"] = typedArr;
	// NOTE: you can create property even without a name (like in JS)
	obj[""] = "Bar";

	JSSmart<CJSValue> jsObj = toJS(obj);
	EXPECT_TRUE(compare(obj, jsObj));
}

// ----------- fromJS() tests -----------

TEST_F(CJSONTest, fromJS_undefined)
{
	JSSmart<CJSValue> jsVal = CJSContext::createUndefined();
	CValue val = fromJS(jsVal);
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, fromJS_null)
{
	JSSmart<CJSValue> jsVal = CJSContext::createNull();
	CValue val = fromJS(jsVal);
	EXPECT_TRUE(compare(val, jsVal));
}

TEST_F(CJSONTest, fromJS_and_toJS_edge_numbers)
{
	JSSmart<CJSArray> jsVal = CJSContext::createArray(16);
	jsVal->set(0,  INT_MIN);
	jsVal->set(1,  INT_MAX);
	jsVal->set(2,  0);
	jsVal->set(3,  42);
	jsVal->set(4,  0.0);
	jsVal->set(5,  3.1415926535);
	jsVal->set(6,  42.0);
	jsVal->set(7,  (double)INT_MIN);
	jsVal->set(8,  (double)INT_MAX);
	jsVal->set(9,  std::pow(2.0, 31) - 1);
	jsVal->set(10, std::pow(2.0, 31));
	jsVal->set(11, -std::pow(2.0, 31));
	jsVal->set(12, -std::pow(2.0, 31) - 1);
	jsVal->set(13, INFINITY);
	jsVal->set(14, NAN);
	jsVal->set(15, -INFINITY);

	CValue val = fromJS(jsVal->toValue());
	EXPECT_TRUE(compare(val, jsVal->toValue()));
	EXPECT_TRUE(val[0].IsInt());		// INT_MIN
	EXPECT_TRUE(val[1].IsInt());		// INT_MAX
	EXPECT_TRUE(val[2].IsInt());		// 0
	EXPECT_TRUE(val[3].IsInt());		// 42
	EXPECT_TRUE(val[4].IsInt());		// 0.0
	EXPECT_TRUE(val[5].IsDouble());		// 3.1415926535
	EXPECT_TRUE(val[6].IsInt());		// 42.0
	EXPECT_TRUE(val[7].IsInt());		// (double)INT_MIN
	EXPECT_TRUE(val[8].IsInt());		// (double)INT_MAX
	EXPECT_TRUE(val[9].IsInt());		// 2^31 - 1 == INT_MAX
	EXPECT_TRUE(val[10].IsDouble());	// 2^31
	EXPECT_TRUE(val[11].IsInt());		// -2^31    == INT_MIN
	EXPECT_TRUE(val[12].IsDouble());	// -2^31 - 1
	EXPECT_TRUE(val[13].IsDouble());	// inf
	EXPECT_TRUE(val[14].IsDouble());	// NaN
	EXPECT_TRUE(val[15].IsDouble());	// -inf

	JSSmart<CJSValue> jsVal2 = toJS(val);
	EXPECT_TRUE(compare(val, jsVal2));
}

TEST_F(CJSONTest, fromJS_typed_arrays)
{
	BYTE* data = NSAllocator::Alloc(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	JSSmart<CJSValue> jsTypedArr = CJSContext::createUint8Array(data, 4, false);
	CValue typedArr = fromJS(jsTypedArr);
	EXPECT_TRUE(compare(typedArr, jsTypedArr));
}

TEST_F(CJSONTest, fromJS_arrays)
{
	BYTE* data = NSAllocator::Alloc(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	JSSmart<CJSValue> jsTypedArr = CJSContext::createUint8Array(data, 4, false);
	JSSmart<CJSArray> jsArrInner = CJSContext::createArray(0);
	jsArrInner->add_bool(true);
	jsArrInner->add_int(42);
	jsArrInner->add_string(L"тест функции fromJS()");
	jsArrInner->add_double(2.71828);
	jsArrInner->add_undefined();
	jsArrInner->add_stringa("abc de f");
	jsArrInner->add_string(L"test");
	JSSmart<CJSArray> jsArr = CJSContext::createArray(0);
	jsArr->add_int(0);
	jsArr->add(jsArrInner->toValue());
	jsArr->add(jsArrInner->toValue());
	jsArr->add_null();
	jsArr->add(jsArrInner->toValue());
	jsArr->add(CJSContext::createArray(4));
	jsArr->add(jsTypedArr);

	CValue arr = fromJS(jsArr->toValue());
	EXPECT_TRUE(compare(arr, jsArr->toValue()));
}

TEST_F(CJSONTest, fromJS_objects)
{
	JSSmart<CJSObject> jsObj = CJSContext::createObject();
	jsObj->set("name", CJSContext::createString(L"Foo"));

	JSSmart<CJSObject> jsParam = CJSContext::createObject();
	jsParam->set("size", 42);
	jsParam->set("arr", CJSContext::createArray(0));

	JSSmart<CJSArray> jsArr = jsParam->get("arr")->toArray();
	jsArr->add_null();
	jsArr->add(CJSContext::createArray(0));
	jsArr->add(CJSContext::createArray(3));
	jsArr->get(2)->toArray()->set(0, 42);
	jsArr->get(2)->toArray()->set(1, CJSContext::createString(L"тест функции fromJS()"));
	jsArr->get(2)->toArray()->set(2, 2.71828);
	jsArr->add(CJSContext::createObject());
	jsArr->add_undefined();
	jsArr->add_stringa("abc de f");
	jsArr->add_bool(true);
	jsParam->set("0", 0);

	jsObj->set("parameters", jsParam);

	BYTE* data = NSAllocator::Alloc(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	JSSmart<CJSValue> jsTypedArr = CJSContext::createUint8Array(data, 4, false);

	jsObj->set("typed", jsTypedArr);
	jsObj->set("", CJSContext::createString("Bar"));

	CValue obj = fromJS(jsObj->toValue());
	EXPECT_TRUE(compare(obj, jsObj->toValue()));
}

TEST_F(CJSONTest, serialization_with_script)
{
	JSSmart<CJSValue> jsObj = m_pContext->runScript(
		"(() => {"
		"	let obj = {};"
		"	obj['name'] = 'Foo';"
		"	obj['parameters'] = { size: 42, arr: [null, [], [42, 'test', 2.71828], {}, undefined, 'abc de f', ''], 0: 0, typedArr: { data: null, count: 0 } };"
		"	obj[''] = 'Bar';"
		"	return obj;"
		"})();"
	);

	CValue obj = fromJS(jsObj);

	EXPECT_EQ(obj["name"].ToStringW(), L"Foo");
	EXPECT_EQ(obj[""].ToStringW(), L"Bar");
	EXPECT_EQ((double)obj["parameters"]["arr"][2][0], 42);
	EXPECT_TRUE(obj["parameters"]["arr"][4].IsUndefined());
	EXPECT_EQ((int)obj["parameters"]["0"], 0);
	EXPECT_TRUE(obj["parameters"]["typedArr"]["data"].IsNull());

	EXPECT_TRUE(compare(obj, jsObj));

	// function test() returns 0 if all checks have passed and number of failed check otherwise
	m_pContext->runScript(
		"function test(obj) {"
		"	if (obj['name'] !== 'Foo')"
		"		return 1;"
		"	if (obj[''] !== 'Bar')"
		"		return 2;"
		"	if (obj['parameters']['arr'][2][0] !== 42)"
		"		return 3;"
		"	if (obj['parameters']['arr'][4] !== undefined)"
		"		return 4;"
		"	if (obj['parameters']['0'] !== 0)"
		"		return 5;"
		"	if (obj['parameters']['typedArr']['data'] !== null)"
		"		return 6;"
		"	return 0;"
		"}"
	);

	JSSmart<CJSObject> global = m_pContext->GetGlobal();
	JSSmart<CJSValue> args[1];
	args[0] = toJS(obj);

	JSSmart<CJSValue> jsCheckResult = global->call_func("test", 1, args);

	EXPECT_TRUE(jsCheckResult->isNumber());
	EXPECT_EQ(jsCheckResult->toInt32(), 0);

	EXPECT_TRUE(compare(obj, args[0]));
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
