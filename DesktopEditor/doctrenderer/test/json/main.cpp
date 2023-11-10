//#include "gtest/gtest.h"
#include "js_internal/js_base.h"
#include "json/json.h"

#include <iostream>
#include <string>

using namespace NSJSBase;

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

class CColorRGB : public NSJSON::CObject
{
public:
	NSJSON::CValue m_nR;
	NSJSON::CValue m_nG;
	NSJSON::CValue m_nB;

public:
	CColorRGB()
	{
		addMember(&m_nR, "r");
		addMember(&m_nG, "g");
		addMember(&m_nB, "b");
	}
};

class CColorRGBA : public NSJSON::CObject
{
public:
	NSJSON::CObject m_oRGB;
	NSJSON::CValue m_nAlpha;

public:
	CColorRGBA()
	{
		addMember(&m_oRGB, "rgb");
		addMember(&m_nAlpha, "alpha");
	}
};

class CTextParameters : public NSJSON::CObject
{
public:
	NSJSON::CValue m_nSize;
	NSJSON::CValue m_bBold;
	NSJSON::CValue m_sFontName;
	NSJSON::CObject m_oColor;
	NSJSON::CObject m_oExtras;

public:
	CTextParameters()
	{
		addMember(&m_nSize, "size");
		addMember(&m_bBold, "bold");
		addMember(&m_sFontName, "fontName");
		addMember(&m_oColor, "color");
		addMember(&m_oExtras, "extras");
	}
};

int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	CJSContextScope scope(pContext);

	CColorRGB oColorRGB;
	oColorRGB.m_nR = 12;
	oColorRGB.m_nG = 34;
	oColorRGB.m_nB = 56;

	CColorRGBA oColorRGBA;
	oColorRGBA.m_oRGB = oColorRGB;
	oColorRGBA.m_nAlpha = 80;

	CTextParameters oTextPr;
	oTextPr.m_nSize = 4.2;
	oTextPr.m_bBold = true;
	oTextPr.m_sFontName = std::string("Times New Roman");
	oTextPr.m_oColor = oColorRGBA;
	oTextPr.m_oExtras.setNull();

	JSSmart<CJSObject> jsObj = oTextPr.toJS()->toObject();
	JSSmart<CJSObject> global = pContext->GetGlobal();
	global->set("textPr", jsObj);
	JSSmart<CJSValue> ret = pContext->runScript("(function () { return JSON.stringify(textPr, null, 4); })();");
	if (ret.IsInit())
	{
		std::cout << ret->toStringA() << std::endl;
	}

	return 0;
}
