/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "../NativeBuilderEmbed.h"
#include "../../docbuilder_p.h"
#include "../../js_internal/v8/v8_base.h"

#define CURRENTWRAPPER CBuilderEmbed

FUNCTION_WRAPPER_V8_2(_builder_OpenFile,     builder_OpenFile)
FUNCTION_WRAPPER_V8_1(_builder_CreateFile,   builder_CreateFile)
FUNCTION_WRAPPER_V8_1(_builder_SetTmpFolder, builder_SetTmpFolder)
FUNCTION_WRAPPER_V8_3(_builder_SaveFile,     builder_SaveFile)
FUNCTION_WRAPPER_V8  (_builder_CloseFile,    builder_CloseFile)
FUNCTION_WRAPPER_V8_2(_builder_OpenTmpFile,  builder_OpenTmpFile)

#undef CURRENTWRAPPER
#define CURRENTWRAPPER CBuilderDocumentEmbed

FUNCTION_WRAPPER_V8(_builder_doc_IsValid,     builder_doc_IsValid)
FUNCTION_WRAPPER_V8(_builder_doc_GetBinary,   builder_doc_GetBinary)
FUNCTION_WRAPPER_V8(_builder_doc_GetFolder,   builder_doc_GetFolder)
FUNCTION_WRAPPER_V8(_builder_doc_CloseFile,   builder_doc_CloseFile)
FUNCTION_WRAPPER_V8(_builder_doc_GetImageMap, builder_doc_GetImageMap)

v8::Local<v8::Value> _builder_CreateNativeTmpDoc(v8::Isolate* isolate, NSDoctRenderer::CDocBuilder* pBuilder, const std::wstring& sFile, const std::wstring& sParams)
{
    v8::Local<v8::ObjectTemplate> _template = v8::ObjectTemplate::New(isolate);
    _template->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    NSV8Objects::Template_Set(_template, "IsValid",     _builder_doc_IsValid);
    NSV8Objects::Template_Set(_template, "GetBinary",   _builder_doc_GetBinary);
    NSV8Objects::Template_Set(_template, "GetFolder",   _builder_doc_GetFolder);
    NSV8Objects::Template_Set(_template, "Close",       _builder_doc_CloseFile);
    NSV8Objects::Template_Set(_template, "GetImageMap", _builder_doc_GetImageMap);

    CBuilderDocumentEmbed* _embed = new CBuilderDocumentEmbed();
    _embed->m_pBuilder = pBuilder;
    _embed->OpenFile(sFile, sParams);

    v8::Local<v8::Object> obj = _template->NewInstance(V8ContextOneArg).ToLocalChecked();
    obj->SetInternalField(0, v8::External::New(isolate, _embed));

    return obj;
}
v8::Local<v8::Value> _builder_CreateNative(v8::Isolate* isolate, NSDoctRenderer::CDocBuilder* builder)
{
    v8::Local<v8::ObjectTemplate> _template = v8::ObjectTemplate::New(isolate);
    _template->SetInternalFieldCount(1);

    NSV8Objects::Template_Set(_template, "OpenFile",     _builder_OpenFile);
    NSV8Objects::Template_Set(_template, "CreateFile",   _builder_CreateFile);
    NSV8Objects::Template_Set(_template, "SetTmpFolder", _builder_SetTmpFolder);
    NSV8Objects::Template_Set(_template, "SaveFile",     _builder_SaveFile);
    NSV8Objects::Template_Set(_template, "CloseFile",    _builder_CloseFile);
    NSV8Objects::Template_Set(_template, "OpenTmpFile",  _builder_OpenTmpFile);

    CBuilderEmbed* _embed = new CBuilderEmbed();
    _embed->m_pBuilder = builder;

    v8::Local<v8::Object> obj = _template->NewInstance(V8ContextOneArg).ToLocalChecked();
    obj->SetInternalField(0, v8::External::New(isolate, _embed));

    return obj;
}

void builder_CreateNativeTmpDoc(const std::string& name, JSSmart<CJSContext> context, NSDoctRenderer::CDocBuilder* builder, const std::wstring& sFile, const std::wstring& sParams)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_context->Global()->Set(context->m_internal->m_context, CreateV8String(current, name.c_str()), _builder_CreateNativeTmpDoc(current, builder, sFile, sParams));
}
void builder_CreateNative      (const std::string& name, JSSmart<CJSContext> context, NSDoctRenderer::CDocBuilder* builder)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_context->Global()->Set(context->m_internal->m_context, CreateV8String(current, name.c_str()), _builder_CreateNative(current, builder));
}

JSSmart<CJSValue> CBuilderEmbed::builder_OpenTmpFile(JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
    std::wstring sPath = path->toStringW();
    std::wstring sParams = params->toStringW();
    v8::Local<v8::Value> obj = _builder_CreateNativeTmpDoc(CV8Worker::GetCurrent(), m_pBuilder, sPath, sParams);
    CJSValueV8* res = new CJSValueV8();
    res->value = obj;
    return res;
}
