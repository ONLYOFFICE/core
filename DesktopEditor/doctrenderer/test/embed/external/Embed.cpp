#include "Embed.h"
#include <iostream>

CTestEmbed::CTestEmbed()
{
	std::cout << "debug: CTestEmbed constructed" << std::endl;
}

CTestEmbed::~CTestEmbed()
{
	std::cout << "debug: CTestEmbed destroyed" << std::endl;
}

#ifdef ENABLE_SUM_DEL
JSSmart<CJSValue> CTestEmbed::FunctionSum(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2)
{
	int n1 = param1->toInt32();
	int n2 = param2->toInt32();
	return CJSContext::createInt(n1 + n2);
}

JSSmart<CJSValue> CTestEmbed::FunctionDel(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2)
{
	int n1 = param1->toInt32();
	int n2 = param2->toInt32();
	return CJSContext::createInt(n1 / n2);
}
#endif

JSSmart<CJSValue> CTestEmbed::FunctionSquare(JSSmart<CJSValue> param)
{
	int n = param->toInt32();
	return CJSContext::createInt(n * n);
}

#ifdef ENABLE_GET
JSSmart<CJSValue> CTestEmbed::FunctionGet()
{
	return CJSContext::createInt(42);
}
#endif

JSSmart<CJSValue> CTestEmbed::FunctionSpecial()
{
	return CJSContext::createUndefined();
}
