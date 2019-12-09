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
#include "docbuilder_p.h"

namespace NSDoctRenderer
{
    CDocBuilderValue_Private::CDocBuilderValue_Private()
    {
        m_isolate = NULL;
    }
    CDocBuilderValue_Private::~CDocBuilderValue_Private()
    {
    }
    void CDocBuilderValue_Private::Clear()
    {
        m_value.Clear();
    }
}

void CV8RealTimeWorker::_LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError)
{
    std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
    std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

    std::cerr << sT << ": " << sE << std::endl;
}

CV8RealTimeWorker::CV8RealTimeWorker(NSDoctRenderer::CDocBuilder* pBuilder)
{
    m_nFileType = -1;

    m_isolate = CV8Worker::getInitializer()->CreateNew();

    m_isolate_scope = new v8::Isolate::Scope(m_isolate);
    m_isolate_locker = new v8::Locker(m_isolate);
    m_handle_scope = new CScopeWrapper<v8::HandleScope>(m_isolate);

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(m_isolate);
    global->Set(m_isolate, "CreateNativeEngine", v8::FunctionTemplate::New(m_isolate, CreateNativeObjectBuilder));
    global->Set(m_isolate, "CreateNativeMemoryStream", v8::FunctionTemplate::New(m_isolate, CreateNativeMemoryStream));
    m_context = v8::Context::New(m_isolate, NULL, global);

    v8::Context::Scope context_scope(m_context);
    v8::TryCatch try_catch(m_isolate);

    m_context->Global()->Set(m_context, v8::String::NewFromUtf8(m_isolate, "builderJS"), _builder_CreateNative(m_isolate, pBuilder));
}
CV8RealTimeWorker::~CV8RealTimeWorker()
{
    RELEASEOBJECT(m_handle_scope);
    m_context.Clear();

    RELEASEOBJECT(m_isolate_locker);
    RELEASEOBJECT(m_isolate_scope);

    m_isolate->Dispose();
    m_isolate = NULL;
}

bool CV8RealTimeWorker::ExecuteCommand(const std::wstring& command, NSDoctRenderer::CDocBuilderValue* retValue)
{
    LOGGER_SPEED_START

    if (retValue)
        retValue->Clear();

    std::string commandA = U_TO_UTF8(command);
    //commandA = "Api." + commandA;

    v8::Context::Scope context_scope(m_context);

    v8::TryCatch try_catch(m_isolate);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(m_isolate, commandA.c_str());
    v8::Local<v8::Script> script = v8::Script::Compile(m_context, source).FromMaybe(v8::Local<v8::Script>());

    LOGGER_SPEED_LAP("compile_command")

    if (try_catch.HasCaught())
    {
        std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
        std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

        _LOGGING_ERROR_(L"execute_compile_code", strCode);
        _LOGGING_ERROR_(L"execute_compile", strException);

        return false;
    }
    else
    {
        v8::Local<v8::Value> retNativeVal = script->Run(m_context).FromMaybe(v8::Local<v8::Value>());

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
            std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

            _LOGGING_ERROR_(L"execute_run_code", strCode);
            _LOGGING_ERROR_(L"execute_run", strException);

            return false;
        }

        if (retValue)
        {
            NSDoctRenderer::CDocBuilderValue_Private* privateRet = (NSDoctRenderer::CDocBuilderValue_Private*)retValue->private_get_internal();
            privateRet->m_isolate = m_isolate;
            privateRet->m_value = retNativeVal;
        }
    }

    LOGGER_SPEED_LAP("run_command")

    return true;
}

std::string CV8RealTimeWorker::GetGlobalVariable()
{
    std::string commandA = "JSON.stringify(GlobalVariable);";

    v8::Context::Scope context_scope(m_context);

    v8::TryCatch try_catch(m_isolate);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(m_isolate, commandA.c_str());
    v8::Local<v8::Script> script = v8::Script::Compile(m_context, source).FromMaybe(v8::Local<v8::Script>());

    std::string sReturn = "{}";

    if (try_catch.HasCaught())
    {
        std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
        std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

        _LOGGING_ERROR_(L"execute_compile_code", strCode);
        _LOGGING_ERROR_(L"execute_compile", strException);

        return "";
    }
    else
    {
        v8::Local<v8::Value> _value = script->Run(m_context).FromMaybe(v8::Local<v8::Value>());

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
            std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

            _LOGGING_ERROR_(L"execute_run_code", strCode);
            _LOGGING_ERROR_(L"execute_run", strException);

            return "";
        }

        if (_value->IsString())
            sReturn = CV8Convert::ToStringA(_value);
    }

    return sReturn;
}

std::wstring CV8RealTimeWorker::GetJSVariable(std::wstring sParam)
{
    std::string sParamA = U_TO_UTF8(sParam);
    NSCommon::string_replaceA(sParamA, "\\\"", "\"");
    std::string commandA = "(function(){ return (" + sParamA + "); })()";

    v8::Context::Scope context_scope(m_context);

    v8::TryCatch try_catch(m_isolate);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(m_isolate, commandA.c_str());
    v8::Local<v8::Script> script = v8::Script::Compile(m_context, source).FromMaybe(v8::Local<v8::Script>());

    if (try_catch.HasCaught())
    {
        std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
        std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

        _LOGGING_ERROR_(L"execute_compile_code", strCode);
        _LOGGING_ERROR_(L"execute_compile", strException);

        return L"jsValue(" + sParam + L")";
    }
    else
    {
        v8::Local<v8::Value> _value = script->Run(m_context).FromMaybe(v8::Local<v8::Value>());

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
            std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

            _LOGGING_ERROR_(L"execute_run_code", strCode);
            _LOGGING_ERROR_(L"execute_run", strException);

            return L"jsValue(" + sParam + L")";
        }

        if (_value->IsString())
            return CV8Convert::ToString(_value);
    }

    return L"jsValue(" + sParam + L")";
}

bool CV8RealTimeWorker::OpenFile(const std::wstring& sBasePath, const std::wstring& path, const std::string& sString, const std::wstring& sCachePath)
{
    LOGGER_SPEED_START

    v8::Context::Scope context_scope(m_context);

    v8::TryCatch try_catch(m_isolate);

    v8::Local<v8::String> source = v8::String::NewFromUtf8(m_isolate, sString.c_str());
    v8::Local<v8::Script> script;

    CCacheDataScript oCachedScript(sCachePath);
    if (sCachePath.empty())
        script = v8::Script::Compile(m_context, source).FromMaybe(v8::Local<v8::Script>());
    else
    {
        script = oCachedScript.Compile(m_context, source);
    }

    LOGGER_SPEED_LAP("compile")

    if (try_catch.HasCaught())
    {
        std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
        std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

        _LOGGING_ERROR_(L"sdk_compile_code", strCode);
        _LOGGING_ERROR_(L"sdk_compile", strException);

        return false;
    }
    else
    {
        script->Run(m_context);

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
            std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

            _LOGGING_ERROR_(L"sdk_run_code", strCode);
            _LOGGING_ERROR_(L"sdk_run", strException);

            return false;
        }
    }
    LOGGER_SPEED_LAP("run")

    if (true)
    {
        std::string sArg = m_sUtf8ArgumentJSON;
        if (sArg.empty())
            sArg = "{}";
        NSCommon::string_replaceA(sArg, "\\", "\\\\");
        NSCommon::string_replaceA(sArg, "\"", "\\\"");
        std::string sArgument = "var Argument = JSON.parse(\"" + sArg + "\");";

        v8::Local<v8::String> _sourceArg = v8::String::NewFromUtf8(m_isolate, sArgument.c_str());
        v8::Local<v8::Script> _scriptArg = v8::Script::Compile(m_context, _sourceArg).FromMaybe(v8::Local<v8::Script>());
        _scriptArg->Run(m_context);

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
            std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

            _LOGGING_ERROR_(L"sdk_argument_code", strCode);
            _LOGGING_ERROR_(L"sdk_argument", strException);

            return false;
        }
    }

    if (true)
    {
        std::string sArg = m_sGlobalVariable;
        if (sArg.empty())
            sArg = "{}";
        NSCommon::string_replaceA(sArg, "\\", "\\\\");
        NSCommon::string_replaceA(sArg, "\"", "\\\"");

        std::string sScriptVar = "var GlobalVariable = JSON.parse(\"" + sArg + "\");";

        v8::Local<v8::String> _sourceArg = v8::String::NewFromUtf8(m_isolate, sScriptVar.c_str());
        v8::Local<v8::Script> _scriptArg = v8::Script::Compile(m_context, _sourceArg).FromMaybe(v8::Local<v8::Script>());
        _scriptArg->Run(m_context);

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
            std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

            _LOGGING_ERROR_(L"sdk_global_var_code", strCode);
            _LOGGING_ERROR_(L"sdk_global_var", strException);

            return false;
        }
    }

    CNativeControl* pNative = NULL;
    bool bIsBreak = false;

    v8::Local<v8::Object> global_js = m_context->Global();
    v8::Handle<v8::Value> args[1];
    args[0] = v8::Int32::New(m_isolate, 0);

    // GET_NATIVE_ENGINE
    if (!bIsBreak)
    {
        v8::Handle<v8::Value> js_func_get_native = global_js->Get(v8::String::NewFromUtf8(m_isolate, "GetNativeEngine"));
        v8::Local<v8::Object> objNative;
        if (js_func_get_native->IsFunction())
        {
            v8::Handle<v8::Function> func_get_native = v8::Handle<v8::Function>::Cast(js_func_get_native);
            v8::Local<v8::Value> js_result2 = func_get_native->Call(global_js, 1, args);

            if (try_catch.HasCaught())
            {
                std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
                std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

                _LOGGING_ERROR_(L"run_code", strCode);
                _LOGGING_ERROR_(L"run", strException);

                bIsBreak = true;
            }
            else
            {
                objNative = js_result2->ToObject();
                v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(objNative->GetInternalField(0));

                pNative = static_cast<CNativeControl*>(field->Value());
            }
        }
    }

    if (pNative != NULL)
    {
        pNative->m_strFontsDirectory = sBasePath + L"/sdkjs/common";
        pNative->m_strImagesDirectory = path + L"/media";

        pNative->CheckFonts();

        if (0 == m_nFileType)
            pNative->m_strEditorType = L"document";
        else if (1 == m_nFileType)
            pNative->m_strEditorType = L"presentation";
        else
            pNative->m_strEditorType = L"spreadsheet";

        pNative->SetFilePath(path + L"/Editor.bin");

        pNative->m_sChangesBuilderPath = path + L"/changes/changes0.json";

        pNative->m_nMaxChangesNumber = -1;
    }

    // OPEN
    if (!bIsBreak)
    {
        v8::Handle<v8::Value> js_func_open = global_js->Get(v8::String::NewFromUtf8(m_isolate, "NativeOpenFileData"));
        if (js_func_open->IsFunction())
        {
            v8::Handle<v8::Function> func_open = v8::Handle<v8::Function>::Cast(js_func_open);

            CChangesWorker oWorkerLoader;
            int nVersion = oWorkerLoader.OpenNative(pNative->GetFilePath());

            v8::Handle<v8::Value> args_open[3];
            args_open[0] = oWorkerLoader.GetDataFull();
            args_open[1] = v8::Integer::New(m_isolate, nVersion);

            std::wstring sXlsx = NSCommon::GetDirectoryName(pNative->GetFilePath()) + L"/Editor.xlsx";
            if (NSFile::CFileBinary::Exists(sXlsx))
            {
                std::string sXlsxA = U_TO_UTF8(sXlsx);
                args_open[2] = v8::String::NewFromUtf8(m_isolate, (char*)(sXlsxA.c_str()));
            }
            else
            {
                args_open[2] = v8::Undefined(m_isolate);
            }

            func_open->Call(global_js, 3, args_open);

            if (try_catch.HasCaught())
            {
                std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
                std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

                _LOGGING_ERROR_(L"open_code", strCode);
                _LOGGING_ERROR_(L"open", strException);

                bIsBreak = true;
            }
        }
    }

    if (!bIsBreak)
        bIsBreak = !this->ExecuteCommand(L"Api.asc_nativeInitBuilder();");
    if (!bIsBreak)
        bIsBreak = !this->ExecuteCommand(L"Api.asc_SetSilentMode(true);");

    LOGGER_SPEED_LAP("open")

    return !bIsBreak;
}

bool CV8RealTimeWorker::SaveFileWithChanges(int type, const std::wstring& _path)
{
    NSDoctRenderer::DoctRendererFormat::FormatFile _formatDst = NSDoctRenderer::DoctRendererFormat::DOCT;
    if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
        _formatDst = NSDoctRenderer::DoctRendererFormat::PPTT;
    else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
        _formatDst = NSDoctRenderer::DoctRendererFormat::XLST;
    else if ((type & AVS_OFFICESTUDIO_FILE_CROSSPLATFORM) || (type & AVS_OFFICESTUDIO_FILE_IMAGE))
        _formatDst = NSDoctRenderer::DoctRendererFormat::PDF;

    v8::Context::Scope context_scope(m_context);
    v8::TryCatch try_catch(m_isolate);

    CNativeControl* pNative = NULL;

    v8::Local<v8::Object> global_js = m_context->Global();
    v8::Handle<v8::Value> args[1];
    args[0] = v8::Int32::New(m_isolate, 0);

    // GET_NATIVE_ENGINE
    if (true)
    {
        v8::Handle<v8::Value> js_func_get_native = global_js->Get(v8::String::NewFromUtf8(m_isolate, "GetNativeEngine"));
        v8::Local<v8::Object> objNative;
        if (js_func_get_native->IsFunction())
        {
            v8::Handle<v8::Function> func_get_native = v8::Handle<v8::Function>::Cast(js_func_get_native);
            v8::Local<v8::Value> js_result2 = func_get_native->Call(global_js, 1, args);

            if (try_catch.HasCaught())
            {
                std::wstring strCode        = CV8Convert::GetSourceLine(try_catch.Message());
                std::wstring strException   = CV8Convert::GetMessage(try_catch.Message());

                _LOGGING_ERROR_(L"run_code", strCode);
                _LOGGING_ERROR_(L"run", strException);
            }
            else
            {
                objNative = js_result2->ToObject();
                v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(objNative->GetInternalField(0));

                pNative = static_cast<CNativeControl*>(field->Value());
            }
        }
    }

    if (pNative == NULL)
        return false;

    if (_formatDst == NSDoctRenderer::DoctRendererFormat::PDF)
        this->ExecuteCommand(L"Api.asc_SetSilentMode(false);");

    std::wstring strError;
    bool bIsError = Doct_renderer_SaveFile_ForBuilder(_formatDst,
                                                      _path,
                                                      pNative,
                                                      m_isolate,
                                                      m_context,
                                                      global_js,
                                                      args,
                                                      try_catch,
                                                      strError);

    if (_formatDst == NSDoctRenderer::DoctRendererFormat::PDF)
        this->ExecuteCommand(L"Api.asc_SetSilentMode(true);");

    return bIsError;
}

namespace NSDoctRenderer
{
    CDocBuilderValue::CDocBuilderValue()
    {
        m_internal = new CDocBuilderValue_Private();
    }
    CDocBuilderValue::CDocBuilderValue(const CDocBuilderValue& src)
    {
        m_internal->m_value = src.m_internal->m_value;
    }
    CDocBuilderValue& CDocBuilderValue::operator=(const CDocBuilderValue& src)
    {
        m_internal->m_value = src.m_internal->m_value;
        return *this;
    }
    CDocBuilderValue::~CDocBuilderValue()
    {
        delete m_internal;
    }
    void* CDocBuilderValue::private_get_internal()
    {
        return (void*)m_internal;
    }

    bool CDocBuilderValue::IsEmpty()
    {
        return m_internal->m_value.IsEmpty();
    }
    void CDocBuilderValue::Clear()
    {
        m_internal->Clear();
    }
    bool CDocBuilderValue::IsNull()
    {
        if (m_internal->m_value.IsEmpty())
            return false;
        return m_internal->m_value->IsNull();
    }
    bool CDocBuilderValue::IsUndefined()
    {
        if (m_internal->m_value.IsEmpty())
            return false;
        return m_internal->m_value->IsUndefined();
    }
    int CDocBuilderValue::ToInt()
    {
        if (m_internal->m_value.IsEmpty() || !m_internal->m_value->IsInt32())
            return 0;
        return CV8Convert::ToInt(m_internal->m_value);
    }
    double CDocBuilderValue::ToDouble()
    {
        if (m_internal->m_value.IsEmpty() || !m_internal->m_value->IsNumber())
            return 0;
        return CV8Convert::ToDouble(m_internal->m_value);
    }
    wchar_t* CDocBuilderValue::ToString()
    {
        if (m_internal->m_value.IsEmpty() || !m_internal->m_value->IsString())
            return NULL;
        std::wstring sValue = CV8Convert::ToString(m_internal->m_value);
        if (sValue.empty())
            return NULL;
        size_t len = sValue.length();
        wchar_t* buffer = new wchar_t[len + 1];
        memcpy(buffer, sValue.c_str(), len * sizeof(wchar_t));
        buffer[len] = '\0';
        return buffer;
    }
    void CDocBuilderValue::FreeString(wchar_t* data)
    {
        delete [] data;
    }
    CDocBuilderValue CDocBuilderValue::GetProperty(const wchar_t* name)
    {
        CDocBuilderValue ret;
        if (m_internal->m_value.IsEmpty() || !m_internal->m_value->IsObject())
            return ret;

        std::wstring sProp(name);
        std::string sPropA = U_TO_UTF8(sProp);

        ret.m_internal->m_isolate = m_internal->m_isolate;
        ret.m_internal->m_value = m_internal->m_value->ToObject()->Get(v8::String::NewFromUtf8(m_internal->m_isolate, sPropA.c_str()));

        return ret;
    }
}

namespace NSDoctRenderer
{
    void ParceParameters(const std::string& command, std::wstring* params, int& nCount)
    {
        const char* _commandsPtr = command.c_str();
        size_t _commandsLen = command.length();
        size_t _currentPos = 0;

        int nIndex = 0;

        while (true)
        {
            while (_currentPos < _commandsLen && !(_commandsPtr[_currentPos] == '\"' && _commandsPtr[_currentPos - 1] != '\\'))
                ++_currentPos;

            ++_currentPos;
            size_t _start = _currentPos;

            while (_currentPos < _commandsLen && !(_commandsPtr[_currentPos] == '\"' && _commandsPtr[_currentPos - 1] != '\\'))
                ++_currentPos;

            if (_currentPos > _start)
            {
                if (_currentPos == (_start + 1))
                    params[nIndex++] = L"";
                else
                    params[nIndex++] = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)(_commandsPtr + _start), (LONG)(_currentPos - _start));
            }

            ++_currentPos;

            if (_currentPos >= _commandsLen)
                break;
        }

        nCount = nIndex;
    }

    bool CDocBuilder::CreateFile(const int& type)
    {
        m_pInternal->m_nFileType = -1;
        if (!NSDirectory::Exists(m_pInternal->m_sTmpFolder))
            NSDirectory::CreateDirectory(m_pInternal->m_sTmpFolder);

        return m_pInternal->CreateFile(type);
    }
    void CDocBuilder::SetTmpFolder(const wchar_t* folder)
    {
        m_pInternal->m_sTmpFolder = std::wstring(folder);
    }
    void CDocBuilder::CloseFile()
    {
        m_pInternal->CloseFile();
    }

    char* CDocBuilder::GetVersion()
    {
        m_pInternal->Init();

        if (0 == m_pInternal->m_arDoctSDK.size())
            return NULL;

        std::wstring sFile;
        for (std::vector<std::wstring>::iterator i = m_pInternal->m_arDoctSDK.begin(); i != m_pInternal->m_arDoctSDK.end(); i++)
        {
            if (std::wstring::npos != i->find(L"sdk-all-min.js"))
            {
                sFile = *i;
                break;
            }
        }

        if (sFile.empty())
            return NULL;

        std::string sData;
        if (!NSFile::CFileBinary::ReadAllTextUtf8A(sFile, sData))
            return NULL;

        std::string::size_type startPos = sData.find("Version:");
        if (std::string::npos == startPos)
            return NULL;

        startPos += 8;

        std::string::size_type endPos = sData.find(')', startPos);
        if (std::string::npos == endPos)
            return NULL;

        size_t sSrcLen = endPos - startPos + 1;
        if (sSrcLen == 0)
            return NULL;

        char* sRet = new char[sSrcLen + 1];
        memcpy(sRet, sData.c_str() + startPos, sSrcLen);
        sRet[sSrcLen] = '\0';
        return sRet;
    }

    bool CDocBuilder::Run(const wchar_t* path)
    {
        std::wstring sPath(path);
        if (!NSFile::CFileBinary::Exists(sPath))
            sPath = NSFile::GetProcessDirectory() + L"/" + sPath;

        std::string sCommands;
        bool bRet = NSFile::CFileBinary::ReadAllTextUtf8A(sPath, sCommands);

        if (!bRet)
        {
            CV8RealTimeWorker::_LOGGING_ERROR_(L"error", L"cannot read run file");
            return bRet;
        }

        return this->RunTextA(sCommands.c_str());
    }

    bool CDocBuilder::RunTextW(const wchar_t* commands)
    {
        std::wstring sCommandsW(commands);
        std::string sCommands = U_TO_UTF8(sCommandsW);
        return this->RunTextA(sCommands.c_str());
    }

    bool CDocBuilder::RunTextA(const char* commands)
    {
        m_pInternal->Init();
        std::list<std::string> _commands;
        size_t _commandsLen = strlen(commands);
        size_t _currentPos = 0;

        while (true)
        {
            while (_currentPos < _commandsLen && (commands[_currentPos] == 0x0d || commands[_currentPos] == 0x0a))
                ++_currentPos;

            size_t _start = _currentPos;

            while (_currentPos < _commandsLen && (commands[_currentPos] != 0x0d && commands[_currentPos] != 0x0a))
                ++_currentPos;

            if (_currentPos > _start)
            {
                size_t _start2 = _start;
                while (_start2 < _currentPos && (commands[_start2] == '\t' || commands[_start2] == ' '))
                    ++_start2;

                if (_currentPos > _start2 && (commands[_start2] != '#' && commands[_start2] != '/'))
                {
                    _commands.push_back(std::string(commands + _start2, _currentPos - _start2));
                    // DEBUG
                    //std::cout << std::string(commands + _start2, _currentPos - _start2) << std::endl;
                }
            }

            if (_currentPos >= _commandsLen)
                break;
        }

        std::string sJsCommands = "";
        std::wstring _builder_params[4]; // с запасом
        for (std::list<std::string>::iterator i = _commands.begin(); i != _commands.end(); i++)
        {
            const std::string& command = *i;
            const char* _data = command.c_str();
            size_t _len = command.length();

            bool bIsBuilder = false;
            if (_len > 8)
            {
                if (_data[0] == 'b' &&
                    _data[1] == 'u' &&
                    _data[2] == 'i' &&
                    _data[3] == 'l' &&
                    _data[4] == 'd' &&
                    _data[5] == 'e' &&
                    _data[6] == 'r' &&
                    _data[7] == '.')
                    bIsBuilder = true;
            }

            bool bIsNoError = true;
            if (bIsBuilder)
            {
                if (!sJsCommands.empty())
                {
                    std::wstring sUnicodeCommand = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sJsCommands.c_str(), (LONG)sJsCommands.length());
                    bIsNoError = this->ExecuteCommand(sUnicodeCommand.c_str());
                    sJsCommands = "";
                    if (!bIsNoError)
                        return false;
                }

                size_t _pos = 8;
                while (_data[_pos] != '(')
                    ++_pos;

                std::string sFuncNum(_data + 8, _pos - 8);
                int nCountParameters = 0;
                ParceParameters(command, _builder_params, nCountParameters);

                for (int nCheckParam = 0; nCheckParam < nCountParameters; ++nCheckParam)
                {
                    if (0 == _builder_params[nCheckParam].find(L"jsValue(") && _builder_params[nCheckParam].length() > 9)
                    {
                        std::wstring sParam = _builder_params[nCheckParam].substr(8, _builder_params[nCheckParam].length() - 9);
                        _builder_params[nCheckParam] = m_pInternal->m_pWorker->GetJSVariable(sParam);
                    }
                }

                if ("OpenFile" == sFuncNum)
                    bIsNoError = (0 == this->OpenFile(_builder_params[0].c_str(), _builder_params[1].c_str()));
                else if ("CreateFile" == sFuncNum)
                {
                    if (L"docx" == _builder_params[0])
                        bIsNoError = this->CreateFile(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
                    else if (L"pptx" == _builder_params[0])
                        bIsNoError = this->CreateFile(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
                    else if (L"xlsx" == _builder_params[0])
                        bIsNoError = this->CreateFile(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
                }
                else if ("SetTmpFolder" == sFuncNum)
                    this->SetTmpFolder(_builder_params[0].c_str());
                else if ("CloseFile" == sFuncNum)
                    this->CloseFile();
                else if ("SaveFile" == sFuncNum)
                {
                    int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;

                    if (L"docx" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
                    else if (L"doc" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
                    else if (L"odt" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
                    else if (L"rtf" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
                    else if (L"txt" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
                    else if (L"pptx" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
                    else if (L"odp" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
                    else if (L"xlsx" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
                    else if (L"xls" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
                    else if (L"ods" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
                    else if (L"csv" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
                    else if (L"pdf" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
                    else if (L"image" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_IMAGE;
                    else if (L"jpg" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_IMAGE;
                    else if (L"png" == _builder_params[0])
                        nFormat = AVS_OFFICESTUDIO_FILE_IMAGE;

                    if (m_pInternal->m_oParams.m_bSaveWithDoctrendererMode)
                    {
                        // перед сохранением в такой схеме нужно скинуть изменения
                        this->ExecuteCommand(L"Api.asc_Save();");
                    }

                    const wchar_t* sParams = NULL;
                    if (nCountParameters > 2)
                        sParams = _builder_params[2].c_str();

                    this->SaveFile(nFormat, _builder_params[1].c_str(), sParams);
                }
                else if ("WriteData" == sFuncNum)
                {
                    bool isAppend = true;
                    if (nCountParameters > 2)
                        isAppend = (L"true" == _builder_params[2]) ? true : false;

                    this->WriteData(_builder_params[0].c_str(), _builder_params[1].c_str(), isAppend);
                }
            }
            else
            {
                //bIsNoError = this->m_pInternal->ExecuteCommand(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)_data, (LONG)_len));
                sJsCommands += command;
                sJsCommands += "\n";
            }

            if (!bIsNoError)
                return false;
        }

        return true;
    }

    void CDocBuilder::SetProperty(const char* param, const wchar_t* value)
    {
        std::string sParam = std::string(param);
        if (sParam == "--use-doctrenderer-scheme")
            m_pInternal->m_oParams.m_bSaveWithDoctrendererMode = true;
        else if (sParam == "--check-fonts")
            m_pInternal->m_oParams.m_bCheckFonts = true;
        else if (sParam == "--work-directory")
            m_pInternal->m_oParams.m_sWorkDir = std::wstring(value);
        else if (sParam == "--cache-scripts")
            m_pInternal->m_bIsCacheScript = (std::wstring(value) == L"true");
        else if (sParam == "--save-use-only-names")
            m_pInternal->m_sFolderForSaveOnlyUseNames = std::wstring(value);
        else if (sParam == "--all-fonts-path")
        {
            m_pInternal->m_strAllFonts = std::wstring(value);
            m_pInternal->m_bIsNotUseConfigAllFontsDir = true;
        }
        else if (sParam == "--argument")
        {
            std::wstring sArg(value);
            m_pInternal->m_oParams.m_sArgumentJSON = U_TO_UTF8(sArg);
        }
    }
    void CDocBuilder::SetPropertyW(const wchar_t* param, const wchar_t* value)
    {
        std::wstring sW(param);
        std::string sA = U_TO_UTF8(sW);
        return this->SetProperty(sA.c_str(), value);
    }

    void CDocBuilder::Initialize()
    {
        CV8Worker::Initialize();
    }

    void CDocBuilder::Dispose()
    {
        CV8Worker::Dispose();
    }

    void CDocBuilder::WriteData(const wchar_t* path, const wchar_t* value, const bool& append)
    {
        return m_pInternal->WriteData(path, value, append);
    }

    bool CDocBuilder::IsSaveWithDoctrendererMode()
    {
        return m_pInternal->m_oParams.m_bSaveWithDoctrendererMode;
    }
}
