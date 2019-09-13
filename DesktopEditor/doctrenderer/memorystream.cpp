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
#include "memorystream.h"

// wrap_methods -------------
CMemoryStream* unwrap_memorystream(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CMemoryStream*>(field->Value());
}

void _ms_write_byte(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    BYTE arg = (BYTE)CV8Convert::ToInt(args[0]);
    pNative->WriteBYTE(arg);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_bool(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    BYTE arg = (BYTE)CV8Convert::ToBool(args[0]);
    pNative->WriteBYTE(arg ? 1 : 0);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_long(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    LONG arg = (LONG)CV8Convert::ToInt(args[0]);
    pNative->WriteLONG(arg);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_double(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    double arg = CV8Convert::ToDouble(args[0]);
    pNative->WriteLONG((LONG)(arg * 100000));

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_writestring1(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());
    v8::String::Value data(ISOLATE_IF_7 args[0]);
    pNative->WriteString((wchar_t*)*data, data.length());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_writestring2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());
    v8::String::Value data(ISOLATE_IF_7 args[0]);
    pNative->WriteString2((wchar_t*)*data, data.length());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_copy(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());

    CMemoryStream* pNative2 = unwrap_memorystream(args[0]->ToObject());
    size_t pos = (size_t)CV8Convert::ToUint(args[1]);
    size_t len = (size_t)CV8Convert::ToUint(args[2]);

    pNative->Copy(pNative2, pos, len);

    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_clearnoattack(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CMemoryStream* pNative = unwrap_memorystream(args.This());
    pNative->ClearNoAttack();
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_pos(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CMemoryStream* pNative = unwrap_memorystream(info.Holder());
    info.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), pNative->GetSize()));
}

v8::Handle<v8::ObjectTemplate> CreateMemoryStreamTemplate(v8::Isolate* isolate)
{
    //v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // property
    result->SetAccessor(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "pos"), _ms_pos); // получить код ошибки

    // прописываем функции - методы объекта
    result->Set(current, "Copy",			v8::FunctionTemplate::New(current, _ms_copy));
    result->Set(current, "ClearNoAttack",	v8::FunctionTemplate::New(current, _ms_clearnoattack));
    result->Set(current, "WriteByte",		v8::FunctionTemplate::New(current, _ms_write_byte));
    result->Set(current, "WriteBool",		v8::FunctionTemplate::New(current, _ms_write_bool));
    result->Set(current, "WriteLong",		v8::FunctionTemplate::New(current, _ms_write_long));
    result->Set(current, "WriteDouble",	v8::FunctionTemplate::New(current, _ms_write_double));
    result->Set(current, "WriteString",	v8::FunctionTemplate::New(current, _ms_writestring1));
    result->Set(current, "WriteString2",	v8::FunctionTemplate::New(current, _ms_writestring2));

    // возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему
    // уничтожиться при уничтожении "нашего" HandleScope - handle_scope

    //return handle_scope.Close(result);
    return result;
}

