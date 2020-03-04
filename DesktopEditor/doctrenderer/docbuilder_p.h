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
#ifndef DOC_BUILDER_PRIVATE
#define DOC_BUILDER_PRIVATE

#include "docbuilder.h"
#include "doctrenderer.h"

#include "../xml/include/xmlutils.h"
#include <iostream>

#define ASC_APPLICATION_FONTS_NO_THUMBNAILS
#include "../fontengine/application_generate_fonts.h"

#include "../common/File.h"
#include "../common/Directory.h"

#include "../../Common/OfficeFileFormats.h"
#include "../../Common/OfficeFileFormatChecker.h"

#include "nativebuilder.h"
#include <list>

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

#ifdef BUIDLER_OPEN_DOWNLOAD_ENABLED
#include "../../Common/FileDownloader/FileDownloader.h"
#endif

#ifdef CreateFile
#undef CreateFile
#endif

namespace NSDoctRenderer
{
    class CDocBuilderValue_Private
    {
    public:
        v8::Isolate* m_isolate;
        v8::Local<v8::Value> m_value;

    public:
        CDocBuilderValue_Private();
        ~CDocBuilderValue_Private();
        void Clear();
    };
}

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

class CV8RealTimeWorker
{
public:
    v8::Isolate* m_isolate;

    v8::Isolate::Scope* m_isolate_scope;
    v8::Locker* m_isolate_locker;

    CScopeWrapper<v8::HandleScope>* m_handle_scope;
    v8::Local<v8::Context> m_context;

    int m_nFileType;
    std::string m_sUtf8ArgumentJSON;

    std::string m_sGlobalVariable;

public:

    CV8RealTimeWorker(NSDoctRenderer::CDocBuilder* pBuilder);
    ~CV8RealTimeWorker();

public:

    static void _LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError);

    bool ExecuteCommand(const std::wstring& command, NSDoctRenderer::CDocBuilderValue* retValue = NULL);

    std::string GetGlobalVariable();
    std::wstring GetJSVariable(std::wstring sParam);

    bool OpenFile(const std::wstring& sBasePath, const std::wstring& path, const std::string& sString, const std::wstring& sCachePath);
    bool SaveFileWithChanges(int type, const std::wstring& _path);
};

namespace NSDoctRenderer
{
    class CAdditionalData
    {
    public:
        CAdditionalData() {}
        virtual ~CAdditionalData() {}
        virtual std::string getParam(const std::wstring& name) { return ""; }
    };

    class CDocBuilderParams
    {
    public:
        CDocBuilderParams()
        {
            m_bCheckFonts = false;
            m_sWorkDir = L"";
            m_bSaveWithDoctrendererMode = false;
            m_sArgumentJSON = "";
        }

    public:
        bool m_bCheckFonts;
        std::wstring m_sWorkDir;
        bool m_bSaveWithDoctrendererMode;
        std::string m_sArgumentJSON;
    };

    class CDocBuilder_Private
    {
    public:
        CArray<std::wstring> m_arrFiles;

        std::vector<std::wstring> m_arDoctSDK;
        std::vector<std::wstring> m_arPpttSDK;
        std::vector<std::wstring> m_arXlstSDK;

        std::wstring m_strEditorType;
        std::wstring m_strFilePath;

        std::wstring m_strAllFonts;
        bool m_bIsNotUseConfigAllFontsDir;

        std::wstring m_sTmpFolder;
        std::wstring m_sFileDir;
        int m_nFileType;

        std::wstring m_sX2tPath;

        CV8RealTimeWorker* m_pWorker;

        CAdditionalData* m_pAdditionalData;

        CDocBuilderParams m_oParams;
        bool m_bIsInit;

        bool m_bIsCacheScript;

        std::wstring m_sFolderForSaveOnlyUseNames;

        std::string m_sGlobalVariable;
        bool m_bIsGlobalVariableUse;

        NSDoctRenderer::CDocBuilder* m_pParent;
    public:
        CDocBuilder_Private()
        {
            m_pParent = NULL;
            m_pWorker = NULL;

            m_nFileType = -1;

            m_sTmpFolder = NSFile::CFileBinary::GetTempPath();

            // под линуксом предыдущая функция создает файл!!!
            if (NSFile::CFileBinary::Exists(m_sTmpFolder))
                NSFile::CFileBinary::Remove(m_sTmpFolder);

            m_pAdditionalData = NULL;
            m_bIsInit = false;
            m_bIsCacheScript = true;

            m_sGlobalVariable = "";
            m_bIsGlobalVariableUse = false;

            m_bIsNotUseConfigAllFontsDir = false;
        }

        void Init()
        {
            if (m_bIsInit)
                return;

            std::wstring sWorkDir = m_oParams.m_sWorkDir;
            if (sWorkDir.empty() || !NSDirectory::Exists(sWorkDir))
            {
                sWorkDir = NSFile::GetProcessDirectory();
                if (!m_oParams.m_sWorkDir.empty())
                {
                    std::wstring sCheck = sWorkDir;
                    if (0 != m_oParams.m_sWorkDir.find('/'))
                        sCheck += L"/";
                    sCheck += m_oParams.m_sWorkDir;
                    if (NSDirectory::Exists(sCheck))
                        sWorkDir = sCheck;
                }
            }
            else
            {
                std::wstring sNatural = NSFile::GetProcessDirectory();
                if (0 != sWorkDir.find('/'))
                    sNatural += L"/";
                sNatural += sWorkDir;
                if (NSDirectory::Exists(sNatural))
                    sWorkDir = sNatural;
            }

            m_sX2tPath = sWorkDir;

            std::wstring sConfigDir = sWorkDir + L"/";
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
                        {
                            if (!m_bIsNotUseConfigAllFontsDir)
                            {
                                m_strAllFonts = strFilePath;

                                if (!NSFile::CFileBinary::Exists(m_strAllFonts) || NSFile::CFileBinary::Exists(sConfigDir + m_strAllFonts))
                                    m_strAllFonts = sConfigDir + m_strAllFonts;
                            }
                            else
                            {
                                m_arrFiles.Add(m_strAllFonts);
                                continue;
                            }
                        }

                        if (NSFile::CFileBinary::Exists(strFilePath) && !NSFile::CFileBinary::Exists(sConfigDir + strFilePath))
                            m_arrFiles.Add(strFilePath);
                        else
                            m_arrFiles.Add(sConfigDir + strFilePath);
                    }
                }
            }

            XmlUtils::CXmlNode oNodeSdk = oNode.ReadNode(L"DoctSdk");
            if (oNodeSdk.IsValid())
                LoadSDK_scripts(oNodeSdk, m_arDoctSDK, sConfigDir);

            oNodeSdk = oNode.ReadNode(L"PpttSdk");
            if (oNodeSdk.IsValid())
                LoadSDK_scripts(oNodeSdk, m_arPpttSDK, sConfigDir);

            oNodeSdk = oNode.ReadNode(L"XlstSdk");
            if (oNodeSdk.IsValid())
                LoadSDK_scripts(oNodeSdk, m_arXlstSDK, sConfigDir);

            CheckFonts(m_oParams.m_bCheckFonts);

            m_bIsInit = true;
        }

        ~CDocBuilder_Private()
        {
            CloseFile();

            RELEASEOBJECT(m_pAdditionalData);
        }

        void LoadSDK_scripts(XmlUtils::CXmlNode& oNode, std::vector<std::wstring>& _files, const std::wstring& strConfigDir)
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
                        !NSFile::CFileBinary::Exists(strConfigDir + strFilePath))
                        _files.push_back(strFilePath);
                    else
                        _files.push_back(strConfigDir + strFilePath);
                }
            }
            else
            {
                std::wstring strFilePath = oNode.GetText();

                if (NSFile::CFileBinary::Exists(strFilePath) &&
                    !NSFile::CFileBinary::Exists(strConfigDir + strFilePath))
                    _files.push_back(strFilePath);
                else
                    _files.push_back(strConfigDir + strFilePath);
            }
        }

        void CheckFonts(bool bIsCheckSystemFonts)
        {
            std::vector<std::string> strFonts;
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
                                strFonts.push_back(s);
                            }
                            nStart = nCur + 1;
                        }
                    }

                    delete[] pBuffer;
                }
            }

            bool bIsEqual = NSFile::CFileBinary::Exists(strFontsSelectionBin);

            if (!bIsEqual || bIsCheckSystemFonts)
            {
                NSFonts::IApplicationFonts* pApplicationF = NSFonts::NSApplication::Create();
                std::vector<std::wstring> strFontsW_Cur = pApplicationF->GetSetupFontFiles();

                if (strFonts.size() != strFontsW_Cur.size())
                    bIsEqual = false;

                if (bIsEqual)
                {
                    int nCount = (int)strFonts.size();
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

                    if (strFonts.size() != 0)
                        NSFile::CFileBinary::Remove(strDirectory + L"/fonts.log");

                    NSFile::CFileBinary oFile;
                    oFile.CreateFileW(strDirectory + L"/fonts.log");
                    int nCount = (int)strFontsW_Cur.size();
                    for (int i = 0; i < nCount; ++i)
                    {
                        oFile.WriteStringUTF8(strFontsW_Cur[i]);
                        oFile.WriteFile((BYTE*)"\n", 1);
                    }
                    oFile.CloseFile();

                    pApplicationF->InitializeFromArrayFiles(strFontsW_Cur, 2);
                    NSCommon::SaveAllFontsJS(pApplicationF, strAllFontsJSPath, L"", strFontsSelectionBin);
                }

                RELEASEOBJECT(pApplicationF);
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
            Init();
#if 1
            CheckFileDir();

            std::wstring sEmptyPath = m_sX2tPath + L"/empty/";
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
            std::wstring sPath = m_sX2tPath + L"/empty/new.";
            if (type & AVS_OFFICESTUDIO_FILE_DOCUMENT)
                sPath += L"docx";
            else if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
                sPath += L"pptx";
            else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
                sPath += L"xlsx";
            return this->OpenFile(sPath, L"");
#endif
        }

        void MoveFileOpen(const std::wstring& from, const std::wstring& to)
        {
#ifdef BUIDLER_OPEN_DOWNLOAD_ENABLED
            int n1 = (int)from.find (L"www");
            int n2 = (int)from.find (L"http");
            int n3 = (int)from.find (L"ftp");
            int n4 = (int)from.find (L"https");

            //если nI сранивать не с 0, то будут проблемы
            //потому что в инсталяции мы кладем файлы в /var/www...
            if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
            {
                CFileDownloader oDownloader(from, false);
                oDownloader.SetFilePath(to);
                if (oDownloader.DownloadSync())
                    return;
            }
#endif

#ifdef BUIDLER_OPEN_BASE64_ENABLED
            if (0 == from.find(L"data:"))
            {
                std::wstring::size_type findBase64 = from.find(L"base64,");
                if (std::wstring::npos != findBase64)
                {
                    int nStartBase64 = (int)findBase64;
                    if (50 > nStartBase64)
                    {
                        nStartBase64 += 7;
                        const wchar_t* pStart = from.c_str() + nStartBase64;
                        int nDataLen = (int)from.length() - nStartBase64;

                        std::string sBase64 = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(pStart, (LONG)nDataLen, false);

                        BYTE* pDataDst = NULL;
                        int nDataDstLen = 0;
                        if (NSFile::CBase64Converter::Decode(sBase64.c_str(), (int)sBase64.length(), pDataDst, nDataLen))
                        {
                            NSFile::CFileBinary oFileDst;
                            if (oFileDst.CreateFileW(to))
                            {
                                oFileDst.WriteFile(pDataDst, (DWORD)nDataDstLen);
                                oFileDst.CloseFile();

                                RELEASEARRAYOBJECTS(pDataDst);
                                return;
                            }

                            RELEASEARRAYOBJECTS(pDataDst);
                        }
                    }
                }
            }
#endif

            NSFile::CFileBinary::Copy(from, to);
        }

        int ConvertToInternalFormat(const std::wstring& sFolder, const std::wstring& sFile, const std::wstring& sParams)
        {
            NSStringUtils::CStringBuilder oBuilder;
            oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
            oBuilder.WriteEncodeXmlString(sFile);
            oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");
            oBuilder.WriteEncodeXmlString(sFolder);
            oBuilder.WriteString(L"/Editor.bin</m_sFileTo><m_nFormatTo>8192</m_nFormatTo>");

            if (!m_bIsNotUseConfigAllFontsDir)
            {
                oBuilder.WriteString(L"<m_sFontDir>");
                oBuilder.WriteEncodeXmlString(m_sX2tPath + L"/sdkjs/common");
                oBuilder.WriteString(L"</m_sFontDir>");
            }
            else
            {
                oBuilder.WriteString(L"<m_sFontDir>");
                oBuilder.WriteEncodeXmlString(NSCommon::GetDirectoryName(m_strAllFonts));
                oBuilder.WriteString(L"</m_sFontDir>");

                oBuilder.WriteString(L"<m_sAllFontsPath>");
                oBuilder.WriteEncodeXmlString(m_strAllFonts);
                oBuilder.WriteString(L"</m_sAllFontsPath>");
            }

            oBuilder.WriteString(L"<m_bIsNoBase64>true</m_bIsNoBase64>");
            oBuilder.WriteString(L"<m_sThemeDir>./sdkjs/slide/themes</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
            oBuilder.WriteString(sParams);
            oBuilder.WriteString(L"</TaskQueueDataConvert>");

            std::wstring sXmlConvert = oBuilder.GetData();

            std::wstring sConverterExe = m_sX2tPath + L"/x2t";

            int nReturnCode = 0;

            std::wstring sTempFileForParams = sFolder + L"/params_from.xml";
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

            return nReturnCode;
        }

        std::wstring GetFileCopyExt(const std::wstring& path)
        {
            std::wstring sExtCopy = NSCommon::GetFileExtention(path);

            if (true)
            {
                // для файлов по ссылке - расширение может быть плохим.
                const wchar_t* sExtCopyPtr = sExtCopy.c_str();
                int nExtCopyLen = sExtCopy.length();
                int nValidIndex = 0;
                while (nValidIndex < nExtCopyLen)
                {
                    wchar_t c = sExtCopyPtr[nValidIndex];
                    if ((c >= 'a' && c <= 'z') ||
                        (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9'))
                    {
                        ++nValidIndex;
                    }
                    else
                        break;
                }

                if (nValidIndex > 0)
                {
                    sExtCopy = sExtCopy.substr(0, nValidIndex);
                }
                else
                {
                    sExtCopy = L"tmp";
                }
            }

            return sExtCopy;
        }

        int OpenFile(const std::wstring& path, const std::wstring& params)
        {
            Init();

            LOGGER_SPEED_START

            CheckFileDir();
            NSDirectory::CreateDirectory(m_sFileDir + L"/changes");

            std::wstring sExtCopy = GetFileCopyExt(path);
            std::wstring sFileCopy = m_sFileDir + L"/origin." + sExtCopy;
            MoveFileOpen(path, sFileCopy);

            COfficeFileFormatChecker oChecker;
            if (!oChecker.isOfficeFile(sFileCopy))
                return false;

            if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_DOCUMENT)
                m_nFileType = 0;
            if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_PRESENTATION)
                m_nFileType = 1;
            if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
                m_nFileType = 2;

            int nReturnCode = ConvertToInternalFormat(m_sFileDir, sFileCopy, params);

            LOGGER_SPEED_LAP("open_convert")

            if (0 == nReturnCode)
                return 0;

            NSDirectory::DeleteDirectory(m_sFileDir);
            m_sFileDir = L"";
            m_nFileType = -1;

            std::wstring sErrorLog = L"open file error (" + std::to_wstring(nReturnCode) + L")";
            CV8RealTimeWorker::_LOGGING_ERROR_(L"error: ", sErrorLog);
            return nReturnCode;
        }

        void CloseFile()
        {
            Init();

            if (NSDirectory::Exists(m_sFileDir))
                NSDirectory::DeleteDirectory(m_sFileDir);

            m_sFileDir = L"";
            m_nFileType = -1;

            if (m_pWorker)
                m_sGlobalVariable = m_pWorker->GetGlobalVariable();
            RELEASEOBJECT(m_pWorker);
        }

        std::wstring GetSaveFilePath(const std::wstring& path)
        {
            std::wstring _path = path;
            if (!m_sFolderForSaveOnlyUseNames.empty())
            {
                _path = m_sFolderForSaveOnlyUseNames;
                wchar_t last = m_sFolderForSaveOnlyUseNames.c_str()[m_sFolderForSaveOnlyUseNames.length() - 1];
                if (last != '/' && last != '\\')
                    _path += L"/";
                _path += NSCommon::GetFileName(path);
            }

            std::wstring sDstFileDir = NSCommon::GetDirectoryName(_path);
            if ((sDstFileDir != _path) && !NSDirectory::Exists(sDstFileDir))
                NSDirectory::CreateDirectories(sDstFileDir);

            return _path;
        }

        int SaveFile(const int& type, const std::wstring& path, const wchar_t* params = NULL)
        {
            Init();

            if (-1 == m_nFileType)
            {
                CV8RealTimeWorker::_LOGGING_ERROR_(L"error (save)", L"file not opened!");
                return false;
            }

            LOGGER_SPEED_START

            std::wstring sFileBin = L"/Editor.bin";

            if (!m_oParams.m_bSaveWithDoctrendererMode && m_pWorker)
            {
                this->m_pWorker->SaveFileWithChanges(type, m_sFileDir + L"/Editor2.bin");
                sFileBin = L"/Editor2.bin";
            }

            NSStringUtils::CStringBuilder oBuilder;

            std::wstring _path = GetSaveFilePath(path);

            oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
            oBuilder.WriteEncodeXmlString(m_sFileDir);
            oBuilder.WriteString(sFileBin + L"</m_sFileFrom><m_sFileTo>");
            oBuilder.WriteEncodeXmlString(_path);
            oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>");
            oBuilder.WriteString(std::to_wstring(type));
            oBuilder.WriteString(L"</m_nFormatTo><m_sThemeDir>");
            oBuilder.WriteEncodeXmlString(L"./sdkjs/slide/themes");
            if (!m_oParams.m_bSaveWithDoctrendererMode)
                oBuilder.WriteString(L"</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
            else
                oBuilder.WriteString(L"</m_sThemeDir><m_bFromChanges>true</m_bFromChanges><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
            oBuilder.WriteString(L"<m_nCsvTxtEncoding>46</m_nCsvTxtEncoding><m_nCsvDelimiter>4</m_nCsvDelimiter>");

            if (!m_bIsNotUseConfigAllFontsDir)
            {
                oBuilder.WriteString(L"<m_sFontDir>");
                oBuilder.WriteEncodeXmlString(m_sX2tPath + L"/sdkjs/common");
                oBuilder.WriteString(L"</m_sFontDir>");
            }
            else
            {
                oBuilder.WriteString(L"<m_sFontDir>");
                oBuilder.WriteEncodeXmlString(NSCommon::GetDirectoryName(m_strAllFonts));
                oBuilder.WriteString(L"</m_sFontDir>");

                oBuilder.WriteString(L"<m_sAllFontsPath>");
                oBuilder.WriteEncodeXmlString(m_strAllFonts);
                oBuilder.WriteString(L"</m_sAllFontsPath>");
            }

            if (NULL != params)
            {
                std::wstring sConvertionParams(params);
                oBuilder.WriteString(sConvertionParams);
            }

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

            LOGGER_SPEED_LAP("save_convert")

            if (0 == nReturnCode)
                return 0;

            std::wstring sErrorLog = L"save file error (" + std::to_wstring(nReturnCode) + L")";
            CV8RealTimeWorker::_LOGGING_ERROR_(L"error: ", sErrorLog);
            return nReturnCode;
        }

        bool ExecuteCommand(const std::wstring& command, CDocBuilderValue* retValue = NULL)
        {
            if (command.length() < 7 && !retValue) // minimum command (!!!)
                return true;

            Init();

            if (-1 == m_nFileType)
            {
                CV8RealTimeWorker::_LOGGING_ERROR_(L"error (command)", L"file not opened!");
                return false;
            }

            if (NULL == m_pWorker)
            {
                m_pWorker = new CV8RealTimeWorker(m_pParent);
                m_pWorker->m_nFileType = m_nFileType;
                m_pWorker->m_sUtf8ArgumentJSON = m_oParams.m_sArgumentJSON;
                m_pWorker->m_sGlobalVariable = m_sGlobalVariable;

                std::wstring sCachePath = L"";
                if (m_bIsCacheScript)
                    sCachePath = GetScriptCache();

                bool bOpen = m_pWorker->OpenFile(m_sX2tPath, m_sFileDir, GetScript(), sCachePath);
                if (!bOpen)
                    return false;
            }

            return m_pWorker->ExecuteCommand(command, retValue);
        }

        std::string GetScript()
        {
            std::vector<std::wstring>* arSdkFiles = NULL;

            std::wstring sResourceFile;
            switch (m_nFileType)
            {
            case 0:
                {
                    arSdkFiles = &m_arDoctSDK;
                    break;
                }
            case 1:
                {
                    arSdkFiles = &m_arPpttSDK;
                    break;
                }
            case 2:
                {
                    arSdkFiles = &m_arXlstSDK;
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

            if (NULL != arSdkFiles)
            {
                for (std::vector<std::wstring>::iterator i = arSdkFiles->begin(); i != arSdkFiles->end(); i++)
                {
                    strScript += ReadScriptFile(*i);
                    strScript += "\n\n";
                }
            }

            if (m_nFileType == 2)
                strScript += "\n$.ready();";

            return strScript;
        }

        std::wstring GetScriptCache()
        {
            std::vector<std::wstring>* arSdkFiles = NULL;
            switch (m_nFileType)
            {
            case 0:
                {
                    arSdkFiles = &m_arDoctSDK;
                    break;
                }
            case 1:
                {
                    arSdkFiles = &m_arPpttSDK;
                    break;
                }
            case 2:
                {
                    arSdkFiles = &m_arXlstSDK;
                    break;
                }
            default:
                return L"";
            }

            if (0 < arSdkFiles->size())
            {
                return NSCommon::GetDirectoryName(*arSdkFiles->begin()) + L"/sdk-all.cache";
            }
            return L"";
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

        void WriteData(const wchar_t* path, const wchar_t* value, const bool& append)
        {
            std::wstring sValue(value);
            std::string sValueA = U_TO_UTF8(sValue);
            NSCommon::string_replaceA(sValueA, "%", "%%");

            std::wstring _sFile(path);
            std::wstring sFile = GetSaveFilePath(_sFile);

            if (!append && NSFile::CFileBinary::Exists(sFile))
                NSFile::CFileBinary::Remove(sFile);

            NSFile::CFileBinary oFile;
            FILE* pFile = oFile.OpenFileNative(sFile, append ? L"a+" : L"a");
            if (pFile)
            {
                fprintf(pFile, sValueA.c_str());
                fclose(pFile);
            }
        }
    };    
}

#endif // DOC_BUILDER_PRIVATE
