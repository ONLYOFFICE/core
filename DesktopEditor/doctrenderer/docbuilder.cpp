#include "docbuilder.h"
//#include "nativecontrol.h"

#include "../xml/include/xmlutils.h"
#include <iostream>

#define ASC_APPLICATION_FONTS_NO_THUMBNAILS
#include "../ChromiumBasedEditors2/lib/src/application_generate_fonts.h"

#include "../common/File.h"
#include "../common/Directory.h"

#include "../../Common/OfficeFileFormats.h"
#include "../../Common/OfficeFileFormatChecker.h"

#include "nativecontrol.h"
#include <list>

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

template <typename T>
class CScopeWrapper
{
private:
    T m_handler;

private:
    CScopeWrapper(const CScopeWrapper&) {}
    void operator=(const CScopeWrapper&) {}

public:

    CScopeWrapper(v8::Isolate* isolate) : m_handler(isolate) {}
};

class CV8Initializer
{
private:
    v8::Platform* m_platform;

public:
    CV8Initializer()
    {
        m_platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(m_platform);

        v8::V8::Initialize();
        v8::V8::InitializeICU();

        enableTypedArrays();
    }
    ~CV8Initializer()
    {
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete m_platform;
    }
};

class CV8RealTimeWorker
{
public:
    v8::Isolate* m_isolate;

    v8::Isolate::Scope* m_isolate_scope;
    v8::Locker* m_isolate_locker;

    CScopeWrapper<v8::HandleScope>* m_handle_scope;
    v8::Local<v8::Context> m_context;

    int m_nFileType;

public:
    static CV8Initializer* m_pInitializer;

public:

    CV8RealTimeWorker()
    {
        if (NULL == m_pInitializer)
            NSDoctRenderer::CDocBuilder::Initialize();

        m_nFileType = -1;

        m_isolate = v8::Isolate::New();

        m_isolate_scope = new v8::Isolate::Scope(m_isolate);
        m_isolate_locker = new v8::Locker(m_isolate);
        m_handle_scope = new CScopeWrapper<v8::HandleScope>(m_isolate);

        v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
        global->Set(v8::String::NewFromUtf8(m_isolate, "CreateNativeEngine"), v8::FunctionTemplate::New(m_isolate, CreateNativeObjectBuilder));
        global->Set(v8::String::NewFromUtf8(m_isolate, "CreateNativeMemoryStream"), v8::FunctionTemplate::New(m_isolate, CreateNativeMemoryStream));

        m_context = v8::Context::New(m_isolate, NULL, global);
    }
    ~CV8RealTimeWorker()
    {
        RELEASEOBJECT(m_handle_scope);
        m_context.Clear();

        RELEASEOBJECT(m_isolate_locker);
        RELEASEOBJECT(m_isolate_scope);

        m_isolate->Dispose();
        m_isolate = NULL;
    }

public:

    static void _LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError)
    {
        std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
        std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

        std::cerr << sT << ": " << sE << std::endl;
    }

    bool ExecuteCommand(const std::wstring& command, bool bIsSave = true)
    {
        std::string commandA = U_TO_UTF8(command);
        commandA = "_api." + commandA;

        if (bIsSave)
            commandA += "_api.asc_Save();";

        v8::Context::Scope context_scope(m_context);

        v8::TryCatch try_catch;

        v8::Local<v8::String> source = v8::String::NewFromUtf8(m_isolate, commandA.c_str());
        v8::Local<v8::Script> script = v8::Script::Compile(source);

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
            std::wstring strException   = to_cstring(try_catch.Message()->Get());

            _LOGGING_ERROR_(L"execute_compile_code", strCode);
            _LOGGING_ERROR_(L"execute_compile", strException);

            return false;
        }
        else
        {
            script->Run();

            if (try_catch.HasCaught())
            {
                std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                std::wstring strException   = to_cstring(try_catch.Message()->Get());

                _LOGGING_ERROR_(L"execute_run_code", strCode);
                _LOGGING_ERROR_(L"execute_run", strException);

                return false;
            }
        }

        return true;
    }

    bool OpenFile(const std::wstring& path, const std::string& sString)
    {
        v8::Context::Scope context_scope(m_context);

        v8::TryCatch try_catch;

        v8::Local<v8::String> source = v8::String::NewFromUtf8(m_isolate, sString.c_str());
        v8::Local<v8::Script> script = v8::Script::Compile(source);

        if (try_catch.HasCaught())
        {
            std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
            std::wstring strException   = to_cstring(try_catch.Message()->Get());

            _LOGGING_ERROR_(L"sdk_compile_code", strCode);
            _LOGGING_ERROR_(L"sdk_compile", strException);

            return false;
        }
        else
        {
            script->Run();

            if (try_catch.HasCaught())
            {
                std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                std::wstring strException   = to_cstring(try_catch.Message()->Get());

                _LOGGING_ERROR_(L"sdk_run_code", strCode);
                _LOGGING_ERROR_(L"sdk_run", strException);

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
                    std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                    std::wstring strException   = to_cstring(try_catch.Message()->Get());

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
            pNative->m_strFontsDirectory = NSFile::GetProcessDirectory() + L"/sdk/Common";
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

                v8::Handle<v8::Value> args_open[2];
                args_open[0] = oWorkerLoader.GetDataFull();
                args_open[1] = v8::Integer::New(m_isolate, nVersion);

                func_open->Call(global_js, 2, args_open);

                if (try_catch.HasCaught())
                {
                    std::wstring strCode        = to_cstring(try_catch.Message()->GetSourceLine());
                    std::wstring strException   = to_cstring(try_catch.Message()->Get());

                    _LOGGING_ERROR_(L"open_code", strCode);
                    _LOGGING_ERROR_(L"open", strException);

                    bIsBreak = true;
                }
            }
        }

        if (!bIsBreak)
            bIsBreak = !this->ExecuteCommand(L"asc_nativeInitBuilder();", false);
        if (!bIsBreak)
            bIsBreak = !this->ExecuteCommand(L"asc_SetSilentMode(true);", false);

        return !bIsBreak;
    }
};

CV8Initializer* CV8RealTimeWorker::m_pInitializer = NULL;

#ifdef CreateFile
#undef CreateFile
#endif

namespace NSDoctRenderer
{
    class CDocBuilder_Private
    {
    public:
        CArray<std::wstring> m_arrFiles;

        std::wstring m_strDoctSDK;
        std::wstring m_strPpttSDK;
        std::wstring m_strXlstSDK;

        std::wstring m_strEditorType;
        std::wstring m_strFilePath;

        std::wstring m_strAllFonts;

        std::wstring m_sTmpFolder;
        std::wstring m_sFileDir;
        int m_nFileType;

        std::wstring m_sX2tPath;

        CV8RealTimeWorker* m_pWorker;
    public:
        CDocBuilder_Private(bool bIsCheckSystemFonts)
        {
            m_sX2tPath = NSFile::GetProcessDirectory();
            m_pWorker = NULL;

            m_nFileType = -1;

#if 0
            m_sX2tPath += L"/converter";
#endif

            std::wstring sConfigDir = NSFile::GetProcessDirectory() + L"/";
            std::wstring sConfigPath = sConfigDir + L"DoctRenderer.config";

            XmlUtils::CXmlNode oNode;
            if (oNode.FromXmlFile(sConfigPath))
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

                        if (std::wstring::npos != strFilePath.find(L"AllFonts.js"))
                            m_strAllFonts = strFilePath;

                        if (!NSFile::CFileBinary::Exists(m_strAllFonts) || NSFile::CFileBinary::Exists(sConfigDir + m_strAllFonts))
                            m_strAllFonts = sConfigDir + m_strAllFonts;

                        if (NSFile::CFileBinary::Exists(strFilePath) && !NSFile::CFileBinary::Exists(sConfigDir + strFilePath))
                            m_arrFiles.Add(strFilePath);
                        else
                            m_arrFiles.Add(sConfigDir + strFilePath);
                    }
                }
            }

            m_strDoctSDK = L"";
            m_strPpttSDK = L"";
            m_strXlstSDK = L"";

            XmlUtils::CXmlNode oNodeSdk = oNode.ReadNode(L"DoctSdk");
            if (oNodeSdk.IsValid())
                m_strDoctSDK = oNodeSdk.GetText();

            oNodeSdk = oNode.ReadNode(L"PpttSdk");
            if (oNodeSdk.IsValid())
                m_strPpttSDK = oNodeSdk.GetText();

            oNodeSdk = oNode.ReadNode(L"XlstSdk");
            if (oNodeSdk.IsValid())
                m_strXlstSDK = oNodeSdk.GetText();

            if (!NSFile::CFileBinary::Exists(m_strDoctSDK))
                m_strDoctSDK = sConfigDir + m_strDoctSDK;

            if (!NSFile::CFileBinary::Exists(m_strPpttSDK))
                m_strPpttSDK = sConfigDir + m_strPpttSDK;

            if (!NSFile::CFileBinary::Exists(m_strXlstSDK))
                m_strXlstSDK = sConfigDir + m_strXlstSDK;

            CheckFonts(bIsCheckSystemFonts);

            m_sTmpFolder = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"DTB");

            // под линуксом предыдущая функция создает файл!!!
            if (NSFile::CFileBinary::Exists(m_sTmpFolder))
                NSFile::CFileBinary::Remove(m_sTmpFolder);
        }

        ~CDocBuilder_Private()
        {
            CloseFile();
        }

        void CheckFonts(bool bIsCheckSystemFonts)
        {
            CArray<std::string> strFonts;
            std::wstring strDirectory = NSCommon::GetDirectoryName(m_strAllFonts);

            std::wstring strAllFontsJSPath = strDirectory + L"/AllFonts.js";
            std::wstring strFontsSelectionBin = strDirectory + L"/font_selection.bin";

            if (true)
            {
                NSFile::CFileBinary oFile;
                if (oFile.OpenFile(strDirectory + L"/fonts.log"))
                {
                    int nSize = oFile.GetFileSize();
                    char* pBuffer = new char[nSize];
                    DWORD dwReaden = 0;
                    oFile.ReadFile((BYTE*)pBuffer, nSize, dwReaden);
                    oFile.CloseFile();

                    int nStart = 0;
                    int nCur = nStart;
                    for (; nCur < nSize; ++nCur)
                    {
                        if (pBuffer[nCur] == '\n')
                        {
                            int nEnd = nCur - 1;
                            if (nEnd > nStart)
                            {
                                std::string s(pBuffer + nStart, nEnd - nStart + 1);
                                strFonts.Add(s);
                            }
                            nStart = nCur + 1;
                        }
                    }

                    delete[] pBuffer;
                }
            }

            bool bIsEqual = NSFile::CFileBinary::Exists(strFontsSelectionBin);

            if (bIsEqual && bIsCheckSystemFonts)
            {
                CApplicationFonts oApplicationF;
                CArray<std::wstring> strFontsW_Cur = oApplicationF.GetSetupFontFiles();

                if (strFonts.GetCount() != strFontsW_Cur.GetCount())
                    bIsEqual = false;

                if (bIsEqual)
                {
                    int nCount = strFonts.GetCount();
                    for (int i = 0; i < nCount; ++i)
                    {
                        if (strFonts[i] != NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strFontsW_Cur[i].c_str(), strFontsW_Cur[i].length()))
                        {
                            bIsEqual = false;
                            break;
                        }
                    }
                }

                if (!bIsEqual)
                {
                    if (NSFile::CFileBinary::Exists(strAllFontsJSPath))
                        NSFile::CFileBinary::Remove(strAllFontsJSPath);
                    if (NSFile::CFileBinary::Exists(strFontsSelectionBin))
                        NSFile::CFileBinary::Remove(strFontsSelectionBin);

                    if (strFonts.GetCount() != 0)
                        NSFile::CFileBinary::Remove(strDirectory + L"/fonts.log");

                    NSFile::CFileBinary oFile;
                    oFile.CreateFileW(strDirectory + L"/fonts.log");
                    int nCount = strFontsW_Cur.GetCount();
                    for (int i = 0; i < nCount; ++i)
                    {
                        oFile.WriteStringUTF8(strFontsW_Cur[i]);
                        oFile.WriteFile((BYTE*)"\n", 1);
                    }
                    oFile.CloseFile();

                    oApplicationF.InitializeFromArrayFiles(strFontsW_Cur, 2);
                    NSCommon::SaveAllFontsJS(oApplicationF, strAllFontsJSPath, L"", strFontsSelectionBin);
                }
            }
        }

        void CheckFileDir()
        {
            m_sFileDir = NSFile::CFileBinary::CreateTempFileWithUniqueName(m_sTmpFolder, L"DE_");
            if (NSFile::CFileBinary::Exists(m_sFileDir))
                NSFile::CFileBinary::Remove(m_sFileDir);

            NSCommon::string_replace(m_sFileDir, L"\\", L"/");

            std::wstring::size_type nPosPoint = m_sFileDir.rfind('.');
            if (nPosPoint != std::wstring::npos && nPosPoint > m_sTmpFolder.length())
            {
                m_sFileDir = m_sFileDir.substr(0, nPosPoint);
            }

            m_nFileType = -1;

            NSDirectory::CreateDirectory(m_sFileDir);
        }

        bool CreateFile(int type)
        {
#if 1
            CheckFileDir();

            std::wstring sEmptyPath = NSFile::GetProcessDirectory() + L"/empty/";
            if (type & AVS_OFFICESTUDIO_FILE_DOCUMENT)
            {
                sEmptyPath = sEmptyPath + L"docx.bin";
                m_nFileType = 0;
            }
            else if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
            {
                sEmptyPath = sEmptyPath + L"pptx.bin";
                m_nFileType = 1;
            }
            else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
            {
                sEmptyPath = sEmptyPath + L"xlsx.bin";
                m_nFileType = 2;
            }
            else
                return false;

            bool bRet = NSFile::CFileBinary::Copy(sEmptyPath, m_sFileDir + L"/Editor.bin");
            if (bRet)
            {
                NSDirectory::CreateDirectory(m_sFileDir + L"/media");
                NSDirectory::CreateDirectory(m_sFileDir + L"/changes");
            }

            return bRet;
#else
            std::wstring sPath = NSFile::GetProcessDirectory() + L"/empty/new.";
            if (type & AVS_OFFICESTUDIO_FILE_DOCUMENT)
                sPath += L"docx";
            else if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
                sPath += L"pptx";
            else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
                sPath += L"xlsx";
            return this->OpenFile(sPath, L"");
#endif
        }

        bool OpenFile(const std::wstring& path, const std::wstring& params)
        {
            CheckFileDir();
            NSDirectory::CreateDirectory(m_sFileDir + L"/changes");

            std::wstring sFileCopy = m_sFileDir + L"/origin." + NSCommon::GetFileExtention(path);
            NSFile::CFileBinary::Copy(path, sFileCopy);

            COfficeFileFormatChecker oChecker;
            if (!oChecker.isOfficeFile(path))
                return false;

            if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_DOCUMENT)
                m_nFileType = 0;
            if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_PRESENTATION)
                m_nFileType = 1;
            if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
                m_nFileType = 2;

            NSStringUtils::CStringBuilder oBuilder;
            oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
            oBuilder.WriteEncodeXmlString(sFileCopy);
            oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");
            oBuilder.WriteEncodeXmlString(m_sFileDir);
            oBuilder.WriteString(L"/Editor.bin</m_sFileTo><m_nFormatTo>8192</m_nFormatTo>");
            oBuilder.WriteString(L"<m_sFontDir>");
            oBuilder.WriteEncodeXmlString(NSFile::GetProcessDirectory() + L"/sdk/Common");
            oBuilder.WriteString(L"</m_sFontDir>");
            oBuilder.WriteString(L"<m_sThemeDir>./sdk/presentationeditor/themes</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
            oBuilder.WriteString(params);
            oBuilder.WriteString(L"</TaskQueueDataConvert>");

            std::wstring sXmlConvert = oBuilder.GetData();

            std::wstring sConverterExe = m_sX2tPath + L"/x2t";

            int nReturnCode = 0;

            std::wstring sTempFileForParams = m_sFileDir + L"/params_from.xml";
            NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

    #ifdef WIN32
            std::wstring sApp = L"x2t32 ";

            if (NSFile::CFileBinary::Exists(sConverterExe + L".exe"))
            {
                sApp = L"x2t ";
                sConverterExe += L".exe";
            }
            else
                sConverterExe += L"32.exe";

            STARTUPINFO sturtupinfo;
            ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
            sturtupinfo.cb = sizeof(STARTUPINFO);

            sApp += (L"\"" + sTempFileForParams + L"\"");
            wchar_t* pCommandLine = NULL;
            if (true)
            {
                pCommandLine = new wchar_t[sApp.length() + 1];
                memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
                pCommandLine[sApp.length()] = (wchar_t)'\0';
            }

            PROCESS_INFORMATION processinfo;
            ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
            BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
                                       NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

            ::WaitForSingleObject(processinfo.hProcess, INFINITE);

            RELEASEARRAYOBJECTS(pCommandLine);

            //get exit code
            DWORD dwExitCode = 0;
            if (GetExitCodeProcess(processinfo.hProcess, &dwExitCode))
            {
                nReturnCode = (int)dwExitCode;
            }

            CloseHandle(processinfo.hProcess);
            CloseHandle(processinfo.hThread);

    #endif

    #ifdef LINUX
            pid_t pid = fork(); // create child process
            int status;

            std::string sProgramm = U_TO_UTF8(sConverterExe);
            std::string sXmlA = U_TO_UTF8(sTempFileForParams);

            switch (pid)
            {
            case -1: // error
                break;

            case 0: // child process
            {
                std::string sLibraryDir = sProgramm;
                std::string sPATH = sProgramm;
                if (std::string::npos != sProgramm.find_last_of('/'))
                {
                    sLibraryDir = "LD_LIBRARY_PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
                    sPATH = "PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
                }

    #ifdef _MAC
                sLibraryDir = "DY" + sLibraryDir;
    #endif

                const char* nargs[3];
                nargs[0] = sProgramm.c_str();
                nargs[1] = sXmlA.c_str();
                nargs[2] = NULL;

    #ifndef _MAC
                const char* nenv[2];
                nenv[0] = sLibraryDir.c_str();
                nenv[1] = NULL;
    #else
                const char* nenv[3];
                nenv[0] = sLibraryDir.c_str();
                nenv[1] = sPATH.c_str();
                nenv[2] = NULL;
    #endif

                execve(sProgramm.c_str(),
                       (char * const *)nargs,
                       (char * const *)nenv);
                exit(EXIT_SUCCESS);
                break;
            }
            default: // parent process, pid now contains the child pid
                while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
                if (WIFEXITED(status))
                {
                    nReturnCode =  WEXITSTATUS(status);
                }
                break;
            }
    #endif

            NSFile::CFileBinary::Remove(sTempFileForParams);

            if (0 == nReturnCode)
                return true;

            NSDirectory::DeleteDirectory(m_sFileDir);
            m_sFileDir = L"";
            m_nFileType = -1;

            CV8RealTimeWorker::_LOGGING_ERROR_(L"error: ", L"open file error");
            return false;
        }

        void CloseFile()
        {
            if (NSDirectory::Exists(m_sFileDir))
                NSDirectory::DeleteDirectory(m_sFileDir);

            m_sFileDir = L"";
            m_nFileType = -1;

            RELEASEOBJECT(m_pWorker);
        }

        bool SaveFile(const int& type, const std::wstring& path)
        {
            if (-1 == m_nFileType)
            {
                CV8RealTimeWorker::_LOGGING_ERROR_(L"error (save)", L"file not opened!");
                return false;
            }

            NSStringUtils::CStringBuilder oBuilder;

            oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
            oBuilder.WriteEncodeXmlString(m_sFileDir);
            oBuilder.WriteString(L"/Editor.bin</m_sFileFrom><m_sFileTo>");
            oBuilder.WriteEncodeXmlString(path);
            oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>");
            oBuilder.WriteString(std::to_wstring(type));
            oBuilder.WriteString(L"</m_nFormatTo><m_sThemeDir>");
            oBuilder.WriteEncodeXmlString(L"./sdk/presentationeditor/themes");
            oBuilder.WriteString(L"</m_sThemeDir><m_bFromChanges>true</m_bFromChanges><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
            oBuilder.WriteString(L"<m_nCsvTxtEncoding>46</m_nCsvTxtEncoding><m_nCsvDelimiter>4</m_nCsvDelimiter>");
            oBuilder.WriteString(L"<m_sFontDir>");
            oBuilder.WriteEncodeXmlString(NSFile::GetProcessDirectory() + L"/sdk/Common");
            oBuilder.WriteString(L"</m_sFontDir>");

            int nDoctRendererParam = 0;
            //if (true) // печать пдф (лист = страница)
            //    nDoctRendererParam |= 0x02;

            oBuilder.WriteString(L"<m_nDoctParams>");
            oBuilder.WriteString(std::to_wstring(nDoctRendererParam));
            oBuilder.WriteString(L"</m_nDoctParams>");

            oBuilder.WriteString(L"</TaskQueueDataConvert>");

            std::wstring sXmlConvert = oBuilder.GetData();

            std::wstring sConverterExe = m_sX2tPath + L"/x2t";

            int nReturnCode = 0;

            std::wstring sTempFileForParams = m_sFileDir + L"/params_to.xml";
            NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

    #ifdef WIN32
            std::wstring sApp = L"x2t32 ";

            if (NSFile::CFileBinary::Exists(sConverterExe + L".exe"))
            {
                sApp = L"x2t ";
                sConverterExe += L".exe";
            }
            else
                sConverterExe += L"32.exe";

            STARTUPINFO sturtupinfo;
            ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
            sturtupinfo.cb = sizeof(STARTUPINFO);

            sApp += (L"\"" + sTempFileForParams + L"\"");
            wchar_t* pCommandLine = NULL;
            if (true)
            {
                pCommandLine = new wchar_t[sApp.length() + 1];
                memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
                pCommandLine[sApp.length()] = (wchar_t)'\0';
            }

            PROCESS_INFORMATION processinfo;
            ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
            BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
                                       NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

            ::WaitForSingleObject(processinfo.hProcess, INFINITE);

            RELEASEARRAYOBJECTS(pCommandLine);

            //get exit code
            DWORD dwExitCode = 0;
            if (GetExitCodeProcess(processinfo.hProcess, &dwExitCode))
            {
                nReturnCode = (int)dwExitCode;
            }

            CloseHandle(processinfo.hProcess);
            CloseHandle(processinfo.hThread);

    #endif

    #ifdef LINUX
            pid_t pid = fork(); // create child process
            int status;

            std::string sProgramm = U_TO_UTF8(sConverterExe);
            std::string sXmlA = U_TO_UTF8(sTempFileForParams);

            switch (pid)
            {
            case -1: // error
                break;

            case 0: // child process
            {
                std::string sLibraryDir = sProgramm;
                std::string sPATH = sProgramm;
                if (std::string::npos != sProgramm.find_last_of('/'))
                {
                    sLibraryDir = "LD_LIBRARY_PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
                    sPATH = "PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
                }

    #ifdef _MAC
                sLibraryDir = "DY" + sLibraryDir;
    #endif

                const char* nargs[3];
                nargs[0] = sProgramm.c_str();
                nargs[1] = sXmlA.c_str();
                nargs[2] = NULL;

    #ifndef _MAC
                const char* nenv[2];
                nenv[0] = sLibraryDir.c_str();
                nenv[1] = NULL;
    #else
                const char* nenv[3];
                nenv[0] = sLibraryDir.c_str();
                nenv[1] = sPATH.c_str();
                nenv[2] = NULL;
    #endif

                execve(sProgramm.c_str(),
                       (char * const *)nargs,
                       (char * const *)nenv);
                exit(EXIT_SUCCESS);
                break;
            }
            default: // parent process, pid now contains the child pid
                while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
                if (WIFEXITED(status))
                {
                    nReturnCode =  WEXITSTATUS(status);
                }
                break;
            }
    #endif

            NSFile::CFileBinary::Remove(sTempFileForParams);

            if (0 == nReturnCode)
                return true;

            CV8RealTimeWorker::_LOGGING_ERROR_(L"error: ", L"save file error");
            return false;
        }

        bool ExecuteCommand(const std::wstring& command)
        {
            if (-1 == m_nFileType)
            {
                CV8RealTimeWorker::_LOGGING_ERROR_(L"error (command)", L"file not opened!");
                return false;
            }

            if (NULL == m_pWorker)
            {
                m_pWorker = new CV8RealTimeWorker();
                m_pWorker->m_nFileType = m_nFileType;

                bool bOpen = m_pWorker->OpenFile(m_sFileDir, GetScript());
                if (!bOpen)
                    return false;
            }

            return m_pWorker->ExecuteCommand(command);
        }

        std::string GetScript()
        {
            std::wstring sResourceFile;
            switch (m_nFileType)
            {
            case 0:
                {
                    sResourceFile = m_strDoctSDK;
                    break;
                }
            case 1:
                {
                    sResourceFile = m_strPpttSDK;
                    break;
                }
            case 2:
                {
                    sResourceFile = m_strXlstSDK;
                    break;
                }
            default:
                return "";
            }

            std::string strScript = "";
            for (size_t i = 0; i < m_arrFiles.GetCount(); ++i)
            {
                strScript += ReadScriptFile(m_arrFiles[i]);
                strScript += "\n\n";
            }

            strScript += ReadScriptFile(sResourceFile);

            if (m_nFileType == 2)
                strScript += "\n$.ready();";

            return strScript;
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
}

namespace NSDoctRenderer
{
    void ParceParameters(const std::string& command, std::wstring* params)
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
    }

    CDocBuilder::CDocBuilder(bool bIsCheckSystemFonts)
    {
        m_pInternal = new CDocBuilder_Private(bIsCheckSystemFonts);
    }
    CDocBuilder::~CDocBuilder()
    {
        RELEASEOBJECT(m_pInternal);
    }

    bool CDocBuilder::OpenFile(const wchar_t* path, const wchar_t* params)
    {
        m_pInternal->m_nFileType = -1;
        if (!NSDirectory::Exists(m_pInternal->m_sTmpFolder))
            NSDirectory::CreateDirectory(m_pInternal->m_sTmpFolder);

        return m_pInternal->OpenFile(path, params);        
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
    bool CDocBuilder::SaveFile(const int& type, const wchar_t* path)
    {
        return m_pInternal->SaveFile(type, path);
    }
    void CDocBuilder::CloseFile()
    {
        m_pInternal->CloseFile();
    }

    bool CDocBuilder::ExecuteCommand(const wchar_t* command)
    {
        return m_pInternal->ExecuteCommand(command);
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

            if (_currentPos > (_start + 1))
                _commands.push_back(std::string(commands + _start, _currentPos - _start));

            if (_currentPos >= _commandsLen)
                break;
        }

        std::wstring _builder_params[4]; // с запасом
        for (std::list<std::string>::iterator i = _commands.begin(); i != _commands.end(); i++)
        {
            const std::string& command = *i;
            const char* _data = command.c_str();
            size_t _len = command.length();

            if (_data[0] == '#')
                continue;

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
                size_t _pos = 8;
                while (_data[_pos] != '(')
                    ++_pos;

                std::string sFuncNum(_data + 8, _pos - 8);
                ParceParameters(command, _builder_params);

                if ("OpenFile" == sFuncNum)
                    bIsNoError = this->OpenFile(_builder_params[0].c_str(), _builder_params[1].c_str());
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

                    this->SaveFile(nFormat, _builder_params[1].c_str());
                }
            }
            else
            {
                bIsNoError = this->m_pInternal->ExecuteCommand(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)_data, (LONG)_len));
            }

            if (!bIsNoError)
                return false;
        }

        return true;
    }

    void CDocBuilder::Initialize()
    {
        if (NULL == CV8RealTimeWorker::m_pInitializer)
            CV8RealTimeWorker::m_pInitializer = new CV8Initializer();
    }

    void CDocBuilder::Dispose()
    {
        if (NULL != CV8RealTimeWorker::m_pInitializer)
            delete CV8RealTimeWorker::m_pInitializer;
        CV8RealTimeWorker::m_pInitializer = NULL;
    }
}
