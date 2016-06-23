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
#ifndef APPLICATION_GENERATE_FONTS_H
#define APPLICATION_GENERATE_FONTS_H

#include <map>
#include <set>

#include "./ApplicationFonts.h"
#include "../common/File.h"
#include "../common/Directory.h"
#include "../common/StringBuilder.h"

#ifndef ASC_APPLICATION_FONTS_NO_THUMBNAILS
#include "../graphics/GraphicsRenderer.h"
#endif

namespace NSCommon
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
    static void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
    {
        size_t posn = 0;
        while (std::string::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }

    static void url_correct(std::wstring& url)
    {
        string_replace(url, L"/./", L"/");

        size_t posn = 0;
        while (std::wstring::npos != (posn = url.find(L"/../")))
        {
            std::wstring::size_type pos2 = url.rfind(L"/", posn - 1);

            if (std::wstring::npos != pos2)
            {
                url.erase(pos2, posn - pos2 + 3);
            }
        }
        
        // MAC
        if (0 == url.find(L"file:/") && 0 != url.find(L"file://"))
        {
            url.replace(0, 6, L"file:///");
        }
    }

    static void makeUpper(std::string& url)
    {
        int nLen = (int)url.length();
        char* pStr = (char*)url.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] >= 'a' && pStr[i] <= 'z')
                pStr[i] = pStr[i] + 'A' - 'a';
        }
    }
    
    static void WriteLog(const std::string& sLogFile, const std::wstring& sData)
    {
        FILE* f = fopen(sLogFile.c_str(), "a+");
        std::string sDataA = U_TO_UTF8(sData);
        fprintf(f, sDataA.c_str());
        fprintf(f, "\n");
        fclose(f);
    }
}

///////////////////////////////////////////////////////
//#define _GENERATE_FONT_MAP_

#ifdef _GENERATE_FONT_MAP_
#include "../../freetype_names/FontMaps/FontDictionary.h"
#endif

namespace NSCommon
{
    static std::wstring GetFileExtention(const std::wstring& sPath)
    {
        std::wstring::size_type nPos = sPath.rfind('.');
        if (nPos != std::wstring::npos)
            return sPath.substr(nPos + 1);
        return sPath;
    }
    static std::wstring GetFileName(const std::wstring& sPath)
    {
        std::wstring::size_type nPos1 = sPath.rfind('\\');
        std::wstring::size_type nPos2 = sPath.rfind('/');
        std::wstring::size_type nPos = std::wstring::npos;

        if (nPos1 != std::wstring::npos)
        {
            nPos = nPos1;
            if (nPos2 != std::wstring::npos && nPos2 > nPos)
                nPos = nPos2;
        }
        else
            nPos = nPos2;

        if (nPos == std::wstring::npos)
            return sPath;
        return sPath.substr(nPos + 1);
    }
    static std::wstring GetDirectoryName(const std::wstring& sPath)
    {
        std::wstring::size_type nPos1 = sPath.rfind('\\');
        std::wstring::size_type nPos2 = sPath.rfind('/');
        std::wstring::size_type nPos = std::wstring::npos;

        if (nPos1 != std::wstring::npos)
        {
            nPos = nPos1;
            if (nPos2 != std::wstring::npos && nPos2 > nPos)
                nPos = nPos2;
        }
        else
            nPos = nPos2;

        if (nPos == std::wstring::npos)
            return sPath;
        return sPath.substr(0, nPos);
    }
}

namespace NSCommon
{
    class CFontInfoJS
    {
    public:
        std::wstring m_sName;
        
        int	m_lIndexR;
        int	m_lFaceIndexR;
        std::wstring m_sStyleR;

        int	m_lIndexI;
        int	m_lFaceIndexI;
        std::wstring m_sStyleI;

        int	m_lIndexB;
        int	m_lFaceIndexB;
        std::wstring m_sStyleB;

        int	m_lIndexBI;
        int	m_lFaceIndexBI;
        std::wstring m_sStyleBI;
        
        CFontInfoJS()
        {
            m_sName			= L"";
            
            m_lIndexR		= -1;
            m_lFaceIndexR	= -1;
            m_lIndexI		= -1;
            m_lFaceIndexI	= -1;
            m_lIndexB		= -1;
            m_lFaceIndexB	= -1;
            m_lIndexBI		= -1;
            m_lFaceIndexBI	= -1;

            m_sStyleR       = L"";
            m_sStyleI       = L"";
            m_sStyleB       = L"";
            m_sStyleBI      = L"";
        }
        
        CFontInfoJS(const CFontInfoJS& oSrc)
        {
            *this = oSrc;
        }
        
        CFontInfoJS& operator=(const CFontInfoJS& oSrc)
        {
            m_sName			= oSrc.m_sName;
            
            m_lIndexR	= oSrc.m_lIndexR;
            m_lIndexI	= oSrc.m_lIndexI;
            m_lIndexB	= oSrc.m_lIndexB;
            m_lIndexBI	= oSrc.m_lIndexBI;
            
            m_lFaceIndexR	= oSrc.m_lFaceIndexR;
            m_lFaceIndexI	= oSrc.m_lFaceIndexI;
            m_lFaceIndexB	= oSrc.m_lFaceIndexB;
            m_lFaceIndexBI	= oSrc.m_lFaceIndexBI;

            m_sStyleR   = oSrc.m_sStyleR;
            m_sStyleI   = oSrc.m_sStyleI;
            m_sStyleB   = oSrc.m_sStyleB;
            m_sStyleBI  = oSrc.m_sStyleBI;
            
            return *this;
        }

        bool IsNeedUpdateR()
        {
            if (m_sStyleR == L"Regular" ||
                m_sStyleR == L"Roman")
                return false;
            return true;
        }
        bool IsNeedUpdateI()
        {
            if (m_sStyleI == L"Italic" ||
                m_sStyleI == L"Oblique")
                return false;
            return true;
        }
        bool IsNeedUpdateB()
        {
            if (m_sStyleB == L"Bold")
                return false;
            return true;
        }
        bool IsNeedUpdateBI()
        {
            if (m_sStyleBI == L"Bold Italic" ||
                m_sStyleBI == L"Bold Oblique" ||
                m_sStyleBI == L"BoldItalic" ||
                m_sStyleBI == L"BoldOblique")
                return false;
            return true;
        }
    };
    
    static void SaveAllFontsJS(CApplicationFonts& applicationFonts, std::wstring strFile, std::wstring strFolderThumbnails, std::wstring strFontSelectionBin)
    {
        CArray<CFontInfo*>* pList = applicationFonts.GetList()->GetFonts();
        int nCount = pList->GetCount();
        
        // сначала строим массив всех файлов шрифтов
        std::map<std::wstring, LONG> mapFontFiles;
        std::map<LONG, std::wstring> mapFontFiles2;
        LONG lFontFiles = 0;
        for (int i = 0; i < nCount; ++i)
        {
            CFontInfo* pInfo = pList->operator [](i);
            
            if (mapFontFiles.find(pInfo->m_wsFontPath) == mapFontFiles.end())
            {
                mapFontFiles.insert(std::pair<std::wstring, LONG>(pInfo->m_wsFontPath, lFontFiles));
                mapFontFiles2.insert(std::pair<LONG, std::wstring>(lFontFiles, pInfo->m_wsFontPath));
                ++lFontFiles;
            }
        }
        // -----------------------------------------
        
        // теперь строим массив всех шрифтов по имени
        std::map<std::wstring, CFontInfoJS> mapFonts;
        CArray<std::wstring> arrFonts;
        
        for (int i = 0; i < nCount; ++i)
        {
            CFontInfo* pInfo = (CFontInfo*)pList->operator [](i);
            std::wstring strPath = pInfo->m_wsFontPath;
            std::wstring strName = pInfo->m_wsFontName;
            
            int lFontIndex = 0;
            int lFaceIndex = 0;
            
            std::map<std::wstring, LONG>::iterator it = mapFontFiles.find(strPath);
            lFontIndex = it->second;
            
            if (pInfo->m_lIndex >= 0)
                lFaceIndex = (int)pInfo->m_lIndex;
            
            std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(pInfo->m_wsFontName);
            if (mapFonts.end() != pPair)
            {
                pPair->second.m_sName = pInfo->m_wsFontName;
                
                if (pInfo->m_bBold && pInfo->m_bItalic)
                {
                    if (pPair->second.IsNeedUpdateBI())
                    {
                        pPair->second.m_lIndexBI = lFontIndex;
                        pPair->second.m_lFaceIndexBI = lFaceIndex;
                        pPair->second.m_sStyleBI = pInfo->m_wsStyle;
                    }
                }
                else if (pInfo->m_bBold)
                {
                    if (pPair->second.IsNeedUpdateB())
                    {
                        pPair->second.m_lIndexB = lFontIndex;
                        pPair->second.m_lFaceIndexB = lFaceIndex;
                        pPair->second.m_sStyleB = pInfo->m_wsStyle;
                    }
                }
                else if (pInfo->m_bItalic)
                {
                    if (pPair->second.IsNeedUpdateI())
                    {
                        pPair->second.m_lIndexI = lFontIndex;
                        pPair->second.m_lFaceIndexI = lFaceIndex;
                        pPair->second.m_sStyleI = pInfo->m_wsStyle;
                    }
                }
                else
                {
                    if (pPair->second.IsNeedUpdateR())
                    {
                        pPair->second.m_lIndexR = lFontIndex;
                        pPair->second.m_lFaceIndexR = lFaceIndex;
                        pPair->second.m_sStyleR = pInfo->m_wsStyle;
                    }
                }
            }
            else
            {
                CFontInfoJS fontInfo;
                
                fontInfo.m_sName = pInfo->m_wsFontName;
                
                if (pInfo->m_bBold && pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexBI = lFontIndex;
                    fontInfo.m_lFaceIndexBI = lFaceIndex;
                    fontInfo.m_sStyleBI = pInfo->m_wsStyle;
                }
                else if (pInfo->m_bBold)
                {
                    fontInfo.m_lIndexB = lFontIndex;
                    fontInfo.m_lFaceIndexB = lFaceIndex;
                    fontInfo.m_sStyleB = pInfo->m_wsStyle;
                }
                else if (pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexI = lFontIndex;
                    fontInfo.m_lFaceIndexI = lFaceIndex;
                    fontInfo.m_sStyleI = pInfo->m_wsStyle;
                }
                else
                {
                    fontInfo.m_lIndexR = lFontIndex;
                    fontInfo.m_lFaceIndexR = lFaceIndex;
                    fontInfo.m_sStyleR = pInfo->m_wsStyle;
                }
                
                mapFonts.insert(std::pair<std::wstring, CFontInfoJS>(fontInfo.m_sName, fontInfo));
                arrFonts.Add(fontInfo.m_sName);
            }
        }
        // -------------------------------------------
        
        // теперь сортируем шрифты по имени ----------
        int nCountFonts = arrFonts.GetCount();
        for (int i = 0; i < nCountFonts; ++i)
        {
            for (int j = i + 1; j < nCountFonts; ++j)
            {
                if (arrFonts[i] > arrFonts[j])
                {
                    std::wstring temp = arrFonts[i];
                    arrFonts[i] = arrFonts[j];
                    arrFonts[j] = temp;
                }
            }
        }
        // -------------------------------------------
#ifndef ASC_APPLICATION_FONTS_NO_THUMBNAILS
        if (L"" != strFolderThumbnails)
        {
            CFontManager* pManager = applicationFonts.GenerateFontManager();
            CFontsCache* pCache = new CFontsCache();
            pCache->SetStreams(applicationFonts.GetStreams());
            pManager->SetOwnerCache(pCache);
            
            for (int iX = 1; iX <= 2; ++iX)
            {
                // создаем картинку для табнейлов
                double dDpi = 96 * iX;
                double dW_mm = 80;
                LONG lH1_px = LONG(7 * dDpi / 25.4);
                LONG lWidthPix = (LONG)(dW_mm * dDpi / 25.4);
                LONG lHeightPix = (LONG)(nCountFonts * lH1_px);
                
                LONG lCountPixels = 4 * lWidthPix * lHeightPix;
                BYTE* pImageData = new BYTE[lCountPixels];
                memset(pImageData, 0xFF, lCountPixels);
                
                CBgraFrame oFrame;
                oFrame.put_Data(pImageData);
                oFrame.put_Width((int)lWidthPix);
                oFrame.put_Height((int)lHeightPix);
                oFrame.put_Stride(4 * (int)lWidthPix);
                
                for (LONG i = 3; i < lWidthPix * lHeightPix * 4; i += 4)
                {
                    pImageData[i] = 0;
                }
                
                CGraphicsRenderer oRenderer;
                oRenderer.CreateFromBgraFrame(&oFrame);
                
                oRenderer.SetFontManager(pManager);
                
                oRenderer.put_Width(lWidthPix * 25.4 / dDpi);
                oRenderer.put_Height(lHeightPix * 25.4 / dDpi);
                
                for (int index = 0; index < nCountFonts; ++index)
                {
                    std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);
                    
                    // thumbnail
                    int lFontIndex = 0;
                    int lFaceIndex = 0;
                    if (pPair->second.m_lIndexR != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexR;
                        lFaceIndex = pPair->second.m_lFaceIndexR;
                    }
                    else if (pPair->second.m_lIndexI != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexI;
                        lFaceIndex = pPair->second.m_lFaceIndexI;
                    }
                    else if (pPair->second.m_lIndexB != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexB;
                        lFaceIndex = pPair->second.m_lFaceIndexB;
                    }
                    else if (pPair->second.m_lIndexBI != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexBI;
                        lFaceIndex = pPair->second.m_lFaceIndexBI;
                    }
                    
                    std::map<LONG, std::wstring>::iterator pPair2 = mapFontFiles2.find(lFontIndex);
                    std::wstring strFontPath = L"";
                    if (mapFontFiles2.end() != pPair2)
                        strFontPath = pPair2->second;
                    
                    oRenderer.put_FontPath(strFontPath);
                    oRenderer.put_FontFaceIndex(lFaceIndex);
                    pManager->LoadFontFromFile(strFontPath, lFaceIndex, 14, dDpi, dDpi);
                    
                    bool bIsSymbol = FALSE;
                    
                    if (pManager->m_pFont)
                    {
                        bIsSymbol = (-1 != (pManager->m_pFont->m_nSymbolic)) ? true : false;
                        
                        if (!bIsSymbol)
                        {
                            TT_OS2* pOS2 = (TT_OS2*)FT_Get_Sfnt_Table(pManager->m_pFont->m_pFace, ft_sfnt_os2);
                            
                            int y = 0;
                            ++y;
                            
                            if (NULL != pOS2)
                            {
                                if (0 == (pOS2->ulCodePageRange1 & 0xF0000000))
                                    bIsSymbol = true;
                            }
                        }
                    }
                    
                    if (bIsSymbol)
                    {
                        CFontSelectFormat oSelectFormat;
                        oSelectFormat.wsName = new std::wstring(L"Courier New");
                        CFontInfo* pInfoCur = pManager->GetFontInfoByParams(oSelectFormat);
                        
                        if (NULL != pInfoCur)
                        {
                            pManager->LoadFontFromFile(pInfoCur->m_wsFontPath, 0, 14, dDpi, dDpi);
                        }
                        oRenderer.put_FontPath(pInfoCur->m_wsFontPath);
                        oRenderer.put_FontFaceIndex(0);
                    }
                    
                    oRenderer.PathCommandStart();
                    oRenderer.BeginCommand(c_nClipType);
                    oRenderer.PathCommandRect(0, 25.4 * (index * lH1_px) / dDpi, dW_mm, 25.4 * lH1_px / dDpi);
                    oRenderer.EndCommand(c_nClipType);
                    oRenderer.PathCommandEnd();
                    
                    oRenderer.put_FontStringGID(FALSE);
                    oRenderer.put_FontCharSpace(0);
                    oRenderer.put_FontSize(14);
                    
                    oRenderer.CommandDrawText(pPair->second.m_sName, 5, 25.4 * (index * lH1_px + lH1_px) / dDpi - 2, 0, 0);
                    
                    oRenderer.BeginCommand(c_nResetClipType);
                    oRenderer.EndCommand(c_nResetClipType);
                    
                    oRenderer.CloseFont();
                    pCache->Clear();
                    applicationFonts.GetStreams()->Clear();
                }
                
                std::wstring strThumbnailPath = strFolderThumbnails + L"/fonts_thumbnail";
                if (iX == 1)
                    strThumbnailPath += L".png";
                else
                    strThumbnailPath += L"@2x.png";
                
                oFrame.SaveFile(strThumbnailPath, 4);
            }
            
            RELEASEOBJECT(pManager);
        }
#endif
        
        // и самое главное. Здесь должен скидываться скрипт для работы со всеми шрифтами.
        // все объекты, которые позволят не знать о существующих фонтах
        if (0 != strFile.length())
        {
            NSStringUtils::CStringBuilder oWriterJS;
            
            // сначала все файлы
            size_t nCountFiles = mapFontFiles.size();
            if (nCountFiles == 0)
                oWriterJS.WriteString(L"window[\"__fonts_files\"] = []; \n\n");
            else
            {
                std::wstring* pMassFiles = new std::wstring[nCountFiles];
                for ( std::map<std::wstring, LONG>::iterator pos = mapFontFiles.begin(); pos != mapFontFiles.end(); ++pos)
                {
                    std::wstring strFontId = pos->first;
                    
                    string_replace(strFontId, L"\\\\", L"\\");
                    string_replace(strFontId, L"\\", L"/");
                    
                    //int nStart = strFontId.find_last_of(wchar_t('\\'));
                    //strFontId = strFontId.substr(nStart + 1);
                    
                    pMassFiles[pos->second] = strFontId;
                }
                
                oWriterJS.WriteString(L"window[\"__fonts_files\"] = [\n");
                for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
                {
                    oWriterJS.WriteString(L"\"");
                    oWriterJS.WriteString(pMassFiles[nIndex]);
                    if (nIndex != (nCountFiles - 1))
                        oWriterJS.WriteString(L"\",\n");
                    else
                        oWriterJS.WriteString(L"\"");
                }
                oWriterJS.WriteString(L"\n];\n\n");
                
                delete [] pMassFiles;
            }
            
            oWriterJS.WriteString(L"window[\"__fonts_infos\"] = [\n");
            
            for (int index = 0; index < nCountFonts; ++index)
            {
                std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);
                
                char buffer[1000];
                sprintf(buffer, "\",%d,%d,%d,%d,%d,%d,%d,%d]", pPair->second.m_lIndexR, pPair->second.m_lFaceIndexR,
                        pPair->second.m_lIndexI, pPair->second.m_lFaceIndexI,
                        pPair->second.m_lIndexB, pPair->second.m_lFaceIndexB,
                        pPair->second.m_lIndexBI, pPair->second.m_lFaceIndexBI);
                
                std::string sBuffer(buffer);
                
                oWriterJS.WriteString(L"[\"");
                oWriterJS.WriteString(pPair->second.m_sName);
                oWriterJS.WriteString(NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sBuffer));
                
                if (index != (nCountFonts - 1))
                    oWriterJS.WriteString(L",\n");
                else
                    oWriterJS.WriteString(L"\n");
            }
            oWriterJS.WriteString(L"];\n\n");
            
            if (true)
            {
                BYTE* pData = NULL;
                LONG lLen = 0;
                applicationFonts.GetList()->ToBuffer(&pData, &lLen, L"", false);
                
                char* cData64 = NULL;
                int nData64Dst = 0;
                NSFile::CBase64Converter::Encode(pData, (int)lLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);
                
                std::wstring sData64 = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(cData64, (LONG)nData64Dst, FALSE);
                
                oWriterJS.WriteString(L"window[\"g_fonts_selection_bin\"] = \"");
                oWriterJS.WriteString(sData64);
                oWriterJS.WriteString(L"\";\n");
                
                RELEASEARRAYOBJECTS(pData);
                RELEASEARRAYOBJECTS(cData64);
            }
            
            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strFile);
            oFile.WriteStringUTF8(oWriterJS.GetData(), true);
            oFile.CloseFile();
        }
        
        if (0 != strFontSelectionBin.length())
        {
            BYTE* pData = NULL;
            LONG lLen = 0;
            applicationFonts.GetList()->ToBuffer(&pData, &lLen, L"", false);
            
            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strFontSelectionBin);
            oFile.WriteFile(pData, (DWORD)lLen);
            oFile.CloseFile();
            
            RELEASEARRAYOBJECTS(pData);
        }
    }
    
#ifdef _GENERATE_FONT_MAP_
    
    void WriteMemoryInt4(const int& value, BYTE*& pData)
    {
        *((FT_Int32*)(pData)) = value;
        pData += 4;
    }
    void WriteMemoryUInt4(const unsigned int& value, BYTE*& pData)
    {
        *((FT_UInt32*)(pData)) = value;
        pData += 4;
    }
    void WriteMemoryInt1(const char& value, BYTE*& pData)
    {
        *((BYTE*)(pData)) = (BYTE)value;
        pData += 1;
    }
    void WriteMemoryUInt1(const BYTE& value, BYTE*& pData)
    {
        *((BYTE*)(pData)) = value;
        pData += 1;
    }
    void WriteMemoryInt2(const short& value, BYTE*& pData)
    {
        *((short*)(pData)) = value;
        pData += 2;
    }
    void WriteMemoryUInt2(const unsigned short& value, BYTE*& pData)
    {
        *((unsigned short*)(pData)) = value;
        pData += 2;
    }
    void WriteMemoryStringA(const std::string& value, BYTE*& pData)
    {
        size_t len = value.length();
        WriteMemoryInt4((int)len, pData);
        if (0 != len)
        {
            memcpy(pData, value.c_str(), len);
            pData += len;
        }
    }
    
    void DumpFontsDictionary(const std::wstring& strDumpFile)
    {
        int nFileDictionaryBinaryLen = 0;
        
        // FD_Ascii_Names ---------------------------------------
        nFileDictionaryBinaryLen += 4;
        for (int i = 0; i < FONTS_DICT_ASCII_NAMES_COUNT; ++i)
        {
            const FD_FontMapRec _rec = FD_Ascii_Names[i];
            
            std::string sName(_rec.m_name);
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += (int)sName.length();
            nFileDictionaryBinaryLen += 4 * 4;
        }
        
        // FD_Ascii_Names_Offsets
        nFileDictionaryBinaryLen += 4 * 256;
        // ------------------------------------------------------
        
        // FD_Unicode_Names -------------------------------------
        nFileDictionaryBinaryLen += 4;
        for (int i = 0; i < FONTS_DICT_UNICODE_NAMES_COUNT; ++i)
        {
            const FD_FontMapRecW _rec = FD_Unicode_Names[i];
            
            std::wstring sName(_rec.m_name);
            std::string sNameA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), (LONG)sName.length());
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += (int)sNameA.length();
            nFileDictionaryBinaryLen += 4 * 4;
        }
        // ------------------------------------------------------
        
        // FD_Ascii_Files ---------------------------------------
        nFileDictionaryBinaryLen += 4;
        for (int i = 0; i < FONTS_DICT_ASCII_FONTS_COUNT; ++i)
        {
            const FD_Font _rec = FD_Ascii_Files[i];
            
            std::string sName(_rec.m_name);
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += (int)sName.length();
            
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += 3 * 4;
            nFileDictionaryBinaryLen += 10;
            nFileDictionaryBinaryLen += 4 * 6;
            nFileDictionaryBinaryLen += 2 * 3;
            nFileDictionaryBinaryLen += 2;
            nFileDictionaryBinaryLen += 2 * 6;
        }
        // ------------------------------------------------------
        
        BYTE* pBinaryData = new BYTE[nFileDictionaryBinaryLen];
        BYTE* pBinaryDataMem = pBinaryData;
        
        // FD_Ascii_Names ---------------------------------------
        WriteMemoryInt4((int)FONTS_DICT_ASCII_NAMES_COUNT, pBinaryDataMem);
        for (int i = 0; i < FONTS_DICT_ASCII_NAMES_COUNT; ++i)
        {
            const FD_FontMapRec _rec = FD_Ascii_Names[i];
            
            std::string sName(_rec.m_name);
            WriteMemoryStringA(sName, pBinaryDataMem);
            
            WriteMemoryInt4(_rec.m_index_r, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_i, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_b, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_bi, pBinaryDataMem);
        }
        
        // FD_Ascii_Names_Offsets
        memcpy(pBinaryDataMem, FD_Ascii_Names_Offsets, 256 * 4);
        pBinaryDataMem += 4 * 256;
        // ------------------------------------------------------
        
        // FD_Unicode_Names -------------------------------------
        WriteMemoryInt4((int)FONTS_DICT_UNICODE_NAMES_COUNT, pBinaryDataMem);
        for (int i = 0; i < FONTS_DICT_UNICODE_NAMES_COUNT; ++i)
        {
            const FD_FontMapRecW _rec = FD_Unicode_Names[i];
            
            std::wstring sName(_rec.m_name);
            std::string sNameA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), (LONG)sName.length());
            
            WriteMemoryStringA(sNameA, pBinaryDataMem);
            
            WriteMemoryInt4(_rec.m_index_r, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_i, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_b, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_bi, pBinaryDataMem);
        }
        // ------------------------------------------------------
        
        // FD_Ascii_Files ---------------------------------------
        WriteMemoryInt4((int)FONTS_DICT_ASCII_FONTS_COUNT, pBinaryDataMem);
        for (int i = 0; i < FONTS_DICT_ASCII_FONTS_COUNT; ++i)
        {
            const FD_Font _rec = FD_Ascii_Files[i];
            
            std::string sName(_rec.m_name);
            WriteMemoryStringA(sName, pBinaryDataMem);
            
            WriteMemoryInt4((int)_rec.m_lIndex, pBinaryDataMem);
            
            WriteMemoryInt4((FT_Int32)_rec.m_bBold, pBinaryDataMem);
            WriteMemoryInt4((FT_Int32)_rec.m_bItalic, pBinaryDataMem);
            WriteMemoryInt4((FT_Int32)_rec.m_bIsFixed, pBinaryDataMem);
            
            memcpy(pBinaryDataMem, _rec.m_aPanose, 10);
            pBinaryDataMem += 10;
            
            WriteMemoryUInt4(_rec.m_ulUnicodeRange1, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulUnicodeRange2, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulUnicodeRange3, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulUnicodeRange4, pBinaryDataMem);
            
            WriteMemoryUInt4(_rec.m_ulCodePageRange1, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulCodePageRange2, pBinaryDataMem);
            
            WriteMemoryUInt2(_rec.m_usWeigth, pBinaryDataMem);
            WriteMemoryUInt2(_rec.m_usWidth, pBinaryDataMem);
            
            WriteMemoryInt2(_rec.m_sFamilyClass, pBinaryDataMem);
            
            WriteMemoryInt2((short)_rec.m_eFontFormat, pBinaryDataMem);
            
            WriteMemoryInt2(_rec.m_shAvgCharWidth, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shAscent, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shDescent, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shLineGap, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shXHeight, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shCapHeight, pBinaryDataMem);
        }
        // ------------------------------------------------------
        
        char* cData64 = NULL;
        int nData64Dst = 0;
        NSFile::CBase64Converter::Encode(pBinaryData, (int)nFileDictionaryBinaryLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);
        
        RELEASEARRAYOBJECTS(pBinaryData);
        
        NSFile::CFileBinary oFile;
        oFile.CreateFileW(strDumpFile);
        oFile.WriteFile((BYTE*)cData64, (DWORD)nData64Dst);
        oFile.CloseFile();
        
        RELEASEARRAYOBJECTS(cData64);
    }
    
#endif
}

#endif // APPLICATION_GENERATE_FONTS_H
