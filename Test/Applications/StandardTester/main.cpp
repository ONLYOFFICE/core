#include <iostream>

#include "../../../Common/OfficeFileFormats.h"
#include "../../../Common/OfficeFileFormatChecker.h"

#include "../../../DesktopEditor/graphics/Timer.h"
#include "../../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../DesktopEditor/fontengine/application_generate_fonts.h"

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

class CConverter;
class CInternalWorker
{
public:
    std::map<int, bool>         m_formats;
    std::vector<std::wstring>   m_files;

    int m_nCount;
    int m_nCurrent;
    int m_nCurrentComplete;

    std::wstring m_sInputFolder;
    std::wstring m_sOutputFolder;

    bool m_bIsStandard;
    bool m_bIsDiffAllInOne;

    NSCriticalSection::CRITICAL_SECTION m_oCS;
    NSCriticalSection::CRITICAL_SECTION m_oCS_OfficeUtils;

public:
    CInternalWorker()
    {
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT, true));

        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP, true));

        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV, true));

        m_nCount = 0;
        m_nCurrent = 0;
        m_nCurrentComplete = 0;

        m_bIsDiffAllInOne = true;

        m_oCS.InitializeCriticalSection();
        m_oCS_OfficeUtils.InitializeCriticalSection();
    }
    ~CInternalWorker()
    {
        m_oCS.DeleteCriticalSection();
        m_oCS_OfficeUtils.DeleteCriticalSection();
    }

    CConverter* GetNextConverter();
    void OnConvertFile(CConverter* pConverter, int nCode);
    void Start(int nCores);
    void Cancel();

    void OpenDir(std::wstring sDir)
    {
        m_sInputFolder = sDir;
        std::vector<std::wstring> arFiles = NSDirectory::GetFiles(sDir, true);
        for (std::vector<std::wstring>::iterator iter = arFiles.begin(); iter != arFiles.end(); iter++)
        {
            std::wstring sExt = NSFile::GetFileExtention(*iter);

            if (sExt == L"docx" || sExt == L"doc" || sExt == L"odt" || sExt == L"rtf" ||
                sExt == L"pptx" || sExt == L"ppt" || sExt == L"odp" ||
                sExt == L"xlsx" || sExt == L"xls" || sExt == L"ods")
            {
                m_files.push_back(*iter);
            }
        }
        m_nCount = (int)m_files.size();
    }

    bool IsWork()
    {
        CTemporaryCS oCS(&m_oCS);
        return (m_nCurrentComplete < m_nCount);
    }
};

namespace NSX2T
{
    int Convert(const std::wstring& sConverterPath, const std::wstring sXmlPath)
    {
        int nReturnCode = 0;
        std::wstring sConverterExe = sConverterPath;

#ifdef WIN32
        NSCommon::string_replace(sConverterExe, L"/", L"\\");

        sConverterExe += L".exe";
        std::wstring sApp = L"x2t ";

        STARTUPINFO sturtupinfo;
        ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
        sturtupinfo.cb = sizeof(STARTUPINFO);

        sApp += (L"\"" + sXmlPath + L"\"");
        wchar_t* pCommandLine = NULL;
        if (true)
        {
            pCommandLine = new wchar_t[sApp.length() + 1];
            memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
            pCommandLine[sApp.length()] = (wchar_t)'\0';
        }

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
        BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
                                   NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

        if (bResult && ghJob)
        {
            AssignProcessToJobObject(ghJob, processinfo.hProcess);
        }

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

        if (ghJob)
        {
            CloseHandle(ghJob);
            ghJob = NULL;
        }

#endif

#ifdef LINUX
        pid_t pid = fork(); // create child process
        int status;

        std::string sProgramm = U_TO_UTF8(sConverterExe);
        std::string sXmlA = U_TO_UTF8(sXmlPath);

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

        return nReturnCode;
    }
}

class CConverter : public NSThreads::CBaseThread
{
public:
    CInternalWorker* m_pInternal;
    std::wstring m_file;
    std::wstring m_folder_dst;
    int m_format;

public:
    CConverter(CInternalWorker* pWorker) : NSThreads::CBaseThread()
    {
        m_pInternal = pWorker;
        m_format = -1;
    }
    virtual ~CConverter()
    {
        Stop();
    }

    virtual DWORD ThreadProc()
    {
        bool bIsOfficeFile = true;
        if (true)
        {
            CTemporaryCS oCS(&m_pInternal->m_oCS_OfficeUtils);

            COfficeFileFormatChecker oChecker;
            bIsOfficeFile = oChecker.isOfficeFile(m_file);
            if (bIsOfficeFile)
                m_format = oChecker.nFileType;
        }

        if (!bIsOfficeFile)
        {
            m_bRunThread = FALSE;
            m_pInternal->OnConvertFile(this, -1);
            return 0;
        }

#if 0
        std::map<int, bool>::iterator find = m_pInternal->m_formats.find(m_format);
        if ((find == m_pInternal->m_formats.end()) || (find->second == false))
        {
            m_bRunThread = FALSE;
            m_pInternal->OnConvertFile(this, -1);
            return 0;
        }
#endif

        std::wstring sProcess = NSFile::GetProcessDirectory();

        NSCommon::string_replace(sProcess, L"\\", L"/");
        NSCommon::string_replace(m_file, L"\\", L"/");

        std::wstring sDirectoryDst = m_folder_dst;
        NSCommon::string_replace(sDirectoryDst, L"\\", L"/");

#ifdef WIN32
        NSCommon::string_replace(m_file, L"//", L"\\\\");
#endif

        NSDirectory::CreateDirectory(sDirectoryDst);

        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");

        std::wstring sFileDst = sDirectoryDst + L"/page.zip";

        oBuilder.WriteEncodeXmlString(m_file);
        oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");

        oBuilder.WriteEncodeXmlString(sFileDst);
        oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>");
        oBuilder.WriteString(std::to_wstring(AVS_OFFICESTUDIO_FILE_IMAGE));
        oBuilder.WriteString(L"</m_nFormatTo><m_sThemeDir>./</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional><m_sAllFontsPath>");
        oBuilder.WriteString(sProcess + L"/fonts");
        oBuilder.WriteString(L"/AllFonts.js</m_sAllFontsPath><m_nCsvTxtEncoding>46</m_nCsvTxtEncoding><m_nCsvDelimiter>4</m_nCsvDelimiter>");

        oBuilder.WriteString(L"<m_sFontDir>");
        oBuilder.WriteEncodeXmlString(sProcess + L"/fonts");
        oBuilder.WriteString(L"</m_sFontDir>");

        oBuilder.WriteString(L"<m_oThumbnail><format>4</format><aspect>2</aspect><first>false</first><width>1000</width><height>1000</height></m_oThumbnail>");

        oBuilder.WriteString(L"</TaskQueueDataConvert>");

        std::wstring sXmlConvert = oBuilder.GetData();

        std::wstring sTempFileForParams = sDirectoryDst + L"/params.xml";
        NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

        if (NSDirectory::Exists(sProcess + L"/converter"))
            sProcess += L"/converter";

        std::wstring sExe = sProcess + L"/x2t";
        int nReturnCode = NSX2T::Convert(sExe, sTempFileForParams);

        NSFile::CFileBinary::Remove(sTempFileForParams);

        if (0 == nReturnCode)
        {
            CTemporaryCS oCS(&m_pInternal->m_oCS_OfficeUtils);

            COfficeUtils oUtils;
            if (S_OK == oUtils.ExtractToDirectory(sFileDst, sDirectoryDst, NULL, 0))
                NSFile::CFileBinary::Remove(sFileDst);
        }

        if (!m_pInternal->m_bIsStandard)
        {
            // смотрим разницу
            std::wstring strDirIn = NSFile::GetDirectoryName(m_file);
            std::wstring strDirOut = sDirectoryDst;

            std::wstring strDiffsMain = NSFile::GetDirectoryName(strDirOut) + L"/DIFF";
            std::wstring strDiffs = strDiffsMain + L"/" + NSFile::GetFileName(m_file);

            int nCountInPages = GetPagesCount(strDirIn);
            int nCountOutPages = GetPagesCount(strDirOut);

            if (nCountInPages != nCountOutPages)
            {
                if (!NSDirectory::Exists(strDiffsMain))
                    NSDirectory::CreateDirectory(strDiffsMain);
                if (!NSDirectory::Exists(strDiffs))
                    NSDirectory::CreateDirectory(strDiffs);

                if (nCountInPages > nCountOutPages)
                    nCountInPages = nCountOutPages;

                std::wstring sFilePagesDiff = strDiffs + L"/pages_count";
                NSFile::CFileBinary oFile;
                oFile.CreateFileW(sFilePagesDiff);
                oFile.CloseFile();

                std::cout << "file (page count) : " << U_TO_UTF8(strDiffs) << std::endl;
            }

            for (int nPage = 0; nPage < nCountInPages; ++nPage)
            {
                std::wstring sPageI = strDirIn + L"/image" + std::to_wstring(nPage + 1) + L".png";
                std::wstring sPageO = strDirOut + L"/image" + std::to_wstring(nPage + 1) + L".png";
                std::wstring sPageDiff = strDiffs + L"/image" + std::to_wstring(nPage + 1) + L".png";

                CBgraFrame frameI;
                frameI.OpenFile(sPageI);

                CBgraFrame frameO;
                frameO.OpenFile(sPageO);

                int nW_I = frameI.get_Width();
                int nH_I = frameI.get_Height();

                int nW_O = frameO.get_Width();
                int nH_O = frameO.get_Height();

                if (nW_I != nW_O || nH_I != nH_O)
                {
                    if (!NSDirectory::Exists(strDiffsMain))
                        NSDirectory::CreateDirectory(strDiffsMain);
                    if (!NSDirectory::Exists(strDiffs))
                        NSDirectory::CreateDirectory(strDiffs);

                    std::wstring sFilePagesDiff = sPageDiff;
                    NSFile::CFileBinary oFile;
                    oFile.CreateFileW(sPageDiff);
                    oFile.WriteStringUTF8(L"sizes!");
                    oFile.CloseFile();

                    std::cout << "file (sizes) : " << U_TO_UTF8(sPageDiff) << ", (" << nW_I << ", " << nH_I << "), (" << nW_O << ", " << nH_O << ")" << std::endl;
                    continue;
                }

                BYTE* pDataI = frameI.get_Data();
                BYTE* pDataO = frameO.get_Data();
                size_t sizeMemory = 4 * nW_I * nH_I;

                if (0 == memcmp(pDataI, pDataO, sizeMemory))
                    continue;

                sizeMemory = nW_I * nH_I;

                int nEpsilonEps = 3;
                int nEpsilonNatural = 5;

                int nDivExist = 0;
                for (int indexPixH = 0; indexPixH < nH_I; indexPixH++)
                {
                    for (int indexPixW = 0; indexPixW < nW_I; indexPixW++)
                    {
                        if (pDataI[0] != pDataO[0] || pDataI[1] != pDataO[1] || pDataI[2] != pDataO[2])
                        {
                            // test epsilon natural
                            if ((abs(pDataI[0] - pDataO[0]) < nEpsilonNatural) &&
                                (abs(pDataI[1] - pDataO[1]) < nEpsilonNatural) &&
                                (abs(pDataI[2] - pDataO[2]) < nEpsilonNatural))
                            {
                                pDataI += 4;
                                pDataO += 4;
                                continue;
                            }

                            // test epsilon left, right, top, bottom
                            int nEpsUp = nEpsilonEps;
                            if (indexPixH > 0)
                            {
                                BYTE* pByteI = frameI.get_Data() + 4 * (indexPixH - 1) * nW_I + 4 * indexPixW;

                                if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
                                    (abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
                                    (abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
                                {
                                    nEpsUp = nEpsilonEps - 1;
                                }
                            }

                            int nEpsDown = nEpsilonEps;
                            if (indexPixH < (nH_I - 1))
                            {
                                BYTE* pByteI = frameI.get_Data() + 4 * (indexPixH + 1) * nW_I + 4 * indexPixW;

                                if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
                                    (abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
                                    (abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
                                {
                                    nEpsDown = nEpsilonEps - 1;
                                }
                            }

                            int nEpsLeft = nEpsilonEps;
                            if (indexPixW > 0)
                            {
                                BYTE* pByteI = pDataI - 4;

                                if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
                                    (abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
                                    (abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
                                {
                                    nEpsLeft = nEpsilonEps - 1;
                                }
                            }

                            int nEpsRight = nEpsilonEps;
                            if (indexPixW < (nW_I - 1))
                            {
                                BYTE* pByteI = pDataI + 4;

                                if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
                                    (abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
                                    (abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
                                {
                                    nEpsRight = nEpsilonEps - 1;
                                }
                            }

                            if ((nEpsLeft < nEpsilonEps) ||
                                (nEpsRight < nEpsilonEps) ||
                                (nEpsUp < nEpsilonEps) ||
                                (nEpsDown < nEpsilonEps))
                            {
                                pDataI += 4;
                                pDataO += 4;
                                continue;
                            }

                            ++nDivExist;

                            if (pDataO[0] == 0x00 && pDataO[1] == 0x00 && pDataO[2] == 0xFF)
                            {
                                pDataO[0] = 0xFF;
                                pDataO[1] = 0x00;
                                pDataO[2] = 0x00;
                            }
                            else
                            {
                                pDataO[0] = 0x00;
                                pDataO[1] = 0x00;
                                pDataO[2] = 0xFF;
                            }
                        }
                        pDataI += 4;
                        pDataO += 4;
                    }
                }

                if (nDivExist > 7)
                {
                    if (!NSDirectory::Exists(strDiffsMain))
                        NSDirectory::CreateDirectory(strDiffsMain);
                    if (!NSDirectory::Exists(strDiffs))
                        NSDirectory::CreateDirectory(strDiffs);

                    if (!m_pInternal->m_bIsDiffAllInOne)
                    {
                        frameO.SaveFile(sPageDiff, 4);
                    }
                    else
                    {
                        CBgraFrame frameOSrc;
                        frameOSrc.OpenFile(sPageO);

                        BYTE* pData1 = frameI.get_Data();
                        BYTE* pData2 = frameOSrc.get_Data();
                        BYTE* pData3 = frameO.get_Data();

                        int nRowW = 4 * nW_I;
                        BYTE* pDataAll = new BYTE[3 * nRowW * nH_I];
                        BYTE* pDataAllSrc = pDataAll;
                        for (int j = 0; j < nH_I; j++)
                        {
                            memcpy(pDataAll, pData1, nRowW);
                            pDataAll += nRowW;
                            pData1 += nRowW;

                            memcpy(pDataAll, pData2, nRowW);
                            pDataAll += nRowW;
                            pData2 += nRowW;

                            memcpy(pDataAll, pData3, nRowW);
                            pDataAll += nRowW;
                            pData3 += nRowW;
                        }

                        CBgraFrame oFrameAll;
                        oFrameAll.put_Data(pDataAllSrc);
                        oFrameAll.put_Width(3 * nW_I);
                        oFrameAll.put_Height(nH_I);
                        oFrameAll.put_Stride(-3 * nRowW);
                        oFrameAll.SaveFile(sPageDiff, 4);
                    }

                    std::cout << "file (diffs) : " << U_TO_UTF8(sPageDiff) << std::endl;
                }
            }
        }

        m_bRunThread = FALSE;

        m_pInternal->OnConvertFile(this, nReturnCode);
        return 0;
    }

    int GetPagesCount(const std::wstring& dir)
    {
        int nCount = 0;
        std::vector<std::wstring> files = NSDirectory::GetFiles(dir, false);
        for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
        {
            std::wstring sExt = NSFile::GetFileExtention(*i);
            if (sExt == L"png")
                ++nCount;
        }
        return nCount;
    }
};

CConverter* CInternalWorker::GetNextConverter()
{
    if (m_nCurrent >= m_nCount)
        return NULL;

    CConverter* pConverter = new CConverter(this);
    pConverter->m_file = m_files[m_nCurrent];
    ++m_nCurrent;
    std::wstring sName = NSFile::GetFileName(pConverter->m_file);

    pConverter->m_folder_dst = m_sOutputFolder + L"/" + sName;
    NSDirectory::CreateDirectory(pConverter->m_folder_dst);

    if (m_bIsStandard)
        NSFile::CFileBinary::Copy(pConverter->m_file, pConverter->m_folder_dst + L"/" + sName);

    pConverter->Start(0);
    return pConverter;
}

void CInternalWorker::OnConvertFile(CConverter* pConverter, int nCode)
{
    CTemporaryCS oCS(&m_oCS);

    std::cout << "file (complete) : " << U_TO_UTF8(m_files[m_nCurrentComplete]) << ", code : " << nCode << std::endl;

    ++m_nCurrentComplete;

    RELEASEOBJECT(pConverter);
    GetNextConverter();
}

void CInternalWorker::Start(int nCores)
{
    CTemporaryCS oCS(&m_oCS);

    int nSizeInit = nCores;
    if (nSizeInit > m_nCount)
        nSizeInit = m_nCount;

    for (int i = 0; i < nSizeInit; ++i)
        GetNextConverter();
}

void CInternalWorker::Cancel()
{
    CTemporaryCS oCS(&m_oCS);
    m_nCount = m_nCurrent;
}

#define ONLYOFFICE_FONTS_VERSION_ 1
void CheckFonts(const bool& bIsUseSystemFonts, std::vector<std::wstring>& arDirs)
{
    std::vector<std::string> strFonts;
    std::wstring strDirectory = NSFile::GetProcessDirectory() + L"/fonts";

    if (!NSDirectory::Exists(strDirectory))
        NSDirectory::CreateDirectory(strDirectory);

    std::wstring strAllFontsJSPath = strDirectory + L"/AllFonts.js";
    std::wstring strThumbnailsFolder = strDirectory;
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

        if (0 != strFonts.size())
        {
            // check version!!!
            std::string sOO_Version = strFonts[0];
            if (0 != sOO_Version.find("ONLYOFFICE_FONTS_VERSION_"))
            {
                strFonts.clear();
            }
            else
            {
                std::string sVersion = sOO_Version.substr(25);
                int nVersion = std::stoi(sVersion);
                if (nVersion != ONLYOFFICE_FONTS_VERSION_)
                    strFonts.clear();
                else
                    strFonts.erase(strFonts.begin());
            }
        }
    }

    NSFonts::IApplicationFonts* oApplicationF = NSFonts::NSApplication::Create();
    std::vector<std::wstring> strFontsW_Cur;

    if (bIsUseSystemFonts)
        strFontsW_Cur = oApplicationF->GetSetupFontFiles();

    for (std::vector<std::wstring>::iterator i = arDirs.begin(); i != arDirs.end(); i++)
    {
        NSDirectory::GetFiles2(*i, strFontsW_Cur, true);
    }

    bool bIsEqual = true;
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

    if (bIsEqual)
    {
        if (!NSFile::CFileBinary::Exists(strFontsSelectionBin))
            bIsEqual = false;
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
        oFile.WriteStringUTF8(L"ONLYOFFICE_FONTS_VERSION_");
        oFile.WriteStringUTF8(std::to_wstring(ONLYOFFICE_FONTS_VERSION_));
        oFile.WriteFile((BYTE*)"\n", 1);
        int nCount = (int)strFontsW_Cur.size();
        for (int i = 0; i < nCount; ++i)
        {
            oFile.WriteStringUTF8(strFontsW_Cur[i]);
            oFile.WriteFile((BYTE*)"\n", 1);
        }
        oFile.CloseFile();

        int nFlag = 3;
        oApplicationF->InitializeFromArrayFiles(strFontsW_Cur, nFlag);

        NSCommon::SaveAllFontsJS(oApplicationF, strAllFontsJSPath, strThumbnailsFolder, strFontsSelectionBin);
    }

    oApplicationF->Release();
}

std::wstring CorrectDir(const std::wstring& sDir)
{
    if (sDir.empty())
        return L"";

    const wchar_t* data = sDir.c_str();

    std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::wstring::size_type pos2 = sDir.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    if (pos2 > 0 && ((data[pos2 - 1] == '\\') || (data[pos2 - 1] == '/')))
        --pos2;

    return sDir.substr(pos1, pos2 - pos1);
}

/*
 *
 * --input="input-standard-files-dir"
 * --output="output-dir"
 * --standard // generate standarts
 * --use-system-fonts="0/1/false/true"
 * --font-dirs="C:\\Windows\\Fonts;/usr/share/fonts;"
 * --cores=4
 *
 */

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    std::vector<std::wstring> arFontsDirs;
    bool bIsStandard = false;
    std::wstring strInputFolder = L"";
    std::wstring strOutputFolder = L"";
    bool bIsUseSystemFonts = true;
    int nCores = 1;

    for (int i = 0; i < argc; ++i)
    {
#ifdef WIN32
        std::wstring sParam(argv[i]);
#else
        std::string sParamA(argv[i]);
        std::wstring sParam = UTF8_TO_U(sParamA);
#endif

        if (sParam.find(L"--") == 0)
        {
            std::wstring sKey = L"";
            std::wstring sValue = L"";

            std::wstring::size_type _pos = sParam.find('=');
            if (std::wstring::npos == _pos)
            {
                sKey = sParam;
            }
            else
            {
                sKey = sParam.substr(0, _pos);
                sValue = sParam.substr(_pos + 1);
            }

            if (sKey == L"--input")
            {
                strInputFolder = CorrectDir(sValue);
            }
            else if (sKey == L"--output")
            {
                strOutputFolder = CorrectDir(sValue);
            }
            else if (sKey == L"--standard")
            {
                bIsStandard = true;
            }
            else if (sKey == L"--use-system-fonts")
            {
                if (sValue == L"0" || sValue == L"false")
                    bIsUseSystemFonts = false;
            }
            else if (sKey == L"--font-dirs")
            {
                const wchar_t* src = sValue.c_str();
                const wchar_t* limit = src + sValue.length();

                const wchar_t* srcPrev = src;
                while (src < limit)
                {
                    if (*src == ';')
                    {
                        if (srcPrev != src)
                        {
                            arFontsDirs.push_back(std::wstring(srcPrev, src - srcPrev));
                        }
                        src++;
                        srcPrev = src;
                    }
                    else
                        src++;
                }

                if (src > srcPrev)
                {
                    arFontsDirs.push_back(std::wstring(srcPrev, src - srcPrev));
                }
            }
            else if (sKey == L"--cores")
            {
                nCores = std::stoi(sValue);
                if (nCores < 1)
                    nCores = 1;
            }
        }
    }

    DWORD dwTime1 = NSTimers::GetTickCount();

    CheckFonts(bIsUseSystemFonts, arFontsDirs);

#if 0
    if (true)
    {
        strInputFolder = L"D:\\standard";
        strOutputFolder = L"D:\\standard\\out";
        bIsStandard = true;
    }
    else
    {
        strInputFolder = L"D:\\standard\\out";
        strOutputFolder = L"D:\\standard\\check";
        bIsStandard = false;
    }
#endif

    CInternalWorker oWorker;
    oWorker.OpenDir(strInputFolder);
    oWorker.m_sOutputFolder = strOutputFolder;
    oWorker.m_bIsStandard = bIsStandard;

    if (!NSDirectory::Exists(strOutputFolder))
        NSDirectory::CreateDirectories(strOutputFolder);

    oWorker.Start(nCores);
    while (oWorker.IsWork())
        NSThreads::Sleep(500);

    DWORD dwTime2 = NSTimers::GetTickCount();

    DWORD dwTimeDelta = (dwTime2 - dwTime1) / 1000;
    std::cout << "time: " << dwTimeDelta << std::endl;

    return 0;
}
