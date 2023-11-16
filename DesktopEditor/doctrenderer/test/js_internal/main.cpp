#include "gtest/gtest.h"

#include "js_internal/js_base.h"

#include <algorithm>

using namespace NSJSBase;

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
	std::vector<std::string> expected = {"number", "name", "func"};
	EXPECT_TRUE(compare(result, expected)) << printInfo("Actual: ", result) << printInfo("Expected: ", expected);
}

TEST_F(CGetPropertyNamesTest, array_as_object)
{
	std::vector<std::string> result = getObjectProperties("{0: 4, 1: 2, 2: undefined, 3: 'foo', 42: 'bar'}");
	std::vector<std::string> expected = {"0", "1", "2", "3", "42"};
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
