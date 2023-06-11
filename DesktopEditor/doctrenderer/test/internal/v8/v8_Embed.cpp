// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.

#include "../Embed.h"
#include "js_embed.h"

class CTestEmbedAdapter : public CJSEmbedObjectAdapterV8
{
public:
	virtual std::vector<std::string> getMethodNames() override
	{
		return std::vector<std::string> {
#ifdef ENABLE_SUM_DEL
			"FunctionSum",
			"FunctionDel",
#endif
#ifdef ENABLE_GET
			"FunctionGet",
#endif
			"FunctionSquare",
		};
	}

	virtual void initFunctions(CJSEmbedObject* pNativeObjBase) override
	{
		CTestEmbed* pNativeObj = static_cast<CTestEmbed*>(pNativeObjBase);
		m_functions = std::vector<EmbedFunctionType> {
#ifdef ENABLE_SUM_DEL
			[pNativeObj](CJSFunctionArguments* args) { return pNativeObj->FunctionSum(args->Get(0), args->Get(1)); },
			[pNativeObj](CJSFunctionArguments* args) { return pNativeObj->FunctionDel(args->Get(0), args->Get(1)); },
#endif
#ifdef ENABLE_GET
			[pNativeObj](CJSFunctionArguments* args) { return pNativeObj->FunctionGet(); },
#endif
			[pNativeObj](CJSFunctionArguments* args) { return pNativeObj->FunctionSquare(args->Get(0)); },
		};
	}
};

CJSEmbedObjectAdapterBase* CTestEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CTestEmbedAdapter();
	return m_pAdapter;
}

std::string CTestEmbed::getName() { return "CTestEmbed"; }

CJSEmbedObject* CTestEmbed::getCreator()
{
	return new CTestEmbed();
}
