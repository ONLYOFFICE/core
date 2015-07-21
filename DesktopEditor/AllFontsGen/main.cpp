#include <iostream>
#include <set>

#include "../fontengine/ApplicationFonts.h"
#include "../graphics/GraphicsRenderer.h"
#include "../common/File.h"

//#define _GENERATE_FONT_MAP_

#ifdef _GENERATE_FONT_MAP_
#include "../freetype_names/FontMaps/FontDictionary.h"
#endif

using namespace std;

namespace NSCommon
{
    void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }

    class CTextItem
    {
    protected:
        wchar_t*	m_pData;
        size_t		m_lSize;

        wchar_t*	m_pDataCur;
        size_t		m_lSizeCur;

    public:
        CTextItem()
        {
            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur	= m_pData;
            m_lSizeCur	= m_lSize;
        }
        CTextItem(const CTextItem& oSrc)
        {
            m_pData = NULL;
            *this = oSrc;
        }
        CTextItem& operator=(const CTextItem& oSrc)
        {
            RELEASEMEM(m_pData);

            m_lSize		= oSrc.m_lSize;
            m_lSizeCur	= oSrc.m_lSizeCur;
            m_pData		= (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

            memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(wchar_t));

            m_pDataCur = m_pData + m_lSizeCur;

            return *this;
        }

        CTextItem(const size_t& nLen)
        {
            m_lSize = nLen;
            m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
        }
        CTextItem(wchar_t* pData, const size_t& nLen)
        {
            m_lSize = nLen;
            m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

            memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));

            m_lSizeCur = m_lSize;
            m_pDataCur = m_pData + m_lSize;
        }
        CTextItem(wchar_t* pData, BYTE* pUnicodeChecker = NULL)
        {
            size_t nLen = GetStringLen(pData);

            m_lSize = nLen;
            m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

            memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));

            m_lSizeCur = m_lSize;
            m_pDataCur = m_pData + m_lSize;

            if (NULL != pUnicodeChecker)
            {
                wchar_t* pMemory = m_pData;
                while (pMemory < m_pDataCur)
                {
                    if (!pUnicodeChecker[*pMemory])
                        *pMemory = wchar_t(' ');
                    ++pMemory;
                }
            }
        }
        virtual ~CTextItem()
        {
            RELEASEMEM(m_pData);
        }

        inline void AddSize(const size_t& nSize)
        {
            if (NULL == m_pData)
            {
                m_lSize = 1000;
                if (nSize > m_lSize)
                    m_lSize = nSize;

                m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

                m_lSizeCur = 0;
                m_pDataCur = m_pData;
                return;
            }

            if ((m_lSizeCur + nSize) > m_lSize)
            {
                while ((m_lSizeCur + nSize) > m_lSize)
                {
                    m_lSize *= 2;
                }

                wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
                if (NULL != pRealloc)
                {
                    // реаллок сработал
                    m_pData		= pRealloc;
                    m_pDataCur	= m_pData + m_lSizeCur;
                }
                else
                {
                    wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
                    memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

                    free(m_pData);
                    m_pData		= pMalloc;
                    m_pDataCur	= m_pData + m_lSizeCur;
                }
            }
        }

    public:

        inline void operator+=(const std::wstring& oTemp)
        {
            WriteString(oTemp.c_str(), oTemp.length());
        }
        inline wchar_t operator[](const size_t& nIndex)
        {
            if (nIndex < m_lSizeCur)
                return m_pData[nIndex];

            return 0;
        }

        inline void AddSpace()
        {
            AddSize(1);
            *m_pDataCur = wchar_t(' ');

            ++m_lSizeCur;
            ++m_pDataCur;
        }
        inline void CorrectUnicode(const BYTE* pUnicodeChecker)
        {
            if (NULL != pUnicodeChecker)
            {
                wchar_t* pMemory = m_pData;
                while (pMemory < m_pDataCur)
                {
                    if (!pUnicodeChecker[*pMemory])
                        *pMemory = wchar_t(' ');
                    ++pMemory;
                }
            }
        }
        inline void RemoveLastSpaces()
        {
            wchar_t* pMemory = m_pDataCur - 1;
            while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
            {
                --pMemory;
                --m_lSizeCur;
                --m_pDataCur;
            }

        }
        inline bool IsSpace()
        {
            if (1 != m_lSizeCur)
                return false;
            return (wchar_t(' ') == *m_pData);
        }

    public:
        inline void WriteString(const wchar_t* pString, const size_t& nLen)
        {
            AddSize(nLen);
            memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
            m_pDataCur += nLen;
            m_lSizeCur += nLen;
        }
        inline size_t GetCurSize()
        {
            return m_lSizeCur;
        }
        inline size_t GetSize()
        {
            return m_lSize;
        }
        inline void Clear()
        {
            RELEASEMEM(m_pData);

            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur	= m_pData;
            m_lSizeCur	= 0;
        }
        inline void ClearNoAttack()
        {
            m_pDataCur	= m_pData;
            m_lSizeCur	= 0;
        }

        inline size_t GetStringLen(wchar_t* pData)
        {
            wchar_t* s = pData;
            for (; *s != 0; ++s);
            return (size_t)(s - pData);
        }

        inline std::wstring GetCString()
        {
            std::wstring str(m_pData, (int)m_lSizeCur);
            return str;
        }
        inline wchar_t* GetBuffer()
        {
            return m_pData;
        }
    };

    class CStringWriter : public CTextItem
    {
    public:
        CStringWriter() : CTextItem()
        {
        }
        virtual ~CStringWriter()
        {
        }

    public:

        inline void Write(CStringWriter& oWriter)
        {
            CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
        }
    };

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

    void SaveAllFontsJS(CApplicationFonts& applicationFonts, std::wstring strFile, std::wstring strFolderThumbnails, std::wstring strFontSelectionBin)
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

                    INT bIsSymbol = FALSE;

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

                    oRenderer.put_FontStringGID(FALSE);
                    oRenderer.put_FontCharSpace(0);
                    oRenderer.put_FontSize(14);

                    oRenderer.CommandDrawText(pPair->second.m_sName, 5, 25.4 * (index * lH1_px + lH1_px) / dDpi - 2, 0, 0);
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
            CStringWriter oWriterJS;

            // сначала все файлы
            size_t nCountFiles = mapFontFiles.size();
            if (nCountFiles == 0)
                oWriterJS += (L"window[\"__fonts_files\"] = []; \n\n");
            else
            {
                std::wstring* pMassFiles = new std::wstring[nCountFiles];
                for ( std::map<std::wstring, LONG>::iterator pos = mapFontFiles.begin(); pos != mapFontFiles.end(); ++pos)
                {
                    std::wstring strFontId = pos->first;

                    string_replace(strFontId, L"\\\\", L"\\");
                    string_replace(strFontId, L"/", L"\\");

                    int nStart = strFontId.find_last_of(wchar_t('\\'));
                    strFontId = strFontId.substr(nStart + 1);

                    pMassFiles[pos->second] = strFontId;
                }

                oWriterJS += (L"window[\"__fonts_files\"] = [\n");
                for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
                {
                    oWriterJS += (L"\"");
                    oWriterJS += (pMassFiles[nIndex]);
                    if (nIndex != (nCountFiles - 1))
                        oWriterJS += (L"\",\n");
                    else
                        oWriterJS += (L"\"");
                }
                oWriterJS += (L"\n];\n\n");

                delete [] pMassFiles;
            }

            oWriterJS += L"window[\"__fonts_infos\"] = [\n";

            for (int index = 0; index < nCountFonts; ++index)
            {
                std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);

                char buffer[1000];
                sprintf(buffer, "\",%d,%d,%d,%d,%d,%d,%d,%d]", pPair->second.m_lIndexR, pPair->second.m_lFaceIndexR,
                         pPair->second.m_lIndexI, pPair->second.m_lFaceIndexI,
                         pPair->second.m_lIndexB, pPair->second.m_lFaceIndexB,
                         pPair->second.m_lIndexBI, pPair->second.m_lFaceIndexBI);

                std::string sBuffer(buffer);

                oWriterJS += L"[\"";
                oWriterJS += pPair->second.m_sName;
                oWriterJS += NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sBuffer);

                if (index != (nCountFonts - 1))
                    oWriterJS += (L",\n");
                else
                    oWriterJS += (L"\n");
            }
            oWriterJS += (L"];\n\n");

            if (true)
            {
                BYTE* pData = NULL;
                LONG lLen = 0;
                applicationFonts.GetList()->ToBuffer(&pData, &lLen, L"", true);

                char* cData64 = NULL;
                int nData64Dst = 0;
                NSFile::CBase64Converter::Encode(pData, (int)lLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

                std::wstring sData64 = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(cData64, (LONG)nData64Dst, FALSE);

                oWriterJS += (L"window[\"g_fonts_selection_bin\"] = \"");
                oWriterJS += sData64;
                oWriterJS += L"\";\n";

                RELEASEARRAYOBJECTS(pData);
                RELEASEARRAYOBJECTS(cData64);
            }

            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strFile);
            oFile.WriteStringUTF8(oWriterJS.GetCString(), true);
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

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
#if 0
    char buf[10];
    wcout << "[\n";
    wcout << itoa(argc, buf, 10) << "\n";

    for (int i = 0; i < argc; ++i)
        wcout << argv[i] << "\n";

    wcout << "]";
#endif

#if 1

#ifdef WIN32
    std::wstring strFontsFolder = L"";
    if (1 < argc)
        strFontsFolder = std::wstring(argv[1]);
    std::wstring strAllFontsJSPath = L"";
    if (2 < argc)
        strAllFontsJSPath = std::wstring(argv[2]);
    std::wstring strThumbnailsFolder = L"";
    if (3 < argc)
        strThumbnailsFolder = std::wstring(argv[3]);
    std::wstring strFontsSelectionBin = L"";
    if (4 < argc)
        strFontsSelectionBin = std::wstring(argv[4]);
#else
    std::wstring strFontsFolder = L"";
    if (1 < argc)
        strFontsFolder = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)argv[1], (LONG)strlen(argv[1]));
    std::wstring strAllFontsJSPath = L"";
    if (2 < argc)
        strAllFontsJSPath = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)argv[2], (LONG)strlen(argv[2]));
    std::wstring strThumbnailsFolder = L"";
    if (3 < argc)
        strThumbnailsFolder = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)argv[3], (LONG)strlen(argv[3]));
    std::wstring strFontsSelectionBin = L"";
    if (4 < argc)
        strFontsSelectionBin = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)argv[4], (LONG)strlen(argv[4]));
#endif

#endif

#if 0
#ifdef WIN32
    //std::wstring strFontsFolder = L"C:/Windows/Fonts";
    std::wstring strFontsFolder = L"D:/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Fonts/native";
    std::wstring strAllFontsJSPath = L"D:/AllFontsGenTest/AllFonts.js";
    std::wstring strThumbnailsFolder = L"D:/AllFontsGenTest";
    std::wstring strFontsSelectionBin = L"D:/AllFontsGenTest/font_selection.bin";
#endif

#ifdef _LINUX
    std::wstring strFontsFolder = L"";
    std::wstring strAllFontsJSPath = L"/home/oleg/AllFontsGen/AllFonts.js";
    std::wstring strThumbnailsFolder = L"/home/oleg/AllFontsGen/";
    std::wstring strFontsSelectionBin = L"/home/oleg/AllFontsGen/font_selection.bin";
#endif

#endif

    CApplicationFonts oApplicationF;

    if (strFontsFolder.length() != 0)
        oApplicationF.InitializeFromFolder(strFontsFolder, false);
    else
        oApplicationF.Initialize(false);

    NSCommon::SaveAllFontsJS(oApplicationF, strAllFontsJSPath, strThumbnailsFolder, strFontsSelectionBin);

#ifdef _GENERATE_FONT_MAP_

    NSCommon::DumpFontsDictionary(L"D:\\fonts_dictionary.txt");

#endif

    return 0;
}

