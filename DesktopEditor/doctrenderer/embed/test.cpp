#include "gtest/gtest.h"

#include <sstream>
#include <string>

#include "Default.h"
#include "../js_internal/js_base.h"

using namespace NSJSBase;

static std::string typedArrayToString(JSSmart<CJSTypedArray> oData)
{
	std::stringstream ss;
	for (int i = 0; i < oData->getCount(); i++)
	{
		ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned>(oData->getData().Data[i]);
	}

	return ss.str();
}


TEST(test_suit, test)
{
	JSSmart<CJSContext> oContext1 = new CJSContext;

	// Create first context
	oContext1->CreateContext();

	// Call hash() on first context
	CJSContextScope scope(oContext1);
	CreateDefaults(oContext1);
	JSSmart<CJSValue> oRes1 = oContext1->runScript(
		"var oHash = new CreateNativeHash;\n"
		"var str = 'test';\n"
		"var hash = oHash.hash(str, str.length, 0);");

	// Print first result
	JSSmart<CJSObject> oGlobal1 = oContext1->GetGlobal();
	JSSmart<CJSTypedArray> oHash = oGlobal1->get("hash")->toTypedArray();

	std::string sHashExp = "dd34716876364a02d0195e2fb9ae2d1b";

	ASSERT_EQ(typedArrayToString(oHash), sHashExp);
}
