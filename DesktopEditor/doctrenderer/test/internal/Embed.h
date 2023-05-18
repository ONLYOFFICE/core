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


	JSSmart<CJSValue> FunctionSum(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2);
	JSSmart<CJSValue> FunctionSquare(JSSmart<CJSValue> param);
	JSSmart<CJSValue> FunctionDel(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2);
	JSSmart<CJSValue> FunctionGet();

// THIS CODE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, SIMPLY REMOVE ALL OF IT, SPECIFY "// @embed_external" COMMENT AND THEN RUN SCRIPT AS BEFORE.
public:
	static std::string getName() { return "CTestEmbed"; }

	static CJSEmbedObject* getCreator()
	{
		return new CTestEmbed();
	}

	virtual std::vector<std::string> getNames() override
	{
		return std::vector<std::string> {
			"FunctionSum",
			"FunctionSquare",
			"FunctionDel",
			"FunctionGet"
		};
	}

	void initFunctions() override
	{
		m_functions.resize(4);
		m_functions[0] = JS_FUNCTION_EMBED_2(FunctionSum);
		m_functions[1] = JS_FUNCTION_EMBED_1(FunctionSquare);
		m_functions[2] = JS_FUNCTION_EMBED_2(FunctionDel);
		m_functions[3] = JS_FUNCTION_EMBED_0(FunctionGet);
	}
// END OF GENERATED CODE
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_
