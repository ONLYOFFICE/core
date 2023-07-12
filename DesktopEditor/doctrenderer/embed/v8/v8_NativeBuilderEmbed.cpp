// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../NativeBuilderEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSNativeBuilderEmbed
{
#define CURRENTWRAPPER CBuilderEmbed

	FUNCTION_WRAPPER_V8_2(_OpenFile, OpenFile)
	FUNCTION_WRAPPER_V8_1(_CreateFile, CreateFile)
	FUNCTION_WRAPPER_V8_1(_SetTmpFolder, SetTmpFolder)
	FUNCTION_WRAPPER_V8_3(_SaveFile, SaveFile)
	FUNCTION_WRAPPER_V8_0(_CloseFile, CloseFile)
	FUNCTION_WRAPPER_V8_2(_OpenTmpFile, OpenTmpFile)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "OpenFile",	_OpenFile);
		NSV8Objects::Template_Set(result, "CreateFile",	_CreateFile);
		NSV8Objects::Template_Set(result, "SetTmpFolder",	_SetTmpFolder);
		NSV8Objects::Template_Set(result, "SaveFile",	_SaveFile);
		NSV8Objects::Template_Set(result, "CloseFile",	_CloseFile);
		NSV8Objects::Template_Set(result, "OpenTmpFile",	_OpenTmpFile);

		return handle_scope.Escape(result);
	}
}

class CBuilderEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSNativeBuilderEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CBuilderEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CBuilderEmbedAdapter();
	return m_pAdapter;
}

std::string CBuilderEmbed::getName() { return "CBuilderEmbed"; }

CJSEmbedObject* CBuilderEmbed::getCreator()
{
	return new CBuilderEmbed();
}
