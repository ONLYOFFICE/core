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

#ifdef WIN32
#include <windows.h>
#endif

#include "HtmlFile.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/StringExt.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/fontengine/application_generate_fonts_common.h"

#include <vector>
#include <map>

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

extern char** environ;

char** linux_environ_get(const std::string& str_library_path = "")
{
    int count = 0;
    for (int i = 0; environ[i] != NULL; i++)
        ++count;
    count += 2;

    char** env = new char*[count];
    for (int i = 0; i < count; i++)
        env[i] = NULL;

    bool is_ld_library_path = false;
    for (int i = 0; environ[i] != NULL; i++)
    {
        std::string s = environ[i];
        if (0 == s.find("LD_LIBRARY_PATH"))
        {
            s += (":" + str_library_path);
            is_ld_library_path = true;
        }
        env[i] = new char[s.length() + 1];
        memcpy(env[i], s.c_str(), s.length() * sizeof(char));
        env[i][s.length()] = '\0';
    }
    if (!is_ld_library_path)
    {
        int index = count - 2;
        std::string s = "LD_LIBRARY_PATH=";
        s += str_library_path;
        env[index] = new char[s.length() + 1];
        memcpy(env[index], s.c_str(), s.length() * sizeof(char));
        env[index][s.length()] = '\0';
    }
    return env;
}
void linux_environ_clear(char** data)
{
    for (int i = 0; data[i] != NULL; i++)
        delete [] data[i];
    delete [] data;
}
void linux_environ_print(char** env)
{
    std::wstring file = NSFile::GetProcessDirectory() + L"/env.log";
    std::string fileA = U_TO_UTF8(file);
    for (int i = 0; env[i] != NULL; i++)
    {
        FILE* f = fopen(fileA.c_str(), "a+");
        std::string s = env[i];

        // replace %%
        size_t posn = 0;
        while (std::string::npos != (posn = s.find("%", posn)))
        {
            s.replace(posn, 1, "%%");
            posn += 2;
        }

        fprintf(f, s.c_str());
        fprintf(f, "\n");
        fclose(f);
    }
}

#endif

class CHtmlFile_Private
{
public:
    bool m_bIsEpub;
    std::wstring m_sHtmlFileInternal;

public:
    CHtmlFile_Private()
    {
        m_bIsEpub = false;
        m_sHtmlFileInternal = L"";

        std::wstring sProcessPath = NSFile::GetProcessDirectory();
        std::wstring sPathConfig = sProcessPath + L"/DoctRenderer.config";
        if (NSFile::CFileBinary::Exists(sPathConfig))
        {
            XmlUtils::CXmlNode oNode;
            if (oNode.FromXmlFile(sPathConfig))
            {
                std::wstring sPath = oNode.ReadValueString(L"htmlfileinternal");
                if (!sPath.empty())
                {
                    if (0 == sPath.find(L"./"))
                        sPath = sProcessPath + sPath.substr(1);
                    m_sHtmlFileInternal = sPath;
                }
            }
        }
    }
};

CHtmlFile::CHtmlFile()
{
    m_internal = new CHtmlFile_Private();
}

CHtmlFile::~CHtmlFile()
{
    RELEASEOBJECT(m_internal);
}

static std::wstring GetSdkPath()
{
    std::wstring sProcess = NSFile::GetProcessDirectory() + L"/";
    std::wstring sPathConfig = sProcess + L"DoctRenderer.config";
    XmlUtils::CXmlNode oNode;
    if (!oNode.FromXmlFile(sPathConfig))
        return L"";

    std::wstring sPath = oNode.ReadValueString(L"DoctSdk");
    if (NSFile::CFileBinary::Exists(sPath) && !NSFile::CFileBinary::Exists(sProcess + sPath))
        return sPath;

    return sProcess + sPath;
}

static std::wstring CorrectHtmlPath(const std::wstring& sPath)
{
    std::wstring sReturn = sPath;
    NSStringExt::Replace(sReturn, L"\\", L"/");

    if (std::wstring::npos != sReturn.find(L"://"))
        return sReturn;

    if (sReturn.find(L"//") == 0)
        return L"file:" + sReturn;

    if (!sPath.empty())
    {
        wchar_t c = sPath.c_str()[0];
        if (c == wchar_t('/'))
            return L"file://" + sReturn;
    }
    return L"file:///" + sReturn;
}

static bool IsLinuxXVFB()
{
    std::wstring sProcess = NSFile::GetProcessDirectory() + L"/";
    std::wstring sPathConfig = sProcess + L"DoctRenderer.config";
    XmlUtils::CXmlNode oNode;
    if (oNode.FromXmlFile(sPathConfig))
    {
        XmlUtils::CXmlNodes oNodes;
        if (oNode.GetNodes(L"htmlnoxvfb", oNodes))
        {
            if (oNodes.GetCount() == 1)
                return false;
        }
    }
    else if (oNode.FromXmlFile(sProcess + L"converter/DoctRenderer.config"))
    {
        XmlUtils::CXmlNodes oNodes;
        if (oNode.GetNodes(L"htmlnoxvfb", oNodes))
        {
            if (oNodes.GetCount() == 1)
                return false;
        }
    }
    return true;
}

static void GetScriptsPath(NSStringUtils::CStringBuilder& oBuilder)
{
    std::wstring sProcess = NSFile::GetProcessDirectory() + L"/";
    std::wstring sPathConfig = sProcess + L"DoctRenderer.config";
    XmlUtils::CXmlNode oNode;
    if (!oNode.FromXmlFile(sPathConfig))
        return;

    XmlUtils::CXmlNodes oNodesFiles;
    if (oNode.GetNodes(L"file", oNodesFiles))
    {
        int nCount = oNodesFiles.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode _node;
            oNodesFiles.GetAt(i, _node);

            std::wstring sFile = _node.GetText();

            if (!NSFile::CFileBinary::Exists(sFile) || NSFile::CFileBinary::Exists(sProcess + sFile))
                sFile = sProcess + sFile;

            sFile =  CorrectHtmlPath(sFile);

            bool bIsNeedAdd = true;
            if (std::wstring::npos != sFile.find(L"/Native/"))
                bIsNeedAdd = false;

            if (bIsNeedAdd)
            {
                oBuilder.WriteString(L"<sdk>");
                oBuilder.WriteEncodeXmlString(sFile);
                oBuilder.WriteString(L"</sdk>");
            }
        }
    }
    XmlUtils::CXmlNodes oNodesHtmlFiles;
    if (oNode.GetNodes(L"htmlfile", oNodesHtmlFiles))
    {
        int nCount = oNodesHtmlFiles.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode _node;
            oNodesHtmlFiles.GetAt(i, _node);

            std::wstring sFile = _node.GetText();

            if (!NSFile::CFileBinary::Exists(sFile) || NSFile::CFileBinary::Exists(sProcess + sFile))
                sFile = sProcess + sFile;

            sFile =  CorrectHtmlPath(sFile);

            oBuilder.WriteString(L"<sdk>");
            oBuilder.WriteEncodeXmlString(sFile);
            oBuilder.WriteString(L"</sdk>");
        }
    }

    XmlUtils::CXmlNode oNodeSdk = oNode.ReadNode(L"DoctSdk");
    XmlUtils::CXmlNodes oNodes;
    if (oNodeSdk.GetNodes(L"file", oNodes))
    {
        int nCount = oNodes.GetCount();
        XmlUtils::CXmlNode _node;
        for (int i = 0; i < nCount; ++i)
        {
            oNodes.GetAt(i, _node);
            std::wstring sPath = _node.GetText();

            if (!NSFile::CFileBinary::Exists(sPath) || NSFile::CFileBinary::Exists(sProcess + sPath))
                sPath = sProcess + sPath;

            oBuilder.WriteString(L"<sdk>");
            oBuilder.WriteEncodeXmlString(CorrectHtmlPath(sPath));
            oBuilder.WriteString(L"</sdk>");
        }
    }
    else
    {
        std::wstring sPath = oNodeSdk.GetText();

        if (!NSFile::CFileBinary::Exists(sPath) || NSFile::CFileBinary::Exists(sProcess + sPath))
            sPath = sProcess + sPath;

        oBuilder.WriteString(L"<sdk>");
        oBuilder.WriteEncodeXmlString(CorrectHtmlPath(sPath));
        oBuilder.WriteString(L"</sdk>");
    }
}

int CHtmlFile::Convert(const std::vector<std::wstring>& arFiles, const std::wstring& sDstfolder)
{
    std::wstring sInternal = m_internal->m_sHtmlFileInternal;
    if (sInternal.empty())
        sInternal = NSFile::GetProcessDirectory() + L"/HtmlFileInternal/";

    sInternal += L"HtmlFileInternal";

#ifdef WIN32
    sInternal += L".exe";
#endif

    int nReturnCode = 0;

    NSStringUtils::CStringBuilder oBuilder;
    oBuilder.WriteString(L"<html>");

    GetScriptsPath(oBuilder);

    // destination
    oBuilder.WriteString(L"<destination>");

    std::wstring sDstOut = sDstfolder;
    NSStringExt::Replace(sDstOut, L"\\", L"/");

    oBuilder.WriteEncodeXmlString(sDstOut);

    if (!sDstOut.empty())
    {
        wchar_t _c = sDstOut.c_str()[sDstOut.length() - 1];
        if (_c != '/')
            oBuilder.AddCharSafe('/');
    }

    oBuilder.WriteString(L"</destination>");

    std::vector<std::wstring> arTmpFiles;
    for (std::vector<std::wstring>::const_iterator iter = arFiles.begin(); iter != arFiles.end(); iter++)
    {
        oBuilder.WriteString(L"<file>");

        std::wstring sFilePath = *iter;
        std::wstring sExt = NSCommon::GetFileExtention(sFilePath);
        NSCommon::makeUpperW(sExt);

        if (sExt == L"HTML" || sExt == L"HTM" || sExt == L"XHTML")
            oBuilder.WriteEncodeXmlString(CorrectHtmlPath(sFilePath));
        else
        {
            std::wstring sTmpDir = L"";
            if (m_internal->m_bIsEpub)
            {
                // чтобы ссылки на картинки остались
                sTmpDir = NSFile::GetDirectoryName(sFilePath);
                if (!NSDirectory::Exists(sTmpDir))
                    sTmpDir = NSDirectory::GetTempPath();
            }
            else
            {
                sTmpDir = NSDirectory::GetTempPath();
            }

            std::wstring sTmpFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTmpDir, L"HTM");
            if (NSFile::CFileBinary::Exists(sTmpFile))
                NSFile::CFileBinary::Remove(sTmpFile);

            sTmpFile = sTmpFile + L".html";

            NSFile::CFileBinary::Copy(sFilePath, sTmpFile);
            oBuilder.WriteEncodeXmlString(CorrectHtmlPath(sTmpFile));
            arTmpFiles.push_back(sTmpFile);
        }

        oBuilder.WriteString(L"</file>");
    }

    oBuilder.WriteString(L"</html>");

#ifdef WIN32
    STARTUPINFO sturtupinfo;
    ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
    sturtupinfo.cb = sizeof(STARTUPINFO);

    std::wstring sTempFileForParams = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"XML");
    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sTempFileForParams);
    oFile.WriteStringUTF8(oBuilder.GetData(), true);
    oFile.CloseFile();

    std::wstring sApp = L"HtmlFileInternal <html>" + sTempFileForParams;
    wchar_t* pCommandLine = NULL;
    if (true)
    {
        pCommandLine = new wchar_t[sApp.length() + 1];
        memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
        pCommandLine[sApp.length()] = (wchar_t)'\0';
    }

#if 0
    std::wstringstream ss;
    ss << L"PATH=" << _wgetenv(L"PATH");
    ss << L";" << NSFile::GetProcessDirectory();
    ss << L";" << (NSFile::GetProcessDirectory() + L"\\..");
    ss << L";" << (NSFile::GetProcessDirectory() + L"\\..\\..");
    std::wstring env = ss.str();

    wchar_t* pCommandLineEnv = NULL;
    if (true)
    {
        pCommandLineEnv = new wchar_t[env.length() + 2];
        memcpy(pCommandLineEnv, env.c_str(), sApp.length() * sizeof(wchar_t));
        pCommandLineEnv[env.length()] = (wchar_t)'\0';
        pCommandLineEnv[env.length() + 1] = (wchar_t)'\0';
    }

    PROCESS_INFORMATION processinfo;
    ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
    BOOL bResult = CreateProcessW(sInternal.c_str(), pCommandLine,
                               NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT, (LPVOID)pCommandLineEnv, NULL, &sturtupinfo, &processinfo);
#else

    HANDLE ghJob = CreateJobObject(NULL, NULL);

    if (ghJob)
    {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

        // Configure all child processes associated with the job to terminate when the
        jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if ( 0 == SetInformationJobObject( ghJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
        {
            CloseHandle(ghJob);
            ghJob = NULL;
        }
    }

    PROCESS_INFORMATION processinfo;
    ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
    BOOL bResult = CreateProcessW(sInternal.c_str(), pCommandLine,
                               NULL, NULL, TRUE, NULL, NULL, NULL, &sturtupinfo, &processinfo);

    if (bResult && ghJob)
    {
        AssignProcessToJobObject(ghJob, processinfo.hProcess);
    }

#endif

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

    NSFile::CFileBinary::Remove(sTempFileForParams);
#endif

#ifdef LINUX
    std::wstring sTempFileForParams = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"XML");
    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sTempFileForParams);
    oFile.WriteStringUTF8(oBuilder.GetData(), true);
    oFile.CloseFile();

    pid_t pid = fork(); // create child process
    int status;

    std::string sProgramm = U_TO_UTF8(sInternal);
    std::string sXmlA = "<html>" + U_TO_UTF8(sTempFileForParams);

    switch (pid)
    {
    case -1: // error
        break;

    case 0: // child process
    {
        std::string sLibraryDir = sProgramm;
        std::string::size_type posLast = sProgramm.find_last_of('/');
        std::string sProgrammDir = sProgramm.substr(0, posLast);
        if (std::string::npos != posLast)
            sLibraryDir = sProgrammDir + ":" + sProgrammDir + "/../:" + sProgrammDir + "/converter/";

        if (!IsLinuxXVFB())
        {
            sLibraryDir = "LD_LIBRARY_PATH=" + sLibraryDir;
            const char* nargs[2];
            nargs[0] = sXmlA.c_str();
            nargs[1] = NULL;

            const char* nenv[4];
            nenv[0] = sLibraryDir.c_str();
            nenv[1] = "DISPLAY=:0";
            nenv[2] = NULL;
			nenv[3] = NULL;

            execve(sProgramm.c_str(),
                   (char * const *)nargs,
                   (char * const *)nenv);
            exit(EXIT_SUCCESS);
        }
        else
        {
            const char* nargs[6];
            nargs[0] = "-a";
            nargs[1] = "--auto-servernum";
            nargs[2] = "--server-num=1";
            nargs[3] = sProgramm.c_str();
            nargs[4] = sXmlA.c_str();
            nargs[5] = NULL;

            /*
            const char* nenv[4];
            nenv[0] = sLibraryDir.c_str();
            nenv[1] = NULL;//"DISPLAY=:99";
            nenv[2] = NULL;
            */

            char** env = linux_environ_get(sLibraryDir);
            //linux_environ_print(env);
            execve("/usr/bin/xvfb-run", (char * const *)nargs, (char * const *)env);
            linux_environ_clear(env);
            exit(EXIT_SUCCESS);
        }

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

    for (std::vector<std::wstring>::iterator i = arTmpFiles.begin(); i != arTmpFiles.end(); i++)
    {
        NSFile::CFileBinary::Remove(*i);
    }
    arTmpFiles.clear();

    NSFile::CFileBinary::Remove(sTempFileForParams);
    return nReturnCode;
}

/////////////////////////////////////////////////////////////////
// EPUB
/////////////////////////////////////////////////////////////////

static std::vector<std::wstring> ParseEpub(const std::wstring& sPackagePath, std::wstring& sMetaInfo)
{
    std::vector<std::wstring> arHtmls;

    XmlUtils::CXmlNode oNodeRoot;
    if (!oNodeRoot.FromXmlFile(sPackagePath))
        return arHtmls;

    XmlUtils::CXmlNode oNodeMeta = oNodeRoot.ReadNodeNoNS(L"metadata");
    if (oNodeMeta.IsValid())
    {
        NSStringUtils::CStringBuilder oBuilder;

        std::wstring sTitle         = oNodeMeta.ReadValueString(L"dc:title");
        std::wstring sCreator       = oNodeMeta.ReadValueString(L"dc:creator");
        std::wstring sPublisher     = oNodeMeta.ReadValueString(L"dc:publisher");
        std::wstring sLanguage      = oNodeMeta.ReadValueString(L"dc:language");
        std::wstring sContributor   = oNodeMeta.ReadValueString(L"dc:contributor");
        std::wstring sDescription   = oNodeMeta.ReadValueString(L"dc:description");
        std::wstring sCoverage      = oNodeMeta.ReadValueString(L"dc:coverage");

        XmlUtils::CXmlNodes oMetaNodes = oNodeMeta.ReadNodesNoNS(L"meta");
        if (oMetaNodes.IsValid())
        {
            int nCountMeta = oMetaNodes.GetCount();
            for (int i = 0; i < nCountMeta; ++i)
            {
                XmlUtils::CXmlNode oNodeTmp;
                oMetaNodes.GetAt(i, oNodeTmp);

                std::wstring sName = oNodeTmp.GetAttribute(L"name");
                if (sName == L"cover")
                    sCoverage = L"1";
            }
        }

        if (!sTitle.empty())
        {
            oBuilder.WriteString(L"<name>");
            oBuilder.WriteEncodeXmlString(sTitle.c_str(), (int)sTitle.length());
            oBuilder.WriteString(L"</name>");
        }
        if (!sCreator.empty())
        {
            oBuilder.WriteString(L"<author>");
            oBuilder.WriteEncodeXmlString(sCreator.c_str(), (int)sCreator.length());
            oBuilder.WriteString(L"</author>");

            oBuilder.WriteString(L"<creator>");
            oBuilder.WriteEncodeXmlString(sCreator.c_str(), (int)sCreator.length());
            oBuilder.WriteString(L"</creator>");
        }
        if (!sPublisher.empty())
        {
            oBuilder.WriteString(L"<publisher>");
            oBuilder.WriteEncodeXmlString(sPublisher.c_str(), (int)sPublisher.length());
            oBuilder.WriteString(L"</publisher>");
        }
        if (!sLanguage.empty())
        {
            oBuilder.WriteString(L"<language>");
            oBuilder.WriteEncodeXmlString(sLanguage.c_str(), (int)sLanguage.length());
            oBuilder.WriteString(L"</language>");
        }
        if (!sContributor.empty())
        {
            oBuilder.WriteString(L"<creator>");
            oBuilder.WriteEncodeXmlString(sContributor.c_str(), (int)sContributor.length());
            oBuilder.WriteString(L"</creator>");
        }
        if (!sDescription.empty())
        {
            oBuilder.WriteString(L"<annotation>");
            oBuilder.WriteEncodeXmlString(sDescription.c_str(), (int)sDescription.length());
            oBuilder.WriteString(L"</annotation>");
        }
        if (!sCoverage.empty())
        {
            oBuilder.WriteString(L"<coverpage>1</coverpage>");
        }

        if (0 != oBuilder.GetCurSize())
            sMetaInfo = L"<meta>" + oBuilder.GetData() + L"</meta>";
    }

    XmlUtils::CXmlNode oNodeSpine = oNodeRoot.ReadNodeNoNS(L"spine");
    if (!oNodeRoot.IsValid())
        return arHtmls;

    XmlUtils::CXmlNodes oNodesItemRef = oNodeSpine.ReadNodesNoNS(L"itemref");
    if (!oNodeSpine.IsValid())
        return arHtmls;

    std::vector<std::wstring> sIds;
    int nCountRefs = oNodesItemRef.GetCount();
    for (int i = 0; i < nCountRefs; ++i)
    {
        XmlUtils::CXmlNode oNodeTmp;
        oNodesItemRef.GetAt(i, oNodeTmp);

        std::wstring sId = oNodeTmp.GetAttribute(L"idref");
        if (!sId.empty())
            sIds.push_back(sId);
    }

    if (0 == sIds.size())
        return arHtmls;

    XmlUtils::CXmlNode oNodeManifest = oNodeRoot.ReadNodeNoNS(L"manifest");
    if (!oNodeRoot.IsValid())
        return arHtmls;
    XmlUtils::CXmlNodes oNodesItems = oNodeManifest.ReadNodesNoNS(L"item");
    if (!oNodeManifest.IsValid())
        return arHtmls;

    size_t pos = sPackagePath.find_last_of((wchar_t)'/');
    std::wstring sPackagePathDir = sPackagePath;
    if (std::wstring::npos != pos)
        sPackagePathDir = sPackagePath.substr(0, pos + 1);

    std::map<std::wstring, std::wstring> mapHtmls;
    int nCountItems = oNodesItems.GetCount();
    for (int i = 0; i < nCountItems; ++i)
    {
        XmlUtils::CXmlNode oNodeTmp;
        oNodesItems.GetAt(i, oNodeTmp);

        std::wstring sMime = oNodeTmp.GetAttribute(L"media-type");
        std::wstring sHRef = oNodeTmp.GetAttribute(L"href");

#if 0
        //Decode URL
        sHRef.Replace(_T("%20"), _T(" "));
        sHRef.Replace(_T("%3B"), _T(";"));
        sHRef.Replace(_T("%2C"), _T(","));
        sHRef.Replace(_T("%26"), _T("&"));
        sHRef.Replace(_T("%3D"), _T("="));
        sHRef.Replace(_T("%2B"), _T("+"));
        sHRef.Replace(_T("%24"), _T("$"));
#endif

        std::wstring sId = oNodeTmp.GetAttribute(L"id");
        if (!sMime.empty() && !sHRef.empty())
            mapHtmls.insert(std::pair<std::wstring, std::wstring>(sId, sPackagePathDir + sHRef));
    }

    for (std::vector<std::wstring>::iterator iter = sIds.begin(); iter != sIds.end(); iter++)
    {
        std::map<std::wstring, std::wstring>::const_iterator i = mapHtmls.find(*iter);
        if (i != mapHtmls.end())
        {
            arHtmls.push_back(i->second);
        }
    }

    return arHtmls;
}

int CHtmlFile::ConvertEpub(const std::wstring& sFolder, std::wstring& sMetaInfo, const std::wstring& sDstfolder)
{
    std::wstring sFolderWithSlash = sFolder;
    NSStringExt::Replace(sFolderWithSlash, L"\\", L"/");

    if (!sFolderWithSlash.empty())
    {
        wchar_t c = sFolderWithSlash.c_str()[sFolderWithSlash.length() - 1];
        if (c != '/' && c != '\\')
            sFolderWithSlash += L"/";
    }

    std::wstring sMimeType = L"";
    if (!NSFile::CFileBinary::ReadAllTextUtf8(sFolderWithSlash + L"mimetype", sMimeType))
        return 1;

    std::wstring::size_type findMime = sMimeType.find(L"application/epub+zip");
    if (findMime == std::wstring::npos || findMime > 10) // 10 - просто число. Заглушка под мега епабы
        return 1;

    std::wstring sContainer = sFolderWithSlash + L"META-INF/container.xml";
    XmlUtils::CXmlNode oNodeContainer;
    if (!oNodeContainer.FromXmlFile(sContainer))
        return 1;

    XmlUtils::CXmlNode oNodeRootFiles = oNodeContainer.ReadNodeNoNS(L"rootfiles");
    if (!oNodeRootFiles.IsValid())
        return 1;

    std::wstring sPackagePathXml;

    XmlUtils::CXmlNodes oNodesRootFile = oNodeRootFiles.ReadNodesNoNS(L"rootfile");
    if (!oNodeRootFiles.IsValid())
        return 1;

    int nCount = oNodesRootFile.GetCount();
    for (int i = 0; i < nCount; ++i)
    {
        XmlUtils::CXmlNode oNodeRF;
        oNodesRootFile.GetAt(i, oNodeRF);

        std::wstring sMime = oNodeRF.GetAttribute(L"media-type");
        std::wstring sPackagePath = oNodeRF.GetAttribute(L"full-path");

        if (!sPackagePath.empty() && L"application/oebps-package+xml" == sMime)
            sPackagePathXml = sFolderWithSlash + sPackagePath;
    }

    if (sPackagePathXml.empty())
        return 1;

    std::vector<std::wstring> arHtmls = ParseEpub(sPackagePathXml, sMetaInfo);
    if (arHtmls.size() == 0)
        return 1;

    m_internal->m_bIsEpub = true;
    int nErr = this->Convert(arHtmls, sDstfolder);
    m_internal->m_bIsEpub = false;
    return nErr;
}

/////////////////////////////////////////////////////////////////
// MHT
/////////////////////////////////////////////////////////////////

#include <list>
#include <algorithm>
#include "../UnicodeConverter/UnicodeConverter.h"

namespace NSMht
{
    char easytolower(char in)
    {
        if (in<='Z' && in>='A')
            return in-('Z'-'z');
        return in;
    }
    wchar_t easytolower_w(wchar_t in)
    {
        if (in<='Z' && in>='A')
            return in-('Z'-'z');
        return in;
    }

    namespace Names
    {
        const std::string boundary_str                  = "boundary=";
        const std::string contentType_str               = "content-type:";
        const std::string contentTransferEncoding_str   = "content-transfer-encoding:";
        const std::string contentLocation_str           = "content-location:";
        const std::string contentCharset_str            = "charset=";
        const std::string contentID_str                 = "content-id:";

        const std::string htmlFileType                  = "text/html";
        const std::string xmlFileType                   = "text/xml";
        const std::string cssFileType                   = "text/css";
        const std::string imageFileType                 = "image/";
        const std::string jsFileType                    = "application/x-javascript";

        const std::string code_7bit                     = "7bit";
        const std::string code_8bit                     = "8bit";
        const std::string code_QuotedPrintable          = "quoted-printable";
        const std::string code_Base64                   = "base64";
    }

    void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }

    class CInnerFile
    {
    public:
        std::string     m_sContentType;
        std::wstring    m_sContentLocation;
        std::wstring    m_sContentID;

        std::string     m_sEncoding;
        std::string     m_sContentEncoding;

        std::string     m_sData;
        std::wstring    m_sDstFilePath;

    public:
        void Save(const std::map<std::wstring, std::wstring>& sMap, NSUnicodeConverter::CUnicodeConverter* pUnicodeConverter)
        {
            bool bIsCSS = (m_sContentType.find(Names::cssFileType) != std::string::npos) ? true : false;
            bool bIsHtml = (m_sContentType.find(Names::htmlFileType) != std::string::npos ||
                    m_sContentType.find(Names::xmlFileType) != std::string::npos ||
                    m_sContentType.find(Names::jsFileType) != std::string::npos) ? true : false;

            if (bIsCSS || bIsHtml)
            {
                if (m_sContentEncoding.find(Names::code_Base64) != std::string::npos)
                {
                    BYTE* pData = NULL;
                    int nLen = 0;
                    NSFile::CBase64Converter::Decode(m_sData.c_str(), m_sData.length(), pData, nLen);

                    m_sData = std::string((char*)pData, nLen);

                    RELEASEARRAYOBJECTS(pData);
                }

                std::string sEnc = m_sEncoding;
                if (sEnc.empty())
                {
                    if (m_sContentEncoding.find(Names::code_7bit) != std::string::npos)
                    {
                        sEnc = "US-ASCII";
                    }
                    else
                    {
                        sEnc = "latin1";
                    }
                }

                std::wstring sRes = pUnicodeConverter->toUnicode(m_sData, sEnc.c_str());

                // дальше конвертим обратно в нужную кодировку, меня пути
                for (std::map<std::wstring, std::wstring>::const_iterator i = sMap.begin(); i != sMap.end(); i++)
                {
                    std::list<std::wstring> listReplace;
                    listReplace.push_back(i->first);

                    // корень
                    if (true)
                    {
                        std::wstring::size_type pos = m_sContentLocation.find(L"//");
                        std::wstring::size_type start = 0;
                        if (pos != std::wstring::npos)
                            start = pos + 3; // '///'

                        pos = m_sContentLocation.find('/', start);
                        if (pos != std::wstring::npos)
                        {
                            std::wstring sMain = m_sContentLocation.substr(0, pos);

                            if (0 == i->first.find(sMain))
                            {
                                listReplace.push_back(i->first.substr(sMain.length()));
                            }
                        }
                    }

                    // и относительная
                    if (true)
                    {
                        std::wstring::size_type pos = m_sContentLocation.rfind('/');
                        if (pos != std::wstring::npos)
                        {
                            std::wstring sUrl = m_sContentLocation.substr(0, pos + 1);
                            if (0 == i->first.find(sUrl))
                            {
                                std::wstring sCandidate = i->first.substr(sUrl.length());
                                listReplace.push_back(sCandidate);
                                listReplace.push_back(L"./" + sCandidate);
                            }
                        }
                    }

                    if (bIsHtml)
                    {
                        std::wstring sReplace = L"\"" + i->second + L"\"";
                        for (std::list<std::wstring>::iterator i = listReplace.begin(); i != listReplace.end(); i++)
                        {
                            std::wstring sFind1 = L"\"" + *i + L"\"";
                            std::wstring sFind2 = L"'" + *i + L"'";

                            string_replace(sRes, sFind1, sReplace);
                            string_replace(sRes, sFind2, sReplace);
                        }
                    }
                    else
                    {
                        std::wstring sReplace = L"url(" + i->second + L")";
                        for (std::list<std::wstring>::iterator i = listReplace.begin(); i != listReplace.end(); i++)
                        {
                            std::wstring sFind1 = L"url(" + *i + L")";
                            std::wstring sFind2 = L"url('" + *i + L"')";
                            std::wstring sFind3 = L"url(\"" + *i + L"\")";
                            string_replace(sRes, sFind1, sReplace);
                            string_replace(sRes, sFind2, sReplace);
                            string_replace(sRes, sFind3, sReplace);
                        }
                    }
                }

                std::string sResA = pUnicodeConverter->fromUnicode(sRes, sEnc.c_str());
                NSFile::CFileBinary oDstFile;
                if (oDstFile.CreateFileW(m_sDstFilePath))
                {
                    oDstFile.WriteFile((BYTE*)sResA.c_str(), (DWORD)sResA.length());
                }
                oDstFile.CloseFile();
            }
            else
            {
                if (m_sContentEncoding.find(Names::code_Base64) != std::string::npos)
                {
                    BYTE* pData = NULL;
                    int nLen = 0;
                    NSFile::CBase64Converter::Decode(m_sData.c_str(), m_sData.length(), pData, nLen);

                    NSFile::CFileBinary oFile;
                    oFile.CreateFileW(m_sDstFilePath);
                    oFile.WriteFile(pData, nLen);
                    oFile.CloseFile();

                    RELEASEARRAYOBJECTS(pData);
                }
                else
                {
                    std::string sEnc = m_sEncoding;
                    if (sEnc.empty())
                    {
                        if (m_sContentEncoding.find(Names::code_7bit) != std::string::npos)
                        {
                            sEnc = "US-ASCII";
                        }
                        else
                        {
                            sEnc = "latin1";
                        }
                    }

                    std::wstring sRes = pUnicodeConverter->toUnicode(m_sData, sEnc.c_str());
                    NSFile::CFileBinary::SaveToFile(m_sDstFilePath, sRes, true);
                }
            }
        }

        void CorrectType()
        {
            if (m_sContentType == "application/octet-stream")
            {
                std::string::size_type pos1 = m_sData.find("<HTML");
                std::string::size_type pos2 = m_sData.find("<html");

                if (pos1 != std::string::npos && pos1 < 100)
                {
                    m_sContentType = Names::htmlFileType;
                    return;
                }
                else if (pos2 != std::string::npos && pos2 < 100)
                {
                    m_sContentType = Names::htmlFileType;
                    return;
                }

                std::wstring::size_type posExt = m_sContentLocation.rfind('.');
                if (posExt != std::wstring::npos)
                {
                    std::wstring sExt = m_sContentLocation.substr(posExt);
                    posExt = sExt.find('?');
                    if (std::wstring::npos != posExt)
                        sExt = sExt.substr(0, posExt);

                    if (sExt == L".js")
                        m_sContentType = Names::jsFileType;
                    else if (sExt == L".png")
                        m_sContentType = "image/png";
                    else if (sExt == L".jpg" || sExt == L".jpeg")
                        m_sContentType = "image/jpg";
                    else if (sExt == L".gif")
                        m_sContentType = "image/gif";
                    else if (sExt == L".css")
                        m_sContentType = Names::cssFileType;
                }
            }
        }
    };

    class CMhtFile
    {
    public:
        CInnerFile              m_oFile;
        std::list<CInnerFile>   m_arFiles;

        std::wstring                            m_sFolder;
        std::map<std::wstring, std::wstring>    m_sUrlMap;

        NSStringUtils::CStringBuilder           m_oBuilder; // temp builder
        NSUnicodeConverter::CUnicodeConverter   m_oUnicodeConverter;

        std::string             m_sEncoding;

    public:
        CMhtFile()
        {
            m_sFolder = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"MHT");

#if 0
            m_sFolder = L"D:\\test\\Document\\MHT";
#endif

            // под линуксом предыдущая функция создает файл!!!
            if (NSFile::CFileBinary::Exists(m_sFolder))
                NSFile::CFileBinary::Remove(m_sFolder);

            NSDirectory::CreateDirectory(m_sFolder);

            m_sEncoding = "latin1";
        }
        ~CMhtFile()
        {
            NSDirectory::DeleteDirectory(m_sFolder);
        }

        std::string ReadFile(const std::wstring& sFileSrc)
        {
            BYTE* pData = NULL;
            DWORD dwSize = 0;
            NSFile::CFileBinary::ReadAllBytes(sFileSrc, &pData, dwSize);

            DWORD nBomSize = 0;

            if (dwSize >= 4)
            {
                DWORD dwBOM = 0;
                dwBOM |= pData[0];
                dwBOM |= (pData[1] << 8);
                dwBOM |= (pData[2] << 16);
                dwBOM |= (pData[3] << 24);

                if (0x00BFBBEF == (dwBOM & 0x00FFFFFF))
                {
                    m_sEncoding = "UTF-8";
                    nBomSize = 3;
                }
                else if (0x0000FFFE == (dwBOM & 0x0000FFFF))
                {
                    m_sEncoding = "UTF-16BE";
                    nBomSize = 2;
                }
                else if (0x0000FEFF == (dwBOM & 0x0000FFFF))
                {
                    m_sEncoding = "UTF-16LE";
                    nBomSize = 2;
                }
            }

            return std::string((char*)(pData + nBomSize), (dwSize - nBomSize));
        }

        int	charFromHex ( const char& _char)
        {
            int p = 0;
            if (_char >= '0' && _char <= '9')
                p = _char - '0';
            else if (_char >= 'A' && _char <= 'F')
                p = _char - 'A' + 10;

            return p;
        }
        std::string decodingQuotedPrintable(const std::string& line)
        {
            int nLength = (int)line.length();
            if (0 == nLength)
                return "";

            const char* pSrcData = line.c_str();
            char* pDstData = new char[nLength + 1];

            int j = 0;
            for (int i = 0; i < nLength; i++)
            {
                if (pSrcData[i] != '=')
                {
                    pDstData[j++] = line[i];
                }
                else
                {
                    if ((i + 2) < nLength)
                        pDstData[j++] = 16 * charFromHex(pSrcData[i + 1]) + charFromHex(pSrcData[i + 2]);

                    i += 2;
                }
            }
            pDstData[j] = '\0';

            std::string result(pDstData);
            delete [] pDstData;

            return result;
        }

        void Convert()
        {
            // сначала делаем мап файлов
            int nNumber = 0;
            for (std::list<CInnerFile>::iterator i = m_arFiles.begin(); i != m_arFiles.end(); i++)
            {
                nNumber++;
                CInnerFile* pFile = i.operator ->();
                std::wstring sFileExt = L".png"; // L".bin" - обычно это картинки. Так и будем сохранять
                if (pFile->m_sContentType.find(Names::cssFileType) != std::string::npos)
                {
                    sFileExt = L".css";
                }
                else if (pFile->m_sContentType.find(Names::imageFileType) != std::string::npos)
                {
                    if (pFile->m_sContentType.find("png") != std::string::npos)
                        sFileExt = L".png";
                    else if (pFile->m_sContentType.find("gif") != std::string::npos)
                        sFileExt = L".gif";
                    else
                        sFileExt = L".jpg";
                }
                else if (pFile->m_sContentType.find("xml") != std::string::npos)
                {
                    sFileExt = L".xml";
                }
                else if (pFile->m_sContentType.find("html") != std::string::npos)
                {
                    sFileExt = L".html";
                }
                else if (pFile->m_sContentType.find("javascript") != std::string::npos)
                {
                    sFileExt = L".js";
                }
                std::wstring sUrl = L"/" + std::to_wstring(nNumber) + sFileExt;
                pFile->m_sDstFilePath = m_sFolder + sUrl;
                m_sUrlMap.insert(std::pair<std::wstring, std::wstring>(pFile->m_sContentLocation, L"." + sUrl));
            }

            for (std::list<CInnerFile>::iterator i = m_arFiles.begin(); i != m_arFiles.end(); i++)
            {
                i->Save(m_sUrlMap, &m_oUnicodeConverter);
            }

            m_oFile.m_sDstFilePath = m_sFolder + L"/index.html";
            m_oFile.Save(m_sUrlMap, &m_oUnicodeConverter);
        }

        inline std::string GetLower(const std::string& sSrc)
        {
            std::string sRet = sSrc;
            std::transform(sRet.begin(), sRet.end(), sRet.begin(), easytolower);
            return sRet;
        }
        inline std::wstring GetLower(const std::wstring& sSrc)
        {
            std::wstring sRet = sSrc;
            std::transform(sRet.begin(), sRet.end(), sRet.begin(), easytolower_w);
            return sRet;
        }

        std::string ParseFilePropertyA(const std::string& line, std::string::size_type pos)
        {
            std::string::size_type _first = pos;

            std::string::size_type _last = line.length();
            const char* pData = line.c_str();
            while ((pData[_first] == ' ' || pData[_first] == '\"') && _first < _last)
                ++_first;

            std::string::size_type _last1 = line.find(';', _first);
            std::string::size_type _last2 = line.find('\"', _first);
            if (_last1 != std::string::npos && _last > _last1)
                _last = _last1;
            if (_last2 != std::string::npos && _last > _last2)
                _last = _last2;

            return line.substr(_first, _last - _first);
        }
        std::wstring ParseFileProperty(const std::wstring& line, std::wstring::size_type pos)
        {
            std::wstring::size_type _first = pos;

            std::wstring::size_type _last = line.length();
            const wchar_t* pData = line.c_str();
            while ((pData[_first] == ' ' || pData[_first] == '\"') && _first < _last)
                ++_first;

            std::wstring::size_type _last1 = line.find(';', _first);
            std::wstring::size_type _last2 = line.find('\"', _first);
            if (_last1 != std::wstring::npos && _last > _last1)
                _last = _last1;
            if (_last2 != std::wstring::npos && _last > _last2)
                _last = _last2;

            return line.substr(_first, _last - _first);
        }

        bool CheckProperty(const std::string& sSrcLower, const std::string& sSrcNatural, const std::string& sProperty, std::string& sValue)
        {
            std::string::size_type posFindHeader = sSrcLower.find(sProperty);
            if (std::string::npos != posFindHeader)
            {
                sValue = this->ParseFilePropertyA(sSrcNatural, posFindHeader + sProperty.length());
                return true;
            }
            return false;
        }
        bool CheckPropertyW(const std::string& sSrcLower, const std::string& sSrcNatural, const std::string& sProperty, std::wstring& sValue)
        {
            std::string::size_type posFindHeader = sSrcLower.find(sProperty);
            if (std::string::npos != posFindHeader)
            {
                sValue = m_oUnicodeConverter.toUnicode(this->ParseFilePropertyA(sSrcNatural, posFindHeader + sProperty.length()), m_sEncoding.c_str());
                return true;
            }
            return false;
        }

        void Parse(const std::wstring& sFileSrc)
        {
            std::string sFileData = this->ReadFile(sFileSrc);
            if (sFileData.empty())
                return;

            std::list<std::string> content;
            char* pChars = (char*)sFileData.c_str();
            int nLenSrc = (int)sFileData.length();
            int nPrevNewLine = 0;

            NSStringUtils::CStringBuilderA oBuilderA;
            for (int i = 0; i < nLenSrc; ++i)
            {
                oBuilderA.ClearNoAttack();
                while (i < nLenSrc)
                {
                    if (pChars[i] == '\r')
                    {
                        content.push_back(oBuilderA.GetData());
                        ++i;
                        nPrevNewLine = i;
                        break;
                    }
                    // BAD symbols \x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19
                    if (pChars[i] >= 0x0A && pChars[i] <= 0x19)
                    {
                        ++i;
                        continue;
                    }
                    oBuilderA.AddCharSafe(pChars[i]);
                    ++i;
                }
            }
            content.push_back(oBuilderA.GetData());

            std::string boundary;
            std::wstring doc_location;

            //пробегаемся по строкам файла MHT
            for (std::list<std::string>::iterator i = content.begin(); i != content.end();)
            {
                // конвертируем строку с кодировкой файла
                std::string sLowerLine = GetLower(*i);

                //Ищем инициализацию boundary в шапке документа MHT(boundary - разделитель внутренних файлов) - обязательный параметр
                if (CheckProperty(sLowerLine, *i, Names::boundary_str, boundary))
                {
                    boundary = "--" + boundary;
                    i++;
                }
                //Ищем инициализацию contentLocation в шапке(наименование главного внутренний файла) - может отсутствовать
                else if (CheckPropertyW(sLowerLine, *i, Names::contentLocation_str, doc_location))
                {
                    i++;
                }
                //если встретили разделитель, то начинаем считывать новый внутренний файл
                else if (*i == boundary && !boundary.empty())
                {
                    CInnerFile oInnerFile;

                    //сначала считывается шапка внутреннего файла, которая отделяется от основного текста файлом как минимум одной пустой строкой
                    while ( i->length() != 0 )
                    {
                        sLowerLine = GetLower(*i);

                        // Проверяем, возможно разделитель поменялся с данного места
                        if (CheckProperty(sLowerLine, *i, Names::boundary_str, boundary))
                        {
                            boundary = "--" + boundary;
                        }
                        //тип файла (image/, text/html, text/css)
                        else if (CheckProperty(sLowerLine, sLowerLine, Names::contentType_str, oInnerFile.m_sContentType))
                        {
                            if (oInnerFile.m_sContentType.find(Names::htmlFileType) != std::string::npos)
                            {
                                if (sLowerLine.find(".gif") != std::string::npos)
                                    oInnerFile.m_sContentType = "image/gif";
                                else if (sLowerLine.find(".png") != std::string::npos)
                                    oInnerFile.m_sContentType = "image/png";
                                else if (sLowerLine.find(".jpg") != std::string::npos || sLowerLine.find(".jpeg") != std::string::npos)
                                    oInnerFile.m_sContentType = "image/jpg";
                            }
                        }
                        //наименование файла
                        else if (CheckPropertyW(sLowerLine, *i, Names::contentLocation_str, oInnerFile.m_sContentLocation)) {}
                        else if (CheckPropertyW(sLowerLine, *i, Names::contentID_str, oInnerFile.m_sContentID)) {}
                        //кодировка (base64, 8bit, quoted-printable)
                        else if (CheckProperty(sLowerLine, sLowerLine, Names::contentTransferEncoding_str, oInnerFile.m_sContentEncoding)) {}
                        else if (CheckProperty(sLowerLine, sLowerLine, Names::contentCharset_str, oInnerFile.m_sEncoding)) {}
                        i++;
                    }
                    while ( i->length() == 0)
                        i++;

                    oBuilderA.ClearNoAttack();
                    bool bIs16 = (oInnerFile.m_sContentEncoding.find(Names::code_QuotedPrintable) != std::string::npos) ? true : false;
                    bool bIsBase64 = (oInnerFile.m_sContentEncoding.find(Names::code_Base64) != std::string::npos) ? true : false;

                    while (i != content.end() && i->find(boundary) == std::string::npos)
                    {
                        if (bIs16)
                        {
                            oBuilderA.WriteString(decodingQuotedPrintable(*i++));
                        }
                        else
                        {
                            oBuilderA.WriteString(*i++);
                        }

                        if (!bIsBase64)
                            oBuilderA.AddCharSafe('\n');
                    }
                    oInnerFile.m_sData = oBuilderA.GetData();
                    oInnerFile.CorrectType();

                    if (m_oFile.m_sData.empty() && oInnerFile.m_sContentType.find(Names::htmlFileType) != std::wstring::npos)
                    {
                        m_oFile = oInnerFile;
                    }
                    else if (m_oFile.m_sData.empty() && oInnerFile.m_sContentType.find(Names::xmlFileType) != std::wstring::npos)
                    {
                        m_oFile = oInnerFile;
                    }
                    else
                    {
                        m_arFiles.push_back(oInnerFile);
                    }
                }
                else
                    i++;
            }

            //встречаются такие документы, где отсутсвует boundary
            if (boundary == "")
            {
                for(std::list<std::string>::iterator i = content.begin(); i != content.end();)
                {
                    CInnerFile oInnerFile;

                    //сначала считывается шапка внутреннего файла, которая отделяется от основного текста файлом как минимум одной пустой строкой
                    while ( i->length() != 0 )
                    {
                        std::string sLowerLine = GetLower(*i);

                        if (CheckProperty(sLowerLine, sLowerLine, Names::contentType_str, oInnerFile.m_sContentType))
                        {
                            if (oInnerFile.m_sContentType.find(Names::htmlFileType) != std::string::npos)
                            {
                                if (sLowerLine.find(".gif") != std::string::npos)
                                    oInnerFile.m_sContentType = "image/gif";
                                else if (sLowerLine.find(".png") != std::string::npos)
                                    oInnerFile.m_sContentType = "image/png";
                                else if (sLowerLine.find(".jpg") != std::string::npos || sLowerLine.find(".jpeg") != std::string::npos)
                                    oInnerFile.m_sContentType = "image/jpg";
                            }
                        }
                        //наименование файла
                        else if (CheckPropertyW(sLowerLine, *i, Names::contentLocation_str, oInnerFile.m_sContentLocation)) {}
                        else if (CheckPropertyW(sLowerLine, *i, Names::contentID_str, oInnerFile.m_sContentID)) {}
                        //кодировка (base64, 8bit, quoted-printable)
                        else if (CheckProperty(sLowerLine, sLowerLine, Names::contentTransferEncoding_str, oInnerFile.m_sContentEncoding)) {}
                        else if (CheckProperty(sLowerLine, sLowerLine, Names::contentCharset_str, oInnerFile.m_sEncoding)) {}
                        i++;
                    }

                    if (oInnerFile.m_sContentType.empty())
                        oInnerFile.m_sContentType = Names::htmlFileType;
                    if (oInnerFile.m_sContentEncoding.empty())
                        oInnerFile.m_sContentEncoding = Names::code_QuotedPrintable;

                    while ( i->length() == 0)
                        i++;

                    oBuilderA.ClearNoAttack();
                    bool bIs16 = (oInnerFile.m_sContentEncoding.find(Names::code_QuotedPrintable) != std::string::npos) ? true : false;
                    bool bIsBase64 = (oInnerFile.m_sContentEncoding.find(Names::code_Base64) != std::string::npos) ? true : false;

                    while (i != content.end() && i->find(boundary) == std::string::npos)
                    {
                        if (bIs16)
                        {
                            oBuilderA.WriteString(decodingQuotedPrintable(*i++));
                        }
                        else
                        {
                            oBuilderA.WriteString(*i++);
                        }

                        if (!bIsBase64)
                            oBuilderA.AddCharSafe('\n');
                    }
                    oInnerFile.m_sData = oBuilderA.GetData();
                    oInnerFile.CorrectType();

                    if (m_oFile.m_sData.empty() && oInnerFile.m_sContentType.find(Names::htmlFileType) != std::wstring::npos)
                    {
                        m_oFile = oInnerFile;
                    }
                    else if (m_oFile.m_sData.empty() && oInnerFile.m_sContentType.find(Names::xmlFileType) != std::wstring::npos)
                    {
                        m_oFile = oInnerFile;
                    }
                    else
                    {
                        m_arFiles.push_back(oInnerFile);
                    }
                }
            }
        }
    };
}

int CHtmlFile::ConvertMht(const std::wstring& sFile, const std::wstring& sDstfolder)
{
    NSMht::CMhtFile oFile;
    oFile.Parse(sFile);
    oFile.Convert();
    std::wstring sFileMht = oFile.m_sFolder + L"/index.html";

    std::vector<std::wstring> arFiles;
    arFiles.push_back(sFileMht);
    return this->Convert(arFiles, sDstfolder);
}
