#ifndef _BUILD_NATIVE_HASH_EMBED_H_
#define _BUILD_NATIVE_HASH_EMBED_H_

#include "../../js_internal/js_base.h"

using namespace NSJSBase;
class CTestEmbed : public CJSEmbedObject
{
public:
	// NOTE: this may be generated
	// TODO: delegate m_functions initialization to CJSEmbedObject
	CTestEmbed()
	{
		m_functions.resize(3);
		m_functions[0] = JS_FUNCTION_EMBED_2(FunctionSum);
		m_functions[1] = JS_FUNCTION_EMBED_1(FunctionSquare);
		m_functions[2] = JS_FUNCTION_EMBED_2(FunctionDel);
	}

	~CTestEmbed()
	{
	}

	virtual void* getObject() override { return NULL; }

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
	// NOTE: this may be generated
	static std::string getName() { return "CTestEmbed"; }

	// NOTE: this may be generated
	static std::string getScript()
	{
		return "function CTestEmbed(){ this.native = CreateEmbedObject(\"CTestEmbed\"); }"
			   "CTestEmbed.prototype.FunctionSum = function(a, b) { return this.native.Call(0, a, b); };"
			   "CTestEmbed.prototype.FunctionSquare = function(a) { return this.native.Call(1, a); };";
	}

	static CJSEmbedObject* getCreator()
	{
		return new CTestEmbed();
	}
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_
