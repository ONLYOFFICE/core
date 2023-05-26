// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.

#include "../Embed.h"

std::string CTestEmbed::getName() { return "CTestEmbed"; }

CJSEmbedObject* CTestEmbed::getCreator()
{
	return new CTestEmbed();
}

void* CTestEmbed::GetDataForEmbedObject(void* data)
{
	return nullptr;
}

#ifndef JS_ENGINE_JAVASCRIPTCORE
std::vector<std::string> CTestEmbed::getMethodNames()
{
	return std::vector<std::string> {
		"FunctionSum",
		"FunctionSquare",
		"FunctionDel",
		"FunctionGet"
	};
}

void CTestEmbed::initFunctions()
{
	m_functions = std::vector<EmbedFunctionType> {
		[this](CJSFunctionArguments* args) { return this->FunctionSum(args->Get(0), args->Get(1)); },
		[this](CJSFunctionArguments* args) { return this->FunctionSquare(args->Get(0)); },
		[this](CJSFunctionArguments* args) { return this->FunctionDel(args->Get(0), args->Get(1)); },
		[this](CJSFunctionArguments* args) { return this->FunctionGet(); }
	};
}
#endif
