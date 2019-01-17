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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "nativecontrol.h"

std::wstring to_cstring(v8::Local<v8::Value> v)
{
    v8::String::Utf8Value data(v);
    if (NULL == *data)
        return L"";

    return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)(*data), data.length());
}

std::string to_cstringA(v8::Local<v8::Value> v)
{
    v8::String::Utf8Value data(v);
    const char* p = (char*)*data;
    if (NULL == p)
        return "";
    return std::string(p);
}

// wrap_methods -------------
CNativeControl* unwrap_nativeobject(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CNativeControl*>(field->Value());
}

void _GetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->GetFilePath());
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}
void _SetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));

    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->SetFilePath(to_cstring(args[0]));
}

void _GetFontsDirectory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->m_strFontsDirectory);
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}

void _GetEditorType(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->m_strEditorType);
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}

void _GetChangesCount(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());

    int nCount = 0;
    if (pNative->m_pChanges != NULL)
        nCount = (int)pNative->m_pChanges->GetCount();
    args.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), nCount));
}
void _GetChangesFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());

    if (args.Length() < 1)
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));

    v8::Local<v8::Int32> intValue = args[0]->ToInt32();
    int nIndex = (int)intValue->Value();

    std::string strFile = "";
    if (pNative->m_pChanges != NULL)
        strFile = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->m_pChanges->operator []((int)nIndex));

    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), strFile.c_str()));
}

void _GetFileId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pNative->GetFileId());
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sReturn.c_str()));
}
void _SetFileId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));

    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->SetFileId(to_cstring(args[0]));
}

void _CheckNextChange(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());

    pNative->m_nCurrentChangesNumber++;
    if (-1 != pNative->m_nMaxChangesNumber)
    {
        if (pNative->m_nCurrentChangesNumber >= pNative->m_nMaxChangesNumber)
        {
            args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), false));
            return;
        }
    }
    args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), true));
}

void _GetFileArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    pNative->getFileData(to_cstring(args[0]), pData, len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pData, (size_t)len);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

    args.GetReturnValue().Set(_array);
}

void _GetFontArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    std::wstring strDir = pNative->m_strFontsDirectory;

#if 0
    if (strDir.length() != 0)
    {
        strDir += L"/";
        strDir += to_cstring(args[0]);
    }
    else
#endif

    // TODO:
    // по идее файлы могут совпадать по имени, но лежать в разных директориях.
    // и поэтому в AllFonts.js надо бы писать пути полные.
    // пока оставим по-старому
    std::wstring sFind = to_cstring(args[0]);
    bool bIsFullFilePath = (std::wstring::npos != sFind.find('\\') || std::wstring::npos != sFind.find('/'));
    if (bIsFullFilePath)
    {
        bIsFullFilePath = NSFile::CFileBinary::Exists(sFind);
    }

    if (!bIsFullFilePath)
    {
        std::map<std::wstring, std::wstring>::iterator pair = pNative->m_map_fonts.find(sFind);
        if (pair != pNative->m_map_fonts.end())
            strDir = pair->second;
        else
            strDir = pNative->m_sDefaultFont;
    }
    else
    {
        strDir = sFind;
    }

    pNative->getFileData(strDir, pData, len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pData, (size_t)len);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

    args.GetReturnValue().Set(_array);
}

void _GetFileString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    pNative->getFileData(to_cstring(args[0]), pData, len);

    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (char*)pData, v8::String::kNormalString, len));
}

void _Save_AllocNative(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    v8::Local<v8::Int32> intValue = args[0]->ToInt32();
    int nLen = (int)intValue->Value();

    pNative->Save_Alloc(nLen);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pNative->m_pSaveBinary, (size_t)pNative->m_nSaveLen);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)pNative->m_nSaveLen);
    args.GetReturnValue().Set(_array);
}

void _Save_ReAllocNative(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    int _pos = args[0]->Int32Value();
    int _len = args[1]->Int32Value();

    pNative->Save_ReAlloc(_pos, _len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pNative->m_pSaveBinary, (size_t)pNative->m_nSaveLen);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)pNative->m_nSaveLen);
    args.GetReturnValue().Set(_array);
}

void _Save_End(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    std::string sHeader = to_cstringA(args[0]);
    int _len = args[1]->Int32Value();

    pNative->Save_End(sHeader, _len);
}

void _ConsoleLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->ConsoleLog(to_cstringA(args[0]));
}

void _SaveChanges(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    pNative->DumpChanges(to_cstringA(args[0]), args[1]->Int32Value(), args[2]->Int32Value());
}

/// ZIP -----
void _zipOpenFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    bool bIsOpen = pNative->m_oZipWorker.Open(to_cstring(args[0]));
    if (!bIsOpen)
    {
        args.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
        return;
    }

    v8::Local<v8::Object> obj = v8::Object::New(v8::Isolate::GetCurrent());
    for (std::vector<std::wstring>::iterator i = pNative->m_oZipWorker.m_arFiles.begin(); i != pNative->m_oZipWorker.m_arFiles.end(); i++)
    {
        std::string sFile = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(*i);

        v8::Local<v8::String> _k = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sFile.c_str(), v8::String::kNormalString, -1);
        v8::Local<v8::String> _v = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sFile.c_str(), v8::String::kNormalString, -1);

        obj->Set(_k, _v);
    }

    args.GetReturnValue().Set(obj);
}

void _zipOpenFileBase64(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());
    bool bIsOpen = pNative->m_oZipWorker.OpenBase64(to_cstringA(args[0]));
    if (!bIsOpen)
    {
        args.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
        return;
    }

    v8::Local<v8::Object> obj = v8::Object::New(v8::Isolate::GetCurrent());
    for (std::vector<std::wstring>::iterator i = pNative->m_oZipWorker.m_arFiles.begin(); i != pNative->m_oZipWorker.m_arFiles.end(); i++)
    {
        std::string sFile = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(*i);

        v8::Local<v8::String> _k = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sFile.c_str(), v8::String::kNormalString, -1);
        v8::Local<v8::String> _v = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sFile.c_str(), v8::String::kNormalString, -1);

        obj->Set(_k, _v);
    }

    args.GetReturnValue().Set(obj);
}

void _zipGetFileAsString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    pNative->m_oZipWorker.GetFileData(to_cstring(args[0]), pData, len);

    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (char*)pData, v8::String::kNormalString, len));
}

void _zipGetFileAsBinary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if (args.Length() < 1)
    {
        args.GetReturnValue().Set(v8::Null(v8::Isolate::GetCurrent()));
        return;
    }

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    BYTE* pData = NULL;
    DWORD len = 0;
    pNative->m_oZipWorker.GetFileData(to_cstring(args[0]), pData, len);

    v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pData, (size_t)len);
    v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

    args.GetReturnValue().Set(_array);
}

void _zipCloseFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CNativeControl* pNative = unwrap_nativeobject(args.This());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    pNative->m_oZipWorker.Close();
}
/// ---------

void _AddImageInChanges(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;

    CNativeControl* pNative = unwrap_nativeobject(args.This());

    std::wstring sImage = to_cstring(args[0]);
    if (sImage.empty())
        return;

    std::map<std::wstring, bool>::const_iterator iter = pNative->m_mapImagesInChanges.find(sImage);
    if (iter == pNative->m_mapImagesInChanges.end())
        pNative->m_mapImagesInChanges.insert(std::pair<std::wstring, bool>(sImage, true));
}

v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplate(v8::Isolate* isolate)
{
    //v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
    result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // прописываем функции - методы объекта
    result->Set(v8::String::NewFromUtf8(current, "SetFilePath"), v8::FunctionTemplate::New(current, _SetFilePath));
    result->Set(v8::String::NewFromUtf8(current, "GetFilePath"), v8::FunctionTemplate::New(current, _GetFilePath));
    result->Set(v8::String::NewFromUtf8(current, "SetFileId"), v8::FunctionTemplate::New(current, _SetFileId));
    result->Set(v8::String::NewFromUtf8(current, "GetFileId"), v8::FunctionTemplate::New(current, _GetFileId));
    result->Set(v8::String::NewFromUtf8(current, "GetFileBinary"), v8::FunctionTemplate::New(current, _GetFileArrayBuffer));
    result->Set(v8::String::NewFromUtf8(current, "GetFontBinary"), v8::FunctionTemplate::New(current, _GetFontArrayBuffer));
    result->Set(v8::String::NewFromUtf8(current, "GetFontsDirectory"), v8::FunctionTemplate::New(current, _GetFontsDirectory));
    result->Set(v8::String::NewFromUtf8(current, "GetFileString"), v8::FunctionTemplate::New(current, _GetFileString));

    result->Set(v8::String::NewFromUtf8(current, "GetEditorType"), v8::FunctionTemplate::New(current, _GetEditorType));
    result->Set(v8::String::NewFromUtf8(current, "CheckNextChange"), v8::FunctionTemplate::New(current, _CheckNextChange));

    result->Set(v8::String::NewFromUtf8(current, "GetCountChanges"), v8::FunctionTemplate::New(current, _GetChangesCount));
    result->Set(v8::String::NewFromUtf8(current, "GetChangesFile"), v8::FunctionTemplate::New(current, _GetChangesFile));

    result->Set(v8::String::NewFromUtf8(current, "Save_AllocNative"), v8::FunctionTemplate::New(current, _Save_AllocNative));
    result->Set(v8::String::NewFromUtf8(current, "Save_ReAllocNative"), v8::FunctionTemplate::New(current, _Save_ReAllocNative));
    result->Set(v8::String::NewFromUtf8(current, "Save_End"), v8::FunctionTemplate::New(current, _Save_End));

    result->Set(v8::String::NewFromUtf8(current, "AddImageInChanges"), v8::FunctionTemplate::New(current, _AddImageInChanges));

    result->Set(v8::String::NewFromUtf8(current, "ConsoleLog"), v8::FunctionTemplate::New(current, _ConsoleLog));

    result->Set(v8::String::NewFromUtf8(current, "ZipOpen"),            v8::FunctionTemplate::New(current, _zipOpenFile));
    result->Set(v8::String::NewFromUtf8(current, "ZipOpenBase64"),      v8::FunctionTemplate::New(current, _zipOpenFileBase64));
    result->Set(v8::String::NewFromUtf8(current, "ZipFileAsString"),    v8::FunctionTemplate::New(current, _zipGetFileAsString));
    result->Set(v8::String::NewFromUtf8(current, "ZipFileAsBinary"),    v8::FunctionTemplate::New(current, _zipGetFileAsBinary));
    result->Set(v8::String::NewFromUtf8(current, "ZipClose"),           v8::FunctionTemplate::New(current, _zipCloseFile));

    // возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему
    // уничтожиться при уничтожении "нашего" HandleScope - handle_scope

    //return handle_scope.Close(result);
    return result;
}

v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplateBuilder(v8::Isolate* isolate)
{
    //v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
    result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // прописываем функции - методы объекта
    result->Set(v8::String::NewFromUtf8(current, "SetFilePath"), v8::FunctionTemplate::New(current, _SetFilePath));
    result->Set(v8::String::NewFromUtf8(current, "GetFilePath"), v8::FunctionTemplate::New(current, _GetFilePath));
    result->Set(v8::String::NewFromUtf8(current, "SetFileId"), v8::FunctionTemplate::New(current, _SetFileId));
    result->Set(v8::String::NewFromUtf8(current, "GetFileId"), v8::FunctionTemplate::New(current, _GetFileId));
    result->Set(v8::String::NewFromUtf8(current, "GetFileBinary"), v8::FunctionTemplate::New(current, _GetFileArrayBuffer));
    result->Set(v8::String::NewFromUtf8(current, "GetFontBinary"), v8::FunctionTemplate::New(current, _GetFontArrayBuffer));
    result->Set(v8::String::NewFromUtf8(current, "GetFontsDirectory"), v8::FunctionTemplate::New(current, _GetFontsDirectory));
    result->Set(v8::String::NewFromUtf8(current, "GetFileString"), v8::FunctionTemplate::New(current, _GetFileString));

    result->Set(v8::String::NewFromUtf8(current, "GetEditorType"), v8::FunctionTemplate::New(current, _GetEditorType));
    result->Set(v8::String::NewFromUtf8(current, "CheckNextChange"), v8::FunctionTemplate::New(current, _CheckNextChange));

    result->Set(v8::String::NewFromUtf8(current, "GetCountChanges"), v8::FunctionTemplate::New(current, _GetChangesCount));
    result->Set(v8::String::NewFromUtf8(current, "GetChangesFile"), v8::FunctionTemplate::New(current, _GetChangesFile));

    result->Set(v8::String::NewFromUtf8(current, "Save_AllocNative"), v8::FunctionTemplate::New(current, _Save_AllocNative));
    result->Set(v8::String::NewFromUtf8(current, "Save_ReAllocNative"), v8::FunctionTemplate::New(current, _Save_ReAllocNative));
    result->Set(v8::String::NewFromUtf8(current, "Save_End"), v8::FunctionTemplate::New(current, _Save_End));

    result->Set(v8::String::NewFromUtf8(current, "AddImageInChanges"), v8::FunctionTemplate::New(current, _AddImageInChanges));

    result->Set(v8::String::NewFromUtf8(current, "ConsoleLog"), v8::FunctionTemplate::New(current, _ConsoleLog));

    result->Set(v8::String::NewFromUtf8(current, "SaveChanges"), v8::FunctionTemplate::New(current, _SaveChanges));

    result->Set(v8::String::NewFromUtf8(current, "ZipOpen"),            v8::FunctionTemplate::New(current, _zipOpenFile));
    result->Set(v8::String::NewFromUtf8(current, "ZipOpenBase64"),      v8::FunctionTemplate::New(current, _zipOpenFileBase64));
    result->Set(v8::String::NewFromUtf8(current, "ZipFileAsString"),    v8::FunctionTemplate::New(current, _zipGetFileAsString));
    result->Set(v8::String::NewFromUtf8(current, "ZipFileAsBinary"),    v8::FunctionTemplate::New(current, _zipGetFileAsBinary));
    result->Set(v8::String::NewFromUtf8(current, "ZipClose"),           v8::FunctionTemplate::New(current, _zipCloseFile));

    // возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему
    // уничтожиться при уничтожении "нашего" HandleScope - handle_scope

    //return handle_scope.Close(result);
    return result;
}
// --------------------------

void CV8Worker::Initialize()
{
    if (NULL == m_pInitializer)
        m_pInitializer = new CV8Initializer();
}

void CV8Worker::Dispose()
{
    if (NULL != m_pInitializer)
        delete m_pInitializer;
    m_pInitializer = NULL;
}

CV8Initializer* CV8Worker::getInitializer()
{
    if (NULL == m_pInitializer)
    {
        m_pInitializer = new CV8Initializer();
    }

    return CV8Worker::m_pInitializer;
}

CV8Initializer* CV8Worker::m_pInitializer = NULL;
// --------------------------
