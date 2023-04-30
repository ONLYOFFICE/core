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

public:
	static std::string getName() { return "CTestEmbed"; }
	static std::string getScript()
	{
		return "function CTestEmbed(){ this.native = CreateEmbedObject(\"CTestEmbed\"); }\
CTestEmbed.prototype.FunctionSum = function(a, b) { return this.native.Call(0, a, b); }";
	}
	static CJSEmbedObject* getCreator()
	{
		return new CTestEmbed();
	}

	virtual JSSmart<CJSValue> Call(const int& index, CJSFunctionArguments* args) override
	{
		switch (index)
		{
		case 0:
		{
			return FunctionSum(args->Get(0), args->Get(1));
		}
		default:
			break;
		}
		return NULL;
	}
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_
