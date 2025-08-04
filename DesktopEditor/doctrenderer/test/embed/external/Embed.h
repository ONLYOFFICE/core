#ifndef CTESTEMBED_H_
#define CTESTEMBED_H_

#include "js_internal/js_base.h"

#define ENABLE_SUM_DEL
#define ENABLE_GET

using namespace NSJSBase;
class CTestEmbed : public CJSEmbedObject
{
public:
	CTestEmbed();
	~CTestEmbed();

	virtual void* getObject() override { return NULL; }

#ifdef ENABLE_SUM_DEL
	JSSmart<CJSValue> FunctionSum(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2);
	JSSmart<CJSValue> FunctionDel(JSSmart<CJSValue> param1, JSSmart<CJSValue> param2);
#endif
	JSSmart<CJSValue> FunctionSquare(JSSmart<CJSValue> param);
#ifdef ENABLE_GET
	JSSmart<CJSValue> FunctionGet();
#endif
	/*[noexport]*/ JSSmart<CJSValue> FunctionSpecial();

	DECLARE_EMBED_METHODS
};

#endif // CTESTEMBED_H_
