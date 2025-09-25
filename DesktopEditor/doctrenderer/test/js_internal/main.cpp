#include "js_internal/js_base.h"

#ifdef JS_INTERNAL_GOOGLE_TEST
#include "gtest/gtest.h"
#include <algorithm>
#else
#include <iostream>
#endif

using namespace NSJSBase;

#ifdef JS_INTERNAL_GOOGLE_TEST
// CJSObject::getPropertyNames() tests
// run with "--gtest_filter=CGetPropertyNamesTest.*" to run only this test suite
class CGetPropertyNamesTest : public testing::Test
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

	std::vector<std::string> getObjectProperties(const std::string& objLiteral)
	{
		JSSmart<CJSObject> obj = m_pContext->runScript("(() => { return " + objLiteral + ";})();")->toObject();
		return obj->getPropertyNames();
	}

	// The order of properties of object in JS is not always the same as insertion order.
	// So this function sorts and compares two vectors with property names.
	bool compare(std::vector<std::string>& lhs, std::vector<std::string>& rhs)
	{
		std::sort(lhs.begin(), lhs.end());
		std::sort(rhs.begin(), rhs.end());
		return lhs == rhs;
	}

	std::string printInfo(const std::string& info, const std::vector<std::string>& names)
	{
		std::string ret = info;
		ret += '[';
		const int lenResult = names.size();
		for (int i = 0; i < lenResult; i++)
		{
			ret += "\"" + names[i] + "\"";
			if (i < lenResult - 1)
				ret += ',';
		}
		ret += "]\n";
		return ret;
	}

public:
	JSSmart<CJSContext> m_pContext;
};


TEST_F(CGetPropertyNamesTest, normal_object)
{
	std::vector<std::string> result = getObjectProperties("{number: 42, name: 'foo', arr: [1, 'abc', 2, 3], func() { return 'bar'; }}");
	std::vector<std::string> expected = {"number", "name", "arr", "func"};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, empty_object)
{
	std::vector<std::string> result = getObjectProperties("{}");
	std::vector<std::string> expected = {};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, only_one_function_in_object)
{
	std::vector<std::string> result = getObjectProperties("{add(a, b) { return a + b; }}");
	std::vector<std::string> expected = {"add"};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, inner_object)
{
	std::vector<std::string> result = getObjectProperties("{inner: {number: 42, name: 'foo'}}");
	std::vector<std::string> expected = {"inner"};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, object_with_null_and_undefined_properties)
{
	std::vector<std::string> result = getObjectProperties("{number: null, name: undefined, func() { return 'bar'; }}");
	std::vector<std::string> expected = {"number", "func"};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, object_with_only_undefined_properties)
{
	std::vector<std::string> result = getObjectProperties("{foo: undefined, bar: undefined}");
	std::vector<std::string> expected = {};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, array_as_object)
{
	std::vector<std::string> result = getObjectProperties("{0: 4, 1: 2, 2: undefined, 3: 'foo', 42: 'bar'}");
	std::vector<std::string> expected = {"0", "1", "3", "42"};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, object_with_prototype)
{
	m_pContext->runScript(
		"var personPrototype = { greet() { return 'Hello, world!'; } };"
		"function Person(name) { this.name = name; }"
	);

	JSSmart<CJSObject> obj = m_pContext->runScript("new Person('Foo');")->toObject();
	std::vector<std::string> result1 = obj->getPropertyNames();
	std::vector<std::string> expected1 = {"name"};
	EXPECT_TRUE(compare(result1, expected1)) << printInfo("Actual: ", result1) << printInfo("Expected: ", expected1);

	m_pContext->runScript("Person.prototype.greet = personPrototype.greet;");
	std::vector<std::string> result2 = obj->getPropertyNames();
	std::vector<std::string> expected2 = {"name", "greet"};
	EXPECT_TRUE(compare(result2, expected2)) << printInfo("Actual: ", result2) << printInfo("Expected: ", expected2);
}

// CJSContext::JSON_Stringify() tests
// run with "--gtest_filter=CJSONStringifyTest.*" to run only this test suite
class CJSONStringifyTest : public testing::Test
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

	JSSmart<CJSObject> createObject(const std::string& objLiteral)
	{
		return m_pContext->runScript("(() => { return " + objLiteral + ";})();")->toObject();
	}

public:
	JSSmart<CJSContext> m_pContext;
};

TEST_F(CJSONStringifyTest, undefined)
{
	JSSmart<CJSValue> jsValue = CJSContext::createUndefined();
	std::string sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "");
}

TEST_F(CJSONStringifyTest, null)
{
	JSSmart<CJSValue> jsValue = CJSContext::createNull();
	std::string sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "null");
}

TEST_F(CJSONStringifyTest, numbers)
{
	JSSmart<CJSValue> jsValue = CJSContext::createInt(42);
	std::string sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "42");

	jsValue = CJSContext::createUInt(7);
	sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "7");

	jsValue = CJSContext::createDouble(3.1415926535);
	sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "3.1415926535");
}

TEST_F(CJSONStringifyTest, strings)
{
	JSSmart<CJSValue> jsValue = CJSContext::createString("");
	std::string sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "\"\"");

	jsValue = CJSContext::createString("foo bar");
	sRes = m_pContext->JSON_Stringify(jsValue);
	EXPECT_EQ(sRes, "\"foo bar\"");
}

TEST_F(CJSONStringifyTest, arrays)
{
	JSSmart<CJSArray> jsArr = CJSContext::createArray(0);
	std::string sRes = m_pContext->JSON_Stringify(jsArr->toValue());
	EXPECT_EQ(sRes, "[]");

	jsArr = CJSContext::createArray(4);
	jsArr->set(0, 42);
	// inner array
	JSSmart<CJSArray> jsArrInner = CJSContext::createArray(2);
	jsArrInner->set(0, CJSContext::createString("foo"));
	jsArrInner->set(1, CJSContext::createDouble(2.71828));
	jsArr->set(1, jsArrInner->toValue());

	jsArr->set(2, CJSContext::createNull());
	jsArr->set(3, CJSContext::createUndefined());
	sRes = m_pContext->JSON_Stringify(jsArr->toValue());
	EXPECT_EQ(sRes, "[42,[\"foo\",2.71828],null,null]");
}

TEST_F(CJSONStringifyTest, typed_arrays)
{
	BYTE* data = NSAllocator::Alloc(4);
	data[0] = 0x1A;
	data[1] = 0x54;
	data[2] = 0xFE;
	data[3] = 0xFF;
	JSSmart<CJSTypedArray> jsTypedArr = CJSContext::createUint8Array(data, 4, false);

	std::string sRes = m_pContext->JSON_Stringify(jsTypedArr->toValue());
	EXPECT_EQ(sRes, "{\"0\":26,\"1\":84,\"2\":254,\"3\":255}");
}

TEST_F(CJSONStringifyTest, empty_object)
{
	JSSmart<CJSObject> jsObj = CJSContext::createObject();
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{}");

	jsObj->set("number", CJSContext::createInt(42));
	jsObj->set("name", CJSContext::createString("foo"));
	sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{\"number\":42,\"name\":\"foo\"}");
}

TEST_F(CJSONStringifyTest, normal_object)
{
	JSSmart<CJSObject> jsObj = createObject("{number: 42, name: 'foo', arr: [1, 'abc', 2, 3], func() { return 'bar'; }}");
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{\"number\":42,\"name\":\"foo\",\"arr\":[1,\"abc\",2,3]}");
}

TEST_F(CJSONStringifyTest, only_one_function_in_object)
{
	JSSmart<CJSObject> jsObj = createObject("{add(a, b) { return a + b; }}");
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{}");
}

TEST_F(CJSONStringifyTest, inner_object)
{
	JSSmart<CJSObject> jsObj = createObject("{inner: {number: 42, name: 'foo'}, own: 'bar'}");
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{\"inner\":{\"number\":42,\"name\":\"foo\"},\"own\":\"bar\"}");
}

TEST_F(CJSONStringifyTest, object_with_null_and_undefined_properties)
{
	JSSmart<CJSObject> jsObj = createObject("{number: null, name: undefined, func() { return 'bar'; }}");
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{\"number\":null}");
}

TEST_F(CJSONStringifyTest, object_with_only_undefined_properties)
{
	JSSmart<CJSObject> jsObj = createObject("{foo: undefined, bar: undefined}");
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{}");
}

TEST_F(CJSONStringifyTest, array_as_object)
{
	JSSmart<CJSObject> jsObj = createObject("{0: 4, 1: 2, 2: undefined, 3: 'foo', 42: 'bar'}");
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{\"0\":4,\"1\":2,\"3\":\"foo\",\"42\":\"bar\"}");
}

TEST_F(CJSONStringifyTest, object_with_prototype)
{
	m_pContext->runScript(
		"var personPrototype = { age: 42, greet() { return 'Hello, world!'; } };"
		"function Person(name) { this.name = name; }"
	);

	JSSmart<CJSObject> jsObj = m_pContext->runScript("new Person('Foo');")->toObject();
	std::string sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	EXPECT_EQ(sRes, "{\"name\":\"Foo\"}");

	m_pContext->runScript("Object.assign(Person.prototype, personPrototype);");
	sRes = m_pContext->JSON_Stringify(jsObj->toValue());
	// expect the same result, because JSON.stringify does not preserve any of the not-owned properties of the object
	EXPECT_EQ(sRes, "{\"name\":\"Foo\"}");
}

#else
int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	CJSContextScope scope(pContext);

	JSSmart<CJSObject> jsObj = CJSContext::createObject();
	jsObj->set("number", CJSContext::createInt(42));
	jsObj->set("name", CJSContext::createString("foo"));

	std::cout << pContext->JSON_Stringify(jsObj->toValue()) << std::endl;

	return 0;
}
#endif	// JS_INTERNAL_GOOGLE_TEST
