/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#include "doctrenderer.h"
#include "docbuilder.h"

#ifdef BOOL
#undef BOOL
#endif

#include "nativecontrol.h"

#include "../xml/include/xmlutils.h"

#include <iostream>

namespace NSDoctRenderer
{
    class CExecuteParams
    {
    public:
        DoctRendererFormat::FormatFile m_eSrcFormat;
        DoctRendererFormat::FormatFile m_eDstFormat;

        std::wstring m_strFontsDirectory;
        std::wstring m_strImagesDirectory;
        std::wstring m_strThemesDirectory;

        std::wstring m_strSrcFilePath;
        std::wstring m_strDstFilePath;

        CArray<std::wstring> m_arChanges;
        int m_nCountChangesItems;

        std::wstring m_strMailMergeDatabasePath;
        std::wstring m_strMailMergeField;
        int m_nMailMergeIndexStart;
        int m_nMailMergeIndexEnd;

        bool m_bIsRetina;
        int m_nSaveToPDFParams;

    public:
        CExecuteParams() : m_arChanges()
        {
            m_eSrcFormat = DoctRendererFormat::INVALID;
            m_eDstFormat = DoctRendererFormat::INVALID;

            m_strFontsDirectory = L"";
            m_strImagesDirectory = L"";
            m_strThemesDirectory = L"";

            m_strSrcFilePath = L"";
            m_strDstFilePath = L"";

            m_nCountChangesItems = -1;

            m_strMailMergeDatabasePath = L"";
            m_strMailMergeField = L"";
            m_nMailMergeIndexStart = -1;
            m_nMailMergeIndexEnd = -1;

            m_bIsRetina = false;
            m_nSaveToPDFParams = 0;
        }
        ~CExecuteParams()
        {
            m_arChanges.RemoveAll();
        }

    public:
        bool FromXml(const std::wstring& strXml)
        {
            XmlUtils::CXmlNode oNode;
            if (!oNode.FromXmlString(strXml))
                return false;

            m_strSrcFilePath = oNode.ReadValueString(L"SrcFilePath");
            m_strDstFilePath = oNode.ReadValueString(L"DstFilePath");

            m_eSrcFormat = (DoctRendererFormat::FormatFile)(oNode.ReadValueInt(L"SrcFileType"));
            m_eDstFormat = (DoctRendererFormat::FormatFile)(oNode.ReadValueInt(L"DstFileType"));

            m_strFontsDirectory = oNode.ReadValueString(L"FontsDirectory");
            m_strImagesDirectory = oNode.ReadValueString(L"ImagesDirectory");
            m_strThemesDirectory = oNode.ReadValueString(L"ThemesDirectory");

            XmlUtils::CXmlNode oNodeChanges;
            if (oNode.GetNode(L"Changes", oNodeChanges))
            {
                m_nCountChangesItems = oNodeChanges.ReadAttributeInt(L"TopItem", -1);

                XmlUtils::CXmlNodes oNodes;
                oNodeChanges.GetNodes(L"Change", oNodes);

                int nCount = oNodes.GetCount();
                for (int i = 0; i < nCount; ++i)
                {
                    XmlUtils::CXmlNode _node;
                    oNodes.GetAt(i, _node);

                    m_arChanges.Add(_node.GetText());
                }
            }

            XmlUtils::CXmlNode oNodeMailMerge;
            if (oNode.GetNode(L"MailMergeData", oNodeMailMerge))
            {
                m_strMailMergeDatabasePath = oNodeMailMerge.ReadAttribute(L"DatabasePath");
                m_nMailMergeIndexStart = oNodeMailMerge.ReadAttributeInt(L"Start", -1);
                m_nMailMergeIndexEnd = oNodeMailMerge.ReadAttributeInt(L"End", -1);
                m_strMailMergeField = oNodeMailMerge.ReadAttribute(L"Field");
            }

            int nParams = oNode.ReadValueInt(L"DoctParams", 0);
            if (nParams & 0x01)
                m_bIsRetina = true;

            if (nParams & 0x02)
                m_nSaveToPDFParams = 1;

            return true;
        }
    };
}

void CreateNativeObject(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> NativeObjectTemplate = CreateNativeControlTemplate(isolate);
    CNativeControl* pNativeObject = new CNativeControl();

    v8::Local<v8::Object> obj = NativeObjectTemplate->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pNativeObject));

    args.GetReturnValue().Set(obj);
}

void CreateNativeObjectBuilder(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> NativeObjectTemplate = CreateNativeControlTemplateBuilder(isolate);
    CNativeControl* pNativeObject = new CNativeControl();

    v8::Local<v8::Object> obj = NativeObjectTemplate->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pNativeObject));

    args.GetReturnValue().Set(obj);
}

void CreateNativeMemoryStream(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> MemoryObjectTemplate = CreateMemoryStreamTemplate(isolate);
    CMemoryStream* pMemoryObject = new CMemoryStream();

    v8::Local<v8::Object> obj = MemoryObjectTemplate->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pMemoryObject));

    args.GetReturnValue().Set(obj);
}

namespace NSDoctRenderer
{
    std::wstring string_replaceAll(std::wstring str, const std::wstring& from, const std::wstring& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::wstring::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }    
}

namespace NSDoctRenderer
{
    static void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }
    static void replace_for_xml(std::wstring& text)
    {
        string_replace(text, L"&", L"&amp;");
        string_replace(text, L"'", L"&apos;");
        string_replace(text, L"<", L"&lt;");
        string_replace(text, L">", L"&gt;");
        string_replace(text, L"\"", L"&quot;");
    }

    class CDoctRenderer_Private
    {
    public:
        CExecuteParams m_oParams;

        std::wstring m_strConfigDir;
        std::wstring m_strConfigPath;
        CArray<std::wstring> m_arrFiles;

        std::vector<std::wstring> m_arDoctSDK;
        std::vector<std::wstring> m_arPpttSDK;
        std::vector<std::wstring> m_arXlstSDK;

        std::wstring m_strEditorType;
        std::wstring m_strFilePath;

        std::vector<std::wstring> m_arImagesInChanges;

        std::wstring m_sConsoleLogFile;
        std::wstring m_sErrorsLogFile;

    public:
        CDoctRenderer_Private(const std::wstring& sAllFontsPath = L"")
        {
            m_strConfigDir = NSFile::GetProcessDirectory() + L"/";
            m_strConfigPath = m_strConfigDir + L"DoctRenderer.config";

            XmlUtils::CXmlNode oNode;
            if (oNode.FromXmlFile(m_strConfigPath))
            {
                XmlUtils::CXmlNodes oNodes;
                if (oNode.GetNodes(L"file", oNodes))
                {
                    int nCount = oNodes.GetCount();
                    XmlUtils::CXmlNode _node;
                    for (int i = 0; i < nCount; ++i)
                    {
                        oNodes.GetAt(i, _node);
                        std::wstring strFilePath = _node.GetText();

                        if (!sAllFontsPath.empty())
                        {
                            std::wstring::size_type nPos = strFilePath.rfind(L"AllFonts.js");
                            if (nPos != std::wstring::npos && ((nPos + 11) == strFilePath.length())) // 11 = std::wstring(L"AllFonts.js").length();
                                strFilePath = sAllFontsPath;
                        }

                        if (NSFile::CFileBinary::Exists(strFilePath) &&
                            !NSFile::CFileBinary::Exists(m_strConfigDir + strFilePath))
                            m_arrFiles.Add(strFilePath);
                        else
                            m_arrFiles.Add(m_strConfigDir + strFilePath);
                    }
                }
            }

            XmlUtils::CXmlNode oNodeSdk = oNode.ReadNode(L"DoctSdk");
            if (oNodeSdk.IsValid())
                LoadSDK_scripts(oNodeSdk, m_arDoctSDK);

            oNodeSdk = oNode.ReadNode(L"PpttSdk");
            if (oNodeSdk.IsValid())
                LoadSDK_scripts(oNodeSdk, m_arPpttSDK);

            oNodeSdk = oNode.ReadNode(L"XlstSdk");
            if (oNodeSdk.IsValid())
                LoadSDK_scripts(oNodeSdk, m_arXlstSDK);

            m_sConsoleLogFile = L"";
            m_sErrorsLogFile = L"";

            XmlUtils::CXmlNode oNodeConsoleLogFile = oNode.ReadNode(L"LogFileConsoleLog");
            if (oNodeConsoleLogFile.IsValid())
            {
                m_sConsoleLogFile = oNodeConsoleLogFile.GetText();
                if (!NSFile::CFileBinary::Exists(m_sConsoleLogFile))
                    m_sConsoleLogFile = m_strConfigDir + m_sConsoleLogFile;
            }

            XmlUtils::CXmlNode oNodeErrorsLogFile = oNode.ReadNode(L"LogFileErrors");
            if (oNodeErrorsLogFile.IsValid())
            {
                m_sErrorsLogFile = oNodeErrorsLogFile.GetText();
                if (!NSFile::CFileBinary::Exists(m_sErrorsLogFile))
                    m_sErrorsLogFile = m_strConfigDir + m_sErrorsLogFile;
            }
        }
        ~CDoctRenderer_Private()
        {

        }

        void LoadSDK_scripts(XmlUtils::CXmlNode& oNode, std::vector<std::wstring>& _files)
        {
            XmlUtils::CXmlNodes oNodes;
            if (oNode.GetNodes(L"file", oNodes))
            {
                int nCount = oNodes.GetCount();
                XmlUtils::CXmlNode _node;
                for (int i = 0; i < nCount; ++i)
                {
                    oNodes.GetAt(i, _node);
                    std::wstring strFilePath = _node.GetText();

                    if (NSFile::CFileBinary::Exists(strFilePath) &&
                        !NSFile::CFileBinary::Exists(m_strConfigDir + strFilePath))
                        _files.push_back(strFilePath);
                    else
                        _files.push_back(m_strConfigDir + strFilePath);
                }
            }
            else
            {
                std::wstring strFilePath = oNode.GetText();

                if (NSFile::CFileBinary::Exists(strFilePath) &&
                    !NSFile::CFileBinary::Exists(m_strConfigDir + strFilePath))
                    _files.push_back(strFilePath);
                else
                    _files.push_back(m_strConfigDir + strFilePath);
            }
        }

    public:

        static void _LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError)
        {
#if 0
            if (m_sErrorsLogFile.empty())
                return;

            FILE* f = NSFile::CFileBinary::OpenFileNative(m_sErrorsLogFile, L"a+");

            std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
            std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

            fprintf(f, sT.c_str());
            fprintf(f, ": ");
            fprintf(f, sE.c_str());
            fprintf(f, "\n");
            fclose(f);
#endif
            std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
            std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

            std::cerr << sT << ": " << sE << std::endl;
        }

        static bool Doct_renderer_SaveFile(CExecuteParams* pParams,
                                       CNativeControl* pNative,
                                       v8::Isolate* isolate,
                                       v8::Local<v8::Object>& global_js,
                                       v8::Handle<v8::Value>* args,
                                       v8::TryCatch& try_catch,
                                       std::wstring& strError,
                                       bool bIsPdfBase64 = false)
        {
            bool bIsBreak = false;
            switch (pParams->m_eDstFormat)
            {
            case DoctRendererFormat::DOCT:
            case DoctRendererFormat::PPTT:
            case DoctRendererFormat::XLST:
            {
                v8::Handle<v8::Value> js_func_get_file_s = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeGetFileData"));
                if (js_func_get_file_s->IsFunction())
                {
                    v8::Handle<v8::Function> func_get_file_s = v8::Handle<v8::Function>::Cast(js_func_get_file_s);
                    v8::Local<v8::Value> js_result2 = func_get_file_s->Call(global_js, 1, args);

                    if (try_catch.HasCaught())
                    {
                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                        _LOGGING_ERROR_(L"save_code", strCode);
                        _LOGGING_ERROR_(L"save", strException);

                        strError = L"code=\"save\"";
                        bIsBreak = true;
                    }
                    else
                    {
                        v8::Local<v8::Uint8Array> pArray = v8::Local<v8::Uint8Array>::Cast(js_result2);
                        BYTE* pData = (BYTE*)pArray->Buffer()->Externalize().Data();

                        NSFile::CFileBinary oFile;
                        if (true == oFile.CreateFileW(pParams->m_strDstFilePath))
                        {
                            oFile.WriteFile((BYTE*)pNative->m_sHeader.c_str(), (DWORD)pNative->m_sHeader.length());

                            char* pDst64 = NULL;
                            int nDstLen = 0;
                            NSFile::CBase64Converter::Encode(pData, pNative->m_nSaveBinaryLen, pDst64, nDstLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

                            oFile.WriteFile((BYTE*)pDst64, (DWORD)nDstLen);

                            RELEASEARRAYOBJECTS(pDst64);
                            oFile.CloseFile();
                        }
                    }
                }
                break;
            }
            case DoctRendererFormat::HTML:
            {
                v8::Handle<v8::Value> js_func_get_file_s = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeGetFileDataHtml"));
                if (js_func_get_file_s->IsFunction())
                {
                    v8::Handle<v8::Function> func_get_file_s = v8::Handle<v8::Function>::Cast(js_func_get_file_s);
                    v8::Local<v8::Value> js_result2 = func_get_file_s->Call(global_js, 1, args);

                    if (try_catch.HasCaught())
                    {
                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                        _LOGGING_ERROR_(L"save_code", strCode);
                        _LOGGING_ERROR_(L"save", strException);

                        strError = L"code=\"save\"";
                        bIsBreak = true;
                    }
                    else
                    {
                        std::string sHTML_Utf8 = to_cstringA(js_result2);

                        NSFile::CFileBinary oFile;
                        if (true == oFile.CreateFileW(pParams->m_strDstFilePath))
                        {
                            oFile.WriteFile((BYTE*)sHTML_Utf8.c_str(), (DWORD)sHTML_Utf8.length());
                            oFile.CloseFile();
                        }
                    }
                }
                break;
            }
            case DoctRendererFormat::PDF:
            {
                v8::Handle<v8::Value> js_func_calculate = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeCalculateFile"));
                v8::Handle<v8::Value> js_func_pages_count = global_js->Get(v8::String::NewFromUtf8(isolate, "GetNativeCountPages"));
                v8::Handle<v8::Value> js_func_get_file_s = global_js->Get(v8::String::NewFromUtf8(isolate, "GetNativeFileDataPDF"));

                // CALCULATE
                if (js_func_calculate->IsFunction())
                {
                    v8::Handle<v8::Function> func_calculate = v8::Handle<v8::Function>::Cast(js_func_calculate);
                    func_calculate->Call(global_js, 1, args);

                    if (try_catch.HasCaught())
                    {
                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                        _LOGGING_ERROR_(L"calculate_code", strCode);
                        _LOGGING_ERROR_(L"calculate", strException);

                        strError = L"code=\"calculate\"";
                        bIsBreak = true;
                    }
                }


                LONG lPagesCount = 0;

                // PAGESCOUNT
                if (!bIsBreak)
                {
                    if (js_func_pages_count->IsFunction())
                    {
                        v8::Handle<v8::Function> func_pages_count = v8::Handle<v8::Function>::Cast(js_func_pages_count);
                        v8::Local<v8::Value> js_result1 = func_pages_count->Call(global_js, 1, args);

                        if (try_catch.HasCaught())
                        {
                            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                            std::wstring strException   = to_cstring(try_catch.Message()->Get());

                            _LOGGING_ERROR_(L"calculate_code", strCode);
                            _LOGGING_ERROR_(L"calculate", strException);

                            strError = L"code=\"calculate\"";
                            bIsBreak = true;
                        }
                        else
                        {
                            v8::Local<v8::Int32> intValue = js_result1->ToInt32();
                            lPagesCount = (LONG)intValue->Value();
                        }
                    }
                }

                // RENDER
                if (!bIsBreak)
                {
                    if (js_func_get_file_s->IsFunction())
                    {
                        v8::Handle<v8::Function> func_get_file_s = v8::Handle<v8::Function>::Cast(js_func_get_file_s);
                        args[0] = v8::Int32::New(isolate, pParams->m_nSaveToPDFParams);
                        v8::Local<v8::Value> js_result2 = func_get_file_s->Call(global_js, 1, args);

                        if (try_catch.HasCaught())
                        {
                            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                            std::wstring strException   = to_cstring(try_catch.Message()->Get());

                            _LOGGING_ERROR_(L"save_code", strCode);
                            _LOGGING_ERROR_(L"save", strException);

                            strError = L"code=\"save\"";
                            bIsBreak = true;
                        }
                        else
                        {
                            v8::Local<v8::Uint8Array> pArray = v8::Local<v8::Uint8Array>::Cast(js_result2);
                            BYTE* pData = (BYTE*)pArray->Buffer()->Externalize().Data();

                            NSFile::CFileBinary oFile;
                            if (true == oFile.CreateFileW(pParams->m_strDstFilePath))
                            {
                                if (!bIsPdfBase64)
                                {
                                    oFile.WriteFile(pData, (DWORD)pNative->m_nSaveBinaryLen);
                                }
                                else
                                {
                                    char* pDataDst = NULL;
                                    int nDataDst = 0;
                                    if (NSFile::CBase64Converter::Encode(pData, pNative->m_nSaveBinaryLen, pDataDst, nDataDst))
                                    {
                                        oFile.WriteFile((BYTE*)pDataDst, (DWORD)nDataDst);
                                        RELEASEARRAYOBJECTS(pDataDst);
                                    }

                                }
                                oFile.CloseFile();
                            }
                        }
                    }
                }
                break;
            }
            default:
                break;
            }
            return bIsBreak;
        }


        bool ExecuteScript(const std::string& strScript, std::wstring& strError, std::wstring& strReturnParams)
        {
            bool bIsBreak = false;
            v8::Isolate* isolate = CV8Worker::getInitializer()->CreateNew();
            if (true)
            {
                v8::Isolate::Scope isolate_cope(isolate);
                v8::Locker isolate_locker(isolate);

                v8::HandleScope handle_scope(isolate);

                v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
                global->Set(v8::String::NewFromUtf8(isolate, "CreateNativeEngine"), v8::FunctionTemplate::New(isolate, CreateNativeObject));
                global->Set(v8::String::NewFromUtf8(isolate, "CreateNativeMemoryStream"), v8::FunctionTemplate::New(isolate, CreateNativeMemoryStream));

                v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);

                v8::Context::Scope context_scope(context);
                v8::TryCatch try_catch;
                v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, strScript.c_str());
                v8::Local<v8::Script> script = v8::Script::Compile(source);

                // COMPILE
                if (try_catch.HasCaught())
                {
                    std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                    std::wstring strException   = to_cstring(try_catch.Message()->Get());

                    _LOGGING_ERROR_(L"compile_code", strCode);
                    _LOGGING_ERROR_(L"compile", strException);

                    strError = L"code=\"compile\"";
                    bIsBreak = true;
                }

                // RUN
                if (!bIsBreak)
                {
                    v8::Local<v8::Value> result = script->Run();

                    if (try_catch.HasCaught())
                    {
                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                        _LOGGING_ERROR_(L"run_code", strCode);
                        _LOGGING_ERROR_(L"run", strException);

                        strError = L"code=\"run\"";
                        bIsBreak = true;
                    }
                }

                if (!bIsBreak && m_oParams.m_bIsRetina)
                {
                    v8::Local<v8::String> sourceParams = v8::String::NewFromUtf8(isolate,
                        "(function(){ if (window && window.SetDoctRendererParams) {window.SetDoctRendererParams({retina:true});} })();");
                    v8::Local<v8::Script> scriptParams = v8::Script::Compile(sourceParams);
                    scriptParams->Run();
                }

                //---------------------------------------------------------------
                v8::Local<v8::Object> global_js = context->Global();
                v8::Handle<v8::Value> args[1];
                args[0] = v8::Int32::New(isolate, 0);

                CNativeControl* pNative = NULL;

                // GET_NATIVE_ENGINE
                if (!bIsBreak)
                {
                    v8::Handle<v8::Value> js_func_get_native = global_js->Get(v8::String::NewFromUtf8(isolate, "GetNativeEngine"));
                    v8::Local<v8::Object> objNative;
                    if (js_func_get_native->IsFunction())
                    {
                        v8::Handle<v8::Function> func_get_native = v8::Handle<v8::Function>::Cast(js_func_get_native);
                        v8::Local<v8::Value> js_result2 = func_get_native->Call(global_js, 1, args);

                        if (try_catch.HasCaught())
                        {
                            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                            std::wstring strException   = to_cstring(try_catch.Message()->Get());

                            _LOGGING_ERROR_(L"run_code", strCode);
                            _LOGGING_ERROR_(L"run", strException);

                            strError = L"code=\"run\"";
                            bIsBreak = true;
                        }
                        else
                        {
                            objNative = js_result2->ToObject();
                            v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(objNative->GetInternalField(0));

                            pNative = static_cast<CNativeControl*>(field->Value());
                            pNative->m_sConsoleLogFile = m_sConsoleLogFile;
                        }
                    }
                }

                if (pNative != NULL)
                {
                    pNative->m_pChanges = &m_oParams.m_arChanges;
                    pNative->m_strFontsDirectory = m_oParams.m_strFontsDirectory;
                    pNative->m_strImagesDirectory = m_oParams.m_strImagesDirectory;

                    pNative->CheckFonts();

                    pNative->m_strEditorType = m_strEditorType;
                    pNative->SetFilePath(m_strFilePath);

                    pNative->m_nMaxChangesNumber = m_oParams.m_nCountChangesItems;
                }

                // OPEN
                if (!bIsBreak)
                {
                    v8::Handle<v8::Value> js_func_open = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeOpenFileData"));
                    if (js_func_open->IsFunction())
                    {
                        v8::Handle<v8::Function> func_open = v8::Handle<v8::Function>::Cast(js_func_open);

                        CChangesWorker oWorkerLoader;
                        int nVersion = oWorkerLoader.OpenNative(pNative->GetFilePath());

                        v8::Handle<v8::Value> args_open[2];
                        args_open[0] = oWorkerLoader.GetDataFull();
                        args_open[1] = v8::Integer::New(isolate, nVersion);

                        func_open->Call(global_js, 2, args_open);

                        if (try_catch.HasCaught())
                        {
                            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                            std::wstring strException   = to_cstring(try_catch.Message()->Get());

                            _LOGGING_ERROR_(L"open_code", strCode);
                            _LOGGING_ERROR_(L"open", strException);

                            strError = L"code=\"open\"";
                            bIsBreak = true;
                        }
                    }
                }

                // CHANGES
                if (!bIsBreak)
                {
                    v8::Handle<v8::Value> js_func_apply_changes = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeApplyChangesData"));
                    if (m_oParams.m_arChanges.GetCount() != 0)
                    {
                        int nCurrentIndex = 0;
                        CChangesWorker oWorker;

                        int nFileType = 0;
                        if (m_strEditorType == L"spreadsheet")
                            nFileType = 1;

                        oWorker.SetFormatChanges(nFileType);
                        oWorker.CheckFiles(m_oParams.m_arChanges);

                        while (!bIsBreak)
                        {
                            nCurrentIndex = oWorker.Open(m_oParams.m_arChanges, nCurrentIndex);
                            bool bIsFull = (nCurrentIndex == m_oParams.m_arChanges.GetCount()) ? true : false;

                            if (js_func_apply_changes->IsFunction())
                            {
                                v8::Handle<v8::Function> func_apply_changes = v8::Handle<v8::Function>::Cast(js_func_apply_changes);
                                v8::Handle<v8::Value> args_changes[2];
                                args_changes[0] = oWorker.GetData();
                                args_changes[1] = v8::Boolean::New(isolate, bIsFull);

                                func_apply_changes->Call(global_js, 2, args_changes);

                                if (try_catch.HasCaught())
                                {
                                    std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                                    std::wstring strException   = to_cstring(try_catch.Message()->Get());

                                    _LOGGING_ERROR_(L"change_code", strCode);
                                    _LOGGING_ERROR_(L"change", strException);

                                    char buffer[50];
                                    sprintf(buffer, "index=\"%d\"", pNative->m_nCurrentChangesNumber);
                                    std::string s(buffer);
                                    strError = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)s.c_str(), (LONG)s.length());
                                    bIsBreak = true;
                                }
                            }

                            if (bIsFull)
                                break;
                        }

                        // images in changes
                        if (NULL != pNative)
                        {
                            for (std::map<std::wstring, bool>::const_iterator iter = pNative->m_mapImagesInChanges.begin();
                                 iter != pNative->m_mapImagesInChanges.end(); iter++)
                            {
                                m_arImagesInChanges.push_back(iter->first);
                            }
                        }
                    }
                }

                bool bIsMailMerge = false;
                if (!m_oParams.m_strMailMergeDatabasePath.empty() &&
                        m_oParams.m_nMailMergeIndexEnd >= m_oParams.m_nMailMergeIndexStart &&
                        m_oParams.m_nMailMergeIndexEnd >= 0)
                {
                    bIsMailMerge = true;
                }

                // MAIL MERGE
                if (!bIsBreak)
                {
                    if (bIsMailMerge)
                    {
                        // OPEN DATABASE
                        NSFile::CFileBinary oFileDataBase;
                        if (oFileDataBase.OpenFile(m_oParams.m_strMailMergeDatabasePath))
                        {
                            DWORD dwSizeBase = (DWORD)oFileDataBase.GetFileSize();
                            DWORD dwSizeRead = 0;
                            BYTE* pBaseData = new BYTE[dwSizeBase + 1];
                            oFileDataBase.ReadFile(pBaseData, dwSizeBase, dwSizeRead);

                            if (dwSizeBase != dwSizeRead)
                            {
                                RELEASEARRAYOBJECTS(pBaseData);
                                bIsBreak = true;

                                strError = L"mailmerge=\"databaseopen\"";
                            }
                            else
                            {
                                int nStart = 0;
                                if (dwSizeBase > 3)
                                {
                                    if (pBaseData[0] == 0xEF && pBaseData[1] == 0xBB && pBaseData[2] == 0xBF)
                                    {
                                        nStart = 3;
                                    }
                                }

                                pBaseData[dwSizeBase] = 0;
                                v8::Handle<v8::Value> js_func_mm_start = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeStartMailMergeByList"));

                                if (js_func_mm_start->IsFunction())
                                {
                                    v8::Handle<v8::Function> func_mm_start = v8::Handle<v8::Function>::Cast(js_func_mm_start);
                                    v8::Handle<v8::Value> args_changes[1];

                                    args_changes[0] = v8::JSON::Parse(v8::String::NewFromUtf8(isolate, (char*)(pBaseData + nStart)));

                                    if (try_catch.HasCaught())
                                    {
                                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                                        _LOGGING_ERROR_(L"change_code", strCode);
                                        _LOGGING_ERROR_(L"change", strException);

                                        strError = L"mailmerge=\"databaseopenjs\"";
                                        bIsBreak = true;
                                    }

                                    //args_changes[0] = v8::String::NewFromUtf8(isolate, (char*)(pBaseData + nStart));

                                    func_mm_start->Call(global_js, 1, args_changes);

                                    if (try_catch.HasCaught())
                                    {
                                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                                        _LOGGING_ERROR_(L"change_code", strCode);
                                        _LOGGING_ERROR_(L"change", strException);

                                        strError = L"mailmerge=\"databaseopenjs\"";
                                        bIsBreak = true;
                                    }
                                }
                            }
                        }
                        else
                        {
                            strError = L"mailmerge=\"databaseopen\"";
                            bIsBreak = true;
                        }

                        if (!bIsBreak)
                        {
                            std::string sFieldUtf8 = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(m_oParams.m_strMailMergeField.c_str(),
                                                                                                       (LONG)m_oParams.m_strMailMergeField.length());

                            strReturnParams += L"<MailMergeFields>";
                            for (int nIndexMM  = m_oParams.m_nMailMergeIndexStart; nIndexMM <= m_oParams.m_nMailMergeIndexEnd && !bIsBreak; ++nIndexMM)
                            {
                                v8::Handle<v8::Value> js_func_mm_preview = global_js->Get(v8::String::NewFromUtf8(isolate, "NativePreviewMailMergeResult"));

                                if (js_func_mm_preview->IsFunction())
                                {
                                    v8::Handle<v8::Function> func_mm_preview = v8::Handle<v8::Function>::Cast(js_func_mm_preview);
                                    v8::Handle<v8::Value> args_changes[1];
                                    args_changes[0] = v8::Integer::New(v8::Isolate::GetCurrent(), nIndexMM);

                                    func_mm_preview->Call(global_js, 1, args_changes);

                                    if (try_catch.HasCaught())
                                    {
                                        std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                                        std::wstring strException   = to_cstring(try_catch.Message()->Get());

                                        _LOGGING_ERROR_(L"change_code", strCode);
                                        _LOGGING_ERROR_(L"change", strException);

                                        strError = L"mailmerge=\"preview" + std::to_wstring(nIndexMM) + L"\"";
                                        bIsBreak = true;
                                    }
                                }

                                std::wstring sSaveFile = L"";
                                if (!bIsBreak)
                                {
                                    // SAVE
                                    std::wstring sSaveOld = m_oParams.m_strDstFilePath;
                                    m_oParams.m_strDstFilePath += (L"/file" + std::to_wstring(nIndexMM));
                                    sSaveFile = m_oParams.m_strDstFilePath;

                                    bIsBreak = Doct_renderer_SaveFile(&m_oParams, pNative, isolate, global_js,
                                                                           args, try_catch, strError);

                                    m_oParams.m_strDstFilePath = sSaveOld;
                                }

                                if (!bIsBreak)
                                {
                                    v8::Handle<v8::Value> js_func_mm_field = global_js->Get(v8::String::NewFromUtf8(isolate, "NativeGetMailMergeFiledValue"));

                                    if (js_func_mm_field->IsFunction())
                                    {
                                        v8::Handle<v8::Function> func_mm_field = v8::Handle<v8::Function>::Cast(js_func_mm_field);
                                        v8::Handle<v8::Value> args_changes[2];
                                        args_changes[0] = v8::Integer::New(v8::Isolate::GetCurrent(), nIndexMM);
                                        args_changes[1] = v8::String::NewFromUtf8(isolate, (char*)sFieldUtf8.c_str());

                                        v8::Local<v8::Value> js_result2 = func_mm_field->Call(global_js, 2, args_changes);

                                        if (try_catch.HasCaught())
                                        {
                                            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                                            std::wstring strException   = to_cstring(try_catch.Message()->Get());

                                            _LOGGING_ERROR_(L"change_code", strCode);
                                            _LOGGING_ERROR_(L"change", strException);

                                            strError = L"mailmerge=\"field" + std::to_wstring(nIndexMM) + L"\"";
                                            bIsBreak = true;
                                        }

                                        std::wstring sField = to_cstring(js_result2);
                                        NSDoctRenderer::replace_for_xml(sField);
                                        NSDoctRenderer::replace_for_xml(sSaveFile);

                                        strReturnParams += L"<file path=\"" + sSaveFile + L"\" field=\"" + sField + L"\" />";
                                    }
                                }
                            }
                            strReturnParams += L"</MailMergeFields>";
                        }
                    }
                }

                // SAVE
                if (!bIsBreak && !bIsMailMerge)
                {
                    bIsBreak = Doct_renderer_SaveFile(&m_oParams, pNative, isolate, global_js, args, try_catch, strError);
                }
            }

            isolate->Dispose();

            // теперь вызываем это в x2t
            //CV8Worker::Dispose();

            return bIsBreak ? false : true;
        }

        std::string ReadScriptFile(const std::wstring& strFile)
        {
            NSFile::CFileBinary oFile;

            if (!oFile.OpenFile(strFile))
                return "";

            int nSize = (int)oFile.GetFileSize();
            if (nSize < 3)
                return "";

            BYTE* pData = new BYTE[nSize];
            DWORD dwReadSize = 0;
            oFile.ReadFile(pData, (DWORD)nSize, dwReadSize);

            int nOffset = 0;
            if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
            {
                nOffset = 3;
            }

            std::string sReturn((char*)(pData + nOffset), nSize - nOffset);

            RELEASEARRAYOBJECTS(pData);
            return sReturn;
        }
    };

    CDoctrenderer::CDoctrenderer(const std::wstring& sAllFontsPath)
    {
        m_pInternal = new CDoctRenderer_Private(sAllFontsPath);
    }

    CDoctrenderer::~CDoctrenderer()
    {
        RELEASEOBJECT(m_pInternal);
    }    

    bool CDoctrenderer::Execute(const std::wstring& strXml, std::wstring& strError)
    {
        strError = L"";
        m_pInternal->m_oParams.FromXml(strXml);
        m_pInternal->m_arImagesInChanges.clear();

        std::vector<std::wstring>* arSdkFiles = NULL;
        switch (m_pInternal->m_oParams.m_eSrcFormat)
        {
        case DoctRendererFormat::DOCT:
            {
                switch (m_pInternal->m_oParams.m_eDstFormat)
                {
                case DoctRendererFormat::DOCT:
                case DoctRendererFormat::PDF:
                case DoctRendererFormat::HTML:
                    {
                        arSdkFiles = &m_pInternal->m_arDoctSDK;
                        m_pInternal->m_strEditorType = L"document";
                        break;
                    }
                default:
                    return false;
                }
                break;
            }
        case DoctRendererFormat::PPTT:
            {
                switch (m_pInternal->m_oParams.m_eDstFormat)
                {
                case DoctRendererFormat::PPTT:
                case DoctRendererFormat::PDF:
                    {
                        arSdkFiles = &m_pInternal->m_arPpttSDK;
                        m_pInternal->m_strEditorType = L"presentation";
                        break;
                    }
                default:
                    return false;
                }
                break;
            }
        case DoctRendererFormat::XLST:
            {
                switch (m_pInternal->m_oParams.m_eDstFormat)
                {
                case DoctRendererFormat::XLST:
                case DoctRendererFormat::PDF:
                    {
                        arSdkFiles = &m_pInternal->m_arXlstSDK;
                        m_pInternal->m_strEditorType = L"spreadsheet";
                        break;
                    }
                default:
                    return false;
                }
                break;
            }
        default:
            return false;
        }

        std::wstring strFileName = m_pInternal->m_oParams.m_strSrcFilePath;
        strFileName += L"/Editor.bin";

        strFileName = string_replaceAll(strFileName, L"\\\\", L"\\");
        strFileName = string_replaceAll(strFileName, L"//", L"/");
        strFileName = string_replaceAll(strFileName, L"\\", L"/");

        m_pInternal->m_strFilePath = strFileName;

        std::string strScript = "";
        for (size_t i = 0; i < m_pInternal->m_arrFiles.GetCount(); ++i)
        {
#if 0
            if (m_arrFiles[i].find(L"AllFonts.js") != std::wstring::npos)
                continue;
#endif

            strScript += m_pInternal->ReadScriptFile(m_pInternal->m_arrFiles[i]);
            strScript += "\n\n";
        }

        if (NULL != arSdkFiles)
        {
            for (std::vector<std::wstring>::iterator i = arSdkFiles->begin(); i != arSdkFiles->end(); i++)
            {
                strScript += m_pInternal->ReadScriptFile(*i);
                strScript += "\n\n";
            }
        }

        if (m_pInternal->m_strEditorType == L"spreadsheet")
            strScript += "\n$.ready();";

        std::wstring sReturnParams = L"";
        bool bResult = m_pInternal->ExecuteScript(strScript, strError, sReturnParams);

        if (strError.length() != 0)
        {
            strError = L"<result><error " + strError + L" />" + sReturnParams + L"</result>";
        }
        else if (sReturnParams.length() != 0)
        {
            strError = L"<result>" + sReturnParams + L"</result>";
        }

        return bResult ? true : false;
    }

    std::vector<std::wstring> CDoctrenderer::GetImagesInChanges()
    {
        return m_pInternal->m_arImagesInChanges;
    }
}

bool Doct_renderer_SaveFile_ForBuilder(int nFormat, const std::wstring& strDstFile,
                               CNativeControl* pNative,
                               v8::Isolate* isolate,
                               v8::Local<v8::Object>& global_js,
                               v8::Handle<v8::Value>* args,
                               v8::TryCatch& try_catch,
                               std::wstring& strError)
{
    NSDoctRenderer::CExecuteParams oParams;
    oParams.m_eDstFormat = (NSDoctRenderer::DoctRendererFormat::FormatFile)nFormat;
    oParams.m_strDstFilePath = strDstFile;

    return NSDoctRenderer::CDoctRenderer_Private::Doct_renderer_SaveFile(&oParams,
            pNative, isolate, global_js, args, try_catch, strError, true);
}
