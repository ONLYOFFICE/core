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
#include "nativebuilder.h"
#include "docbuilder_p.h"

void CBuilderDocumentEmbed::OpenFile(const std::wstring& sFile, const std::wstring& sParams)
{
    NSDoctRenderer::CDocBuilder_Private* pBuilder = GetPrivate(m_pBuilder);

    std::wstring sTmpDir = pBuilder->m_sTmpFolder;

    m_sFolder = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTmpDir, L"DE_");
    if (NSFile::CFileBinary::Exists(m_sFolder))
        NSFile::CFileBinary::Remove(m_sFolder);

    NSCommon::string_replace(m_sFolder, L"\\", L"/");

    std::wstring::size_type nPosPoint = m_sFolder.rfind('.');
    if (nPosPoint != std::wstring::npos && nPosPoint > sTmpDir.length())
    {
        m_sFolder = m_sFolder.substr(0, nPosPoint);
    }

    NSDirectory::CreateDirectory(m_sFolder);

    std::wstring sExtCopy = pBuilder->GetFileCopyExt(sFile);
    std::wstring sFileCopy = m_sFolder + L"/origin." + sExtCopy;

    pBuilder->MoveFileOpen(sFile, sFileCopy);
    int nConvertResult = pBuilder->ConvertToInternalFormat(m_sFolder, sFileCopy, sParams);

    if (0 == nConvertResult)
        m_bIsValid = true;
}
void CBuilderDocumentEmbed::CloseFile()
{
    if (!m_sFolder.empty())
        NSDirectory::DeleteDirectory(m_sFolder);
    m_bIsValid = false;
    m_sFolder = L"";
}

CBuilderEmbed* unwrap_builder_embed(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CBuilderEmbed*>(field->Value());
}
CBuilderDocumentEmbed* unwrap_builder_doc_embed(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<CBuilderDocumentEmbed*>(field->Value());
}

void _builder_OpenFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderEmbed* builder = unwrap_builder_embed(args.This());
    std::wstring sPath = CV8Convert::ToString(args[0]);
    std::wstring sParams = (args.Length() > 1) ? CV8Convert::ToString(args[1]) : L"";
    int ret = builder->m_pBuilder->OpenFile(sPath.c_str(), sParams.c_str());
    args.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), ret));
}
void _builder_CreateFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderEmbed* builder = unwrap_builder_embed(args.This());
    bool ret = builder->m_pBuilder->CreateFile(CV8Convert::ToInt(args[0]));
    args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), ret));
}
void _builder_SetTmpFolder(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderEmbed* builder = unwrap_builder_embed(args.This());
    std::wstring sPath = CV8Convert::ToString(args[0]);
    builder->m_pBuilder->SetTmpFolder(sPath.c_str());
}
void _builder_SaveFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderEmbed* builder = unwrap_builder_embed(args.This());
    int type = CV8Convert::ToInt(args[0]);
    std::wstring sPath = CV8Convert::ToString(args[1]);
    std::wstring sParams = (args.Length() > 2) ? CV8Convert::ToString(args[2]) : L"";
    int ret = builder->m_pBuilder->SaveFile(type, sPath.c_str(), sParams.empty() ? NULL : sParams.c_str());
    args.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), ret));
}
void _builder_CloseFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderEmbed* builder = unwrap_builder_embed(args.This());
    builder->m_pBuilder->CloseFile();
}

void _builder_OpenTmpFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderEmbed* builder = unwrap_builder_embed(args.This());
    std::wstring sPath = CV8Convert::ToString(args[0]);
    std::wstring sParams = (args.Length() > 1) ? CV8Convert::ToString(args[1]) : L"";
    v8::Local<v8::Value> obj = _builder_CreateNativeTmpDoc(v8::Isolate::GetCurrent(), builder->m_pBuilder, sPath, sParams);
    args.GetReturnValue().Set(obj);
}

/////////////////////////////////////////////////////////////////////////////////
void _builder_doc_IsValid(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderDocumentEmbed* doc = unwrap_builder_doc_embed(args.This());
    args.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), doc->m_bIsValid));
}
void _builder_doc_GetBinary(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderDocumentEmbed* doc = unwrap_builder_doc_embed(args.This());

    BYTE* pData = NULL;
    DWORD dwSize = 0;
    NSFile::CFileBinary::ReadAllBytes(doc->m_sFolder + L"/Editor.bin", &pData, dwSize);

    if (0 == dwSize)
    {
        args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    }
    else
    {
        v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), (void*)pData, (size_t)dwSize);
        v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)dwSize);
        args.GetReturnValue().Set(_array);
    }
}
void _builder_doc_GetFolder(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderDocumentEmbed* doc = unwrap_builder_doc_embed(args.This());
    std::string sUtf8 = U_TO_UTF8((doc->m_sFolder));
    args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sUtf8.c_str()));
}
void _builder_doc_CloseFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderDocumentEmbed* doc = unwrap_builder_doc_embed(args.This());
    doc->CloseFile();
}
void _builder_doc_GetImageMap(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    CBuilderDocumentEmbed* doc = unwrap_builder_doc_embed(args.This());
    std::vector<std::wstring> files = NSDirectory::GetFiles(doc->m_sFolder + L"/media");

    v8::Local<v8::Object> obj = v8::Object::New(v8::Isolate::GetCurrent());
    for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
    {
        std::wstring sFile = *i; NSCommon::string_replace(sFile, L"\\", L"/");
        std::wstring sName = L"media/" + NSFile::GetFileName(sFile);

        std::string sFileA = U_TO_UTF8(sFile);
        std::string sNameA = U_TO_UTF8(sName);

        v8::Local<v8::String> _k = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sNameA.c_str(), v8::String::kNormalString, -1);
        v8::Local<v8::String> _v = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), sFileA.c_str(), v8::String::kNormalString, -1);

        obj->Set(_k, _v);
    }

    args.GetReturnValue().Set(obj);
}

/////////////////////////////////////////////////////////////////////////////////
v8::Local<v8::Value> _builder_CreateNativeTmpDoc(v8::Isolate* isolate, NSDoctRenderer::CDocBuilder* pBuilder, const std::wstring& sFile, const std::wstring& sParams)
{
    v8::Local<v8::ObjectTemplate> _template = v8::ObjectTemplate::New(isolate);
    _template->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

    _template->Set(isolate, "IsValid", v8::FunctionTemplate::New(isolate, _builder_doc_IsValid));
    _template->Set(isolate, "GetBinary", v8::FunctionTemplate::New(isolate, _builder_doc_GetBinary));
    _template->Set(isolate, "GetFolder", v8::FunctionTemplate::New(isolate, _builder_doc_GetFolder));
    _template->Set(isolate, "Close", v8::FunctionTemplate::New(isolate, _builder_doc_CloseFile));
    _template->Set(isolate, "GetImageMap", v8::FunctionTemplate::New(isolate, _builder_doc_GetImageMap));

    CBuilderDocumentEmbed* _embed = new CBuilderDocumentEmbed();
    _embed->m_pBuilder = pBuilder;
    _embed->OpenFile(sFile, sParams);

    v8::Local<v8::Object> obj = _template->NewInstance();
    obj->SetInternalField(0, v8::External::New(isolate, _embed));

    return obj;
}
v8::Local<v8::Value> _builder_CreateNative(v8::Isolate* isolate, NSDoctRenderer::CDocBuilder* builder)
{
    v8::Local<v8::ObjectTemplate> _template = v8::ObjectTemplate::New(isolate);
    _template->SetInternalFieldCount(1);

    _template->Set(isolate, "OpenFile", v8::FunctionTemplate::New(isolate, _builder_OpenFile));
    _template->Set(isolate, "CreateFile", v8::FunctionTemplate::New(isolate, _builder_CreateFile));
    _template->Set(isolate, "SetTmpFolder", v8::FunctionTemplate::New(isolate, _builder_SetTmpFolder));
    _template->Set(isolate, "SaveFile", v8::FunctionTemplate::New(isolate, _builder_SaveFile));
    _template->Set(isolate, "CloseFile", v8::FunctionTemplate::New(isolate, _builder_CloseFile));
    _template->Set(isolate, "OpenTmpFile", v8::FunctionTemplate::New(isolate, _builder_OpenTmpFile));

    CBuilderEmbed* _embed = new CBuilderEmbed();
    _embed->m_pBuilder = builder;

    v8::Local<v8::Object> obj = _template->NewInstance();
    obj->SetInternalField(0, v8::External::New(isolate, _embed));

    return obj;
}
