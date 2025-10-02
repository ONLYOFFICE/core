// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../DrawingFileEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSDrawingFileEmbed
{
#define CURRENTWRAPPER CDrawingFileEmbed

	FUNCTION_WRAPPER_V8_2(_OpenFile, OpenFile)
	FUNCTION_WRAPPER_V8_0(_CloseFile, CloseFile)
	FUNCTION_WRAPPER_V8_0(_GetType, GetType)
	FUNCTION_WRAPPER_V8_0(_GetErrorCode, GetErrorCode)
	FUNCTION_WRAPPER_V8_0(_GetInfo, GetInfo)
	FUNCTION_WRAPPER_V8_4(_GetPixmap, GetPixmap)
	FUNCTION_WRAPPER_V8_1(_DestroyPixmap, DestroyPixmap)
	FUNCTION_WRAPPER_V8_1(_GetLinks, GetLinks)
	FUNCTION_WRAPPER_V8_0(_GetStructure, GetStructure)
	FUNCTION_WRAPPER_V8_0(_GetInteractiveFormsInfo, GetInteractiveFormsInfo)
	FUNCTION_WRAPPER_V8_1(_GetInteractiveFormsFonts, GetInteractiveFormsFonts)
	FUNCTION_WRAPPER_V8_7(_GetInteractiveFormsAP, GetInteractiveFormsAP)
	FUNCTION_WRAPPER_V8_5(_GetButtonIcons, GetButtonIcons)
	FUNCTION_WRAPPER_V8_1(_GetAnnotationsInfo, GetAnnotationsInfo)
	FUNCTION_WRAPPER_V8_6(_GetAnnotationsAP, GetAnnotationsAP)
	FUNCTION_WRAPPER_V8_1(_GetFontBinary, GetFontBinary)
	FUNCTION_WRAPPER_V8_1(_GetGlyphs, GetGlyphs)
	FUNCTION_WRAPPER_V8_0(_DestroyTextInfo, DestroyTextInfo)
	FUNCTION_WRAPPER_V8_0(_IsNeedCMap, IsNeedCMap)
	FUNCTION_WRAPPER_V8_2(_ScanPage, ScanPage)
	FUNCTION_WRAPPER_V8_1(_GetImageBase64, GetImageBase64)
	FUNCTION_WRAPPER_V8_1(_FreeWasmData, FreeWasmData)
	FUNCTION_WRAPPER_V8_2(_SplitPages, SplitPages)
	FUNCTION_WRAPPER_V8_3(_MergePages, MergePages)
	FUNCTION_WRAPPER_V8_0(_UnmergePages, UnmergePages)
	FUNCTION_WRAPPER_V8_3(_RedactPage, RedactPage)
	FUNCTION_WRAPPER_V8_0(_UndoRedact, UndoRedact)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "OpenFile",	_OpenFile);
		NSV8Objects::Template_Set(result, "CloseFile",	_CloseFile);
		NSV8Objects::Template_Set(result, "GetType",	_GetType);
		NSV8Objects::Template_Set(result, "GetErrorCode",	_GetErrorCode);
		NSV8Objects::Template_Set(result, "GetInfo",	_GetInfo);
		NSV8Objects::Template_Set(result, "GetPixmap",	_GetPixmap);
		NSV8Objects::Template_Set(result, "DestroyPixmap",	_DestroyPixmap);
		NSV8Objects::Template_Set(result, "GetLinks",	_GetLinks);
		NSV8Objects::Template_Set(result, "GetStructure",	_GetStructure);
		NSV8Objects::Template_Set(result, "GetInteractiveFormsInfo",	_GetInteractiveFormsInfo);
		NSV8Objects::Template_Set(result, "GetInteractiveFormsFonts",	_GetInteractiveFormsFonts);
		NSV8Objects::Template_Set(result, "GetInteractiveFormsAP",	_GetInteractiveFormsAP);
		NSV8Objects::Template_Set(result, "GetButtonIcons",	_GetButtonIcons);
		NSV8Objects::Template_Set(result, "GetAnnotationsInfo",	_GetAnnotationsInfo);
		NSV8Objects::Template_Set(result, "GetAnnotationsAP",	_GetAnnotationsAP);
		NSV8Objects::Template_Set(result, "GetFontBinary",	_GetFontBinary);
		NSV8Objects::Template_Set(result, "GetGlyphs",	_GetGlyphs);
		NSV8Objects::Template_Set(result, "DestroyTextInfo",	_DestroyTextInfo);
		NSV8Objects::Template_Set(result, "IsNeedCMap",	_IsNeedCMap);
		NSV8Objects::Template_Set(result, "ScanPage",	_ScanPage);
		NSV8Objects::Template_Set(result, "GetImageBase64",	_GetImageBase64);
		NSV8Objects::Template_Set(result, "FreeWasmData",	_FreeWasmData);
		NSV8Objects::Template_Set(result, "SplitPages",	_SplitPages);
		NSV8Objects::Template_Set(result, "MergePages",	_MergePages);
		NSV8Objects::Template_Set(result, "UnmergePages",	_UnmergePages);
		NSV8Objects::Template_Set(result, "RedactPage", _RedactPage);
		NSV8Objects::Template_Set(result, "UndoRedact",	_UndoRedact);

		return handle_scope.Escape(result);
	}
}

class CDrawingFileEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSDrawingFileEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CDrawingFileEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CDrawingFileEmbedAdapter();
	return m_pAdapter;
}

std::string CDrawingFileEmbed::getName() { return "CDrawingFileEmbed"; }

CJSEmbedObject* CDrawingFileEmbed::getCreator()
{
	return new CDrawingFileEmbed();
}
