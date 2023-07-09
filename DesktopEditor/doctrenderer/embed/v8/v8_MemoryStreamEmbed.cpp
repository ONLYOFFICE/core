// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../MemoryStreamEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSMemoryStreamEmbed
{
#define CURRENTWRAPPER CMemoryStreamEmbed

	FUNCTION_WRAPPER_V8_3(_Copy, Copy)
	FUNCTION_WRAPPER_V8_0(_ClearNoAttack, ClearNoAttack)
	FUNCTION_WRAPPER_V8_1(_WriteByte, WriteByte)
	FUNCTION_WRAPPER_V8_1(_WriteBool, WriteBool)
	FUNCTION_WRAPPER_V8_1(_WriteLong, WriteLong)
	FUNCTION_WRAPPER_V8_1(_WriteDouble, WriteDouble)
	FUNCTION_WRAPPER_V8_1(_WriteDouble2, WriteDouble2)
	FUNCTION_WRAPPER_V8_1(_WriteStringA, WriteStringA)
	FUNCTION_WRAPPER_V8_1(_WriteString, WriteString)
	FUNCTION_WRAPPER_V8_1(_WriteString2, WriteString2)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "Copy",	_Copy);
		NSV8Objects::Template_Set(result, "ClearNoAttack",	_ClearNoAttack);
		NSV8Objects::Template_Set(result, "WriteByte",	_WriteByte);
		NSV8Objects::Template_Set(result, "WriteBool",	_WriteBool);
		NSV8Objects::Template_Set(result, "WriteLong",	_WriteLong);
		NSV8Objects::Template_Set(result, "WriteDouble",	_WriteDouble);
		NSV8Objects::Template_Set(result, "WriteDouble2",	_WriteDouble2);
		NSV8Objects::Template_Set(result, "WriteStringA",	_WriteStringA);
		NSV8Objects::Template_Set(result, "WriteString",	_WriteString);
		NSV8Objects::Template_Set(result, "WriteString2",	_WriteString2);

		return handle_scope.Escape(result);
	}
}

class CMemoryStreamEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSMemoryStreamEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CMemoryStreamEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CMemoryStreamEmbedAdapter();
	return m_pAdapter;
}

std::string CMemoryStreamEmbed::getName() { return "CMemoryStreamEmbed"; }

CJSEmbedObject* CMemoryStreamEmbed::getCreator()
{
	return new CMemoryStreamEmbed();
}
