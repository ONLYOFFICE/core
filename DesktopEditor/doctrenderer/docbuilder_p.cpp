﻿/*
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

void CV8RealTimeWorker::_LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError)
{
    std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
    std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

    std::cerr << sT << ": " << sE << std::endl;
}

using namespace NSJSBase;

CV8RealTimeWorker::CV8RealTimeWorker(NSDoctRenderer::CDocBuilder* pBuilder)
{
    m_nFileType = -1;

    m_context = new CJSContext();
    m_context->Initialize();

    m_isolate_scope = m_context->CreateIsolateScope();
    m_handle_scope  = m_context->CreateLocalScope();

    m_context->CreateGlobalForContext();
    CNativeControlEmbed::CreateObjectBuilderInContext("CreateNativeEngine", m_context);
    CMemoryStreamEmbed::CreateObjectInContext  ("CreateNativeMemoryStream", m_context);
    CGraphicsEmbed::CreateObjectInContext          ("CreateNativeGraphics", m_context);
    m_context->CreateContext();

    JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
    JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

    builder_CreateNative("builderJS", m_context, pBuilder);
}
CV8RealTimeWorker::~CV8RealTimeWorker()
{
    m_handle_scope = NULL;
    m_isolate_scope = NULL;
    m_context->Dispose();
}

bool CV8RealTimeWorker::ExecuteCommand(const std::wstring& command, NSDoctRenderer::CDocBuilderValue* retValue)
{
    LOGGER_SPEED_START

    if (retValue)
        retValue->Clear();

    std::string commandA = U_TO_UTF8(command);
    //commandA = "Api." + commandA;

    JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
    JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

    LOGGER_SPEED_LAP("compile_command")

    JSSmart<CJSValue> retNativeVal = m_context->runScript(commandA, try_catch);
    if(try_catch->Check())
        return false;

    if (retValue)
    {
        NSDoctRenderer::CDocBuilderValue_Private* privateRet = (NSDoctRenderer::CDocBuilderValue_Private*)retValue->private_get_internal();
        privateRet->m_context = m_context;
        privateRet->m_value = retNativeVal;
    }

    LOGGER_SPEED_LAP("run_command")

    return true;
}

std::string CV8RealTimeWorker::GetGlobalVariable()
{
    std::string commandA = "JSON.stringify(GlobalVariable);";

    JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
    JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

    JSSmart<CJSValue> _value = m_context->runScript(commandA, try_catch);
    if(try_catch->Check())
        return "";
    if(_value->isString())
        return _value->toStringA();
    return "{}";
}

std::wstring CV8RealTimeWorker::GetJSVariable(std::wstring sParam)
{
    std::string sParamA = U_TO_UTF8(sParam);
    NSStringUtils::string_replaceA(sParamA, "\\\"", "\"");
    std::string commandA = "(function(){ return (" + sParamA + "); })()";

    JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
    JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

    JSSmart<CJSValue> _value = m_context->runScript(commandA, try_catch);
    if(try_catch->Check())
        return L"jsValue(" + sParam + L")";
    if(_value->isString())
        return _value->toStringW();
    return L"jsValue(" + sParam + L")";
}

bool CV8RealTimeWorker::OpenFile(const std::wstring& sBasePath, const std::wstring& path, const std::string& sString, const std::wstring& sCachePath, CV8Params* pParams)
{
    LOGGER_SPEED_START

    JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
    JSSmart<CJSTryCatch>         try_catch = m_context->GetExceptions();

    LOGGER_SPEED_LAP("compile");

    m_context->runScript(sString, try_catch, sCachePath);
    if(try_catch->Check())
        return false;

    LOGGER_SPEED_LAP("run")

    if (true)
    {
        std::string sArg = m_sUtf8ArgumentJSON;
        if (sArg.empty())
            sArg = "{}";
        NSStringUtils::string_replaceA(sArg, "\\", "\\\\");
        NSStringUtils::string_replaceA(sArg, "\"", "\\\"");
        std::string sArgument = "var Argument = JSON.parse(\"" + sArg + "\");";

        m_context->runScript(sArgument, try_catch);
        if (try_catch->Check())
            return false;
    }

    if (true)
    {
        std::string sArg = m_sGlobalVariable;
        if (sArg.empty())
            sArg = "{}";
        NSStringUtils::string_replaceA(sArg, "\\", "\\\\");
        NSStringUtils::string_replaceA(sArg, "\"", "\\\"");

        std::string sScriptVar = "var GlobalVariable = JSON.parse(\"" + sArg + "\");";

        m_context->runScript(sScriptVar, try_catch);
        if (try_catch->Check())
            return false;
    }

    NSNativeControl::CNativeControl* pNative = NULL;
    bool bIsBreak = false;

    JSSmart<CJSObject> global_js = m_context->GetGlobal();
    JSSmart<CJSValue> args[1];
    args[0] = CJSContext::createInt(0);

    // GET_NATIVE_ENGINE
    if (!bIsBreak)
    {
        JSSmart<CJSValue> js_result2 = global_js->call_func("GetNativeEngine", 1, args);
        if (try_catch->Check())
            bIsBreak = true;
        else
        {
            JSSmart<CJSObject> objNative = js_result2->toObject();
            pNative = (NSNativeControl::CNativeControl*)objNative->getNative()->getObject();            
        }
    }

    if (pNative != NULL)
    {
        if (pParams)
        {
            pNative->m_oParams = *pParams;
        }

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
        CChangesWorker oWorkerLoader;
        int nVersion = oWorkerLoader.OpenNative(pNative->GetFilePath());

        JSSmart<CJSValue> args_open[3];
        args_open[0] = oWorkerLoader.GetDataFull()->toObject()->toValue();
        args_open[1] = CJSContext::createInt(nVersion);
        std::wstring sXlsx = NSFile::GetDirectoryName(pNative->GetFilePath()) + L"/Editor.xlsx";
        args_open[2] = NSFile::CFileBinary::Exists(sXlsx) ? CJSContext::createString(sXlsx) : CJSContext::createUndefined();

        global_js->call_func("NativeOpenFileData", 3, args_open);
        if (try_catch->Check())
            bIsBreak = true;
    }

    if (!bIsBreak)
        bIsBreak = !this->ExecuteCommand(L"Api.asc_nativeInitBuilder();");
    if (!bIsBreak)
        bIsBreak = !this->ExecuteCommand(L"Api.asc_SetSilentMode(true);");

    LOGGER_SPEED_LAP("open")

    return !bIsBreak;
}

bool CV8RealTimeWorker::SaveFileWithChanges(int type, const std::wstring& _path, const std::wstring& sJsonParams)
{
    NSDoctRenderer::DoctRendererFormat::FormatFile _formatDst = NSDoctRenderer::DoctRendererFormat::DOCT;
    if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
        _formatDst = NSDoctRenderer::DoctRendererFormat::PPTT;
    else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
        _formatDst = NSDoctRenderer::DoctRendererFormat::XLST;
    else if ((type & AVS_OFFICESTUDIO_FILE_CROSSPLATFORM) || (type & AVS_OFFICESTUDIO_FILE_IMAGE))
        _formatDst = NSDoctRenderer::DoctRendererFormat::PDF;

    JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
    JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

    NSNativeControl::CNativeControl* pNative = NULL;

    JSSmart<CJSObject> global_js = m_context->GetGlobal();
    JSSmart<CJSValue> args[1];
    args[0] = CJSContext::createInt(0);

    // GET_NATIVE_ENGINE
    if (true)
    {
        JSSmart<CJSValue> js_result2 = global_js->call_func("GetNativeEngine", 1, args);
        if (!try_catch->Check())
        {
            JSSmart<CJSObject> objNative = js_result2->toObject();
            pNative = (NSNativeControl::CNativeControl*)objNative->getNative()->getObject();
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
                                                      m_context,
                                                      args,
                                                      strError,
                                                      sJsonParams);

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
        m_internal = new CDocBuilderValue_Private();
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
        return m_internal->m_value->isEmpty();
    }
    void CDocBuilderValue::Clear()
    {
        m_internal->Clear();
    }
    bool CDocBuilderValue::IsNull()
    {
        return m_internal->m_value->isNull();
    }
    bool CDocBuilderValue::IsUndefined()
    {
        return m_internal->m_value->isUndefined();
    }
    int CDocBuilderValue::ToInt()
    {
        if (IsEmpty() || !m_internal->m_value->isNumber())
            return 0;
        return m_internal->m_value->toInt32();
    }
    double CDocBuilderValue::ToDouble()
    {
        if (IsEmpty() || !m_internal->m_value->isNumber())
            return 0;
        return m_internal->m_value->toDouble();
    }
    wchar_t* CDocBuilderValue::ToString()
    {
        if (IsEmpty() || !m_internal->m_value->isString())
            return NULL;
        std::wstring sValue = m_internal->m_value->toStringW();
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
        if (IsEmpty() || !m_internal->m_value->isObject())
            return ret;

        std::wstring sProp(name);
        std::string sPropA = U_TO_UTF8(sProp);

        ret.m_internal->m_context = m_internal->m_context;
        ret.m_internal->m_value = m_internal->m_value->toObject()->get(sPropA.c_str());

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
        if (m_pInternal->m_bIsServerSafeVersion)
            return;
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

        if (!sJsCommands.empty())
        {
            // Такого быть не должно!!! Так как результат никуда не сохранится. пустое действие.
            std::wstring sUnicodeCommand = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sJsCommands.c_str(), (LONG)sJsCommands.length());
            bool bIsNoError = this->ExecuteCommand(sUnicodeCommand.c_str());
            sJsCommands = "";
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
        {
            m_pInternal->m_bIsServerSafeVersion = true;
            m_pInternal->m_sFolderForSaveOnlyUseNames = std::wstring(value);
        }
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
        else if (sParam == "--fonts-system")
        {
            m_pInternal->m_oParams.m_bIsSystemFonts = (std::wstring(value) == L"true");
        }
        else if (sParam == "--fonts-dir")
        {
            m_pInternal->m_oParams.m_arFontDirs.push_back(std::wstring(value));
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
        CJSContext::ExternalInitialize();
    }

    void CDocBuilder::Dispose()
    {
        CJSContext::ExternalDispose();
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
