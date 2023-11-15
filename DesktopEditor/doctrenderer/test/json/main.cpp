//#include "gtest/gtest.h"
#include "js_internal/js_base.h"
#include "json/serialization.h"

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

	// top object (declared
	CObject oTextPr;

	CObject* pColorRGB = new CObject;
	pColorRGB->set("r", new CValue(12));
	pColorRGB->set("g", new CValue(34));
	pColorRGB->set("b", new CValue(56));

	CObject* pColorRGBA = new CObject;
	pColorRGBA->set("rgb", pColorRGB);
	pColorRGBA->set("alpha", new CValue(80));

	oTextPr.set("size", new CValue(4.2));
	oTextPr.set("color", pColorRGBA);
	CObject* pFont = new CObject();
	oTextPr.set("font", pFont);
	pFont->set("fontName", new CValue(std::wstring(L"Times New Roman")));
	pFont->set("bold", new CValue(true));
	// undefined member:
	pFont->set("italic", new CValue());
	// null member:
	CObject* pExtras = new CObject();
	oTextPr.set("extras", pExtras);
	pExtras->setNull();
	// array
	CArray* pArrNumbers = new CArray();
	oTextPr.set("numbers", pArrNumbers);
	pArrNumbers->add(new CValue(10000));
	// add new elements to array via object's get()
	static_cast<CArray*>(oTextPr["numbers"])->addNull();
	static_cast<CArray*>(oTextPr["numbers"])->add(new CValue(42));
	static_cast<CArray*>(oTextPr["numbers"])->add(new CValue(0));
	// inner array
	CArray* pArrInner = new CArray();
	pArrInner->add(new CValue(std::string("test!")));
	pArrInner->add(new CValue(3.14));
	pArrNumbers->add(pArrInner);
	// typed array
	BYTE* pData = new BYTE[4];
	pData[0] = 11;
	pData[1] = 23;
	pData[2] = 58;
	pData[3] = 13;
	// typed array
	CTypedArray* pTypedArray = new CTypedArray(pData, 4);
	pArrInner->add(pTypedArray);

	JSSmart<CJSObject> jsObj = toJS(&oTextPr)->toObject();
	JSSmart<CJSObject> global = pContext->GetGlobal();
	global->set("textPr", jsObj);
	JSSmart<CJSValue> ret = pContext->runScript("(function () { return JSON.stringify(textPr, null, 4); })();");
	if (ret.IsInit())
	{
		std::cout << ret->toStringA() << std::endl;
	}

	return 0;
}
