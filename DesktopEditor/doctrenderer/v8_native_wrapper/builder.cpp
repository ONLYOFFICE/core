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
#include "builder.h"
#include "../../../Common/OfficeFileFormats.h"
#include "../../fontengine/application_generate_fonts_common.h"

// wrap_methods -------------
CBuilderObject* unwrap_builder(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CBuilderObject*>(field->Value());
}

void _b_openfile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderObject* builder = unwrap_builder(args.This());
    if (!builder->m_pBuilder)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }


    std::wstring sFile;
    std::wstring sParams;

    if (args.Length() > 0)
        sFile = to_cstring(args[0]);
    if (args.Length() > 1)
        sParams = to_cstring(args[1]);

    int nRet = builder->m_pBuilder->OpenFile(sFile.c_str(), sParams.c_str());
    args.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), nRet));
}
void _b_createfile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderObject* builder = unwrap_builder(args.This());
    if (!builder->m_pBuilder)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    std::string sType = "docx";
    if (args.Length() > 0)
        sType = to_cstringA(args[0]);

    bool isNoError = false;
    if ("docx" == sType)
        isNoError = builder->m_pBuilder->CreateFile(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
    else if ("pptx" == sType)
        isNoError = builder->m_pBuilder->CreateFile(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
    else if ("xlsx" == sType)
        isNoError = builder->m_pBuilder->CreateFile(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);

    args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), isNoError));
}
void _b_settmpfolder(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderObject* builder = unwrap_builder(args.This());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (!builder->m_pBuilder)
        return;

    if (args.Length() > 0)
    {
        std::wstring sFolder = to_cstring(args[0]);
        builder->m_pBuilder->SetTmpFolder(sFolder.c_str());
    }
}
void _b_savefile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderObject* builder = unwrap_builder(args.This());
    if (!builder->m_pBuilder || args.Length() < 2)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
        return;
    }

    int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
    std::string sFormat = to_cstringA(args[0]);

    if ("docx" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
    else if ("doc" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
    else if ("odt" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
    else if ("rtf" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
    else if ("txt" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
    else if ("pptx" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
    else if ("odp" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
    else if ("xlsx" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
    else if ("xls" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
    else if ("ods" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
    else if ("csv" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
    else if ("pdf" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
    else if ("image" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_IMAGE;
    else if ("jpg" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_IMAGE;
    else if ("png" == sFormat)
        nFormat = AVS_OFFICESTUDIO_FILE_IMAGE;

    if (builder->m_pBuilder->IsSaveWithDoctrendererMode())
    {
        // перед сохранением в такой схеме нужно скинуть изменения
        builder->m_pBuilder->ExecuteCommand(L"_api.asc_Save();");
    }

    std::wstring sFile = to_cstring(args[1]);
    std::wstring sFileParams;
    if (args.Length() > 2)
        sFileParams = to_cstring(args[2]);

    int nRet = builder->m_pBuilder->SaveFile(nFormat, sFile.c_str(), sFileParams.empty() ? NULL : sFileParams.c_str());
    args.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), nRet));
}
void _b_closefile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderObject* builder = unwrap_builder(args.This());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (!builder->m_pBuilder)
        return;
    builder->m_pBuilder->CloseFile();
}
void _b_writefile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderObject* builder = unwrap_builder(args.This());
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (!builder->m_pBuilder || args.Length() < 2)
        return;

    bool bIsAppend = true;
    if (args.Length() > 2)
        bIsAppend = args[2]->BooleanValue();

    std::wstring sFile = to_cstring(args[0]);
    std::string sData = to_cstringA(args[1]);
    NSCommon::string_replaceA(sData, "%", "%%");

    if (!bIsAppend)
        NSFile::CFileBinary::Remove(sFile);

    NSFile::CFileBinary oFile;
    FILE* pFile = oFile.OpenFileNative(sFile, bIsAppend ? L"a+" : L"a");
    if (pFile)
    {
        fprintf(pFile, sData.c_str());
        fclose(pFile);
    }
}

v8::Handle<v8::ObjectTemplate> CreateBuilderTemplate(v8::Isolate* isolate)
{
    //v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
    result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    v8::Isolate* current = v8::Isolate::GetCurrent();

    void _b_openfile(const v8::FunctionCallbackInfo<v8::Value>& args);
    void _b_createfile(const v8::FunctionCallbackInfo<v8::Value>& args);
    void _b_settmpfolder(const v8::FunctionCallbackInfo<v8::Value>& args);
    void _b_savefile(const v8::FunctionCallbackInfo<v8::Value>& args);
    void _b_closefile(const v8::FunctionCallbackInfo<v8::Value>& args);

    // прописываем функции - методы объекта
    result->Set(v8::String::NewFromUtf8(current, "OpenFile"),		v8::FunctionTemplate::New(current, _b_openfile));
    result->Set(v8::String::NewFromUtf8(current, "CreateFile"),     v8::FunctionTemplate::New(current, _b_createfile));
    result->Set(v8::String::NewFromUtf8(current, "SetTmpFolder"),	v8::FunctionTemplate::New(current, _b_settmpfolder));
    result->Set(v8::String::NewFromUtf8(current, "SaveFile"),		v8::FunctionTemplate::New(current, _b_savefile));
    result->Set(v8::String::NewFromUtf8(current, "CloseFile"),		v8::FunctionTemplate::New(current, _b_closefile));
    result->Set(v8::String::NewFromUtf8(current, "Write"),          v8::FunctionTemplate::New(current, _b_writefile));

    // возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему
    // уничтожиться при уничтожении "нашего" HandleScope - handle_scope

    //return handle_scope.Close(result);
    return result;
}

void CreateBuilderObject(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> NativeObjectTemplate = CreateBuilderTemplate(isolate);
    CBuilderObject* pNativeObject = new CBuilderObject();

    v8::Local<v8::Object> obj = NativeObjectTemplate->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pNativeObject));

    args.GetReturnValue().Set(obj);
}

