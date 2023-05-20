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

// @embed_external
};

#endif // _BUILD_NATIVE_HASH_EMBED_H_
