//#include "gtest/gtest.h"
#include "js_internal/js_base.h"
#include "json/serialization.h""

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
	CColorRGB m_oRGB;
	NSJSON::CValue m_nAlpha;

public:
	CColorRGBA()
	{
		addMember(&m_oRGB, "rgb");
		addMember(&m_nAlpha, "alpha");
	}
};

class CFontFamily : public NSJSON::CObject
{
public:
	NSJSON::CValue m_sFontName;
	NSJSON::CValue m_bBold;
	NSJSON::CValue m_bItalic;

public:
	CFontFamily()
	{
		addMember(&m_sFontName, "fontName");
		addMember(&m_bBold, "bold");
		addMember(&m_bItalic, "italic");
	}
};

class CTextParameters : public NSJSON::CObject
{
public:
	NSJSON::CValue m_nSize;
	CFontFamily m_oFont;
	CColorRGBA m_oColor;
	NSJSON::CObject m_oExtras;
	NSJSON::CArray m_arrNumbers;

public:
	CTextParameters()
	{
		addMember(&m_nSize, "size");
		addMember(&m_oFont, "font");
		addMember(&m_oColor, "color");
		addMember(&m_oExtras, "extras");
		addMember(&m_arrNumbers, "numbers");
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
	oTextPr.m_oColor = oColorRGBA;
	oTextPr.m_oFont.m_sFontName = std::wstring(L"Times New Roman");
	oTextPr.m_oFont.m_bBold = true;
	// undefined member:
//	oTextPr.m_oFont.m_bItalic = true;
	// null member:
	oTextPr.m_oExtras.setNull();
	oTextPr.m_arrNumbers.add(new NSJSON::CValue(10000));
	oTextPr.m_arrNumbers.addNull();
	oTextPr.m_arrNumbers.add(new NSJSON::CValue(42));
	oTextPr.m_arrNumbers.add(new NSJSON::CValue(0));
	NSJSON::CArray* pArray = new NSJSON::CArray();
	pArray->add(new NSJSON::CValue(std::string("test!")));
	pArray->addNull();
	oTextPr.m_arrNumbers.add(pArray);

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
