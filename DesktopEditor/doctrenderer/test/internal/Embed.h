#ifndef _BUILD_NATIVE_HASH_EMBED_H_
#define _BUILD_NATIVE_HASH_EMBED_H_

#include "../../js_internal/js_base.h"

using namespace NSJSBase;
class CTestEmbed : public CJSEmbedObject
{
public:
	// NOTE: this method may be generated
	CTestEmbed() : CJSEmbedObject({
								  JS_FUNCTION_EMBED_2(FunctionSum),
								  JS_FUNCTION_EMBED_1(FunctionSquare),
								  JS_FUNCTION_EMBED_2(FunctionDel)})
	{
	}

	~CTestEmbed()
	{
	}

	virtual void* getObject() override { return NULL; }

	// NOTE: this method may be generated
	virtual std::vector<std::string> getNames() override
	{
		return std::vector<std::string> {
			"FunctionSum",
			"FunctionSquare",
			"FunctionDel"
		};
	}

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

public:
	// NOTE: this method may be generated
	static std::string getName() { return "CTestEmbed"; }

	static CJSEmbedObject* getCreator()
	{
		return new CTestEmbed();
	}
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_
