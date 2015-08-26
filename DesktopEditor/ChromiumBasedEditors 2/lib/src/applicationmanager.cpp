#include "../include/applicationmanager.h"
#include <map>

#include "../../graphics/TemporaryCS.h"
#include "../../graphics/BaseThread.h"
#include "../../graphics/Timer.h"

#include "../src/cookiesworker.h"

#include <iostream>
#include <set>

#include "../../fontengine/ApplicationFonts.h"
#include "../../graphics/GraphicsRenderer.h"
#include "../../common/File.h"
#include "../../common/Directory.h"
#include "../../common/StringBuilder.h"

#include "./filedownloader.h"

#define LOCK_CS_SCRIPT 0

///////////////////////////////////////////////////////
//#define _GENERATE_FONT_MAP_

#ifdef _GENERATE_FONT_MAP_
#include "../../freetype_names/FontMaps/FontDictionary.h"
#endif

using namespace std;

namespace NSCommon
{
    class CFontInfoJS
    {
    public:
        std::wstring m_sName;

        int	m_lIndexR;
        int	m_lFaceIndexR;
        int	m_lIndexI;
        int	m_lFaceIndexI;
        int	m_lIndexB;
        int	m_lFaceIndexB;
        int	m_lIndexBI;
        int	m_lFaceIndexBI;

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

            return *this;
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
                    pPair->second.m_lIndexBI = lFontIndex;
                    pPair->second.m_lFaceIndexBI = lFaceIndex;
                }
                else if (pInfo->m_bBold)
                {
                    pPair->second.m_lIndexB = lFontIndex;
                    pPair->second.m_lFaceIndexB = lFaceIndex;
                }
                else if (pInfo->m_bItalic)
                {
                    pPair->second.m_lIndexI = lFontIndex;
                    pPair->second.m_lFaceIndexI = lFaceIndex;
                }
                else
                {
                    pPair->second.m_lIndexR = lFontIndex;
                    pPair->second.m_lFaceIndexR = lFaceIndex;
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
                }
                else if (pInfo->m_bBold)
                {
                    fontInfo.m_lIndexB = lFontIndex;
                    fontInfo.m_lFaceIndexB = lFaceIndex;
                }
                else if (pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexI = lFontIndex;
                    fontInfo.m_lFaceIndexI = lFaceIndex;
                }
                else
                {
                    fontInfo.m_lIndexR = lFontIndex;
                    fontInfo.m_lFaceIndexR = lFaceIndex;
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
                    LONG lFontIndex = 0;
                    LONG lFaceIndex = 0;
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
                    pManager->LoadFontFromFile(strFontPath, lFaceIndex, 14, dDpi, dDpi);

                    BOOL bIsSymbol = FALSE;

                    if (pManager->m_pFont)
                    {
                        bIsSymbol = (-1 != (pManager->m_pFont->m_nSymbolic)) ? TRUE : FALSE;

                        if (!bIsSymbol)
                        {
                            TT_OS2* pOS2 = (TT_OS2*)FT_Get_Sfnt_Table(pManager->m_pFont->m_pFace, ft_sfnt_os2);

                            int y = 0;
                            ++y;

                            if (NULL != pOS2)
                            {
                                if (0 == (pOS2->ulCodePageRange1 & 0xF0000000))
                                    bIsSymbol = TRUE;
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

///////////////////////////////////////////////////////

class CCefScriptLoader : public NSThreads::CBaseThread
{
public:
    class ICefScriptLoaderCallback
    {
    public:
        virtual void OnLoad(CCefScriptLoader*, bool error) {}
    };

public:
    std::string     m_sVersion;
    std::wstring    m_sUrl;
    std::wstring    m_sDestination;

    CAscApplicationManager* m_pManager;
    ICefScriptLoaderCallback* m_pListener;

public:

    CCefScriptLoader() : NSThreads::CBaseThread()
    {
        m_pListener = NULL;
        m_pManager = NULL;
    }

protected:
    virtual DWORD ThreadProc()
    {
        if (std::wstring::npos != m_sUrl.find(L"sdk/Common/AllFonts.js"))
        {
            while (!m_pManager->IsInitFonts())
            {
                NSThreads::Sleep( 10 );
            }

            m_bRunThread = FALSE;

            if (NULL != m_pListener)
                m_pListener->OnLoad(this, false);
        }
        else
        {
            CFileDownloader oDownloader(m_sUrl, false);
            oDownloader.SetFilePath(m_sDestination);
            oDownloader.Start( 0 );
            while ( oDownloader.IsRunned() )
            {
                ::Sleep( 10 );
            }

            m_bRunThread = FALSE;

            if (NULL != m_pListener)
                m_pListener->OnLoad(this, oDownloader.IsFileDownloaded() ? false : true);
        }

        return 0;
    }
};

///////////////////////////////////////////////////////
class CEditorFrameId
{
public:
    int EditorId;
    int FrameId;
};

class CAscApplicationManager_Private;
class CTimerKeyboardChecker : public NSTimers::CTimer
{
    CAscApplicationManager_Private* m_pManager;
public:
    CTimerKeyboardChecker(CAscApplicationManager_Private* pManagerPrivate) : NSTimers::CTimer()
    {
        m_pManager = pManagerPrivate;
        SetInterval(100);
    }
    virtual ~CTimerKeyboardChecker()
    {
    }

protected:
    virtual void OnTimer();
};

class CAscApplicationManager_Private : public CefBase, public CCookieFoundCallback, public NSThreads::CBaseThread, public CCefScriptLoader::ICefScriptLoaderCallback
{
public:
    CAscSpellChecker    m_oSpellChecker;
    CAscKeyboardChecker m_oKeyboardChecker;

    int                 m_nIdCounter;

    NSEditorApi::CAscMenuEventListener* m_pListener;

    std::map<int, CCefView*> m_mapViews;    

    CAscApplicationManager* m_pMain;

    CApplicationFonts* m_pApplicationFonts;

    NSCriticalSection::CRITICAL_SECTION m_oCS_Scripts;
    std::map<std::wstring, std::vector<CEditorFrameId>> m_mapLoadedScripts;

    CApplicationCEF* m_pApplication;
    bool m_bDebugInfoSupport;

    int m_nIsCefSaveDialogWait;

    CTimerKeyboardChecker m_oKeyboardTimer;

public:
    CAscApplicationManager_Private() : m_oKeyboardTimer(this)
    {
        m_pListener = NULL;
        m_nIdCounter = 0;
        this->AddRef();
        m_pMain = NULL;

        m_pApplicationFonts = NULL;

        m_pApplication = NULL;

        m_bDebugInfoSupport = false;

        m_nIsCefSaveDialogWait = -1;

        m_oCS_Scripts.InitializeCriticalSection();
    }
    virtual ~CAscApplicationManager_Private()
    {
        CloseApplication();
        m_oCS_Scripts.DeleteCriticalSection();
    }

    void CloseApplication()
    {
        m_oKeyboardTimer.Stop();
        Stop();
        m_oSpellChecker.End();
    }

    void Logout(std::wstring strUrl, CefRefPtr<CefCookieManager> manager)
    {
        CCefCookieVisitor* pVisitor = new CCefCookieVisitor();
        pVisitor->m_pCallback       = this;
        pVisitor->m_bIsDelete       = true;
        pVisitor->m_sDomain         = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strUrl.c_str(), strUrl.length());

        pVisitor->CheckCookiePresent(CefCookieManager::GetGlobalManager(NULL));
    }

    virtual void OnFoundCookie(bool bIsPresent, std::string sValue)
    {
        // not used
    }

    virtual void OnSetCookie()
    {
        // not used
    }

    virtual void OnDeleteCookie(bool bIsPresent)
    {
        if (NULL != m_pMain && NULL != m_pMain->GetEventListener())
        {
            NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
            pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONLOGOUT;

            m_pMain->GetEventListener()->OnEvent(pEvent);
        }
    }

public:
    IMPLEMENT_REFCOUNTING(CAscApplicationManager_Private);

public:
    virtual void OnLoad(CCefScriptLoader* pLoader, bool error)
    {
        m_pMain->LockCS(LOCK_CS_SCRIPT);

        std::map<std::wstring, std::vector<CEditorFrameId>>::iterator i = m_mapLoadedScripts.find(pLoader->m_sDestination);

        if (i != m_mapLoadedScripts.end())
        {
            // другого и быть не может

            NSEditorApi::CAscEditorScript* pData = new NSEditorApi::CAscEditorScript();
            pData->put_Url(pLoader->m_sUrl);
            pData->put_Destination(pLoader->m_sDestination);

            NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
            pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_SCRIPT_EDITOR_VERSION;
            pEvent->m_pData = pData;

            std::vector<CEditorFrameId>& _arr = i->second;
            for (std::vector<CEditorFrameId>::iterator it = _arr.begin(); it != _arr.end(); it++)
            {
                CCefView* pView = m_pMain->GetViewById((*it).EditorId);

                if (NULL != pView)
                {
                    pEvent->AddRef();

                    pData->put_FrameId((*it).FrameId);
                    pView->Apply(pEvent);
                }
            }

            RELEASEINTERFACE(pEvent);
        }

        m_mapLoadedScripts.erase(i);
        RELEASEOBJECT(pLoader);

        m_pMain->UnlockCS(LOCK_CS_SCRIPT);
    }

protected:

    virtual DWORD ThreadProc()
    {
        //DWORD dwTime1 = NSTimers::GetTickCount();

        CArray<std::string> strFonts;
        std::wstring strDirectory = m_pMain->m_oSettings.fonts_cache_info_path;

        std::wstring strAllFontsJSPath = strDirectory + L"/AllFonts.js";
        std::wstring strThumbnailsFolder = strDirectory;
        std::wstring strFontsSelectionBin = L"";

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

        CApplicationFonts* oApplicationF = new CApplicationFonts();
        CArray<std::wstring> strFontsW_Cur;

        if (m_pMain->m_oSettings.use_system_fonts)
            strFontsW_Cur = oApplicationF->GetSetupFontFiles();

        for (std::vector<std::wstring>::iterator i = m_pMain->m_oSettings.additional_fonts_folder.begin(); i != m_pMain->m_oSettings.additional_fonts_folder.end(); i++)
        {
            NSDirectory::GetFiles2(*i, strFontsW_Cur, true);
        }

        bool bIsEqual = true;
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

            oApplicationF->InitializeFromArrayFiles(strFontsW_Cur);

            NSCommon::SaveAllFontsJS(*oApplicationF, strAllFontsJSPath, strThumbnailsFolder, strFontsSelectionBin);

    #ifdef _GENERATE_FONT_MAP_
            NSCommon::DumpFontsDictionary(L"D:\\fonts_dictionary.txt");
    #endif
        }

        //DWORD dwTime2 = NSTimers::GetTickCount();

    #if 0
        FILE* f = fopen("D:\\FONTS\\speed.log", "a+");
        fprintf(f, "equal: %d, time: %d\n", bIsEqual ? 1 : 0, (int)(dwTime2 - dwTime1));
        fclose(f);
    #endif

        m_pApplicationFonts = oApplicationF;

        m_bRunThread = FALSE;

        // TODO:
        m_pApplicationFonts->InitializeFromArrayFiles(strFontsW_Cur);

        return 0;
    }
};

void CTimerKeyboardChecker::OnTimer()
{
    if (m_pManager->m_pMain)
        m_pManager->m_pMain->OnNeedCheckKeyboard();
}

CAscApplicationManager::CAscApplicationManager()
{
    m_pInternal = new CAscApplicationManager_Private();
    m_pInternal->m_pMain = this;
}

CAscApplicationManager::~CAscApplicationManager()
{
    m_pInternal->Release();
}

void CAscApplicationManager::StartSpellChecker()
{
    m_pInternal->m_oSpellChecker.SetApplicationManager(this);
    m_pInternal->m_oSpellChecker.Init(m_oSettings.spell_dictionaries_path);
    m_pInternal->m_oSpellChecker.Start();
}

void CAscApplicationManager::StopSpellChecker()
{
    m_pInternal->m_oSpellChecker.End();
}

void CAscApplicationManager::SpellCheck(const int& nEditorId, const std::string& sTask, int nId)
{
    m_pInternal->m_oSpellChecker.AddTask(nEditorId, sTask, nId);
}

void CAscApplicationManager::StartKeyboardChecker()
{
    m_pInternal->m_oKeyboardTimer.Start(0);
}
void CAscApplicationManager::OnNeedCheckKeyboard()
{
    // none. evaluate in UI thread
}

void CAscApplicationManager::CheckKeyboard()
{
    m_pInternal->m_oKeyboardChecker.Check(this, -1);
}

void CAscApplicationManager::SendKeyboardAttack()
{
    m_pInternal->m_oKeyboardChecker.Send(this);
}

void CAscApplicationManager::CheckFonts(bool bAsync)
{
    if (!NSDirectory::Exists(m_oSettings.fonts_cache_info_path))
        NSDirectory::CreateDirectoryW(m_oSettings.fonts_cache_info_path);

    bool bIsStarted = m_pInternal->IsRunned();
    bool bIsInit = IsInitFonts();

    if (bIsInit)
        return;

    if (bIsStarted)
    {
        if (!bAsync)
        {
            while (!IsInitFonts())
            {
                NSThreads::Sleep(10);
            }
        }
        return;
    }

    m_pInternal->Start(0);
    if (!bAsync)
    {
        while (!IsInitFonts())
        {
            NSThreads::Sleep(10);
        }
    }
}

void CAscApplicationManager::SetEventListener(NSEditorApi::CAscMenuEventListener* pListener)
{
    m_pInternal->m_pListener = pListener;
}

NSEditorApi::CAscMenuEventListener* CAscApplicationManager::GetEventListener()
{
    return m_pInternal->m_pListener;
}

void CAscApplicationManager::Apply(NSEditorApi::CAscMenuEvent* pEvent)
{
    if (NULL == pEvent)
        return;

    switch (pEvent->m_nType)
    {
        case ASC_MENU_EVENT_TYPE_CEF_KEYBOARDLAYOUT:
        {
            for (std::map<int, CCefView*>::iterator i = m_pInternal->m_mapViews.begin(); i != m_pInternal->m_mapViews.end(); i++)
            {
                ADDREFINTERFACE(pEvent);
                i->second->Apply(pEvent);
            }
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SPELLCHECK:
        {
            NSEditorApi::CAscSpellCheckType* pData = (NSEditorApi::CAscSpellCheckType*)pEvent->m_pData;

            std::map<int, CCefView*>::iterator i = m_pInternal->m_mapViews.find(pData->get_EditorId());
            if (i != m_pInternal->m_mapViews.end())
            {
                ADDREFINTERFACE(pEvent);
                i->second->Apply(pEvent);
            }

            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD:
        {
            NSEditorApi::CAscDownloadFileInfo* pData = (NSEditorApi::CAscDownloadFileInfo*)pEvent->m_pData;

            if (pData->get_IsComplete())
            {
                int nId = pData->get_Id();
                CCefView* pCefView = this->GetViewById(nId);

                if (-1 != pCefView->GetParentCef())
                    this->DestroyCefView(pData->get_Id());
            }

            if (m_pInternal->m_pListener)
            {
                ADDREFINTERFACE(pEvent);
                m_pInternal->m_pListener->OnEvent(pEvent);
            }

            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SCRIPT_EDITOR_VERSION:
        {
            NSEditorApi::CAscEditorScript* pData = (NSEditorApi::CAscEditorScript*)pEvent->m_pData;

            this->LockCS(LOCK_CS_SCRIPT);

            // смотрим, зажат ли этот файл
            std::map<std::wstring, std::vector<CEditorFrameId>>::iterator _find = m_pInternal->m_mapLoadedScripts.find(pData->get_Destination());
            if (_find != m_pInternal->m_mapLoadedScripts.end())
            {
                // он качается. нужно записать id в список тех, кто хочет получить скрипт после его загрузки
                CEditorFrameId _id;
                _id.EditorId = pData->get_Id();
                _id.FrameId = pData->get_FrameId();
                _find->second.push_back(_id);
            }
            else
            {
                // смотрим, есть ли файл на диске
                if (NSFile::CFileBinary::Exists(pData->get_Destination()))
                {
                    CCefView* pView = this->GetViewById(pData->get_Id());
                    if (NULL != pView)
                    {
                        pEvent->AddRef();
                        pView->Apply(pEvent);
                    }
                }
                else
                {
                    // создаем список ожидания
                    std::vector<CEditorFrameId> _arr;
                    CEditorFrameId _id;
                    _id.EditorId = pData->get_Id();
                    _id.FrameId = pData->get_FrameId();
                    _arr.push_back(_id);

                    m_pInternal->m_mapLoadedScripts.insert(std::pair<std::wstring, std::vector<CEditorFrameId>>(pData->get_Destination(), _arr));

                    CCefScriptLoader* pLoader = new CCefScriptLoader();
                    pLoader->m_pListener = this->m_pInternal;
                    pLoader->m_pManager = this;
                    pLoader->m_sUrl = pData->get_Url();
                    pLoader->m_sDestination = pData->get_Destination();
                    pLoader->Start(0);
                }
            }

            this->UnlockCS(LOCK_CS_SCRIPT);
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SAVEFILEDIALOG:
        {
            NSEditorApi::CAscSaveDialog* pData = (NSEditorApi::CAscSaveDialog*)pEvent->m_pData;
            m_pInternal->m_nIsCefSaveDialogWait = pData->get_Id();

            this->StartSaveDialog(pData->get_FilePath());
            break;
        }
    }

    RELEASEINTERFACE(pEvent);
}

NSEditorApi::CAscMenuEvent* CAscApplicationManager::ApplySync(NSEditorApi::CAscMenuEvent* pEvent)
{
    return NULL;
}

CCefView* CAscApplicationManager::CreateCefView(CCefViewWidgetImpl* parent)
{
    m_pInternal->m_nIdCounter++;
    CCefView* pView = new CCefView(parent, m_pInternal->m_nIdCounter);
    pView->SetAppManager(this);

    m_pInternal->m_mapViews[m_pInternal->m_nIdCounter] = pView;
    return pView;
}

CCefViewEditor* CAscApplicationManager::CreateCefEditor(CCefViewWidgetImpl* parent)
{
    m_pInternal->m_nIdCounter++;
    CCefViewEditor* pView = new CCefViewEditor(parent, m_pInternal->m_nIdCounter);
    pView->SetAppManager(this);

    m_pInternal->m_mapViews[m_pInternal->m_nIdCounter] = pView;
    return pView;
}

CCefView* CAscApplicationManager::GetViewById(int nId)
{
    std::map<int, CCefView*>::iterator i = m_pInternal->m_mapViews.find(nId);
    if (i == m_pInternal->m_mapViews.end())
        return NULL;
    return i->second;
}
CCefView* CAscApplicationManager::GetViewByUrl(const std::wstring& url)
{
    for (std::map<int, CCefView*>::iterator i = m_pInternal->m_mapViews.begin(); i != m_pInternal->m_mapViews.end(); ++i)
    {
        if (i->second->GetUrl() == url)
            return i->second;
    }
    return NULL;
}

void CAscApplicationManager::DestroyCefView(int nId)
{
    if (-1 == nId)
    {
        for (std::map<int, CCefView*>::iterator i = m_pInternal->m_mapViews.begin(); i != m_pInternal->m_mapViews.end(); i++)
        {
            CCefView* pView = i->second;
            if (pView)
            {
                NSEditorApi::CAscMenuEvent* pEventD = new NSEditorApi::CAscMenuEvent();
                pEventD->m_nType = ASC_MENU_EVENT_TYPE_CEF_DESTROY;
                pView->Apply(pEventD);
            }
        }

        m_pInternal->m_mapViews.clear();
    }
    else
    {
        std::map<int, CCefView*>::iterator i = m_pInternal->m_mapViews.find(nId);
        if (i == m_pInternal->m_mapViews.end())
            return;

        CCefView* pView = i->second;
        if (pView)
        {
            NSEditorApi::CAscMenuEvent* pEventD = new NSEditorApi::CAscMenuEvent();
            pEventD->m_nType = ASC_MENU_EVENT_TYPE_CEF_DESTROY;
            pView->Apply(pEventD);
        }

        m_pInternal->m_mapViews.erase(i);
    }
}

void CAscApplicationManager::Logout(std::wstring strUrl)
{
    m_pInternal->Logout(strUrl, CefCookieManager::GetGlobalManager(NULL));
}

void CAscApplicationManager::LockCS(int nId)
{
    m_pInternal->m_oCS_Scripts.Enter();
}

void CAscApplicationManager::UnlockCS(int nId)
{
    m_pInternal->m_oCS_Scripts.Leave();
}

bool CAscApplicationManager::IsInitFonts()
{
    return (NULL != m_pInternal->m_pApplicationFonts) ? true : false;
}

CApplicationCEF* CAscApplicationManager::GetApplication()
{
    return m_pInternal->m_pApplication;
}

void CAscApplicationManager::SetApplication(CApplicationCEF* pApp)
{
    m_pInternal->m_pApplication = pApp;
}

void CAscApplicationManager::SetDebugInfoSupport(bool bIsSupport)
{
    m_pInternal->m_bDebugInfoSupport = bIsSupport;
}

bool CAscApplicationManager::GetDebugInfoSupport()
{
    return m_pInternal->m_bDebugInfoSupport;
}

void CAscApplicationManager::CloseApplication()
{
    this->DestroyCefView(-1);
    this->m_pInternal->CloseApplication();
    if (NULL != m_pInternal->m_pApplication)
        m_pInternal->m_pApplication->Close();
}

CApplicationFonts* CAscApplicationManager::GetApplicationFonts()
{
    return m_pInternal->m_pApplicationFonts;
}

void CAscApplicationManager::StartSaveDialog(const std::wstring& sName)
{
    // в этой реализации - всегда отмена
    // хочешь пользоваться - пиши враппер
    this->EndSaveDialog(L"");
}

void CAscApplicationManager::EndSaveDialog(const std::wstring& sPath)
{
#ifdef WIN32
    std::wstring sPathWin = sPath;
    NSCommon::string_replace(sPathWin, L"/", L"\\");
#endif

    CCefView* pView = this->GetViewById(m_pInternal->m_nIsCefSaveDialogWait);
    if (NULL != pView)
    {
        NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent(ASC_MENU_EVENT_TYPE_CEF_SAVEFILEDIALOG);
        NSEditorApi::CAscSaveDialog* pData = new NSEditorApi::CAscSaveDialog();
        pData->put_Id(pView->GetId());

#ifdef WIN32
        pData->put_FilePath(sPathWin);
#else
        pData->put_FilePath(sPath);
#endif
        pEvent->m_pData = pData;

        pView->Apply(pEvent);
    }
}

int CAscApplicationManager::GetPlatformKeyboardLayout()
{
#ifdef WIN32
    HWND wFocus = GetFocus();
    DWORD dwThread = GetWindowThreadProcessId(wFocus, 0);
    HKL hkl = GetKeyboardLayout(dwThread);
    int nLang = LOWORD(hkl);
    return nLang;
#endif
    return -1;
}
