#ifndef _BUILD_NATIVE_HASH_EMBED_H_
#define _BUILD_NATIVE_HASH_EMBED_H_

#include "../../js_internal/js_base.h"

using namespace NSJSBase;
class CTestEmbed : public CJSEmbedObject
{
public:
	CTestEmbed()
	{
	}

	~CTestEmbed()
	{
	}

	virtual void* getObject() override { return NULL; }


	JSSmart<CJSValue> FunctionSum(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2)
	{
		int n1 = param1->toInt32();
		int n2 = param2->toInt32();
		return CJSContext::createInt(n1 + n2);
	}

	JSSmart<CJSValue> FunctionSquare(JSSmart<CJSValue> param)
	{
		int n = param->toInt32();
		return CJSContext::createInt(n * n);
	}

	JSSmart<CJSValue> FunctionDel(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2)
	{
		int n1 = param1->toInt32();
		int n2 = param2->toInt32();
		return CJSContext::createInt(n1 / n2);
	}

	JSSmart<CJSValue> FunctionGet()
	{
		return CJSContext::createInt(42);
	}

	// @embed_external
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_
