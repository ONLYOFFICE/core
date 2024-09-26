#include "json/serialization.h"

#ifdef JSON_GOOGLE_TEST
#include "gtest/gtest.h"
#include <algorithm>
#include <limits>
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
			else
			{
				if (makeExpects)
				{
					EXPECT_TRUE(value.IsString());
					EXPECT_TRUE(jsValue->isString());
				}
				if (!jsValue->isString())
					return false;

				std::string val = value.ToStringA();
				std::string jsVal = jsValue->toStringA();
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
	EXPECT_TRUE(val.IsDouble());
	EXPECT_TRUE(val.IsInt());
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
#else
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((int)val, 0);
	EXPECT_EQ((double)val, 0.0);
#endif
	EXPECT_EQ((std::wstring)val, L"test");

	val = L"test";
#ifdef JSON_DEBUG
	EXPECT_THROW((bool)val, std::bad_cast);
	EXPECT_THROW((int)val, std::bad_cast);
	EXPECT_THROW((double)val, std::bad_cast);
#else
	EXPECT_EQ((bool)val, false);
	EXPECT_EQ((int)val, 0);
	EXPECT_EQ((double)val, 0.0);
#endif
	EXPECT_EQ((std::string)val, "test");

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

// ----------- CValue::ToJSON() tests -----------
namespace
{
	// helper function
	std::string normalizeJSONObject(const std::string& str, int& pos)
	{
		// '{' was checked
		pos++;

		std::vector<std::string> keyValuePairs;
		std::string keyValue;
		int nSquareBrackets = 0;

		for (; str[pos] != '}'; pos++)
		{
			if (str[pos] == '{')
			{
				keyValue += normalizeJSONObject(str, pos);
			}
			else if (str[pos] == ',' && nSquareBrackets == 0)
			{
				keyValuePairs.emplace_back(std::move(keyValue));
				keyValue = "";
			}
			else
			{
				if (str[pos] == '[')
				{
					nSquareBrackets++;
				}
				else if (str[pos] == ']')
				{
					nSquareBrackets--;
				}
				keyValue += str[pos];
			}
		}

		if (!keyValue.empty())
			keyValuePairs.emplace_back(std::move(keyValue));

		std::sort(keyValuePairs.begin(), keyValuePairs.end());
		std::string result = "{";
		for (const std::string& keyValue : keyValuePairs)
		{
			result += keyValue;
			result += ',';
		}
		// pop last ','
		if (result.back() == ',')
			result.pop_back();
		result += '}';

		return result;
	}

	// test function that sorts key-value pairs for objects in JSON-strings
	// works only with valid JSONs
	std::string normalizeJSON(const std::string& str)
	{
		const int n = (int)str.size();

		if (str[0] != '{' && str[0] != '[')
			return str;

		std::string result;
		for (int pos = 0; pos < n; pos++)
		{
			if (str[pos] == '{')
			{
				result += normalizeJSONObject(str, pos);
				// decrement because it will be incremented again
			}
			else
			{
				result += str[pos];
			}
		}

		return result;
	}
}

TEST_F(CJSONTest, ToJSON_undefined)
{
	CValue val = CValue::CreateUndefined();
	EXPECT_EQ(val.ToJSON(), "");
}

TEST_F(CJSONTest, ToJSON_null)
{
	CValue val = CValue::CreateNull();
	EXPECT_EQ(val.ToJSON(), "null");
}

TEST_F(CJSONTest, ToJSON_bool)
{
	CValue val(false);
	EXPECT_EQ(val.ToJSON(), "false");
	val = true;
	EXPECT_EQ(val.ToJSON(), "true");
}

TEST_F(CJSONTest, ToJSON_int)
{
	CValue val = 42;
	EXPECT_EQ(val.ToJSON(), "42");
	val = 0;
	EXPECT_EQ(val.ToJSON(), "0");
	val = -73;
	EXPECT_EQ(val.ToJSON(), "-73");
	val = INT_MIN;
	EXPECT_EQ(val.ToJSON(), "-2147483648");
	val = INT_MAX;
	EXPECT_EQ(val.ToJSON(), "2147483647");
}

TEST_F(CJSONTest, ToJSON_double)
{
	CValue val = 4.2;
	EXPECT_EQ(val.ToJSON(), "4.2");
	val = 42.0;
	EXPECT_EQ(val.ToJSON(), "42");
	val = 0.0;
	EXPECT_EQ(val.ToJSON(), "0");
	val = 3.1415926535;
	EXPECT_EQ(val.ToJSON(), "3.1415926535");
	val = (double)INT_MIN;
	EXPECT_EQ(val.ToJSON(), "-2147483648");
	val = (double)INT_MAX;
	EXPECT_EQ(val.ToJSON(), "2147483647");
}

TEST_F(CJSONTest, ToJSON_double_critical_values)
{
	CValue val = 8e30;
	EXPECT_EQ(val.ToJSON(), "8e+30");
	val = 8e-30;
	EXPECT_EQ(val.ToJSON(), "8e-30");

	val = INFINITY;
	EXPECT_EQ(val.ToJSON(), "null");
	val = NAN;
	EXPECT_EQ(val.ToJSON(), "null");
	val = -INFINITY;
	EXPECT_EQ(val.ToJSON(), "null");

	val = std::numeric_limits<double>::max();
	std::string strResult = val.ToJSON();
	// we cannot expect predefined specified result from this test case so just check if it is not empty
	EXPECT_TRUE(!strResult.empty());
	EXPECT_TRUE(strResult != "null");

	val = std::numeric_limits<double>::min();
	strResult = val.ToJSON();
	// we cannot expect predefined specified result from this test case so just check if it is not empty
	EXPECT_TRUE(!strResult.empty());
	EXPECT_TRUE(strResult != "null");
}

TEST_F(CJSONTest, ToJSON_string)
{
	CValue val = "  test   ";
	EXPECT_EQ(val.ToJSON(), "\"  test   \"");
	val = "";
	EXPECT_EQ(val.ToJSON(), "\"\"");
	val = L"  test ";
	EXPECT_EQ(val.ToJSON(), "\"  test \"");
	// test parsing of special symbols inside strings
	val = "foo: {bar: 42}[} ,))";
	EXPECT_EQ(val.ToJSON(), "\"foo: {bar: 42}[} ,))\"");
}

TEST_F(CJSONTest, ToJSON_string_escaped_characters)
{
	CValue val = "HEL\"LO";
	std::string strRes = val.ToJSON();
	EXPECT_EQ(strRes, "\"HEL\\\"LO\"");

	val = "HEL\\/LO";
	strRes = val.ToJSON();
	EXPECT_EQ(strRes, "\"HEL\\\\/LO\"");

	val = "\tH\bE\fL\nL\nO\r";
	strRes = val.ToJSON();
	EXPECT_EQ(strRes, "\"\\tH\\bE\\fL\\nL\\nO\\r\"");

	// other symbols
	std::string spec = "/";
	for (char ch = 0; ch <= 0x21; ch++)
	{
		spec += ch;
	}
	spec += '/';

	val = spec;
	strRes = val.ToJSON();
	EXPECT_EQ(strRes, "\"/\\u0000\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\b\\t\\n\\u000b"
					  "\\f\\r\\u000e\\u000f\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017"
					  "\\u0018\\u0019\\u001a\\u001b\\u001c\\u001d\\u001e\\u001f !/\"");
}

TEST_F(CJSONTest, ToJSON_typed_arrays)
{
	BYTE* data = CValue::AllocTypedArray(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	CValue typedArr = CValue::CreateTypedArray(data, 4, false);
	EXPECT_EQ(typedArr.ToJSON(), "{\"0\":26,\"1\":84,\"2\":254,\"3\":255}");
}

TEST_F(CJSONTest, ToJSON_arrays)
{
	CValue arr = CValue::CreateArray(0);
	EXPECT_EQ(arr.ToJSON(), "[]");

	arr = CValue::CreateArray(4);
	arr[0] = 42;
	arr[1] = "foo";
	arr[2] = 2.71828;
	EXPECT_EQ(arr.ToJSON(), "[42,\"foo\",2.71828,null]");

	CValue arrInner = {CValue::CreateNull(), 73.0};
	arr[3] = arrInner;
	EXPECT_EQ(arr.ToJSON(), "[42,\"foo\",2.71828,[null,73]]");
}

TEST_F(CJSONTest, ToJSON_arrays_complex)
{
	BYTE* data = CValue::AllocTypedArray(5);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	data[4] = 0x00;
	CValue typedArr = CValue::CreateTypedArray(data, 5, false);

	CValue arrInner = {true, 42, L"test function ToJSON()", 2.71828, CValue(), "abc de f", L"test"};
	CValue arr = {0, arrInner, arrInner, CValue::CreateNull(), arrInner, CValue::CreateArray(4), typedArr};

	std::string strRes = arr.ToJSON();
	std::string strExpected = "[0,"
							  "[true,42,\"test function ToJSON()\",2.71828,null,\"abc de f\",\"test\"],"
							  "[true,42,\"test function ToJSON()\",2.71828,null,\"abc de f\",\"test\"],"
							  "null,"
							  "[true,42,\"test function ToJSON()\",2.71828,null,\"abc de f\",\"test\"],"
							  "[null,null,null,null],"
							  "{\"0\":26,\"1\":84,\"2\":254,\"3\":255,\"4\":0}]";

	EXPECT_EQ(strRes, strExpected);
}

TEST_F(CJSONTest, ToJSON_arrays_circular)
{
	// NOTE: BE CAREFULL WHEN CREATING CIRCULAR REFERENCE DEPENDENCY IN YOUR ARRAY OR OBJECT!
	CValue arr = CValue::CreateArray(2);
	CValue ref = arr;
	arr[0] = 3;
	arr[1] = ref;

	// here you will get stack overflow, because each inner array reference will be recursively converted to JSON-string
	// arr.ToJSON();

	// remove recursions
	CValue arrRec = arr[1][1];
	arrRec[1] = 42;

	EXPECT_EQ(arr.ToJSON(), "[3,42]");
}

TEST_F(CJSONTest, ToJSON_objects)
{
	CValue obj = CValue::CreateObject();
	EXPECT_EQ(obj.ToJSON(), "{}");

	obj["name"] = "Foo";
	obj["number"] = 42;
	std::string strRes = normalizeJSON(obj.ToJSON());
	std::string strExpected = normalizeJSON("{\"name\":\"Foo\",\"number\":42}");
	EXPECT_EQ(strRes, strExpected);

	obj["undef"] = CValue::CreateUndefined();
	obj["null"] = CValue::CreateNull();
	strRes = normalizeJSON(obj.ToJSON());
	strExpected = normalizeJSON("{\"name\":\"Foo\",\"number\":42,\"null\":null}");
	EXPECT_EQ(strRes, strExpected);
}

TEST_F(CJSONTest, ToJSON_objects_complex)
{
	CValue obj = CValue::CreateObject();

	obj["name"] = L"Foo";
	obj["parameters"] = CValue::CreateObject();
	CValueRef parameters = obj["parameters"];
	parameters["0"] = 0;
	parameters["size"] = 42;
	parameters["arr"] = {CValue::CreateNull(), CValue::CreateArray(1), {42, L"test function ToJSON()", 2.71828}, CValue::CreateObject(), CValue(), "abc de f", L"test"};

	BYTE* data = CValue::AllocTypedArray(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	CValue typedArr = CValue::CreateTypedArray(data, 4, false);
	obj["typed"] = typedArr;
	// property without a name
	obj[""] = "Bar";

	std::string strRes = normalizeJSON(obj.ToJSON());
	std::string strExpected = normalizeJSON( \
		"{"
		"\"name\":\"Foo\","
		"\"parameters\":{\"arr\":[null,[null],[42,\"test function ToJSON()\",2.71828],{},null,\"abc de f\",\"test\"],\"size\":42,\"0\":0},"
		"\"typed\":{\"0\":26,\"1\":84,\"2\":254,\"3\":255},"
		"\"\":\"Bar\""
		"}");

	EXPECT_EQ(strRes, strExpected);
}

// ----------- CValue::FromJSON() tests -----------
TEST_F(CJSONTest, FromJSON_null)
{
	std::string strJson = "null";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsNull());
}

TEST_F(CJSONTest, FromJSON_null_invalid)
{
	// empty json string is invalid
	std::string strJson = "";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// there is no undefined in JSON standard
	strJson = "undefined";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "nil";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "n";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "NULL";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "nullptr";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
}

TEST_F(CJSONTest, FromJSON_bool)
{
	std::string strJson = "false";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsBool());
	EXPECT_EQ((bool)val, false);

	strJson = "true ";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsBool());
	EXPECT_EQ((bool)val, true);
}

TEST_F(CJSONTest, FromJSON_bool_invalid)
{
	std::string strJson = "fals";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "truE";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "f";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "t";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "True";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
}

TEST_F(CJSONTest, FromJSON_int)
{
	std::string strJson = "42";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, 42);

	strJson = "0";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, 0);

	strJson = "-73";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, -73);

	strJson = "-2147483648";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, std::numeric_limits<int>::min());

	strJson = "2147483647";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, std::numeric_limits<int>::max());

	strJson = "2147483648";
	val = CValue::FromJSON(strJson);
	// this value does not fit into 32-bit integer limits
	EXPECT_FALSE(val.IsInt());
	EXPECT_TRUE(val.IsDouble());
}

TEST_F(CJSONTest, FromJSON_double)
{
	std::string strJson = "4.2";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_DOUBLE_EQ((double)val, 4.2);

	strJson = "42.0";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, 42);

	strJson = "0.0";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, 0);

	strJson = "3.1415926535";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_DOUBLE_EQ((double)val, 3.1415926535);

	strJson = "2147483648";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_DOUBLE_EQ((double)val, 2147483648.0);
}

TEST_F(CJSONTest, FromJSON_double_critical_values)
{
	std::string strJson = "-8e30";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_DOUBLE_EQ((double)val, -8e30);

	strJson = "8e+30";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_DOUBLE_EQ((double)val, 8e30);

	strJson = "8e-30";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_DOUBLE_EQ((double)val, 8e-30);

	// divide by 10 to prevent getting 'inf' when converting to string
	CValue maxDouble = std::numeric_limits<double>::max() / 10;
	strJson = maxDouble.ToJSON();
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_NEAR((double)val, (double)maxDouble, 1e-15 * std::numeric_limits<double>::max());

	// multiply by 10 to prevent getting '0' when converting to string
	CValue minDouble = std::numeric_limits<double>::min() * 10;
	strJson = minDouble.ToJSON();
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_NEAR((double)val, (double)minDouble, 1e-15);

	// if value is outside of double range, it is expected to be inf
	strJson = "10e+1000";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_EQ((double)val, INFINITY);

	strJson = "-10e1000";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_EQ((double)val, -INFINITY);

	// expected zero for too small values
	strJson = "10e-1000";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_EQ((double)val, 0.0);
}

TEST_F(CJSONTest, FromJSON_numbers_invalid)
{
	// leading zeroes are invalid in JSON standard, but here it is considered valid
	std::string strJson = "0123";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, 123);
	// multiple decimal points
	strJson = "12.34.56";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// trailing decimal point also considered invalid in JSON, but allowed here
	strJson = "42.";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsDouble());
	EXPECT_EQ((int)val, 42);
	// invalid exponent format
	strJson = "1e";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "1e+";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "1e+a";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "1e+-";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// invalid characters
	strJson = "123abc";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "123\"abc\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "123-42";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// sign without digits
	strJson = "-";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// sign without digits
	strJson = "-";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// whitespace in numbers
	strJson = "1 234";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "1 a";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
}

TEST_F(CJSONTest, FromJSON_string)
{
	std::string strJson = "\"  test   \"";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "  test   ");

	strJson = "\"\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "");

	// test for parsing of special symbols inside strings
	strJson = "\"foo: {bar: 42}[} ,))\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "foo: {bar: 42}[} ,))");

	// test for using unicode codes instead of ASCII characters
	strJson = "\"\\u0061\\u006E\\u0073\\u0077\\u0065\\u0072\\u0020\\u0069\\u0073\\u0020\\u0034\\u0032\\u0021\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "answer is 42!");
}

TEST_F(CJSONTest, FromJSON_string_escaped_characters)
{
	std::string strJson = "\"\\\\\"";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "\\");

	strJson = "\"HEL\\\"LO\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "HEL\"LO");

	strJson = "\"HEL\\\\/LO\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "HEL\\/LO");

	strJson = "\"\\tH\\bE\\fL\\nL\\nO\\r\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), "\tH\bE\fL\nL\nO\r");

	// characters from 0x00 to 0x21 (inclusive)
	strJson = "\"/\\u0000\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\b\\t\\n\\u000b"
			  "\\f\\r\\u000e\\u000f\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017"
			  "\\u0018\\u0019\\u001a\\u001b\\u001c\\u001d\\u001e\\u001f !\\/\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());

	std::string spec = "/";
	for (char ch = 0; ch <= 0x21; ch++)
	{
		spec += ch;
	}
	spec += '/';

	EXPECT_EQ(val.ToStringA(), spec);
}

TEST_F(CJSONTest, FromJSON_string_invalid)
{
	// using unescaped control characters
	std::string strJson = "\"this\nis a test";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = "\"\t\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// no closing quote
	strJson = "\"  test  ";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// no symbol after backslash
	strJson = "\"\\\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// wrong symbol after backslash
	strJson = "\"\\x\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// invalid unicode sequence
	strJson = "\"\\u\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "\"\\u00\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "\"\\u0100\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "\"\\u001\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "\"\\u00GH\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "\"\\u00fH\"";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
}

TEST_F(CJSONTest, FromJSON_whitespace_symbols)
{
	// allowed space symbols are: 0x09 (\t), 0x0A (\n), 0x0D (\r) and 0x20 (space)
	std::string strJson = "    null  ";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsNull());

	strJson = "\t \n\n42\r";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsInt());
	EXPECT_EQ((int)val, 42);

	strJson = "\n\t  \" foo\\n \"\n \t\n";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsString());
	EXPECT_EQ(val.ToStringA(), " foo\n ");

	strJson = " \t\v 42 \t";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());

	strJson = " a 42 \t";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
}

TEST_F(CJSONTest, FromJSON_arrays)
{
	std::string strJson = "[]";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 0);
	EXPECT_TRUE(val[0].IsUndefined());

	strJson = "[1, 2, 3]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 3);
	EXPECT_EQ((int)val[0], 1);
	EXPECT_EQ((int)val[1], 2);
	EXPECT_EQ((int)val[2], 3);

	strJson = " [ \"][\" ] ";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 1);
	EXPECT_EQ(val[0].ToStringA(), "][");

	strJson = " [  73 ,  null\n\t,4.2]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 3);
	EXPECT_EQ((int)val[0], 73);
	EXPECT_TRUE(val[1].IsNull());
	EXPECT_DOUBLE_EQ((double)val[2], 4.2);

	strJson = "[1, \"two\", 3.14, true, null]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 5);
	EXPECT_EQ(val[0].ToInt(), 1);
	EXPECT_EQ(val[1].ToStringA(), "two");
	EXPECT_DOUBLE_EQ(val[2].ToDouble(), 3.14);
	EXPECT_TRUE(val[3].ToBool());
	EXPECT_TRUE(val[4].IsNull());
}

TEST_F(CJSONTest, FromJSON_arrays_inner)
{
	std::string strJson = "[[1, 2, 3]]";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 1);
	EXPECT_TRUE(val[0].IsArray());
	EXPECT_EQ(val[0].GetCount(), 3);
	EXPECT_EQ((int)val[0][0], 1);
	EXPECT_EQ((int)val[0][1], 2);
	EXPECT_EQ((int)val[0][2], 3);

	strJson = "[1, [2, 3], [4, [5, 6]], 7]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 4);
	EXPECT_EQ((int)val[0], 1);
	EXPECT_TRUE(val[1].IsArray());
	EXPECT_EQ(val[1].GetCount(), 2);
	EXPECT_EQ((int)val[1][0], 2);
	EXPECT_EQ((int)val[1][1], 3);
	EXPECT_TRUE(val[2].IsArray());
	EXPECT_EQ(val[2].GetCount(), 2);
	EXPECT_EQ((int)val[2][0], 4);
	EXPECT_TRUE(val[2][1].IsArray());
	EXPECT_EQ(val[2][1].GetCount(), 2);
	EXPECT_EQ((int)val[2][1][0], 5);
	EXPECT_EQ((int)val[2][1][1], 6);
	EXPECT_EQ((int)val[3], 7);

	strJson = "[[],[[]],[[],[]]]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsArray());
	EXPECT_EQ(val.GetCount(), 3);
	EXPECT_TRUE(val[0].IsArray());
	EXPECT_EQ(val[0].GetCount(), 0);
	EXPECT_TRUE(val[1].IsArray());
	EXPECT_EQ(val[1].GetCount(), 1);
	EXPECT_TRUE(val[1][0].IsArray());
	EXPECT_EQ(val[1][0].GetCount(), 0);
	EXPECT_TRUE(val[2].IsArray());
	EXPECT_EQ(val[2].GetCount(), 2);
	EXPECT_TRUE(val[2][0].IsArray());
	EXPECT_EQ(val[2][0].GetCount(), 0);
	EXPECT_TRUE(val[2][1].IsArray());
	EXPECT_EQ(val[2][1].GetCount(), 0);
}

TEST_F(CJSONTest, FromJSON_arrays_invalid)
{
	// no closing bracket
	std::string strJson = "[";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "[1,2,3";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// trailing comma
	strJson = "[1,2,]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// leading comma
	strJson = "[,1]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// missing comma
	strJson = "[1 2, 3]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// extra commas
	strJson = "[1,,2,3]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "[,]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// invalid elements
	strJson = "[1, 2, undefined, 4]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// non-JSON values as elements
	strJson = "[1, 2, f(), 4]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// invalid syntax within elements
	strJson = "[\"foo\", 123, [1, 2, }, 4]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// whitespace issues
	strJson = "[1,  2  3, 4]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// empty array without brackets
	strJson = "]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// extra brackets
	strJson = "[[1, 2, 3]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "[1, 2, 3]]";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// elements outside of the array
	strJson = "[1, 2, 3], 5";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "[1, 2, 3] 4";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
}

TEST_F(CJSONTest, FromJSON_objects)
{
	std::string strJson = "{}";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 0);

	strJson = "{\"name\" : \"Foo\"}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 1);
	EXPECT_EQ(val["name"].ToStringA(), "Foo");

	strJson = "{\"name\":\"Bar\",\"value\":42}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 2);
	EXPECT_EQ(val["name"].ToStringA(), "Bar");
	EXPECT_EQ((int)val["value"], 42);

	strJson = "{\"\\u0070\\u0069\":\n3.1415\t, \"\":null ,\"flag\":true,\"arr \":[4,2],\"str\":\"test\"}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 5);
	EXPECT_DOUBLE_EQ((double)val["pi"], 3.1415);
	EXPECT_TRUE(val[""].IsNull());
	EXPECT_EQ((bool)val["flag"], true);
	EXPECT_TRUE(val["arr "].IsArray());
	EXPECT_EQ(val["arr "].GetCount(), 2);
	EXPECT_EQ((int)val["arr "][0], 4);
	EXPECT_EQ((int)val["arr "][1], 2);
	EXPECT_EQ(val["str"].ToStringA(), "test");
}

TEST_F(CJSONTest, FromJSON_objects_as_typed_arrays)
{
	// typed arrays in JSON are represented as objects with properties "0", "1", ... and corresponding integer values
	// if you still want to make typed array from such object, you need to do it manually
	std::string strJson = "{\"0\":26,\"1\":179,\"2\":254,\"3\":255,\"4\":0}";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ((int)val["0"], 26);
	EXPECT_EQ((int)val["1"], 179);
	EXPECT_EQ((int)val["2"], 254);
	EXPECT_EQ((int)val["3"], 255);
	EXPECT_EQ((int)val["4"], 0);
}

TEST_F(CJSONTest, FromJSON_objects_inner)
{
	std::string strJson = "{\"inner\":{}}";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 1);
	EXPECT_TRUE(val["inner"].IsObject());
	EXPECT_TRUE(val["inner"].GetPropertyNames().empty());
}

TEST_F(CJSONTest, FromJSON_objects_and_arrays)
{
	// more complex example
	std::string strJson = \
		"{\n"
		"  \"name\": \"Foo\",\n"
		"  \"parameters\": {\n"
		"    \"size\": 42,\n"
		"    \"arr\": [\n"
		"      null,\n"
		"      [],\n"
		"      [42, \"test\", 2.71828],\n"
		"      {\n"
		"        \"name\": \"Bar\",\n"
		"        \"good\": false\n"
		"      },\n"
		"      null,\n"
		"      \"abc de f\",\n"
		"      \"\"\n"
		"    ],\n"
		"    \"0\": 0,\n"
		"    \"data\": {\n"
		"      \"data\": null,\n"
		"      \"count\": 0\n"
		"    }\n"
		"  },\n"
		"  \"\": \"empty\"\n"
		"}\n";

	CValue val = CValue::FromJSON(strJson);

	// Check root-level object
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 3);
	// Check "name" property
	EXPECT_EQ(val["name"].ToStringA(), "Foo");
	// Check "parameters" object
	EXPECT_TRUE(val["parameters"].IsObject());
	EXPECT_EQ(val["parameters"].GetPropertyNames().size(), 4);
	// Check "size" property
	EXPECT_EQ((int)val["parameters"]["size"], 42);
	// Check "arr" property (array)
	EXPECT_TRUE(val["parameters"]["arr"].IsArray());
	EXPECT_EQ(val["parameters"]["arr"].GetCount(), 7);
	// First element of "arr" (null)
	EXPECT_TRUE(val["parameters"]["arr"][0].IsNull());
	// Second element of "arr" (empty array)
	EXPECT_TRUE(val["parameters"]["arr"][1].IsArray());
	EXPECT_EQ(val["parameters"]["arr"][1].GetCount(), 0);
	// Third element of "arr" (array with mixed types)
	EXPECT_TRUE(val["parameters"]["arr"][2].IsArray());
	EXPECT_EQ(val["parameters"]["arr"][2].GetCount(), 3);
	EXPECT_EQ((int)val["parameters"]["arr"][2][0], 42);
	EXPECT_EQ(val["parameters"]["arr"][2][1].ToStringA(), "test");
	EXPECT_DOUBLE_EQ((double)val["parameters"]["arr"][2][2], 2.71828);
	// Fourth element of "arr" (object)
	EXPECT_TRUE(val["parameters"]["arr"][3].IsObject());
	EXPECT_EQ(val["parameters"]["arr"][3].GetPropertyNames().size(), 2);
	EXPECT_EQ(val["parameters"]["arr"][3]["name"].ToStringA(), "Bar");
	EXPECT_TRUE(val["parameters"]["arr"][3]["good"].IsBool());
	EXPECT_FALSE(val["parameters"]["arr"][3]["good"]);
	// Fifth element of "arr" (null)
	EXPECT_TRUE(val["parameters"]["arr"][4].IsNull());
	// Sixth element of "arr" (string)
	EXPECT_EQ(val["parameters"]["arr"][5].ToStringA(), "abc de f");
	// Seventh element of "arr" (empty string)
	EXPECT_EQ(val["parameters"]["arr"][6].ToStringA(), "");
	// Check "0" property (numeric value)
	EXPECT_EQ((int)val["parameters"]["0"], 0);
	// Check "data" object inside "parameters"
	EXPECT_TRUE(val["parameters"]["data"].IsObject());
	EXPECT_EQ(val["parameters"]["data"].GetPropertyNames().size(), 2);
	// Check "data" property inside "data" (null)
	EXPECT_TRUE(val["parameters"]["data"]["data"].IsNull());
	// Check "count" property inside "data"
	EXPECT_EQ((int)val["parameters"]["data"]["count"], 0);
}

TEST_F(CJSONTest, FromJSON_objects_invalid)
{
	// no closing bracket
	std::string strJson = "{";
	CValue val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"key1\": \"value1\", \"key2\": 42";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// trailing comma
	strJson = "{ \"key1\": \"value1\", \"key2\": 42, }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// duplicating properties are technically allowed. Only the last value will apply.
	strJson = "{ \"key\": \"value1\", \"key\": \"value2\" }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsObject());
	EXPECT_EQ(val.GetPropertyNames().size(), 1);
	EXPECT_EQ(val["key"].ToStringA(), "value2");
	// keys are not enslosed in double quotes
	strJson = "{ key1: \"value1\", \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// missing colon between the key and the value
	strJson = "{ \"key1\" \"value1\", \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{\"key\"}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{key}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// wrong value formats
	strJson = "{ \"key1\": undefined, \"key2\": 42 }"; ;
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"key1\": 12.34.56, \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"key1\": \"value1, \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// non-string key
	strJson = "{ 123: \"value\", \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// missing coma
	strJson = "{ \"key1\": \"value1\" \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// some invalid inner object
	strJson = "{ \"outerKey\": { \"innerKey\": \"value\", \"anotherInnerKey\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"outerKey\": { innerKey: \"value\" } }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"outerKey\": { \"innerKey\" \"value\" } }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"outerKey\": { \"innerKey\": \"value\", }, \"key2\": 42 }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{ \"outerKey\": { \"innerKey\": \"value\", \"anotherInnerKey\": 42, \"thirdKey\": { \"deepKey\": \"value\" } }";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// no opening bracket
	strJson = "}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "\"key1\": \"value1\", \"key2\": 42}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// extra brackets
	strJson = "{{\"key1\": \"value1\", \"key2\": 42}}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{{\"key1\": \"value1\", \"key2\": 42}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{\"key1\": \"value1\", \"key2\": 42}}";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	// elements outside of the object
	strJson = "{\"key1\": \"value1\", \"key2\": 42}, \"key3\": 3.14";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
	strJson = "{\"key1\": \"value1\", \"key2\": 42} \"key3\": 3.14";
	val = CValue::FromJSON(strJson);
	EXPECT_TRUE(val.IsUndefined());
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
