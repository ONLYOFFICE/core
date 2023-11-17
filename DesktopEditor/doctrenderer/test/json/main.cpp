//#include "gtest/gtest.h"
#include "js_internal/js_base.h"
//#include "json/serialization.h"
#include "json/json.h"

#include <iostream>
#include <string>

using namespace NSJSBase;
using namespace NSJSON;

/*
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

public:
	JSSmart<CJSContext> m_pContext;
};

TEST_F(..., ...)
{
	...
}
*/

int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	CJSContextScope scope(pContext);

	// top object with text parameters
	CValue textPr = CValue::CreateObject();

	CValue colorRGB = CValue::CreateObject();
	colorRGB["r"] = 12;
	colorRGB["g"] = 34;
	colorRGB["b"] = 56;

	CValue colorRGBA = CValue::CreateObject();
	colorRGBA["rgb"] = colorRGB;
	colorRGBA["alpha"] = 80;

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
	/*
	BYTE* pData = new BYTE[4];
	pData[0] = 11;
	pData[1] = 23;
	pData[2] = 58;
	pData[3] = 13;
	// add typed array
	...
	*/

	// convert to JS
	/*
	JSSmart<CJSObject> jsObj = toJS(textPr)->toObject();
	JSSmart<CJSObject> global = pContext->GetGlobal();
	global->set("textPr", jsObj);
	JSSmart<CJSValue> ret = pContext->runScript("(function () { return JSON.stringify(textPr, null, 4); })();");
	if (ret.IsInit())
	{
		std::cout << ret->toStringA() << std::endl;
	}
	*/

	return 0;
}
