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
#include <iostream>

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/graphics/Timer.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../Common/OfficeFileFormats.h"
#include "../../DesktopEditor/doctrenderer/doctrenderer.h"
#include "../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

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
std::wstring CorrectValue(const std::wstring& value)
{
    if (value.empty())
        return L"";

    const wchar_t* data = value.c_str();

    std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::wstring::size_type pos2 = value.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    return value.substr(pos1, pos2 - pos1);
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
void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
{
    size_t posn = 0;
    while (std::string::npos != (posn = text.find(replaceFrom, posn)))
    {
        text.replace(posn, replaceFrom.length(), replaceTo);
        posn += replaceTo.length();
    }
}

namespace NSX2T
{
    int Convert(const std::wstring& sConverterPath, const std::wstring sXmlPath)
    {
        int nReturnCode = 0;
        std::wstring sConverterExe = sConverterPath;

#ifdef WIN32
        string_replace(sConverterExe, L"/", L"\\");

        sConverterExe += L".exe";
        std::wstring sApp = L"x2t ";

        STARTUPINFOW sturtupinfo;
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

void ParseStringAsInts(const std::string& s, std::vector<int>& arr)
{
    const char* data = s.c_str();
    int curOld = 0;
    int cur = 0;
    int valCur = 0;
    int len = (int)s.length();

    while (cur < len)
    {
        if (data[cur] == ',')
        {
            if (cur > curOld)
                arr.push_back(valCur);

            valCur = 0;
            curOld = cur + 1;
        }
        else
        {
            valCur *= 10;
            valCur += (data[cur] - '0');
        }
        ++cur;
    }

    if (cur > curOld)
        arr.push_back(valCur);
}

std::wstring GetThemePathByScale(const double& dScale)
{
    int nScaleOut = (int)(dScale * 100 + 0.5);

    if (nScaleOut == 100)
        return L".png";
    else if ((nScaleOut % 100) == 0)
        return L"@" + std::to_wstring((int)(nScaleOut / 100)) + L"x.png";
    else if ((nScaleOut % 10) == 0)
        return L"@" + std::to_wstring((int)(nScaleOut / 100)) + L"." + std::to_wstring((int)((nScaleOut / 10) % 10)) + L"x.png";

    return L"@" + std::to_wstring((int)(nScaleOut / 100)) + L"." + std::to_wstring((int)(nScaleOut % 100)) + L"x.png";
}

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    std::wstring sSrcThemesDir = L"";
    std::wstring sX2tPath = L"";
    std::wstring sOutputThumbnails = L"";
    std::wstring sAllFonts = L"";
    bool bIsNeedCorrectSdkAll = false;
    std::wstring sThemesParams = L"";
    std::wstring sPostfix = L"";

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

            if (sKey == L"--converter-dir")
            {
                sX2tPath = CorrectDir(sValue);
            }
            else if (sKey == L"--src")
            {
                sSrcThemesDir = CorrectDir(sValue);
            }
            else if (sKey == L"--output")
            {
                sOutputThumbnails = CorrectDir(sValue);
            }
            else if (sKey == L"--change-sdk")
            {
                sValue = CorrectValue(sValue);
                if (L"1" == sValue || L"true" == sValue)
                {
                    bIsNeedCorrectSdkAll = true;
                }
            }
            else if (sKey == L"--allfonts")
            {
                sValue = CorrectValue(sValue);
                sAllFonts = sValue;
            }
            else if (sKey == L"--params")
            {
                sThemesParams = sValue;
            }
            else if (sKey == L"--postfix")
            {
                sPostfix = sValue;
            }
        }
    }

    std::vector<int> arParams;
    if (!sThemesParams.empty())
    {
        std::string sThemesParamsA = U_TO_UTF8(sThemesParams);
        ParseStringAsInts(sThemesParamsA, arParams);

        if (2 == arParams.size())
        {
            double dKoef1 = arParams[0] / 88.0;
            double dKoef2 = arParams[1] / 40.0;

            sThemesParams += (L"," + std::to_wstring((int)(6 * dKoef1)));
            sThemesParams += (L"," + std::to_wstring((int)(3 * dKoef2)));
            sThemesParams += (L"," + std::to_wstring((int)(4 * dKoef1)));
            sThemesParams += (L"," + std::to_wstring((int)(31 * dKoef2)));
            sThemesParams += (L"," + std::to_wstring((int)(1 * dKoef1)));
            sThemesParams += (L"," + std::to_wstring((int)(8 * dKoef1)));
            sThemesParams += (L"," + std::to_wstring((int)(11 * dKoef2)));
            sThemesParams += (L"," + std::to_wstring((int)(18 * dKoef2)));
        }
    }
    int nParamsCount = (int)arParams.size();

    std::vector<std::wstring> arThemesTmp = NSDirectory::GetFiles(sSrcThemesDir, true);
    std::vector<std::wstring> arThemes;
    for (std::vector<std::wstring>::iterator iter = arThemesTmp.begin(); iter != arThemesTmp.end(); iter++)
    {
        if (NSFile::GetFileExtention(*iter) == L"pptx" ||
            NSFile::GetFileExtention(*iter) == L"potx" ||
            NSFile::GetFileExtention(*iter) == L"pptm" ||
            NSFile::GetFileExtention(*iter) == L"potm")
        {
            arThemes.push_back(*iter);
        }
    }

    std::sort(arThemes.begin(), arThemes.end());

    NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();
    pApplicationFonts->InitializeFromFolder(sX2tPath);

    NSDoctRenderer::CDocBuilder::Initialize();

    int nRasterW = 88;
    int nRasterH = 40;
    if (nParamsCount >= 2)
    {
        nRasterW = arParams[0];
        nRasterH = arParams[1];
    }

    NSStringUtils::CStringBuilder oBuilderJS;
    oBuilderJS.WriteString(L"[");

    #define COUNT_FONTS_SCALE 5
    double support_scales[COUNT_FONTS_SCALE] = { 1, 1.25, 1.5, 1.75, 2 };

    int nThemeIndex = 0;
    for (std::vector<std::wstring>::iterator iter = arThemes.begin(); iter != arThemes.end(); iter++)
    {
        ++nThemeIndex;
        std::wstring sOut = sSrcThemesDir + L"/theme" + std::to_wstring(nThemeIndex);

        std::wstring sInput = *iter;
        NSStringUtils::CStringBuilder oBuilder;

        if (sPostfix.empty())
        {
            if (NSDirectory::Exists(sOut))
                NSDirectory::DeleteDirectory(sOut);

            NSDirectory::CreateDirectory(sOut);

            oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
            oBuilder.WriteEncodeXmlString(sInput);
            oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");
            oBuilder.WriteEncodeXmlString(sOut + L"/theme.bin");
            oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>8192</m_nFormatTo><m_sThemeDir>./</m_sThemeDir>");
            oBuilder.WriteString(L"<m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
            if (!sAllFonts.empty())
            {
                oBuilder.WriteString(L"<m_sAllFontsPath>");
                oBuilder.WriteString(sAllFonts);
                oBuilder.WriteString(L"</m_sAllFontsPath>");
            }
            oBuilder.WriteString(L"</TaskQueueDataConvert>");

            std::wstring sXmlConvert = oBuilder.GetData();

            std::wstring sTempFileForParams = sOut + L"/params.xml";
            NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

            int nReturnCode = NSX2T::Convert(sX2tPath + L"/x2t", sTempFileForParams);
            NSFile::CFileBinary::Remove(sTempFileForParams);

            if (0 != nReturnCode)
            {
                std::cout << "could not use " << U_TO_UTF8(sInput) << std::endl;
                --nThemeIndex;
                NSDirectory::DeleteDirectory(sOut);
                continue;
            }
        }
        else if (!NSDirectory::Exists(sOut))
        {
            --nThemeIndex;
            continue;
        }

        oBuilder.ClearNoAttack();
        oBuilder.WriteString(L"<Settings><SrcFileType>");
        oBuilder.AddInt(NSDoctRenderer::DoctRendererFormat::PPTT);
        oBuilder.WriteString(L"</SrcFileType><DstFileType>");
        oBuilder.AddInt(NSDoctRenderer::DoctRendererFormat::PPTX_THEME_THUMBNAIL);
        oBuilder.WriteString(L"</DstFileType><SrcFilePath>");
        oBuilder.WriteEncodeXmlString(sOut + L"/theme.bin");
        oBuilder.WriteString(L"</SrcFilePath><DstFilePath>");
        oBuilder.WriteEncodeXmlString(sOut);
        oBuilder.WriteString(L"</DstFilePath><FontsDirectory>");
        oBuilder.WriteEncodeXmlString(sX2tPath);
        oBuilder.WriteString(L"</FontsDirectory><ImagesDirectory>");
        oBuilder.WriteEncodeXmlString(sOut + L"/media");
        oBuilder.WriteString(L"</ImagesDirectory><ThemesDirectory>");
        oBuilder.WriteEncodeXmlString(sOut);
        oBuilder.WriteString(L"</ThemesDirectory>");

        if (!sThemesParams.empty())
        {
            oBuilder.WriteString(L"<ThemesThumbnailsParams>");
            oBuilder.WriteEncodeXmlString(sThemesParams);
            oBuilder.WriteString(L"</ThemesThumbnailsParams>");
        }
#if 0
            oBuilder.WriteString(_T("<DoctParams>"));
            oBuilder.AddInt(0);
            oBuilder.WriteString(_T("</DoctParams>"));
#endif
        oBuilder.WriteString(L"</Settings>");
        std::wstring sXmlDoctRenderer = oBuilder.GetData();

        NSDoctRenderer::CDoctrenderer oRenderer;
        oRenderer.LoadConfig(sX2tPath, sAllFonts);

        std::wstring sError;
        bool bIsSuccess = oRenderer.Execute(sXmlDoctRenderer, sError);

        if (!bIsSuccess || !sError.empty())
        {
            std::cout << "could not use " << U_TO_UTF8(sInput) << ": " << U_TO_UTF8(sError) << std::endl;
            --nThemeIndex;
            NSDirectory::DeleteDirectory(sOut);
            continue;
        }

        std::vector<std::wstring> arOutFiles = NSDirectory::GetFiles(sOut, false);
        std::wstring sThemeFile = L"";
        for (std::vector<std::wstring>::iterator iter = arOutFiles.begin(); iter != arOutFiles.end(); iter++)
        {
            std::wstring sTmpFile = *iter;
            if (L"theme" == NSFile::GetFileExtention(sTmpFile))
            {
                sThemeFile = sTmpFile;
                break;
            }
        }

        BYTE* pData = NULL;
        DWORD nBytesCount = 0;
        if (NSFile::CFileBinary::ReadAllBytes(sThemeFile, &pData, nBytesCount))
        {
            NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);
			imageWriter.SetMediaDirectory(sOut);
			imageWriter.SetThemesDirectory(L"");
			imageWriter.SetTempDirectory(sOut);
            imageWriter.SetApplication(pApplicationFonts);
            imageWriter.SetRasterFormat(4);
            imageWriter.SetSaveType(0);
            imageWriter.SetIsOnlyFirst(true);

            if (sPostfix.empty())
            {
                for (int nScale = 0; nScale < COUNT_FONTS_SCALE; nScale++)
                {
                    double dScale = support_scales[nScale];
                    imageWriter.SetRasterW((int)(nRasterW * dScale));
                    imageWriter.SetRasterH((int)(nRasterH * dScale));
                    imageWriter.SetFileName(sOut + L"/thumbnail" + GetThemePathByScale(dScale));
                    imageWriter.ConvertBuffer(pData, nBytesCount);
                }
            }
            else
            {
                imageWriter.SetRasterW(nRasterW);
                imageWriter.SetRasterH(nRasterH);
                imageWriter.SetFileName(sOut + L"/thumbnail_" + sPostfix + L".png");
                imageWriter.ConvertBuffer(pData, nBytesCount);
            }

            RELEASEARRAYOBJECTS(pData);
        }

        oBuilderJS.WriteString(L"\"");
        std::wstring sThemeName = NSFile::GetFileName(sThemeFile);
        oBuilderJS.WriteString(sThemeName.substr(0, sThemeName.length() - 6)); // '.theme'
        oBuilderJS.WriteString(L"\",");

        NSFile::CFileBinary::Remove(sThemeFile);
    }

    if (nThemeIndex > 0)
    {
        // remove ','
        oBuilderJS.Skip(-1);
        oBuilderJS.WriteString(L"]");
    }

    if (sPostfix.empty())
    {
        NSFile::CFileBinary::SaveToFile(sSrcThemesDir + L"/themes.js", L"AscCommon.g_defaultThemes = " + oBuilderJS.GetData() + L";");

        // теперь нужно пропатчить sdk-all.js
        std::wstring sPathDoctRendererConfig = sX2tPath + L"/DoctRenderer.config";
        XmlUtils::CXmlNode oNode;
        if (bIsNeedCorrectSdkAll && oNode.FromXmlFile(sPathDoctRendererConfig))
        {
            XmlUtils::CXmlNodes oNodesFile = oNode.GetNode(L"PpttSdk").GetNodes(L"file");
            for (int i = 0; i < oNodesFile.GetCount(); ++i)
            {
                XmlUtils::CXmlNode oNodeFile;
                oNodesFile.GetAt(i, oNodeFile);

                std::wstring sFileSdk = oNodeFile.GetText();

                if (!NSFile::CFileBinary::Exists(sFileSdk) || NSFile::CFileBinary::Exists(sX2tPath + L"/" + sFileSdk))
                    sFileSdk = sX2tPath + L"/" + sFileSdk;

                std::wstring sSdkContent;
                if (NSFile::CFileBinary::ReadAllTextUtf8(sFileSdk, sSdkContent))
                {
                    std::wstring sStart = L"(function(){AscCommon.g_defaultThemes=";
                    std::wstring sEnd = L";})();";

                    std::wstring sNewContent = L"";

                    std::wstring::size_type posStart = sSdkContent.find(sStart);
                    if (posStart != std::wstring::npos)
                    {
                        std::wstring::size_type posEnd = sSdkContent.find(sEnd, posStart);

                        sNewContent = sSdkContent.substr(0, posStart);
                        sNewContent += (sStart + oBuilderJS.GetData() + sEnd);
                        sNewContent += sSdkContent.substr(posEnd + sEnd.length());
                    }
                    else
                    {
                        sNewContent = sSdkContent + L"\n" + sStart + oBuilderJS.GetData() + sEnd;

                        NSFile::CFileBinary::Remove(sFileSdk);
                        NSFile::CFileBinary::SaveToFile(sFileSdk, sSdkContent);
                    }

                    NSFile::CFileBinary::Remove(sFileSdk);
                    NSFile::CFileBinary::SaveToFile(sFileSdk, sNewContent);
                }
            }
        }

        if (nThemeIndex > 0)
        {
            int nRasterW1 = nRasterW;
            int nRasterH1 = nRasterH;

            for (int nScale = 0; nScale < COUNT_FONTS_SCALE; nScale++)
            {
                double dScale = support_scales[nScale];

                nRasterW = (int)(dScale * nRasterW1);
                nRasterH = (int)(dScale * nRasterH1);

                int nRow = 4 * nRasterW;
                int nSize = nRow * nRasterH;
                BYTE* pData = new BYTE[nSize * nThemeIndex];
                BYTE* pDataCur = pData;

                std::wstring sCurrentPath = L"thumbnail" + GetThemePathByScale(dScale);
                for (int nIndex = 1; nIndex <= nThemeIndex; ++nIndex)
                {
                    CBgraFrame oFrame;
                    oFrame.OpenFile(sSrcThemesDir + L"/theme" + std::to_wstring(nIndex) + L"/" + sCurrentPath);

                    if (false)
                    {
                        // flip
                        memcpy(pDataCur, oFrame.get_Data(), nSize);
                        pDataCur += nSize;
                    }
                    else
                    {
                        BYTE* pTmp = oFrame.get_Data() + nRow * (nRasterH - 1);
                        for (int nH = 0; nH < nRasterH; ++nH)
                        {
                            memcpy(pDataCur, pTmp, nRow);
                            pDataCur += nRow;
                            pTmp -= nRow;
                        }
                    }                    
                }

                CBgraFrame oFrame;
                oFrame.put_Data(pData);
                oFrame.put_Width(nRasterW);
                oFrame.put_Height(nRasterH * nThemeIndex);
                oFrame.put_Stride(nRow);

                oFrame.SaveFile(sOutputThumbnails + L"/themes_" + sCurrentPath, 4);
            }
        }
    }

    NSDoctRenderer::CDocBuilder::Dispose();

    pApplicationFonts->Release();

    if (arThemes.size() == 0)
    {
        std::cout << "no themes found" << std::endl;
        return 0;
    }

    return 0;
}
